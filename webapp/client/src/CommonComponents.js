/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from 'react';
import { Alert } from 'react-bootstrap';

export class BadJobNotice extends React.Component {
  render() {
    return (
      <Alert bsStyle="info" className="text-left">
        All server errors, slow jobs, and failed jobs are automatically logged
        and reviewed frequently by the developers. Please feel free to report
        this problem to{' '}
        <a href="https://www.facebook.com/MonkeyCAMDev">MonkeyCAMDev</a> on
        Facebook as well, especially if you see it occurring often!
      </Alert>
    );
  }
}
