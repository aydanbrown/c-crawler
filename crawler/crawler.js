const mysql = require("mysql");
const config = require("./nlib/config");
const log = require("./nlib/log");

const url_batch_count = 30;
const url_error_wait = 1000;
const pending_limit = 100;
const pending_limit_wait = 1000;
const no_pending_wait = 1000;

var total_downloads = 0;
var total_processed = 0;
var total_articles = 0;
var failed_downloads = 0;
var failed_processed = 0;

var connection = mysql.createConnection({
  host: config.database.host,
  user: config.database.user,
  password: config.database.pass,
  database: config.database.name
});

connection.connect();

const download = require("./nlib/download")(config, connection);
const process = require("./nlib/process")(config, connection);
const urls = require("./nlib/urls")(config, connection);

var pending = [];

function batch_download() {
  if(pending.length > pending_limit) {
    log.wrn("Reached pending limit " + pending.length + "/" + pending_limit + " trying again in " + (pending_limit_wait / 1000) + " seconds");
    return setTimeout(batch_download, pending_limit_wait);
  }
  urls.list(url_batch_count, function(err, rows) {
    if(err) {
      log.err("Failed to list urls", err);
      log.wrn("Trying again in " + (url_error_wait / 1000) + " seconds");
      setTimeout(batch_download, url_error_wait);
    } else {
      // log.clr("Got " + rows.length + " urls", "green");
      var remaining = rows.length;
      rows.forEach(function(row) {
        download(row, function(dwn_err, result) {
          total_downloads++;
          if(dwn_err) {
            log.err("Failed to download [" + row.id + "] " + row.url, dwn_err);
            failed_downloads++;
          }
          urls.update_status(row.id, dwn_err ? -1 : 1, function(err, result) {
            if(err) log.err("Failed to update status [" + row.id + "] " + row.url, err);
            pending.push({ id: row.id, url: row.url });
            remaining--;
            if(remaining == 0) {
              // log("Downloaded " + rows.length + " pages");
              batch_download();
            }
          });
        });
      });
    }
  });
}

function process_pending() {
  if(pending.length == 0) {
    log.wrn("Nothing pending, trying again in " + (no_pending_wait / 1000) + " seconds");
    return setTimeout(process_pending, no_pending_wait);
  }
  process(pending[0].id, pending[0].url, function(err, is_article) {
    total_processed++;
    if(is_article) total_articles++
    if(err) {
      log.err("Failed to process pending [" + pending[0].id + "] " + pending[0].url, err);
      failed_processed++;
    }
    // log.clr("Processed " + (is_article ? "article" : "non-article") + " [" + pending[0].id + "] " + pending[0].url, "cyan");
    // -2=process_error, -1=download_error, 0=default, 1=pending, 2=done_not_article, 3=done_article
    urls.update_status(pending[0].id, err ? -2 : (is_article ? 2 : 3), function(err) {
      if(err) log.err("Failed to update processed url status [" + pending[0].id + "] " + pending[0].url);
      pending.splice(0, 1);
      process_pending();
    });
  });
}

function update_metrics() {
  connection.query("INSERT INTO metrics (downloads, processed, articles, failed_downloads, failed_processed) VALUES (?, ?, ?, ?, ?)", [total_downloads, total_processed, total_articles, failed_downloads, failed_processed], function(err) {
    if(err) return log.err("Failed to upload metrics", err);
  });
  console.log("Total downloads: " + total_downloads);
  console.log("Total processed: " + total_processed);
  console.log("Total articles: " + total_articles);
  console.log("Failed downloads: " + failed_downloads);
  console.log("Failed processed: " + failed_processed);
  total_downloads = 0;
  total_processed = 0;
  total_articles = 0;
  failed_downloads = 0;
  failed_processed = 0;
}

function end() {
  connection.end();
}

batch_download();
setTimeout(process_pending, 1000);

setInterval(update_metrics, 10000);
