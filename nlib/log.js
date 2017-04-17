var c = {
  reset: "\x1b[0m",
  bright: "\x1b[1m",
  dim: "\x1b[2m",
  black: "\x1b[30m",
  red: "\x1b[31m",
  green: "\x1b[32m",
  yellow: "\x1b[33m",
  blue: "\x1b[34m",
  magenta: "\x1b[35m",
  cyan: "\x1b[36m",
  white: "\x1b[37m"
}

function log(val) {
  console.log(val);
}

log.c = c;

log.clr = function(val, clr) {
  console.log(c[clr] + val + c.reset);
}

log.err = function(val, err) {
  console.log(c.red);
  if(err) {
    console.log(val);
    console.error(err);
  } else {
    console.error(val);
  }
  console.log(c.reset);
}

log.wrn = function(val) {
  console.log(c.yellow + val + c.reset);
}

module.exports = log;
