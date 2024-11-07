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
	shapeVertex = new Vertex(VAO, 5, VBO, 18);
}

void Pyramid::Update()
{
	if (rotateY)
		yDeg += yDir;

	if (animeSideFaces) {
		if (isSidesOpened) {
			if (angle[0] > 0.f) {
				for (int i = 0; i < 4; ++i) {
					angle[i] -= 5.f;
				}
			}

			else {
				for (int i = 0; i < 4; ++i) {
					angle[i] = 0.f;
				}

				isSidesOpened = false;
				animeSideFaces = false;
			}
		}

		else {
			if (angle[0] < 233.14f) {
				for (int i = 0; i < 4; ++i) {
					angle[i] += 5.f;
				}
			}

			else {
				for (int i = 0; i < 4; ++i) {
					angle[i] = 233.14f;
				}

				isSidesOpened = true;
				animeSideFaces = false;
			}
		}
	}

	if (animeOnceFace) {
		//	접힘
		if (isSidesOpened) {
			if (curSide >= 0) {
				if (angle[curSide] > 0.f)
					angle[curSide] -= 5.f;

				//	다 접히면 각 초기화하고 다음 면으로
				else {
					--curSide;
					std::cout << "curSide: " << curSide << '\n';
				}
			}

			//	다 접힘
			else {
				curSide = 0;
				animeOnceFace = false;
				isSidesOpened = false;
				std::cout << "끝" << '\n';
			}
		}

		//	열림
		else {
			if (curSide < 4) {
				if (angle[curSide] < 116.57f)
					angle[curSide] += 5.f;

				//	다 펼쳐지면 각 초기화하고 다음 면으로
				else {
					++curSide;
					std::cout << "curSide: " << curSide << '\n';
				}
			}

			//	다 열림
			else {
				curSide = 4;
				animeOnceFace = false;
				isSidesOpened = true;
				std::cout << "끝" << '\n';
			}
		}
		/*if (curSide < 4) {
			if (isSidesOpened) {
				if (angle[curSide] > 0.f)
					angle[curSide] -= 5.f;

				//	다 접히면 각 초기화하고 다음 면으로
				else {
					++curSide;
					std::cout << curSide << '\n';
				}
			}

			else {
				if (angle[curSide] < 116.57f)
					angle[curSide] += 5.f;

				//	다 펼쳐지면 각 초기화하고 다음 면으로
				else {
					++curSide;
					std::cout << curSide << '\n';
				}
			}
		}

		//	4면 다 끝난 경우
		else {
			curSide = 0;
			isSidesOpened = (isSidesOpened) ? false : true;
			animeOnceFace = false;
		}*/
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

		float curAngle;
		//	앞면
		{
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(0.f, 0.5f, -0.5f));
			glm::mat4 rot = glm::rotate(unitMat, glm::radians(angle[0]), glm::vec3(1.f, 0.f, 0.f));
			glm::mat4 ret = glm::translate(unitMat, glm::vec3(0.f, -0.5f, 0.5f));

			matrix = SRT * ret * rot * mov;
			
			drawFace(uLoc, matrix, 3, 0);
			matrix = SRT;
		}

		//	오른쪽면
		{
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(-0.5f, 0.5f, 0.f));
			glm::mat4 rot = glm::rotate(unitMat, glm::radians(angle[1]), glm::vec3(0.f, 0.f, -1.f));
			glm::mat4 ret = glm::translate(unitMat, glm::vec3(0.5f, -0.5f, 0.f));

			matrix = SRT * ret * rot * mov;

			drawFace(uLoc, matrix, 3, 3);
			matrix = SRT;
		}

		//	뒷면
		{
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(0.f, 0.5f, 0.5f));
			glm::mat4 rot = glm::rotate(unitMat, glm::radians(angle[2]), glm::vec3(-1.f, 0.f, 0.f));
			glm::mat4 ret = glm::translate(unitMat, glm::vec3(0.f, -0.5f, -0.5f));

			matrix = SRT * ret * rot * mov;

			drawFace(uLoc, matrix, 3, 6);
			matrix = SRT;
		}

		//	왼쪽면
		{
			glm::mat4 unitMat(1.f);
			glm::mat4 mov = glm::translate(unitMat, glm::vec3(0.5f, 0.5f, 0.f));
			glm::mat4 rot = glm::rotate(unitMat, glm::radians(angle[3]), glm::vec3(0.f, 0.f, 1.f));
			glm::mat4 ret = glm::translate(unitMat, glm::vec3(-0.5f, -0.5f, 0.f));

			matrix = SRT * ret * rot * mov;

			drawFace(uLoc, matrix, 3, 9);
			matrix = SRT;
		}

		//	밑면
		drawFace(uLoc, matrix, 6, 12);
	}
}

void Pyramid::drawFace(GLuint uLoc, glm::mat4 srt, GLsizei count, int start)
{
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(srt));
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(start * sizeof(unsigned int)));
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
}

void Pyramid::openFace()
{
}

void Pyramid::setAnimeMode(int mode)
{
	switch (mode) {
	case 1:
		if (!animeOnceFace) {
			animeSideFaces = true;
			isMoving(mode);
		}

		break;

	case 2:
		if (!animeSideFaces) {
			animeOnceFace = true;
			isMoving(mode);
		}

		break;

	default:
		break;
	}
}

void Pyramid::isMoving(int mode)
{
	if (angle[curSide] > 0.f && angle[curSide] < static_cast<float>(mode) * 116.57)
		isSidesOpened = !isSidesOpened;
}
