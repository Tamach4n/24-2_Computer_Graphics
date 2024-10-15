#include "MoveTriangle.h"
#include <iostream>
#include <math.h>
#include <random>

MoveTriangle::MoveTriangle()
{
	// ->x, y�� ���� �ӵ��� ���� �Ǹ�, �׻� ���� ��θ� ���� �ǹǷ�, x, y�� ���� �������� �Ѵ�.
	moveSpeedX = (rand() % 5 + 1) * 0.002f;	// ���� ���ǵ�
	moveSpeedY = (rand() % 5 + 1) * 0.002f;	// ���� ���ǵ�

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
	case 1:		// ƨ���
	{		
		posX += moveSpeedX * boundSignX;
		posY += moveSpeedY * boundSignY;

		// �� �浹
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

	case 2: 	// �¿�� �������
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

	case 3: 	// �簢 �����̷�
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
	

	case 4: 	// �� �����̷�
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
	glBindVertexArray(VAO[lookIndex]);		// ������ 4���� ������ �ٶ󺸴� �ﰢ�� �������, �̰����� ����

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
