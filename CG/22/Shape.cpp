#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';
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

void Shape::Draw(const Shader* shaderProgram)
{
}

//	-----------------------------------------------------------------------

Robot::Robot()
{
	std::cout << "Robot::Robot()\n";
}

void Robot::init()
{
	armRotateLevel = 0;
	dArmAngle = 15.f;
	armAngle = 0.f;
	isJumping = false;
	jumpPeak = false;
	jumpPos = 0.f;
	groundYPos = 0.f;
	pos = glm::vec3(0.f);
	Angle = 0.f;
	dir = glm::vec3(0.f);
	speed = 0.01f;
	canMove = true;
	isSteppingOn = false;
	isMoving = false;
	Outside = false;
}

void Robot::initBuffer()
{	//	이건 그냥 비효율적
	float VAO[] = {
		-0.5f, -0.5f, -0.5f,   0.f, 0.f, 0.f,
		 0.5f, -0.5f, -0.5f,   0.f, 0.f, 0.f,
		 0.5f,  0.5f, -0.5f,   0.f, 0.f, 0.f,
		-0.5f,  0.5f, -0.5f,   0.f, 0.f, 0.f,

		-0.5f, -0.5f,  0.5f,   0.f, 0.f, 0.f,
		 0.5f, -0.5f,  0.5f,   0.f, 0.f, 0.f,
		 0.5f,  0.5f,  0.5f,   0.f, 0.f, 0.f,
		-0.5f,  0.5f,  0.5f,   0.f, 0.f, 0.f
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
		isMoving = false;
		armRotateLevel = 0;
		armAngle = 0.f;
		break;

	case Dir_Left:
		dir = glm::vec3(-1.f, 0.f, 0.f);
		Angle = 270.f;
		isMoving = true;
		break;

	case Dir_Right:
		dir = glm::vec3(1.f, 0.f, 0.f);
		Angle = 90.f;
		isMoving = true;
		break;

	case Dir_Front:
		dir = glm::vec3(0.f, 0.f, 1.f);
		Angle = 0.f;
		isMoving = true;
		break;

	case Dir_Back:
		dir = glm::vec3(0.f, 0.f, -1.f);
		Angle = 180.f;
		isMoving = true;
		break;
	}

}

void Robot::adjSpeed(int i)
{
	if ((i > 0 && speed < 0.03f) or (i < 0 && speed > 0.001f))
		speed += 0.001f * i;

	std::cout << "Speed: " << speed << '\n';
}

void Robot::steppingOn()
{
	pos.y -= 0.01f;
}

bool Robot::checkCollision(const class Obstacle* obs)
{
	//	어쨌든 동작하시잖아
	glm::vec3 oPos = obs->getPos();
	glm::vec3 newPos = pos + dir * speed;

	if (pow(oPos.x - newPos.x, 2) + pow(oPos.z - newPos.z, 2) <= 0.3f * 0.3f) {
		if (newPos.y <= oPos.y && !isSteppingOn) {
			std::cout << "cant move\n";
			this->canMove = false;
			return true;
		}
	}

	this->canMove = true;
	return false;
	/*glm::vec4 hbo = obs->getHitbox();
	glm::vec3 oPos = obs->getPos();
	glm::vec3 newPos = pos + dir * speed;

	if ((newPos.x + 0.1f > hbo.z) && (newPos.x - 0.1f < hbo.x) && (newPos.z + 0.1f > hbo.w) && (newPos.z - 0.1f < hbo.y)) {
		std::cout << "cant move\n";
		this->canMove = false;
		return true;
	}

	canMove = true;
	return false;*/
}

bool Robot::checkCollision(const Butai* butai)
{
	glm::vec4 bPos = butai->getArea();
	glm::vec3 newPos = pos + dir * speed;

	if (newPos.z + 0.1f > bPos.w) {
		if (butai->getIsOpened()) {
			std::cout << "Falling~\n";
			groundYPos = -10.f;
			freeDiveTime = std::chrono::system_clock::now();
			this->canMove = true;
			Outside = true;
			return false;
		}

		else {
			std::cout << "cant move\n";
			this->canMove = false;
			return true;
		}
	}

	else if (newPos.x - 0.1f < bPos.x || newPos.x + 0.1f > bPos.z || newPos.z - 0.1f < bPos.y) {
		std::cout << "cant move\n";
		this->canMove = false;
		return true;
	}

	this->canMove = this->canMove ? true : false;
	return false;
}

