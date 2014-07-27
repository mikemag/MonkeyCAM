/*
 * Copyright 2013-2014 Michael M. Magruder (https://github.com/mikemag)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

function Matrix3x3Det(a, b, c, d, e, f, g, h, i) {
  return (
    (a * e * i) -
    (a * f * h) +
    (d * h * c) -
    (d * b * i) +
    (g * b * f) -
    (g * e * c) );
}

function circleThruThreePoints(p1X, p1Y, p2X, p2Y, p3X, p3Y) {
    var p1x2y2 = Math.pow(p1X, 2) + Math.pow(p1Y, 2);
    var p2x2y2 = Math.pow(p2X, 2) + Math.pow(p2Y, 2);
    var p3x2y2 = Math.pow(p3X, 2) + Math.pow(p3Y, 2);
    var a = Matrix3x3Det(p1X, p1Y, 1,
                         p2X, p2Y, 1,
                         p3X, p3Y, 1);
    var d = Matrix3x3Det(p1x2y2, p1Y, 1,
                         p2x2y2, p2Y, 1,
                         p3x2y2, p3Y, 1) * -1;
    var e = Matrix3x3Det(p1x2y2, p1X, 1,
                         p2x2y2, p2X, 1,
                         p3x2y2, p3X, 1);
    var f = Matrix3x3Det(p1x2y2, p1X, p1Y,
                         p2x2y2, p2X, p2Y,
                         p3x2y2, p3X, p3Y) * -1;
    var centerX = -0.5 * d / a;
    var centerY = -0.5 * e / a;
    var radius = Math.sqrt(((Math.pow(d, 2) + Math.pow(e, 2)) /
                            (4 * Math.pow(a, 2))) - (f / a));
    return [centerX, centerY, radius];
}

// mmmfixme: should be able to nuke this now.
function arcPath(points, p1X, p1Y, p2X, p2Y, p3X, p3Y, d) {
    var c = circleThruThreePoints(p1X, p1Y, p2X, p2Y, p3X, p3Y);
    var cx = c[0];
    var cy = c[1];
    var r = c[2];
    console.log('c: ' + cx + ',' + cy + ', r=' + r);
    // Floats suck, of course, so take steps to ensure atan2() can be reasonable.
    var cdx1 = p1X - cx;
    var cdy1 = p1Y - cy;
    var cdx3 = p3X - cx;
    var cdy3 = p3Y - cy;
    if (Math.abs(cdx1) < 0.001) cdx1 = 0.0;
    if (Math.abs(cdy1) < 0.001) cdy1 = 0.0;
    if (Math.abs(cdx3) < 0.001) cdx3 = 0.0;
    if (Math.abs(cdy3) < 0.001) cdy3 = 0.0;
    var startAngle = Math.atan2(cdy1, cdx1);
    var endAngle = Math.atan2(cdy3, cdx3);
    // Convert the interval to [0, 2PI)
    if (startAngle < 0) startAngle += 2 * Math.PI;
    if (endAngle < 0) {
        endAngle += 2 * Math.PI;
        if (startAngle == 0 && d == 2) startAngle = 2 * Math.PI;
    }
    var pointCount = 100;
    var angleDelta = (endAngle - startAngle) / pointCount;
    if (startAngle > endAngle) angleDelta *= -1;
    if (d == 2) angleDelta *= -1;
    points.push([p1X, p1Y]);
    for (var i = 1; i < pointCount; i++) {
        var angle = startAngle + (angleDelta * i);
        points.push([cx + r * Math.cos(angle), cy + r * Math.sin(angle)]);
    }
    points.push([p3X, p3Y]);
}
