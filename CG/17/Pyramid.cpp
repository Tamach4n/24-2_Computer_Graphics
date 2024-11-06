#include "Pyramid.h"

Pyramid::Pyramid()
{
	xPos = yPos = zPos = 0.f;
	xDeg = 5.f;
	yDeg = 5.f;
	zDeg = 0.f;
	xRot = yRot = zRot = 0.f;
	xDir = zDir = 0.f;
	yDir = 5.f;
	isLine = false;
	rotateY = false;
}

void Pyramid::initVerts()
{
	std::cout << "Pyramid::initVerts()" << '\n';

	float VAO[] = {
		 0.0f,  0.5f,  0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 1.0f
	};

	unsigned int VBO[] = {
		0, 1, 2,
		0, 2, 4,
		0, 4, 3,
		0, 3, 1,
		1, 3, 4, 1, 4, 2
	};

	animeSideFaces = false;
	animeOnceFace = false;
	isSidesOpened = false;
	curSide = 0.f;
	angle = 0.f;

	shapeVertex = new Vertex(VAO, 5, VBO, 18);
}

void Pyramid::Update()
{
	if (rotateY)
		yDeg += yDir;

	if (animeSideFaces) {
		if (isSidesOpened) {
			if (angle > 0.f)
				angle -= 5.f;

			else {
				isSidesOpened = false;
				animeSideFaces = false;
			}
		}

		else {
			if (angle < 135.f)
				angle -= 5.f;

			else {
				isSidesOpened = false;
				animeSideFaces = true;
			}
		}
	}

	if (animeOnceFace) {
		//	면마다 변화
		if (curSide < 4) {
			if (isSidesOpened) {
				if (angle > 0.f)
					angle -= 5.f;

				//	다 접히면 각 초기화하고 다음 면으로
				else {
					angle = 135.f;
					++curSide;
					std::cout << curSide << '\n';
				}
			}

			else {
				if (angle < 135.f)
					angle += 5.f;

				//	다 펼쳐지면 각 초기화하고 다음 면으로
				else {
					angle = 0.f;
					++curSide;
					std::cout << curSide << '\n';
				}
			}
		}

		//	4면 다 끝난 경우
		else {
			curSide = 0;
			angle = (isSidesOpened) ? 135.f : 0.f;
			isSidesOpened = (isSidesOpened) ? true : false;
			animeOnceFace = false;
			std::cout << "끝" << '\n';
		}
	}
}

void Pyramid::Draw(GLuint shaderProgram)
{
	GLuint uLoc = glGetUniformLocation(shaderProgram, "modelTransform");

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(xDeg), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(yDeg), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Rz = glm::rotate(glm::mat4(1.f), glm::radians(zDeg), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(xPos, yPos, zPos));
		glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));

		glm::mat4 SRT = T * Rz * Ry * Rx * S;
		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(SRT));

		glm::mat4 matrix = SRT;

		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

		//	앞면
		{

		}

		//	오른쪽면
		{

		}

		//	뒷면
		{

		}

		//	왼쪽면
		{

		}

		//	밑면
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
	}
}

void Pyramid::drawFace(GLuint uLoc, glm::mat4 srt, GLsizei count, int start)
{
}

void Pyramid::openFace()
{
}

void Pyramid::setAnimeMode(int mode)
{
	switch (mode) {
	case 1:
		animeSideFaces = !animeSideFaces;
		break;

	case 2:
		animeOnceFace = !animeOnceFace;
		break;

	default:
		break;
	}
}
