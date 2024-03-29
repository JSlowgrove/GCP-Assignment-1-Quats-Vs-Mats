//DISCLAMER - This is a modified version of code from one of my other assignments.

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "GL/glew.h"
#include "FileLoader.h"

/**
@brief Creates an object from an text file, this can then be used with OpenGL.
*/
class Object
{	
public:
	/**
	@brief Creates a vertex array object using a obj file location and OpenGL.
	@param objFileName The name of the obj file.
	*/
	Object(std::string objFileName);

	/**
	@brief Creates a vertex array object using a obj file location and OpenGL.
	@param objFileName The name of the obj file.
	@param material The name of the material file.
	*/
	Object(std::string objFileName, std::string material);

	/**
	@brief Destructs an Object.
	*/
	~Object();
	
	/**
	@brief Returns the vertex array object.
	@returns The vertex array object.
	*/
	GLuint getVAO();

	/**
	@brief Returns the Texture ID.
	@returns The texture ID.
	*/
	GLuint getTextureID();

	/**
	@brief Returns the number of vertices.
	@returns The number of vertices.
	*/
	unsigned int getNumberOfVertices();

private:
	///The Vertex Array Object for use with OpenGL
	GLuint vertexArrayObject;
	///Number of vertices in the model
	unsigned int numberOfVertices;
	///The name of the material
	std::string material;
	///The Texture
	GLuint textureID;

	/**
	@brief Initialise the texture.
	@param vertexTextures The vertex textures vector.
	*/
	void initialiseTexture(std::vector<float> vertexTextures);

	/**
	@brief Initialise the vertex array object.
	@param objFileName The name of the obj file.
	*/
	void InitialiseVAO(std::string objFileName);
};