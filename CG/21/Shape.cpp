#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';
	init();
}

Shape::Shape(float r,float degree)
{
	std::cout << "Shape(float r, float degree)" << '\n';
	init();
}

Shape::~Shape()
{
	delete shapeVertex;
}

void Shape::clearBuffer()
{
	delete shapeVertex;
}

void Shape::init()
{
	clearBuffer();
}

void Shape::initBuffer()
{	
	float VAO[] = {
		-0.5f, -0.5f, -0.5f,   0.1f, 0.1f, 0.1f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f
	};

	unsigned int VBO[] = {
		// 앞면
		4, 5, 6, 6, 7, 4,
		// 뒷면
		0, 1, 2, 2, 3, 0,
		// 왼쪽 면
		0, 3, 7, 7, 4, 0,
		// 오른쪽 면
		1, 5, 6, 6, 2, 1,
		// 아래쪽 면
		0, 1, 5, 5, 4, 0,
		// 위쪽 면
		3, 2, 6, 6, 7, 3
	};

	shapeVertex = new Vertex(VAO, 8, VBO, 36);
}

void Shape::setActive(Shader* shader)
{
	shapeVertex->setActive();
}

void Shape::Reset()
{
}

void Shape::Update()
{
}

void Shape::Draw(GLuint shaderProgram)
{
	/*
	GLuint worldLoc = glGetUniformLocation(shaderProgram, "modelTransform");

	if (worldLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		const glm::mat4 unitMat(1.f);

		glm::mat4 moveAll = glm::translate(unitMat, glm::vec3(movePos, 0.f, 0.f));
		glm::mat4 rotateTopArm = glm::rotate(unitMat, glm::radians(topRotateAngle), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 finalMat(1.f);

		glm::mat4 Ty = glm::translate(unitMat, glm::vec3(0.f, 0.5f, 0.f));
		glm::mat4 S(1.f);

		glm::mat4 movLeft(1.f);
		glm::mat4 movRight(1.f);
		glm::mat4 moveOnMono(1.f);

		//	몸통
		{
			S = glm::scale(unitMat, glm::vec3(0.5f, 0.25f, 0.5f));

			finalMat = moveAll * S * Ty;
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
		}

		//	상체
		{
			S = glm::scale(unitMat, glm::vec3(0.25f, 0.125f, 0.25f));
			moveOnMono = glm::translate(unitMat, glm::vec3(0.f, 0.25f, 0.f));

			finalMat = moveAll * moveOnMono * rotateTopArm * S * Ty;
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
		}

		//	팔
		{
			S = glm::scale(unitMat, glm::vec3(0.0625f, 0.25f, 0.0625f));

			movLeft = glm::translate(unitMat, glm::vec3(-0.0625f, correctionArm, 0.f));
			movRight = glm::translate(unitMat, glm::vec3(0.0625f, correctionArm, 0.f));
			moveOnMono = glm::translate(unitMat, glm::vec3(0.f, 0.375f, 0.f));

			glm::mat4 rotateArmLeft = glm::rotate(unitMat, glm::radians(-armRotateAngle), glm::vec3(0.f, 0.f, 1.f));
			glm::mat4 rotateArmRight = glm::rotate(unitMat, glm::radians(armRotateAngle), glm::vec3(0.f, 0.f, 1.f));

			//	좌
			finalMat = moveAll * moveOnMono * rotateTopArm * movLeft * rotateArmLeft * S * Ty;
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

			//	우
			finalMat = moveAll * moveOnMono * rotateTopArm * movRight * rotateArmRight * S * Ty;
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
		}

		//	포신
		{
			Ty = glm::translate(unitMat, glm::vec3(0.f, 0.5f, 0.5f));	//	z도 이동한다?!
			S = glm::scale(unitMat, glm::vec3(0.0625f, 0.0625f, 0.25f));

			movLeft = glm::translate(unitMat, glm::vec3(-0.2f + barrelMovePos, 0.f, correctionBarrel));
			movRight = glm::translate(unitMat, glm::vec3(0.2f - barrelMovePos, 0.f, correctionBarrel));
			moveOnMono = glm::translate(unitMat, glm::vec3(0.f, 0.0625f, 0.25f));

			glm::mat4 rotateBarrelLeft = glm::rotate(unitMat, glm::radians(-barrelRotateAngle), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 rotateBarrelRight = glm::rotate(unitMat, glm::radians(barrelRotateAngle), glm::vec3(0.f, 1.f, 0.f));

			//	좌
			finalMat = moveAll * moveOnMono * movLeft * rotateBarrelLeft * S * Ty;
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

			if (isCombined)	return;

			//	우
			finalMat = moveAll * moveOnMono * movRight * rotateBarrelRight * S * Ty;
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
		}
	}*/
}

