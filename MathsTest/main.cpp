//DISCLAMER - This is a modified version of code from one of my other assignments.

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include "GL/glew.h"
#include "PGGAssignment2Code/StateManager.h"
#include "States/MainState.h"

/**************************************************************************************************************/

/*Forces the game to run on the NIVDIA GPU
http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
*/
extern "C"
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

/**************************************************************************************************************/

// An initialisation function, mainly for GLEW
// This will also print to console the version of OpenGL we are using
bool InitGL()
{
	// GLEW has a problem with loading core OpenGL
	// See here: https://www.opengl.org/wiki/OpenGL_Loading_Library
	// The temporary workaround is to enable its 'experimental' features
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		Core::Logging::logE("GLEW failed to initialise: " + std::string(reinterpret_cast<const char *>(glewGetErrorString(err))));
		return false;
	}
	Core::Logging::logI("Using GLEW " + std::string(reinterpret_cast<const char *>(glewGetString(GLEW_VERSION))));

	Core::Logging::logI("OpenGL Vendor: " + std::string(reinterpret_cast<const char *>(glGetString(GL_VENDOR))));
	Core::Logging::logI("OpenGL Renderer: " + std::string(reinterpret_cast<const char *>(glGetString(GL_RENDERER))));
	Core::Logging::logI("OpenGL Version: " + std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))));
	Core::Logging::logI("OpenGL Shading Language Version: " + std::string(reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION))));

	return true;
}

/**************************************************************************************************************/

int main(int argc, char *argv[])
{
	/*Initialise SDL*/
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) /*Check SDL initialisation*/
	{
		/*Failed initialisation*/
		Core::Logging::logE("Failed to initialise SDL");
		return -1;
	}

	/*Initialise SDL_ttf*/
	if (TTF_Init() < 0) /*Check SDL_ttf initialisation*/
	{
		/*Failed initialisation*/
		Core::Logging::logE("SDL_ttf init failed: " + std::string(TTF_GetError()));
		return -1;
	}

	// This is how we set the context profile
	// We need to do this through SDL, so that it can set up the OpenGL drawing context that matches this
	// (of course in reality we have no guarantee this will be available and should provide fallback methods if it's not!)
	// Anyway, we basically first say which version of OpenGL we want to use
	// So let's say 4.3:
	// Major version number (4):
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	// Minor version number (3):
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Then we say whether we want the core profile or the compatibility profile
	// Flag options are either: SDL_GL_CONTEXT_PROFILE_CORE   or   SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
	// We'll go for the core profile
	// This means we are using the latest version and cannot use the deprecated functions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/*Time Check*/
	unsigned int lastTime = SDL_GetTicks();

	/*Create Window*/
	int winPosX = 100;
	int winPosY = 100;
	int winWidth = 720;
	int winHeight = 576;
	SDL_Window *window = SDL_CreateWindow("GCP Assignment 1",  /*The first parameter is the window title*/
		winPosX, winPosY,
		winWidth, winHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	/*Create Renderer from the window*/
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	// Now that the SDL renderer is created for the window, we can create an OpenGL context for it!
	// This will allow us to actually use OpenGL to draw to the window
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	// Call our initialisation function to set up GLEW and print out some GL info to console
	if (!InitGL())
	{
		return -1;
	}

	// Enable the depth test to make sure triangles in front are always in front no matter the order they are drawn
	glEnable(GL_DEPTH_TEST);

	/*setup state manager*/
	StateManager * stateManager = new StateManager();
	/*set the initial state*/
	stateManager->addState(new MainState(stateManager, window, winWidth, winHeight));

	/*Start Game Loop*/
	bool go = true;
	while (go)
	{
		/*Time Check*/
		unsigned int current = SDL_GetTicks();
		float deltaTime = (float)(current - lastTime) / 1000.0f;
		lastTime = current;

		/*handle the current state inputs*/
		go = stateManager->input();

		/*update the current state*/
		stateManager->update(deltaTime);

		/*draw the states*/
		stateManager->draw();

		/*Time Limiter*/
		if (deltaTime < (1.0f / 50.0f))
		{
			SDL_Delay((unsigned int)(((1.0f / 50.0f) - deltaTime)*1000.0f));
		}
	}
	/*destroy data*/
	delete stateManager;
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return -1;
}