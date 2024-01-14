#pragma once
#include "glm/glm.hpp"
#include "Core/Vector.h"

struct Vertex {
    Vector3 Position;        // position
    Vector3 Normal;         // normal
    Vector2 TexCoords;    // texCoords
    
    Vector3 Tangent; // tangent
    Vector3 Bitangent; // bitangent

    Vertex()
    {

    }
};