//	-----------------------------------------------------------------------

Robot::Robot()
{
	std::cout << "Robot::Robot()\n";
}

void Robot::init()
{
	armRotateLevel = 0;
	dArmAngle = 3.f;
	armAngle = 0.f;
	isJumping = false;
	jumpPeak = false;
	jumpPos = 0.f;
	groundYPos = 0.f;
	pos = glm::vec3(0.f);
	Angle = 90.f;
	dir = glm::vec3(0.f);
	speed = 0.01f;
}

void Robot::initBuffer()
{
	float VAO[] = {
		-0.5f, -0.5f, -0.5f,   0.48f, 0.58f, 0.78f,
		 0.5f, -0.5f, -0.5f,   0.48f, 0.58f, 0.78f,
		 0.5f,  0.5f, -0.5f,   0.48f, 0.58f, 0.78f,
		-0.5f,  0.5f, -0.5f,   0.48f, 0.58f, 0.78f,
							  
		-0.5f, -0.5f,  0.5f,   0.48f, 0.58f, 0.78f,
		 0.5f, -0.5f,  0.5f,   0.48f, 0.58f, 0.78f,
		 0.5f,  0.5f,  0.5f,   1.f, 0.58f, 0.78f,
		-0.5f,  0.5f,  0.5f,   0.48f, 0.58f, 0.78f
	};

	unsigned int VBO[] = {
		// 앞면
		4, 5, 6, 6, 7, 4,
		// 뒷면
		0, 1, 2, 2, 3, 0,
		// 왼쪽 면
		0, 3, 7, 7, 4, 0,
		// 오른쪽 면
		1, 5, 6, 6, 2, 1,
		// 아래쪽 면
		0, 1, 5, 5, 4, 0,
		// 위쪽 면
		3, 2, 6, 6, 7, 3
	};

	shapeVertex = new Vertex(VAO, 8, VBO, 36);
}

void Robot::setGroundPos(float y)
{
	groundYPos = y;
}

void Robot::setDir(Direction d)
{
	switch (d) {
	case Dir_None:
		dir = glm::vec3(0.f, 0.f, 0.f);
		break;

	case Dir_Left:
		dir = glm::vec3(-1.f, 0.f, 0.f);
		Angle = 270.f;
		break;

	case Dir_Right:
		dir = glm::vec3(1.f, 0.f, 0.f);
		Angle = 90.f;
		break;

	case Dir_Front:
		dir = glm::vec3(0.f, 0.f, 1.f);
		Angle = 0.f;
		break;

	case Dir_Back:
		dir = glm::vec3(0.f, 0.f, -1.f);
		Angle = 180.f;
		break;
	}
}

void Robot::adjSpeed(int i)
{
	if ((i > 0 && speed < 0.03f) or (i < 0 && speed > 0.001f))
		speed += 0.001f * i;

	std::cout << "Speed: " << speed << '\n';
}

void Robot::Update()
{
	pos += dir * speed;

	if (isJumping) {
		if (jumpPeak) {
			if (jumpPos > groundYPos)
				jumpPos -= 0.05f;

			else {
				jumpPos = 0.f;
				jumpPeak = false;
				isJumping = false;
				std::cout << "Jump: " << isJumping << '\n';
			}
		}

		else {
			if (jumpPos < 0.3f)
				jumpPos += 0.05f;

			else
				jumpPeak = true;
		}
	}
}

