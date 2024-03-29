//DISCLAMER - This is a modified version of code from one of my other assignments.

#pragma once

#include <SDL.h>
#include <iostream>
#include <string.h>

///Forward declaration of StateManager for the pointer to the StateManager
class StateManager;

/**
@brief Creates a State object to be inherited.
*/
class State
{
public:
	/**
	@brief Constructs a State object.
	@param stateManager A pointer to the StateManager.
	@param window A pointer to the window in use.
	@param screenWidth The screen width.
	@param screenHeight The screen height.
	*/
	State(StateManager* stateManager, SDL_Window* window, int screenWidth, int screenHeight);

	/**
	@brief A virtual destructor for the State object.
	*/
	virtual ~State();

	/**
	@brief A pure virtual function to handle the user input for use with the State.
	@returns If false then quit State.
	*/
	virtual bool input() = 0;

	/**
	@brief A pure virtual function to update the State to allow the State to run.
	@param dt The delta time.
	*/
	virtual void update(float dt) = 0;

	/**
	@brief A pure virtual function to draw to the screen using the window.
	*/
	virtual void draw() = 0;

	/**
	@brief Gets the window
	@returns A pointer to the window
	*/
	SDL_Window* getWindow();

protected:
	///A pointer to the state manager
	StateManager* stateManager;
	///The window to display to
	SDL_Window* window;
	///The width of the screen
	int screenWidth;
	///The height of the screen
	int screenHeight;
};