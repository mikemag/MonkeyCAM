/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

var nconf = require('nconf');
var path = require('path');

nconf
  .argv()
  .env()
  .file({ file: path.join(__dirname, 'config.json') });

module.exports = nconf;
