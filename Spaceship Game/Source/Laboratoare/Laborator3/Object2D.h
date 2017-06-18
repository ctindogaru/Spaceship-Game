#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateShip(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateBullet(std::string name, glm::vec3 leftBottomCorner, float width, glm::vec3 color, bool fill = false);
}

