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

var NoseShapeSimpleBezier = React.createClass({

    getInitialState: function() {
        return {pullStart: this.props.board.noseWidth / 3,
                pullEnd: this.props.board.noseLength / 3};
    },

    render: function() {
        this.state.pullStart = this.props.board.noseWidth / 3;
        this.state.pullEnd = this.props.board.noseLength / 3;
        this.draw();
        return (
                <div>
                  <Dimension name="Nose length">
                    {this.props.board.noseLength}
                  </Dimension>
                  <Dimension name="Nose width">
                    {this.props.board.noseWidth}
                  </Dimension>
                  <Dimension name="Pull start">{this.state.pullStart}</Dimension>
                  <Dimension name="Pull end">{this.state.pullEnd}</Dimension>
                </div>
        );
    },

    draw: function() {
        var b = this.props.board;
        var context = startDesignDrawing(context);
        context.beginPath();
        context.moveTo(0, 0);
        context.bezierCurveTo(0, this.state.pullStart,
                              this.state.pullEnd, b.noseWidth / 2,
                              b.noseLength, b.noseWidth / 2);
        context.moveTo(0, 0);
        context.bezierCurveTo(0, -this.state.pullStart,
                              this.state.pullEnd, b.noseWidth / -2,
                              b.noseLength, b.noseWidth / -2);
        context.lineWidth = 0.5;
        context.strokeStyle = 'white';
        context.stroke();
        endDesignDrawing(context);
    },

    getName: function() {
        return "Simple Bezier";
    },

    getDefinition: function() {
        // @todo: return the definition of this part to be sent to the server
        // for accurate rendering.
    },
});

var NoseShapeFatBezier = React.createClass({

    getInitialState: function() {
        return {pullStart: this.props.board.noseWidth / 1.8,
                pullEnd: this.props.board.noseLength / 1.8};
    },

    render: function() {
        this.state.pullStart = this.props.board.noseWidth / 1.8;
        this.state.pullEnd = this.props.board.noseLength / 1.8;
        this.draw();
        return (
                <div>
                  <Dimension name="Nose length">
                    {this.props.board.noseLength}
                  </Dimension>
                  <Dimension name="Nose width">
                    {this.props.board.noseWidth}
                  </Dimension>
                  <Dimension name="Pull start">{this.state.pullStart}</Dimension>
                  <Dimension name="Pull end">{this.state.pullEnd}</Dimension>
                </div>
        );
    },

    draw: function() {
        var b = this.props.board;
        var context = startDesignDrawing(context);
        context.beginPath();
        context.moveTo(0, 0);
        context.bezierCurveTo(0, this.state.pullStart,
                              this.state.pullEnd, b.noseWidth / 2,
                              b.noseLength, b.noseWidth / 2);
        context.moveTo(0, 0);
        context.bezierCurveTo(0, -this.state.pullStart,
                              this.state.pullEnd, b.noseWidth / -2,
                              b.noseLength, b.noseWidth / -2);
        context.lineWidth = 0.5;
        context.strokeStyle = 'white';
        context.stroke();
        endDesignDrawing(context);
    },

    getName: function() {
        return "Fat Bezier";
    },

    getDefinition: function() {
        // @todo: return the definition of this part to be sent to the server
        // for accurate rendering.
    },
});

var TailShapeSimpleBezier = React.createClass({

    getInitialState: function() {
        return {pullStart: this.props.board.tailWidth / 3,
                pullEnd: this.props.board.tailLength / 3};
    },

    render: function() {
        this.state.pullStart = this.props.board.tailWidth / 3;
        this.state.pullEnd = this.props.board.tailLength / 3;
        this.draw();
        return (
                <div>
                  <Dimension name="Tail length">
                    {this.props.board.tailLength}
                  </Dimension>
                  <Dimension name="Tail width">
                    {this.props.board.tailWidth}
                  </Dimension>
                  <Dimension name="Pull start">{this.state.pullStart}</Dimension>
                  <Dimension name="Pull end">{this.state.pullEnd}</Dimension>
                </div>
        );
    },

    draw: function() {
        var b = this.props.board;
        var context = startDesignDrawing(context);
        context.beginPath();
        context.moveTo(b.overallLength, 0);
        var tailX = b.noseLength + b.eeLength;
        context.bezierCurveTo(b.overallLength, this.state.pullStart,
                              b.overallLength - this.state.pullEnd,
                              b.tailWidth / 2,
                              tailX, b.tailWidth / 2);
        context.moveTo(b.overallLength, 0);
        context.bezierCurveTo(b.overallLength, -this.state.pullStart,
                              b.overallLength - this.state.pullEnd,
                              b.tailWidth / -2,
                              tailX, b.tailWidth / -2);
        context.lineWidth = 0.5;
        context.strokeStyle = 'white';
        context.stroke();
        endDesignDrawing(context);
    },

    getName: function() {
        return "Simple Bezier";
    },

    getDefinition: function() {
        // @todo: return the definition of this part to be sent to the server
        // for accurate rendering.
    },
});

var EdgeShapeSimpleArc = React.createClass({

    getInitialState: function() {
        return {sidecutRadius: this.props.board.sidecutRadius,
                eeLength: this.props.board.eeLength};
    },

    render: function() {
        this.state.sidecutRadius = this.props.board.sidecutRadius;
        this.state.eeLength = this.props.board.eeLength;
        this.draw();
        return (
                <div>
                  <Dimension name="EE length">
                    {this.props.board.eeLength}
                  </Dimension>
                  <Dimension name="Waist width">
                    {this.props.board.waistWidth}
                  </Dimension>
                  <Dimension name="Taper" unit="mm">
                    {this.props.board.taper}
                  </Dimension>
                  <Dimension name="Sidecut radius">
                    {this.state.sidecutRadius}
                  </Dimension>
                  <Dimension name="Sidecut depth">
                    {this.props.board.sidecutDepth}
                  </Dimension>
                </div>
        );
    },

    draw: function() {
        var b = this.props.board;
        edgeDesignShape(
            b.noseLength,
            b.noseWidth / -2,
            b.noseLength + (b.eeLength / 2),
            b.waistWidth / -2,
            b.noseLength + b.eeLength,
            b.tailWidth / -2,
            true
        );
        edgeDesignShape(
            b.noseLength,
            b.noseWidth / 2,
            b.noseLength + (b.eeLength / 2),
            b.waistWidth / 2,
            b.noseLength + b.eeLength,
            b.tailWidth / 2,
            false
        );
    },

    getName: function() {
        return "Simple Arc";
    },

    getDefinition: function() {
        // @todo: return the definition of this part to be sent to the server
        // for accurate rendering.
    },
});
