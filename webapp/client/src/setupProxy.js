/*
 * Copyright 2013-2019 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

const proxy = require('http-proxy-middleware');

module.exports = function(app) {
  app.use(
    proxy('/addJob', {
      target: 'http://localhost:3003/',
      ws: true
    })
  );
  app.use(
    proxy('/getJobStatus', {
      target: 'http://localhost:3003/',
      ws: true
    })
  );
  app.use(
    proxy('/getJobFullResults', {
      target: 'http://localhost:3003/',
      ws: true
    })
  );
  app.use(
    proxy('/getJobInputs', {
      target: 'http://localhost:3003/',
      ws: true
    })
  );
};
