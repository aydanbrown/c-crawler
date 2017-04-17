const mysql = require("mysql");
const config = require("./nlib/config");

var db_name = "crawler";

var seed_url = "https://www.theguardian.com/international";

var tables = {
  urls: [
    "id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT",
    "url VARCHAR(300) NOT NULL UNIQUE",
    "status TINYINT NOT NULL DEFAULT 0",
    "occurances INT UNSIGNED NOT NULL DEFAULT 1",
    "added TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
  ],
  content: [
    "id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT",
    "url_ref INT UNSIGNED NOT NULL UNIQUE",
    "title VARCHAR(" + config.size.title + ")",
    "description VARCHAR(" + config.size.description + ")",
    "keywords VARCHAR(" + config.size.keywords + ")",
    "author VARCHAR(" + config.size.author + ")",
    "section VARCHAR(" + config.size.section + ")",
    "published TIMESTAMP",
    "h1 VARCHAR(" + config.size.h1 + ")",
    "h2 VARCHAR(" + config.size.h2 + ")",
    "h3 VARCHAR(" + config.size.h3 + ")",
    "h4 VARCHAR(" + config.size.h4 + ")",
    "h5 VARCHAR(" + config.size.h5 + ")",
    "h6 VARCHAR(" + config.size.h6 + ")",
    "p VARCHAR(" + config.size.p + ")",
    "added TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
  ]
}

var connection = mysql.createConnection({
  host: config.database.host,
  user: config.database.user,
  password: config.database.pass,
  database: config.database.name
});

connection.connect();

connection.query("DROP DATABASE " + db_name, function(err) {
  if(err) console.error(err);
  connection.query("CREATE DATABASE " + db_name, function(err) {
    if(err) {
      console.error(err);
      connection.end();
    } else {
      connection.query("USE " + db_name, function(err) {
        if(err) {
          console.error(err);
          connection.end();
        } else {
          var pending = 0;
          for(var table in tables) {
            pending++;
            connection.query(build_query(table, tables[table]), function(err) {
              if(err) console.error(err);
              pending--;
              if(pending == 0) {
                connection.query("INSERT INTO urls (url) VALUES (\"" + seed_url + "\")", function(err) {
                  if(err) {
                    console.error(err);
                  }
                  connection.end();
                });
              }
            });
          }
        }
      });
    }
  });
});

function build_query(name, columns) {
  var query = "CREATE TABLE " + name + " (" + columns.join(',') + ")";
  console.log(query);
  return query;
}
