const spawn = require('child_process').spawn;
const fs = require('fs');
const log = require('./log');

const article = {
  content: {
    h1: "h1",
    h2: "h2"
  },
  meta: {
    description: "description",
    keywords: "keywords",
    "article:section": "section",
    "article:published_time": "published",
    "og:title": "title"
  }
}

var config;
var connection;

function process(id, url, callback) {
  var input = config.download_dir + "/" + id;
  if(!fs.existsSync(input)) return callback("File does not exist: " + input);
  upload_urls(input, function(err) {
    if(err) log.err("Failed to upload urls", err);
    verify_article(input, function(err) {
      if(err) {
        clean(input, function() { callback(null, false); });
      } else {
        extract_content(input, function(err) {
          if(err) log.err("Error while extracting content, continuing...");
          extract_metadata(input, function(err) {
            if(err) log.err("Error while extracting metadata, continuing...");
            upload_article(url, function(err) {
              clean(input, function() { callback(err, true); });
            });
          })
        });
      }
    });
  });
}

function execute(cmd, args, callback) {
  spawn(config.bin_dir + "/" + cmd, args).on("close", callback);
}

function clean(input, callback) {
  unlink_file(input);
  unlink_file(config.process_dir + "/urls");
  for(var folder in article) {
    for(var file in article[folder]) {
      unlink_file(config.process_dir + "/" + folder + "/" + file);
    }
  }
  callback();
}

function upload_urls(input, callback) {
  var dir = config.process_dir + "/urls";
  execute("url-extract", [config.domain, input, dir], function(err) {
    if(err) log.err("There was an issue extracting the urls, trying to upload anyway");
    fs.readFile(dir, "utf8", function(err, data) {
      if(err) {
        log.err("There was an issue reading the url file: " + dir);
        callback(err);
      } else {
        var values = "(\"" + config.domain + data.split("\n").join("\"), (\"" + config.domain) + "\")";
        connection.query("INSERT INTO urls (url) VALUES " + values + " ON DUPLICATE KEY UPDATE occurances = occurances + 1", function(err) {
          if(err) log.err("There was an issue uploading the urls");
          callback(err);
        });
      }
    });
  });
}

function verify_article(input, callback) {
  execute("verify-article", [input], callback);
}

function extract_content(input, callback) {
  execute("content-extract", [input, config.process_dir], callback);
}

function extract_metadata(input, callback) {
  execute("meta-extract", [input, config.process_dir], callback);
}

function upload_article(url, callback) {
  var columns = ["url"];
  var values = [url];
  for(var folder in article) {
    for(var file in article[folder]) {
      var col = article[folder][file]
      columns.push(col);
      var value = read_file(config.process_dir + "/" + folder + "/" + file);
      if(value.length > config.size[col]) {
        log.wrn(col + " length is too long (" + value.length + "/" + config.size[col] + "), cutting it down");
        value = value.substring(0, config.size[col]);
      }
      if(col == "published") value = value.split(".")[0].split("T").join(" ");
      values.push(value);
    }
  }
  connection.query("INSERT INTO content (" + columns.join(",") + ") VALUES (" + "?".repeat(values.length).split("").join(",") + ")", values, callback);
}

function read_file(dir) {
  try {
    return fs.readFileSync(dir, "ascii");
  } catch(err) {
    return "";
  }
}

function unlink_file(dir) {
  try {
    fs.unlinkSync(dir, "utf8");
  } catch(err) {
    // console.log("Failed to unlink file: " + dir);
    // console.error(err);
  }
}

module.exports = function(conf, con) {
  config = conf;
  connection = con;

  return process;
}