void Robot::Update()
{
	if (isMoving) {
		if (armRotateLevel == 1) {
			if (armAngle < 60.f)
				armAngle += dArmAngle;

			else
				armRotateLevel = 2;
		}

		else {
			if (armAngle > -60.f)
				armAngle -= dArmAngle;

			else
				armRotateLevel = 1;
		}
	}

	if (Outside) {
		auto now = std::chrono::system_clock::now();
		auto s = std::chrono::duration_cast<std::chrono::nanoseconds>(now - freeDiveTime);
		freeDiveTime = now;
		float d = (0.00000049 * s.count() * s.count()) / 2;
		pos.y -= d;	
		//std::cout << d << '\n';
	}

	std::cout << canMove << '\n';
	if (canMove) {
		pos += dir * speed;
	}
	
	if (isJumping) {
		if (jumpPeak) {
			if (jumpPos > groundYPos + 0.01f) {
				jumpPos -= 0.05f;
				pos.y -= 0.05f;
			}

			else {
				jumpPos = 0.f;
				jumpPeak = false;
				isJumping = false;
				std::cout << "Jump: " << isJumping << '\n';
			}
		}

		else {
			if (jumpPos < 0.3f) {
				jumpPos += 0.05f;
				pos.y += 0.05f;
			}

			else
				jumpPeak = true;
		}
	}

	else if (!Outside && pos.y > groundYPos) {
		pos.y -= 0.01f;
	}
}

