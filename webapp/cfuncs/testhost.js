/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

const express = require('express');
const bodyParser = require('body-parser');
const cfuncs = require('./index.js');

var router = express.Router();
router.use(bodyParser.json());
router.post('/getJobStatus', cfuncs.getJobStatus);
router.post('/getJobFullResults', cfuncs.getJobFullResults);
router.post('/getJobInputs', cfuncs.getJobInputs);
router.post('/addJob', cfuncs.addJob);

var testhost = express();
testhost.use(bodyParser.json());
testhost.use(bodyParser.urlencoded({ extended: false }));
testhost.use('/', router);

var debug = require('debug')('apiserver:server');
var http = require('http');

var port = parseInt(process.env.PORT || '3003');
console.log('Test Host listening on port', port);
testhost.set('port', port);

var server = http.createServer(testhost);

server.listen(port);
server.on('error', onError);
server.on('listening', onListening);

function onError(error) {
  if (error.syscall !== 'listen') {
    throw error;
  }

  var bind = 'Port ' + port;
  switch (error.code) {
    case 'EACCES':
      console.error(bind + ' requires elevated privileges');
      process.exit(1);
      break;
    case 'EADDRINUSE':
      console.error(bind + ' is already in use');
      process.exit(1);
      break;
    default:
      throw error;
  }
}

function onListening() {
  var addr = server.address();
  var bind = typeof addr === 'string' ? 'pipe ' + addr : 'port ' + addr.port;
  console.log('MonkeyCAM Cloud Functions Test Host listening on ' + bind);
}
