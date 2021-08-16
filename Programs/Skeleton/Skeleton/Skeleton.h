#pragma once
#include "Scene.h"
#include "framework.h"
#include "Shaders.h"
#include <iostream>


static Scene* scene = NULL;
static GPUProgram gpuProgram; // vertex and fragment shaders
static unsigned int vao;	   // virtual world on the GPU
