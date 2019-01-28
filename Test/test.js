const NativeWebGL = require('./native-webgl');
const webGLCanvas = NativeWebGL.createCanvas();
while (true) {
    webGLCanvas.clearColor(1.0, 0.0, 0.0, 1.0);
    webGLCanvas.clear(webGLCanvas.COLOR_BUFFER_BIT);
	webGLCanvas.flush();
}