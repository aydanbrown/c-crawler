const https = require("https");
const fs = require("fs");

var config = {};
var connection = {};

module.exports = function(conf, con)
{
  config = conf;
  connection = con;

  return download;
}

function download(row, callback) {
  var dir = config.download_dir + "/" + row.id;
  var file = fs.createWriteStream(dir);
  var stream = https.get(row.url, function(response) {
    response.pipe(file)
    .on("finish", function() {
      callback(null, {id: row.id, url: row.url, file: dir});
    })
    .on("error", function(err) {
      callback(err);
    });
  }).on("error", function(err) {
    callback(err);
  });
}
