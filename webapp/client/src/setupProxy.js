/*
 * Copyright 2013-2019 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

const { createProxyMiddleware } = require('http-proxy-middleware');

module.exports = function(app) {
  app.use(
    '/addJob',
    createProxyMiddleware({
      target: 'http://localhost:3003/',
      ws: true
    })
  );
  app.use(
    '/getJobStatus',
    createProxyMiddleware({
      target: 'http://localhost:3003/',
      ws: true
    })
  );
  app.use(
    '/getJobFullResults',
    createProxyMiddleware({
      target: 'http://localhost:3003/',
      ws: true
    })
  );
  app.use(
    '/getJobInputs',
    createProxyMiddleware({
      target: 'http://localhost:3003/',
      ws: true
    })
  );
};
