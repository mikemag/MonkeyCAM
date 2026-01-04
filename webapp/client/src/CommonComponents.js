/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from 'react';
import { Alert } from 'react-bootstrap';

const development = process.env.NODE_ENV === 'development';
let cfURLRoot = 'https://monkeycam-worker-923789674618.us-central1.run.app';

if (development) {
  cfURLRoot = ''; // Let the proxy setting in package.json take care of these when running locally
}

class BadJobNotice extends React.Component {
  render() {
    return (
      <Alert variant="info" className="text-left">
        Please report this problem at{' '}
        <a href="https://github.com/mikemag/MonkeyCAM/discussions">MonkeyCAM Discussions</a>, especially if you see it occurring often!
      </Alert>
    );
  }
}

export { BadJobNotice, cfURLRoot };
