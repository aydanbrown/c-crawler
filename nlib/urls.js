var config;
var connection;

function list(limit, callback) {
  connection.query("SELECT id, url FROM urls WHERE status = 0 LIMIT " + limit, function(err, rows) {
    if(err) return callback(err);
    if(rows.length == 0) return callback("No urls returned");
    callback(null, rows);
  });
}

function update_status(id, status, callback) {
  connection.query("UPDATE urls SET status = " + status + " WHERE id = " + id, callback);
}

module.exports = function(conf, conn) {
  config = conf;
  connection = conn;

  return {
    list: list,
    update_status: update_status
  };
}
