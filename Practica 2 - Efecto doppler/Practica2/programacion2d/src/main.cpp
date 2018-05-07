#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define LITE_GFX_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SOURCE_CIRCLE_RAD 50
#define LISTENER_CIRCLE_RAD 25

#include <litegfx.h>
#include <stb_image.h>
#include <stb_truetype.h>
#include <iostream>
#include <vector>
#include <glfw3.h>
#include <al.h>
#include <alc.h>
#include <vec2.h>
#include <textureManager.h>
#include <Collider.h>
#include <Sprite.h>
#include <World.h>
#include <AudioBuffer.h>
#include <AudioSource.h>
#include <Listener.h>

using namespace std;

int main() {
	// Inicializamos GLFW
	if (!glfwInit()) {
		cout << "Error: No se ha podido inicializar GLFW" << endl;
		return -1;
	}
	atexit(glfwTerminate);

	// Creamos la ventana
	glfwWindowHint(GLFW_RESIZABLE, false);
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Programacion 2D - Colisiones", nullptr, nullptr);
	if (!window) {
		cout << "Error: No se ha podido crear la ventana" << endl;
		return -1;
	}

	// Activamos contexto de OpenGL
	glfwMakeContextCurrent(window);

	// Inicializamos LiteGFX
	lgfx_setup2d(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Iniciamos el contexto de al
	ALCdevice* device = alcOpenDevice(nullptr);
	ALCcontext* alContext = alcCreateContext(device, nullptr);
	ALCboolean makeContext = alcMakeContextCurrent(alContext);

	if (!makeContext) {
		cout << "Error: No se ha podido crear el contexto de al" << endl;
		return -1;
	}

	// Cargo audio
	AudioBuffer* music = AudioBuffer::load("data/engine.wav");
	if (!music) {
		cout << "Error: No se ha podido cargar el wav" << endl;
		return -1;
	}

	AudioSource source(music);
	source.setLooping(true);
	source.play();

	Vec3D sourceVel = source.getVelocity();
	Vec3D listenerVel(0.0f, 0.0f, 0.0f);

	Vec3D sourcePos = source.getPosition();
	Vec3D listenerPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 160, 0.0f);

	Vec3D sourceOldPos = source.getPosition();
	Vec3D listenerOldPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 160, 0.0f);

	setListenerPosition(listenerPos.mX, listenerPos.mY, listenerPos.mZ);
	setListenerVelocity(listenerVel.mX, listenerVel.mY, listenerVel.mZ);
	setListenerOrientation(0.0f, 0.0f, 0.0f);

	// Bucle principal
	double lastTime = glfwGetTime();
	float angle = 0.0f;
	float second = 0.0f;
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		lgfx_clearcolorbuffer(0, 0, 0);
		lgfx_setblend(BLEND_ALPHA);

		// Actualizamos delta
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		second += deltaTime;

		// Actualizamos tamaño de ventana
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		lgfx_setviewport(0, 0, screenWidth, screenHeight);

		const Vec2 screenSize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			listenerPos.mX -= 50 * deltaTime;
			setListenerPosition(listenerPos.mX, listenerPos.mY, listenerPos.mZ);
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			listenerPos.mX += 50 * deltaTime;
			setListenerPosition(listenerPos.mX, listenerPos.mY, listenerPos.mZ);
		}

		angle += 90 * deltaTime;
		float radAngle = angle * DEGRE_TO_RAD;

		float circleX = (screenSize.getX() / 8) * sin(radAngle) + (screenSize.getX() / 2);
		float circleY = (screenSize.getX() / 8) * cos(radAngle) + (screenSize.getY() / 2);

		sourcePos.mX = circleX;
		sourcePos.mY = circleY;
		source.setPosition(sourcePos.mX, sourcePos.mY, sourcePos.mZ);

		listenerVel.mX = (listenerPos.mX - listenerOldPos.mX) / deltaTime;
		
		listenerOldPos.mX = listenerPos.mX;

		sourceVel.mX = (sourcePos.mX - sourceOldPos.mX) / deltaTime;
		sourceVel.mY = (sourcePos.mY - sourceOldPos.mY) / deltaTime;
		
		sourceOldPos.mX = sourcePos.mX;
		sourceOldPos.mY = sourcePos.mY;

		alDopplerFactor(1);
		source.setVelocity(sourceVel.mX, sourceVel.mY, 0);
		setListenerVelocity(listenerVel.mX, 0.0f, 0.0f);

		lgfx_setcolor(0, 1, 0, 1);
		lgfx_drawoval(sourcePos.mX, sourcePos.mY, SOURCE_CIRCLE_RAD, SOURCE_CIRCLE_RAD);

		lgfx_setcolor(1, 1, 1, 1);
		lgfx_drawoval(listenerPos.mX, listenerPos.mY, LISTENER_CIRCLE_RAD, LISTENER_CIRCLE_RAD);

		// Actualizamos ventana y eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	alcDestroyContext(alContext);
	alcCloseDevice(device);

    return 0;
}