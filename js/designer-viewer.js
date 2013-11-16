/** @jsx React.DOM */

/*
 * Copyright 2013 Michael M. Magruder (https://github.com/mikemag)
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

window.addEventListener('resize', onWindowResize, false);
onWindowResize();

function onWindowResize() {
    var canvas = document.getElementById('designerCanvas');
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight - canvas.parentNode.offsetTop;
    clearDesignDrawing();
}

function startDesignDrawing() {
    var canvas = document.getElementById('designerCanvas');
    var context = canvas.getContext('2d');
    context.save();
    context.translate(canvas.width / 2, canvas.height / 2);
    context.scale(4, 4);
    return context;
}

function endDesignDrawing(context) {
    context.restore();
}

function clearDesignDrawing() {
    var canvas = document.getElementById('designerCanvas');
    var context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);
    context = startDesignDrawing();
    context.rect(0, -16, 168, 32);
    context.lineWidth = 0.5;
    context.strokeStyle = '#777777';;
    context.stroke();
    endDesignDrawing(context);
}

function edgeDesignShape(p1X, p1Y, p2X, p2Y, p3X, p3Y, counterClockwise) {
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
    var endAngle = Math.atan2(cdy1, cdx1);
    var startAngle = Math.atan2(cdy3, cdx3);
    // Convert the interval to [0, 2PI)
    if (startAngle < 0) startAngle += 2 * Math.PI;
    if (endAngle < 0) endAngle += 2 * Math.PI;
    // Stupid canvas is upside down, and arcs are backwards...
    cy = -cy;
    startAngle = 0 - startAngle;
    endAngle = 0 - endAngle;

    var context = startDesignDrawing(context);
    context.beginPath();
    context.arc(cx, cy, r, startAngle, endAngle, counterClockwise);
    context.lineWidth = 0.5;
    context.strokeStyle = 'white';
    context.stroke();
    endDesignDrawing(context);
}

function boardDesign(noseLength, eeLength, tailLength, waistWidth, sidecutRadius,
                     taper) {
    this.noseLength = noseLength;
    this.eeLength = eeLength;
    this.tailLength = tailLength;
    this.waistWidth = waistWidth;
    this.sidecutRadius = sidecutRadius;
    this.taper = taper;
    this.sidecutDepth = sidecutRadius -
        Math.sqrt(((sidecutRadius * sidecutRadius) - (eeLength * eeLength / 4)));
    this.noseWidth = waistWidth + (this.sidecutDepth * 2) + (taper / 20);
    this.tailWidth = waistWidth + (this.sidecutDepth * 2) - (taper / 20);
    this.overallLength = noseLength + eeLength + tailLength;
}

var ShapeSelector = React.createClass({

    render: function() {
        var shapeOptions = [];
        for (var i = 0; i < this.props.shapes.length; i++) {
            shapeOptions.push(
                    <option value={i}>
                    {this.props.shapes[i].getName()}
                </option>);
        }
        return (
                <div>
                  <b>{this.props.children}: </b>
                  <select value={this.props.selected} onChange={this.onChange}>
                    {shapeOptions}
                  </select>
                  {this.props.shapes[this.props.selected]}
                </div>
        );
    },

    onChange: function(e) {
        var newState = new Object();
        newState[this.props.stateName] = e.target.value;
        this.props.boardBasics.setState(newState);
    },

});
