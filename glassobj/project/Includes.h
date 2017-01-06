#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h>

#include "Settings.h"
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "Mesh.h"
#include "Model.h"


#endif // !INCLUDES_H