void Robot::Draw(const Shader* shaderProgram)
{
	shapeVertex->setActive();

	glm::mat4 unit(1.f);

	glm::mat4 S;
	glm::mat4 T = glm::translate(unit, pos);
	glm::mat4 onFloor;
	glm::mat4 Look = glm::rotate(unit, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 result;

	//	코
	{
		S = glm::scale(unit, glm::vec3(0.02f, 0.02f, 0.02f));
		onFloor = glm::translate(unit, glm::vec3(0.f, 0.375f, 0.08f));
		result = T * Look * onFloor * S;
		shaderProgram->setUniformM("modelTransform", result);
		shaderProgram->setUniform3("uColor", 1.f, 0.f, 0.f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
	}
	//	머리
	{
		S = glm::scale(unit, glm::vec3(0.16f, 0.1f, 0.16f));
		onFloor = glm::translate(unit, glm::vec3(0.f, 0.35f, 0.f));
		result = T * Look * onFloor * S;
		shaderProgram->setUniformM("modelTransform", result);
		shaderProgram->setUniform3("uColor", 0.48f, 0.58f, 0.78f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
	}
	//	몸통
	{
		S = glm::scale(unit, glm::vec3(0.2f, 0.25f, 0.2f));
		onFloor = glm::translate(unit, glm::vec3(0.f, 0.175f, 0.f));
		result = T * Look * onFloor * S;
		shaderProgram->setUniformM("modelTransform", result);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
	}
	//	팔
	{
		S = glm::scale(unit, glm::vec3(0.05f, 0.15f, 0.05f));
		glm::mat4 t11 = glm::translate(unit, glm::vec3(0.f, -0.035f, 0.f));
		glm::mat4 rotateArmLeft = glm::rotate(unit, glm::radians(armAngle), glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 rotateArmRight = glm::rotate(unit, glm::radians(-armAngle), glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 t12 = glm::translate(unit, glm::vec3(0.f, 0.035f, 0.f));
		glm::mat4 t21 = glm::translate(unit, glm::vec3(-0.025f, 0.f, 0.f));
		glm::mat4 armLeft = glm::rotate(unit, glm::radians(30.f), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 armRight = glm::rotate(unit, glm::radians(-30.f), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 t22 = glm::translate(unit, glm::vec3(0.025f, 0.f, 0.f));
		onFloor = glm::translate(unit, glm::vec3(0.15f, 0.2f, 0.f));

		result = T * Look * onFloor * t12 * rotateArmLeft * t11 * t22 * armLeft * t21 * S;
		shaderProgram->setUniformM("modelTransform", result);
		shaderProgram->setUniform3("uColor", 0.f, 0.9f, 0.f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

		onFloor = glm::translate(unit, glm::vec3(-0.15f, 0.2f, 0.f));
		result = T * Look * onFloor * t12 * rotateArmRight * t11 * t22 * armRight * t21 * S;
		shaderProgram->setUniformM("modelTransform", result);
		shaderProgram->setUniform3("uColor", 0.f, 0.f, 0.9f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
	}
	//	다리
	{
		S = glm::scale(unit, glm::vec3(0.07f, 0.05f, 0.07f));
		glm::mat4 t1 = glm::translate(unit, glm::vec3(0.f, -0.025f, 0.f));
		glm::mat4 rotateLegLeft = glm::rotate(unit, glm::radians(-armAngle), glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 rotateLegRight = glm::rotate(unit, glm::radians(armAngle), glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 t2 = glm::translate(unit, glm::vec3(0.f, 0.025f, 0.f));
		onFloor = glm::translate(unit, glm::vec3(0.045f, 0.025f, 0.f));

		result = T * Look * onFloor * t2 * rotateLegLeft * t1 * S;
		shaderProgram->setUniformM("modelTransform", result);
		shaderProgram->setUniform3("uColor", 0.f, 0.9f, 0.f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

		onFloor = glm::translate(unit, glm::vec3(-0.045f, 0.025f, 0.f));
		result = T * Look * onFloor * t2 * rotateLegRight * t1 * S;
		shaderProgram->setUniformM("modelTransform", result);
		shaderProgram->setUniform3("uColor", 0.f, 0.f, 0.9f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
	}

	shaderProgram->setUniform3("uColor", 0.f, 0.f, 0.f);
}

void Robot::DrawParts(GLuint shaderProgram)
{
}

//	------------------------------------------------------------------------
//	------------------------------------------------------------------------

Obstacle::Obstacle()
{
	std::cout << "Obstacle::Obstacle()\n";
}

void Obstacle::init()
{
	std::uniform_real_distribution<float> urd(-0.7f, 0.7f);
	float x, z;

	while (1) {
		x = urd(rd);

		if (x < -0.2f || x > 0.2f)	break;
	}

	while (1) {
		z = urd(rd);

		if (z < -0.2f || z > 0.2f)	break;
	}

	isSteppedOn = false;
	pos = glm::vec3(x, 0.2f, z);

	initBuffer();
}

void Obstacle::initBuffer()
{
	float VAO[] = {
		-0.5f, -0.5f, -0.5f,   0.2f, 0.2f, 0.2f,
		 0.5f, -0.5f, -0.5f,   0.2f, 0.2f, 0.2f,
		 0.5f,  0.5f, -0.5f,   0.2f, 0.2f, 0.2f,
		-0.5f,  0.5f, -0.5f,   0.2f, 0.2f, 0.2f,

		-0.5f, -0.5f,  0.5f,   0.2f, 0.2f, 0.2f,
		 0.5f, -0.5f,  0.5f,   0.2f, 0.2f, 0.2f,
		 0.5f,  0.5f,  0.5f,   0.2f, 0.2f, 0.2f,
		-0.5f,  0.5f,  0.5f,   0.2f, 0.2f, 0.2f
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

void Obstacle::setState(bool st)
{
	isSteppedOn = st;
}

void Obstacle::Update()
{
	if (isSteppedOn) {
		if (pos.y > 0.0001f)
			pos.y -= 0.01f;
	}

	else {
		if (pos.y < 0.1999f)
			pos.y += 0.01f;
	}
}

void Obstacle::Draw(const Shader* shaderProgram)
{
	shapeVertex->setActive();

	glm::mat4 unit(1.f);

	glm::mat4 S = glm::scale(unit, glm::vec3(0.4f, 0.2f, 0.4f));
	glm::mat4 T = glm::translate(unit, pos);
	glm::mat4 onFloor = glm::translate(unit, glm::vec3(0.f, -0.1f, 0.f));
	glm::mat4 result = onFloor * T * S;

	shaderProgram->setUniformM("modelTransform", result);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
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
	doorPos = 0.0f;
	isOpened = false;
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

void Butai::Draw(const Shader* shaderProgram)
{
	shapeVertex->setActive();

	//glm::mat4 m = glm::scale(glm::mat4(1.f), glm::vec3(1.f));
	glm::mat4 m = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, 0.f));
	shaderProgram->setUniformM("modelTransform", m);

	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

	{
		m = glm::translate(glm::mat4(1.f), glm::vec3(-doorPos, 1.f, 0.f));
		shaderProgram->setUniformM("modelTransform", m);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

		m = glm::translate(glm::mat4(1.f), glm::vec3(doorPos, 1.f, 0.f));
		shaderProgram->setUniformM("modelTransform", m);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
	}
}
