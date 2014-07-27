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

// @TODO: this is just a bit of fooling around getting some basic
// visualization working, and experimenting with three.js. This is by
// no means complete!

if (!Detector.webgl) Detector.addGetWebGLMessage();

var renderer, scene, camera, controls;
var WIDTH = window.innerWidth,
HEIGHT = window.innerHeight;

var GCodeRegexG = /[gG]([0-9]+)/g;
var GCodeRegexCoord = /([xXyYzZ])(-?[0-9]+\.[0-9]+)/g;
var GCodeRegexFeed = /[fF]([0-9]+)/g;
var GCodeRegexR = /[rR](-?[0-9]+\.[0-9]+)/g;

init();
animate();

function circle(geometry, color, x1, y1, z1, x2, y2, z2, r, g) {
    var xd = x2 - x1;
    var yd = y2 - y1;
    var coordlen = Math.sqrt(xd * xd + yd * yd);
    var midx = (x1 + x2) / 2;
    var midy = (y1 + y2) / 2;
    var unx = -yd / coordlen;
    var uny = xd / coordlen;
    var d = Math.sqrt(r * r - (coordlen/2) * (coordlen/2));
    var cx = midx - d * unx;
    var cy = midy - d * uny;
    // Floats suck, of course, so take steps to ensure atan2() can be reasonable.
    var cdx1 = x1 - cx;
    var cdy1 = y1 - cy;
    var cdx2 = x2 - cx;
    var cdy2 = y2 - cy;
    if (Math.abs(cdx1) < 0.001) cdx1 = 0.0;
    if (Math.abs(cdy1) < 0.001) cdy1 = 0.0;
    if (Math.abs(cdx2) < 0.001) cdx2 = 0.0;
    if (Math.abs(cdy2) < 0.001) cdy2 = 0.0;
    var startAngle = Math.atan2(cdy1, cdx1);
    var endAngle = Math.atan2(cdy2, cdx2);
    // Convert the interval to [0, 2PI)
    if (startAngle < 0) startAngle += 2 * Math.PI;
    if (endAngle < 0) {
        endAngle += 2 * Math.PI;
        if (startAngle == 0 && g == 2) startAngle = 2 * Math.PI;
    }
    var pointCount = 10;
    var angleDelta = (endAngle - startAngle) / pointCount;
    if (startAngle > endAngle) angleDelta *= -1;
    if (g == 2) angleDelta *= -1;
    for (var i = 0; i < pointCount; i++) {
        var angle = startAngle + (angleDelta * i);
        geometry.vertices.push(
            new THREE.Vector3(
                cx + r * Math.cos(angle),
                cy + r * Math.sin(angle),
                z1));
        geometry.colors.push(new THREE.Color(color));
    }
    geometry.vertices.push(new THREE.Vector3(x2, y2, z2));
    geometry.colors.push(new THREE.Color(color));
}

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
    var g = 0;

    geometryPath.colors = [];
    for (var i = 0; i < gcode.length; i++) {
        GCodeRegexG.lastindex = 0;
        GCodeRegexCoord.lastIndex = 0;
        var gMatch;
        var feeding = false;
        var r;
        while ((gMatch = GCodeRegexG.exec(gcode[i])) != null) {
            g = parseInt(gMatch[1]);
            switch (g) {
            case 0:
                feeding = false;
                break;
            case 1:
                feeding = true;
                break;
            case 2:
            case 3:
                var rMatch = GCodeRegexR.exec(gcode[i]);
                if (rMatch != null) r = parseFloat(rMatch[1]) * 2.54;
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
        var oldx = x; var oldy = y; var oldz = z;
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
        var color;
        if ((feedRate == -1) || !feeding) {
            color = 0xff7700;
        } else {
            color = 0x0077ff;
        }
        if (g == 0 || g == 1) {
            geometryPath.vertices.push(new THREE.Vector3(x, y, z));
            geometryPath.colors.push(new THREE.Color(color));
        } else if (g == 2 || g == 3) {
            circle(geometryPath, color, oldx, oldy, oldz, x, y, z, r, g);
        }
    }
    return geometryPath;
}

function addGcode(scene, material, name, yoffset, zoffset) {
    var geometryPath = gcodeToGeometry(gcodeFiles[name]);
    geometryPath.computeLineDistances();
    var object = new THREE.Line(geometryPath, material, THREE.LineStrip);
    object.position.x = -168 / 2;
    object.position.y = yoffset;
    object.position.z = zoffset;
    scene.add(object);
}

function init() {
    camera = new THREE.PerspectiveCamera(60, WIDTH / HEIGHT, 1, 300);
    camera.position.z = 150;

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
        linewidth: 1,
        vertexColors: THREE.VertexColors
    });

    addGcode(scene, material, "BaseCutout", 40, 0);
    addGcode(scene, material, "NoseTailSpacerCutout", 40, -10);
    addGcode(scene, material, "TopProfile", 0, 0);
    addGcode(scene, material, "TopCutout", 0, -10);
    addGcode(scene, material, "EdgeTrench", -40, 0);
    addGcode(scene, material, "GuideHoles", -40, -10);
    addGcode(scene, material, "CoreAlignmentMarks", -40, -20);
    addGcode(scene, material, "CoreEdgeGroove", -40, -30);
    addGcode(scene, material, "InsertHoles", -40, -40);

    renderer = new THREE.WebGLRenderer( { antialias: true, alpha: false } );
    renderer.setClearColor(0x111111, 1);
    renderer.setSize(WIDTH, HEIGHT);

    var container = document.getElementById('three_d_container');
    container.appendChild(renderer.domElement);

    controls = new THREE.TrackballControls(camera, renderer.domElement);
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
    controls.update();
    renderer.render(scene, camera);
}
