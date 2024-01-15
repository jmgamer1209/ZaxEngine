#pragma once
#include "glm/glm.hpp"
#include "Core/Vector.h"

struct Vertex {
    Vector3 position;        // position
    Vector3 normal;         // normal
    Vector2 texCoord;    // texCoord
    
    Vector3 tangent; // tangent
    Vector3 bitangent; // bitangent

    Vertex()
    {

    }

    Vertex(Vector3 postion)
    {
        this->position = postion;
    }

    Vertex(Vector3 postion, Vector3 normal, Vector2 texCoord)
    {
        this->position = postion;
        this->normal = normal;
        this->texCoord = texCoord;
    }
};
