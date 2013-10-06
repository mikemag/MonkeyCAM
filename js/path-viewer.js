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

// @TODO: this is just a bit of fooling around getting some basic
// visualization working, and experimenting with three.js. This is by
// no means complete!

if (!Detector.webgl) Detector.addGetWebGLMessage();

var renderer, scene, camera, controls;
var objects = [];
var WIDTH = window.innerWidth,
HEIGHT = window.innerHeight;

var GCodeRegexG = /[gG]([0-9]+)/g;
var GCodeRegexCoord = /([xXyYzZ])(-?[0-9]+\.[0-9]+)/g;
var GCodeRegexFeed = /[fF]([0-9]+)/g;

init();
animate();

function gcodeParseFeedRate(gcodeLine) {
    GCodeRegexFeed.lastIndex = 0;
    var feedRate = -1;
    var m;
    while ((m = GCodeRegexFeed.exec(gcodeLine)) != null) {
        feedRate = parseInt(m[1]);
    }
    return feedRate;
}

function gcodeToGeometry(gcode) {
    var geometryPath = new THREE.Geometry();
    var x = 0.0;
    var y = 0.0;
    var z = 0.0;
    var incremental = false;
    var wco = 54;
    var feedRate = -1;

    var colors = [];
    for (var i = 0; i < gcode.length; i++) {
        GCodeRegexG.lastindex = 0;
        GCodeRegexCoord.lastIndex = 0;
        var gMatch;
        var feeding = false;
        while ((gMatch = GCodeRegexG.exec(gcode[i])) != null) {
            var g = parseInt(gMatch[1]);
            switch (g) {
            case 0:
                feeding = false;
                break;
            case 1:
            case 2: // @todo: need real circle represenation
            case 3:
                feeding = true;
                break;
            case 54:
            case 55:
                wco = g;
                break;
            case 90:
                incremental = false;
                break;
            case 91:
                incremental = true;
                break;
            default:
                //console.log("Unknown gcode: " + gMatch[1]);
                continue;
            }
        }
        if (feeding) {
            var f = gcodeParseFeedRate(gcode[i]);
            if (f != -1) feedRate = f;
        }
        var coordMatch;
        if (gcode[i][0] != '(') { // @TODO: real comment treatment
            while ((coordMatch = GCodeRegexCoord.exec(gcode[i])) != null) {
                var c = parseFloat(coordMatch[2]) * 2.54;
                switch (coordMatch[1]) {
                case "x":
                case "X":
                    if (!incremental) x = c; else x += c;
                    break;
                case "y":
                case "Y":
                    if (!incremental) y = c; else y += c;
                    break;
                case "z":
                case "Z":
                    if (!incremental) z = c * 3; else z += c * 3;
                    break;
                }
            }
        }
        geometryPath.vertices.push(new THREE.Vector3(x, y, z));
        if ((feedRate == -1) || !feeding) {
            colors[i] = new THREE.Color(0xff7700);
        } else {
            colors[i] = new THREE.Color(0x0077ff);
        }
    }
    geometryPath.colors = colors;
    return geometryPath;
}

function init() {
    camera = new THREE.PerspectiveCamera(60, WIDTH / HEIGHT, 1, 300);
    camera.position.z = 150;

    controls = new THREE.TrackballControls(camera);
    controls.rotateSpeed = 1.0;
    controls.zoomSpeed = 1.2;
    controls.panSpeed = 0.8;
    controls.noZoom = false;
    controls.noPan = false;
    controls.staticMoving = false;
    controls.dynamicDampingFactor = 0.2;
    controls.maxDistance = 250;
    controls.minDistance = 10;
    controls.keys = [ 16 /*shift*/, 17 /*ctrl*/, 18 /*alt*/ ];

    scene = new THREE.Scene();
    scene.fog = new THREE.Fog(0x111111, 150, 275);

    var dashedMaterial = new THREE.LineDashedMaterial({
        color: 0xffaa00,
        dashSize: 3,
        gapSize: 1,
        linewidth: 2
    });
    var material = new THREE.LineBasicMaterial({
        color: 0xffffff,
        opacity: 1,
        linewidth: 3,
        vertexColors: THREE.VertexColors
    });

    var yoffset = 60;
    for (gcodeFileName in gcodeFiles) {
        var geometryPath = gcodeToGeometry(gcodeFiles[gcodeFileName]);
        geometryPath.computeLineDistances();
        var object = new THREE.Line(geometryPath, material, THREE.LineStrip);
        object.position.x = -168 / 2;
        object.position.y = yoffset;
        objects.push(object);
        scene.add(object);
        yoffset -= 18;
    }

    renderer = new THREE.WebGLRenderer( { antialias: true, alpha: false } );
    renderer.setClearColor(0x111111, 1);
    renderer.setSize(WIDTH, HEIGHT);

    var container = document.getElementById('three_d_container');
    container.appendChild(renderer.domElement);

    window.addEventListener('resize', onWindowResize, false);
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
}

function animate() {
    requestAnimationFrame(animate);
    render();
}

function render() {
    var time = Date.now();
    for (var i = 0; i < objects.length; i++) {
        var object = objects[i];
        //object.rotation.x = 0.25 * time * 0.001;
        //object.rotation.y = 0.25 * time * 0.001;
        //object.rotation.z = 0.25 * time * 0.001;
    }
    controls.update();
    renderer.render(scene, camera);
}