void Robot::Draw(GLuint shaderProgram)
{
	shapeVertex->setActive();

	GLuint worldLoc = glGetUniformLocation(shaderProgram, "modelTransform");

	if (worldLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 unit(1.f);

		glm::mat4 S = glm::scale(unit, glm::vec3(0.2f, 0.4f, 0.2f));
		glm::mat4 T = glm::translate(unit, pos);
		glm::mat4 onFloor = glm::translate(unit, glm::vec3(0.f, 0.2f + jumpPos, 0.f));
		glm::mat4 Look = glm::rotate(unit, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 result = onFloor * T * Look * S;

		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(result));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
	}
}

void Robot::DrawParts(GLuint shaderProgram)
{
}

//	------------------------------------------------------------------------
//	------------------------------------------------------------------------

Cbstacle::Cbstacle()
{
	std::cout << "Obstacle::Obstacle()\n";
}

void Cbstacle::init()
{
	std::uniform_real_distribution<float> urd(-0.7f, 0.7f);

	isSteppedOn = false;
	fumi = 0.f;
	pos = glm::vec3(urd(rd), 0.f, urd(rd));
}

void Cbstacle::initBuffer()
{
}

void Cbstacle::setState(bool st)
{
	isSteppedOn = st;
}

void Cbstacle::Update()
{
	if (isSteppedOn) {
		if (fumi > -0.2f)
			fumi -= 0.01f;
	}

	else {
		if (fumi < 0.f)
			fumi += 0.01f;
	}
}

void Cbstacle::Draw(GLuint shaderProgram)
{
}

void Cbstacle::DrawParts(GLuint shaderProgram)
{
}

//	------------------------------------------------------------------------
//	------------------------------------------------------------------------

Butai::Butai()
{
	std::cout << "Butai::Butai()\n";
	init();
	initBuffer();
}

void Butai::init()
{
	doorPos = 1.0f;
	isOpened = true;
	isOpening = false;
}

void Butai::initBuffer()
{
	float VAO[] = {
		// 앞면 (빨간색)
		-1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  // 왼쪽 아래
		 0.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  // 오른쪽 아래
		 0.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  // 오른쪽 위
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  // 왼쪽 위

		 0.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,

		// 뒷면 (초록색)
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,

		// 왼쪽 면 (파란색)
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		// 오른쪽 면 (노란색)
		 1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// 아래쪽 면 (하늘색)
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,

		// 위쪽 면 (보라색)
		-1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f
	};

	unsigned int VBO[] = {
		// 앞면
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		// 뒷면
		8, 9, 10, 10, 11, 8,
		// 왼쪽 면
		12, 13, 14, 14, 15, 12,
		// 오른쪽 면
		16, 17, 18, 18, 19, 16,
		// 아래쪽 면
		20, 21, 22, 22, 23, 20,
		// 위쪽 면
		24, 25, 26, 26, 27, 24
	};

	shapeVertex = new Vertex(VAO, 28, VBO, 42);
}

void Butai::setOpen()
{
	if (isOpening) {
		if (isOpened)
			isOpened = false;

		else
			isOpened = true;
	}

	else {
		std::cout << "Opening\n";
		isOpening = true;
	}
}

void Butai::Update()
{
	if (isOpening) {
		if (isOpened) {
			if (doorPos > 0.01f)
				doorPos -= 0.05f;

			else {
				std::cout << "Closed\n";
				isOpened = false;
				isOpening = false;
			}
		}

		else {
			if (doorPos < 1.0f)
				doorPos += 0.05f;

			else {
				std::cout << "Opened\n";
				isOpened = true;
				isOpening = false;
			}
		}
	}
}

void Butai::Draw(GLuint shaderProgram)
{
	shapeVertex->setActive();

	GLuint worldLoc = glGetUniformLocation(shaderProgram, "modelTransform");

	if (worldLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		//glm::mat4 m = glm::scale(glm::mat4(1.f), glm::vec3(1.f));
		glm::mat4 m = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(m));

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
		
		{
			m = glm::translate(glm::mat4(1.f), glm::vec3(-doorPos, 1.f, 0.f));
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(m));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

			m = glm::translate(glm::mat4(1.f), glm::vec3(doorPos, 1.f, 0.f));
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(m));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
		}
	}
}
