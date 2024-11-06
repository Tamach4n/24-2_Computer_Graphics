#include "Cube.h"

Cube::Cube() 
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

void Cube::initVerts()
{
	std::cout << "Cube::initVerts()" << '\n';

	float VAO[] = {
		-0.5f, -0.5f, -0.5f,   0.0f, 0.1f, 0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f
	};

	unsigned int VBO[] = {
		// ¾Õ¸é
		4, 5, 6, 6, 7, 4,
		// µÞ¸é
		0, 1, 2, 2, 3, 0,
		// ¿ÞÂÊ ¸é
		0, 3, 7, 7, 4, 0,
		// ¿À¸¥ÂÊ ¸é
		1, 5, 6, 6, 2, 1,
		// ¾Æ·¡ÂÊ ¸é
		0, 1, 5, 5, 4, 0,
		// À§ÂÊ ¸é
		3, 2, 6, 6, 7, 3
	};

	animeTopFace = false;
	animeFrontFace = false;
	animeSideFace = false;
	animeBackFace = false;

	isFrontOpened = false;
	isSideOpened = false;
	isBackOpened = false;

	topAngle = 0.f;
	frontAngle = 0.f;
	sideDis = 0.f;
	backAngle = 0.f;

	shapeVertex = new Vertex(VAO, 8, VBO, 36);
}

void Cube::Update()
{
	if (rotateY)
		yDeg += yDir;

	if (animeTopFace)
		topAngle += 5.f;

	if (animeFrontFace) {
		if (isFrontOpened) {
			if (frontAngle > 0.f)
				frontAngle -= 5.f;

			else {
				animeFrontFace = false;
				isFrontOpened = false;
			}
		}

		else {
			if (frontAngle < 90.f)
				frontAngle += 5.f;

			else {
				animeFrontFace = false;
				isFrontOpened = true;
			}
		}
	}

	if (animeSideFace) {
		if (isSideOpened) {
			if (sideDis > 0.f)
				sideDis -= 0.05f;

			else {
				animeSideFace = false;
				isSideOpened = false;
			}
		}

		else {
			if (sideDis < 1.f)
				sideDis += 0.05f;

			else {
				animeSideFace = false;
				isSideOpened = true;
			}
		}
	}

	if (animeBackFace) {
		if (isBackOpened) {
			if (backAngle > 0.f)
				backAngle -= 5.f;

			else {
				animeBackFace = false;
				isBackOpened = false;
			}
		}

		else {
			if (backAngle < 90.f)
				backAngle += 5.f;

			else {
				animeBackFace = false;
				isBackOpened = true;
			}
		}
	}
}

void Cube::Draw(GLuint shaderProgram)
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

		//	 À­¸é
		if (animeTopFace) {
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(0.f, -0.5f, 0.f));
			glm::mat4 rot = glm::rotate(unitMat, glm::radians(topAngle), glm::vec3(1.f, 0.f, 0.f));
			glm::mat4 ret = glm::translate(unitMat, glm::vec3(0.f, 0.5f, 0.f));

			matrix = SRT * ret * rot * mov;			
		}

		drawFace(uLoc, matrix, 6, 30);
		matrix = SRT;

		//	¾Õ¸é
		{
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(0.f, 0.5f, -0.5f));
			glm::mat4 rot = glm::rotate(unitMat, glm::radians(frontAngle), glm::vec3(1.f, 0.f, 0.f));
			glm::mat4 ret = glm::translate(unitMat, glm::vec3(0.f, -0.5f, 0.5f));

			matrix = SRT * ret * rot * mov;

			drawFace(uLoc, matrix, 6, 0);
			matrix = SRT;
		}

		//	¿·¸é
		{
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(0.f, sideDis, 0.f));

			matrix = SRT * mov;

			drawFace(uLoc, matrix, 6, 12);
			drawFace(uLoc, matrix, 6, 18);
			matrix = SRT;
		}

		//	µÞ¸é
		{
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(0.f, 0.5f, 0.5f));
			glm::mat4 rot = glm::rotate(unitMat, glm::radians(-backAngle), glm::vec3(1.f, 0.f, 0.f));
			glm::mat4 ret = glm::translate(unitMat, glm::vec3(0.f, -0.5f, -0.5f));

			matrix = SRT * ret * rot * mov;

			drawFace(uLoc, matrix, 6, 6);
			matrix = SRT;
		}

		//	¹Ø¸é
		drawFace(uLoc, matrix, 6, 24);
	}
}

void Cube::drawFace(GLuint uLoc, glm::mat4 srt, GLsizei count, int start)
{
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(srt));
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(start * sizeof(unsigned int)));
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
}

void Cube::openFace()
{
}

void Cube::setAnimeMode(int mode)
{
	std::cout << "Cube::setAnimeMode()" << '\n';
	switch (mode) {
	case 1:
		animeTopFace = !animeTopFace;

		if (!animeTopFace)
			topAngle = 0;

		break;

	case 2:
		animeFrontFace = !animeFrontFace;
		break;

	case 3:
		animeSideFace = !animeSideFace;
		break;

	case 4:
		animeBackFace = !animeBackFace;
		break;

	default:
		break;
	}
}