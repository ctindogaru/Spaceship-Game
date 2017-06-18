#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateShip(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(-length / 2, -length * sqrt(3) / 2, 0), color),
		VertexFormat(corner + glm::vec3(length / 2, -length * sqrt(3) / 2, 0), color),
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length / 2, length * sqrt(3) / 2, 0), color),
		VertexFormat(corner + glm::vec3(-length / 2, length * sqrt(3) / 2, 0), color)
	};

	Mesh* ship = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 4 };

	if (!fill) {
		ship->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	ship->InitFromData(vertices, indices);
	return ship;
}

Mesh* Object2D::CreateBullet(std::string name, glm::vec3 leftBottomCorner, float width, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;
	float length = 4 * width;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* bullet = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		bullet->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	bullet->InitFromData(vertices, indices);
	return bullet;
}