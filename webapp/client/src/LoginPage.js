/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from 'react';
import { Alert } from 'react-bootstrap';

const LoginPage = () => (
  <div>
    <Alert bsStyle="success">
      <p>
        <strong>Login is not yet implemented!</strong> Eventually we'll let you
        create an account and save ski and board designs, machine definitions,
        results, etc.
      </p>
      <p> Creating an account will always be optional.</p>
    </Alert>
  </div>
);

export default LoginPage;
