#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define LITE_GFX_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

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
	AudioBuffer* music = AudioBuffer::load("data/benitoKamelas.wav");
	if (!music) {
		cout << "Error: No se ha podido cargar el wav" << endl;
		return -1;
	}

	AudioSource source(music);
	source.play();

	Vec3D listenerPosition();
	
	float pitch = 0.0f;

	Vec3D sourcePos = source.getPosition();
	Vec3D listenerPos(0.0f, 0.0f, -0.2f);

	setListenerPosition(listenerPos.mX, listenerPos.mY, listenerPos.mZ);
	// Bucle principal
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		// Actualizamos delta
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		// Actualizamos tamaño de ventana
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		lgfx_setviewport(0, 0, screenWidth, screenHeight);

		const Vec2 screenSize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

		if (glfwGetKey(window, GLFW_KEY_UP)) {
			pitch += 0.1f;
			source.setPitch(pitch);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			pitch -= 0.1f;
			source.setPitch(pitch);
		}
		else if (glfwGetKey(window, GLFW_KEY_R)) {
			pitch = 1.0f;
			source.setPitch(pitch);
		}
		
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			sourcePos.mX -= 50 * deltaTime;
			source.setPosition(sourcePos.mX, sourcePos.mY, sourcePos.mZ);
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			sourcePos.mX += 50 * deltaTime;
			source.setPosition(sourcePos.mX, sourcePos.mY, sourcePos.mZ);
		}

		// Actualizamos ventana y eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	alcDestroyContext(alContext);
	alcCloseDevice(device);

    return 0;
}