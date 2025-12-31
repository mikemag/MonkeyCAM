/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import 'bootstrap/dist/css/bootstrap.min.css';
import 'codemirror/lib/codemirror.css';
import 'codemirror/theme/eclipse.css';

import React from 'react';
import { createRoot } from 'react-dom/client';
import App from './App';
import sw from './registerServiceWorker';
import './index.css';
import { BrowserRouter } from 'react-router-dom';

const container = document.getElementById('root');
const root = createRoot(container);
root.render(
  <BrowserRouter>
    <App />
  </BrowserRouter>
);

// @TODO: hrm, service workers and cache control are in a right pain in
// the ass. Ditching it for now. Needs more investigation.
sw.unregister();
