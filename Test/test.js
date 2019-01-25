const NativeWebGL = require('./native-webgl');
const webGLCanvas = NativeWebGL.create_canvas();
while (true) {
	webGLCanvas.flush();
}