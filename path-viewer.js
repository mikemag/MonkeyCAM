if (!Detector.webgl) Detector.addGetWebGLMessage();

var renderer, scene, camera;
var objects = [];
var WIDTH = window.innerWidth,
HEIGHT = window.innerHeight;

init();
animate();

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
        color: 0x00aaff,
        opacity: 1,
        linewidth: 3
    });

    var geometryPath = getPathCore();
    geometryPath.computeLineDistances();
    var object = new THREE.Line(geometryPath, dashedMaterial, THREE.LineStrip);
    objects.push(object);
    scene.add(object);

    var geometryPath = getPathOverall();
    geometryPath.computeLineDistances();
    var object = new THREE.Line(geometryPath, material, THREE.LineStrip);
    objects.push(object);
    scene.add(object);

    var geometryPath = getPathProfile();
    geometryPath.computeLineDistances();
    var object = new THREE.Line(geometryPath, dashedMaterial, THREE.LineStrip);
    objects.push(object);
    scene.add(object);

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
    stats.update();
}

function render() {
    var time = Date.now() * 0.001;
    for (var i = 0; i < objects.length; i++) {
        var object = objects[i];
        object.rotation.x = 0.25 * time;
        //object.rotation.y = 0.25 * time;
        object.rotation.z = 0.25 * time;
    }
    renderer.render(scene, camera);
}
