#include "MoveTriangle.h"
#include <iostream>
#include <math.h>
#include <random>

MoveTriangle::MoveTriangle()
{
	// ->x, y로 같은 속도로 가게 되면, 항상 같은 경로를 가게 되므로, x, y를 각각 랜덤으로 한다.
	moveSpeedX = (rand() % 5 + 1) * 0.002f;	// 랜덤 스피드
	moveSpeedY = (rand() % 5 + 1) * 0.002f;	// 랜덤 스피드

	boundSignX = 0;		// -1 or 1
	boundSignY = 0;
	lookIndex = 0;

	degree = 0;
	edge = 0;
	zz = 0.f;
	moveYY = false;
}

void MoveTriangle::update(int mode)
{
	switch (mode) {
	case 1:		// 튕기기
	{		
		posX += moveSpeedX * boundSignX;
		posY += moveSpeedY * boundSignY;

		// 벽 충돌
		if (posX >= 0.8f) {
			boundSignX = -1;
			lookIndex = 2;
		}

		else if (posX <= -0.8f) {
			boundSignX = 1;
			lookIndex = 3;
		}

		if (posY >= 0.8f) {
			boundSignY = -1;
			lookIndex = 1;
		}

		else if (posY <= -0.8f) {
			boundSignY = 1;
			lookIndex = 0;
		}

		break;
	}

	case 2: 	// 좌우로 지그재그
	{
		posX += moveSpeedX * boundSignX;
		posY += moveSpeedY * boundSignY;

		if(moveYY)
			zz += abs(moveSpeedY * boundSignY);

		if (posX >= 0.8f) {
			moveYY = true;
			boundSignX = 0;
			boundSignY = -1;
			lookIndex = 2;
		}

		else if (posX <= -0.8f) {
			moveYY = true;
			boundSignX = 0;
			boundSignY = -1;
			lookIndex = 3;
		}

		if (zz >= 0.2f) {
			zz = boundSignY = 0;
			moveYY = false;

			if (posX >= 0.8f) {
				boundSignX = -1;
			}

			else if (posX <= -0.8f) {
				boundSignX = 1;
			}
		}

		if (posY <= -0.8f) {
			boundSignX = 0;
			boundSignY = 0;
			zz = 0.f;
			moveYY = false;
		}

		break;
	}

	case 3: 	// 사각 스파이럴
		posX += moveSpeedX * boundSignX;
		posY += moveSpeedY * boundSignY;

		if (zz >= 0.9f) {
			boundSignX = 0;
			boundSignY = 0;
		}

		else if (edge == 0 && posX - zz <= -0.8f) {
			boundSignX = 0;
			boundSignY = -1;
			lookIndex = 3;
			edge = 1;
			zz += 0.1;
		}

		else if (edge == 1 && posY - zz <= -0.8f) {
			boundSignX = 1;
			boundSignY = 0;
			lookIndex = 0;
			edge = 2;
			zz += 0.1;
		}

		else if (edge == 2 && posX + zz >= 0.8f) {
			boundSignX = 0;
			boundSignY = 1;
			lookIndex = 2;
			edge = 3;
			zz += 0.1;
		}

		else if (edge == 3 && posY + zz >= 0.8f) {
			boundSignX = -1;
			boundSignY = 0;
			lookIndex = 1;
			edge = 0;
			zz += 0.1;
		}

		std::cout << zz << '\n';

		break;
	

	case 4: 	// 원 스파이럴
		if (zz <= 0.8f) {
			posX += zz * cos(degree);
			posY += zz * sin(degree);

			zz += 0.005f;
			++degree;
		}

		break;
	}
}

void MoveTriangle::draw(GLuint shaderID, GLuint* VAO) const
{
	glBindVertexArray(VAO[lookIndex]);		// 씬에서 4가지 방향을 바라보는 삼각형 만들었고, 이곳에서 선택

	int uLoc = glGetUniformLocation(shaderID, "uPos");

	if (uLoc < 0)
		std::cerr << "Error: Not Found uPos in uniform\n";

	glUniform2f(uLoc, posX, posY);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MoveTriangle::setMode(int mode)
{
	degree = 0;
	zz = 0.f;
	edge = 0;
	moveYY = false;

	switch (mode) {
	case 1:
	{
		posX = posY = 0.f;

		switch (rand() % 4) {
		case 0:
			boundSignX = rand() % 2 * 2 - 1;		// -1 or 1
			boundSignY = 1;
			lookIndex = 0;
			break;

		case 1:
			boundSignX = rand() % 2 * 2 - 1;
			boundSignY = -1;
			lookIndex = 1;
			break;

		case 2:
			boundSignX = -1;
			boundSignY = rand() % 2 * 2 - 1;
			lookIndex = 2;
			break;

		case 3:
			boundSignX = 1;
			boundSignY = rand() % 2 * 2 - 1;
			lookIndex = 3;
			break;
		}

		break;
	}

	case 2:
		posY = 0.8f;
		boundSignX = rand() % 2 * 2 - 1;
		boundSignY = 0;
		break;

	case 3:
		posY = 0.8f;
		boundSignX = -1;
		boundSignY = 0;
		lookIndex = 1;
		break;

	case 4:
		posX = posY = 0.f;
		zz = (rand() % 5) / static_cast<float>(100);
		break;

	default:
		break;
	}
}
