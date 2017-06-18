#include "Laborator3.h"

#include <vector>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include "Functions.h"

using namespace std;

struct bullet {
	float x;
	float y;
	float angle;
};

vector<bullet> bullets;

struct ship {
	float x;
	float y;
	float speed;
	float scale;
	float time;
	string name;
};

vector<ship> ships;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	resolutionX = resolution.x;
	resolutionY = resolution.y;

	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, resolutionX, 0, resolutionY, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	centerX = resolutionX / 2;
	centerY = resolutionY / 2;
	
	glm::vec3 origin = glm::vec3(0, 0, 0);
	
	// initialize tx and ty (the translation steps)
	translateX = centerX;
	translateY = centerY;

	// initialize sx and sy (the scale factors)
	
	// initialize angularStep
	angularStep = 0;
	
	spawnTime = 2;
	lastSpawnTime = Engine::GetElapsedTime();
	timeElapsed = 6;

	srand(time(NULL));

	Mesh* ship = Object2D::CreateShip("ship", origin, 25, glm::vec3(1, 0, 0), true);
	AddMeshToList(ship);
	Mesh* bullet = Object2D::CreateBullet("bullet", origin, 5, glm::vec3(1, 1, 0), true);
	AddMeshToList(bullet);
	// ship with 1 life
	Mesh* enemyShip0 = Object2D::CreateShip("enemyShip0", origin, 40, glm::vec3(0, 1, 1), true);
	AddMeshToList(enemyShip0);
	// ship with 2 lives
	Mesh* enemyShip1 = Object2D::CreateShip("enemyShip1", origin, 40, glm::vec3(1, 1, 0), true);
	AddMeshToList(enemyShip1);
	// enemyShip1 after a bullet
	Mesh* enemyShip2 = Object2D::CreateShip("enemyShip2", origin, 40, glm::vec3(1, 0, 0), true);
	AddMeshToList(enemyShip2);
	// a life
	Mesh* life = Object2D::CreateBullet("life", origin, 10, glm::vec3(1, 1, 0), true);
	AddMeshToList(life);
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolutionX, resolutionY);
}

void Laborator3::Update(float deltaTimeSeconds)
{
	if (numberOfLives == 0) {
		glm::vec3 origin = glm::vec3(0, 0, 0);
		Mesh* endOfTheGame = Object2D::CreateSquare("endOfTheGame", origin, resolutionY, glm::vec3(color, 0, 0), true);
		AddMeshToList(endOfTheGame);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Scale(1.78, 1);
		RenderMesh2D(meshes["endOfTheGame"], shaders["VertexColor"], modelMatrix);

		color += 1 * (deltaTimeSeconds / 2);
	}
	else {
		// updatez nava principala - pozitia si unghiul
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(translateX, translateY);
		modelMatrix *= Transform2D::Rotate(angularStep);
		RenderMesh2D(meshes["ship"], shaders["VertexColor"], modelMatrix);


		// updatez gloantele - doar pozitia, unghiul ramane acelasi
		for (int i = 0; i < bullets.size(); i++) {
			bullet bullet = bullets[i];

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(bullet.x, bullet.y);
			modelMatrix *= Transform2D::Rotate(bullet.angle);
			RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

			bullets[i].x += 10 * cos(bullet.angle);
			bullets[i].y += 10 * sin(bullet.angle);

			if (bullets[i].x > resolutionX || bullets[i].x < 0 || bullets[i].y > resolutionY || bullets[i].y < 0)
				bullets.erase(bullets.begin() + i);
		}


		// updatez navele inamice - pozitia si unghiul
		for (int i = 0; i < ships.size(); i++) {
			ship ship = ships[i];
			float angle = computeAngle(ship.x, ship.y, translateX, translateY);

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(ship.x, ship.y);
			modelMatrix *= Transform2D::Rotate(angle);
			modelMatrix *= Transform2D::Scale(ship.scale, ship.scale);
			if (ship.scale > 0.5 && ship.scale < 1) {
				float scaleShip = deltaTimeSeconds / 0.25;
				ships[i].scale -= 0.5 * scaleShip;
			}
			RenderMesh2D(meshes[ship.name], shaders["VertexColor"], modelMatrix);

			ships[i].x -= ship.speed * cos(angle);
			ships[i].y -= ship.speed * sin(angle);
		}

		//updatez vietile pe ecran
		for (int i = 0; i < numberOfLives; i++) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(1150 + i * 40, 650);
			modelMatrix *= Transform2D::Rotate(M_PI / 2);
			RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
		}

		// verific coliziunile
		// de fiecare data cand gasesc o coliziune nu mai caut alta pana la urmatorul update()
		bool collision = false;
		for (int i = 0; i < ships.size(); i++) {
			float distance;

			// verifica coliziunea navei principale cu navele inamice si distruge nava inamica daca s-a gasit coliziune
			distance = computeDistance(ships[i].x, ships[i].y, translateX, translateY);
			if (distance < 30) {
				if (numberOfLives > 0)
					numberOfLives--;
				ships.erase(ships.begin() + i);
				break;
			}
			// verifica coliziunea navelor inamice cu gloantele
			if (!collision)
				for (int j = 0; j < bullets.size(); j++) {
					distance = computeDistance(ships[i].x, ships[i].y, bullets[j].x, bullets[j].y);
					if (distance < 35) {
						bullets.erase(bullets.begin() + j);

						if (ships[i].name.compare("enemyShip0") == 0 || ships[i].name.compare("enemyShip2") == 0) {
							ships.erase(ships.begin() + i);
							score++;
							cout << "Score: " << score << endl;
						}
						else if (ships[i].name.compare("enemyShip1") == 0) {
							ships[i].speed *= 2;
							ships[i].name = "enemyShip2";
							ships[i].time = Engine::GetElapsedTime();
							ships[i].scale = 0.99;
						}

						collision = true;
						break;
					}
				}
			if (collision)
				break;
		}


		// generez navele inamice
		if (Engine::GetElapsedTime() - lastSpawnTime > spawnTime) {
			//la fiecare 6s scad timpul de generare al navelor cu 0.1s; timpul va scadea pana cand ajunge la 0.5s
			if (spawnTime > 0.5 && Engine::GetElapsedTime() > timeElapsed) {
				spawnTime -= 0.1;
				timeElapsed += 6;
			}
			lastSpawnTime = Engine::GetElapsedTime();

			ship ship;
			int radius = 400;
			int minX = translateX - radius;
			int maxX = translateX + radius;
			int x = rand() % (maxX - minX + 1) + minX;
			int y;
			int radical = sqrt(radius * radius - (x - translateX) * (x - translateX));
			int random = rand() % 2;
			if (random)
				y = translateY + radical;
			else y = translateY - radical;

			ship.x = x;
			ship.y = y;

			ship.speed = (rand() % (int) (5 - 0.5 + 1)) + 0.5;
			ship.time = Engine::GetElapsedTime();
			ship.scale = 1;

			int randomShip = rand() % 2;
			ship.name = "enemyShip" + to_string(randomShip);

			ships.push_back(ship);
		}
	}
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		translateY += 4;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		translateY -= 4;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		translateX += 4;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		translateX -= 4;
	}
}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	mouseY = resolutionY - mouseY;
	angularStep = computeAngle(mouseX, mouseY, translateX, translateY);
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == 1) {
		bullet bullet;

		bullet.x = translateX;
		bullet.y = translateY;
		bullet.angle = angularStep;

		bullets.push_back(bullet);
	}
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
