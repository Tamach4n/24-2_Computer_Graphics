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

Shape::Shape(const Shape& other)
{
	this->pos = other.pos;
	this->deg = other.deg;
	this->rot = other.rot;
}

Shape& Shape::operator=(const Shape& other)
{
	/*if (this != &other) {
		delete shapeVertex;
		pos = other.pos;
		deg = other.deg;
		dir = other.dir;
		rotateY = other.rotateY;
		rotateZ = other.rotateZ;
	}*/

	return *this;
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
	movePos = 0.f;
	isMovePosiX = false;
	isMoveNegaX = false;
	topRotateAngle = 0.f;
	isTopRotatePosiY = false;
	isTopRotateNegaY = false;
	barrelRotateAngle = 0.f;
	isBarrelRotatePosiY = false;
	isBarrelRotateNegaY = false;
	barrelMovePos = 0.f;
	correctionBarrel = 0.f;
	wait = nullptr;
	isCombined = false;
	isBarrelmovePosiX = false;
	isBarrelmoveNegaX = false;
	armRotateAngle = 0.f;
	correctionArm = 0.f;
	isArmRotatePosiZ = false;
	isArmRotateNegaZ = false;
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

void Shape::initPlatBuffer()
{
	float VAO[] = {
		-2.f, 0.f,  2.f,	1.f, 0.f, 0.f,
		 2.f, 0.f,  2.f,	0.f, 1.f, 0.f,
		-2.f, 0.f, -2.f,	0.f, 0.f, 1.f,
		 2.f, 0.f, -2.f,	1.f, 1.f, 0.f
	};

	unsigned int VBO[] = {
		0, 1, 2,
		2, 3, 1
	};

	shapeVertex = new Vertex(VAO, 4, VBO, 6);
}

void Shape::setActive()
{
	shapeVertex->setActive();
}

void Shape::setPos(float x, float y, float z)
{
	pos = { x,y,z };
}

void Shape::setRotation(float x, float y, float z)
{
	//rot = { x,y,z };
}

void Shape::setDirection(float x, float y, float z)
{
	rot = { x,y,z };
}

void Shape::setMove(float x, float y, float z)
{
	dir.x = x;
	dir.y = y;
	dir.z = z;
}

void Shape::setMoveX(int i)
{
	if (i > 0) {
		isMovePosiX = !isMovePosiX;
		isMoveNegaX = false;
	}

	else if (i < 0) {
		isMoveNegaX = !isMoveNegaX;
		isMovePosiX = false;
	}

	else {	//	i == 0
		isMovePosiX = false;
		isMoveNegaX = false;
	}
}

void Shape::setRotateTop(int i)
{
	if (i > 0) {
		isTopRotatePosiY = !isTopRotatePosiY;
		isTopRotateNegaY = false;
	}

	else if (i < 0) {
		isTopRotateNegaY = !isTopRotateNegaY;
		isTopRotatePosiY = false;
	}

	else {
		isTopRotatePosiY = false;
		isTopRotateNegaY = false;
	}
}

void Shape::setRotateBarrel(int i)
{
	if (i > 0) {
		isBarrelRotatePosiY = !isBarrelRotatePosiY;
		isBarrelRotateNegaY = false;
	}

	else if (i < 0) {
		isBarrelRotateNegaY = !isBarrelRotateNegaY;
		isBarrelRotatePosiY = false;
	}

	else {
		isBarrelRotateNegaY = false;
		isBarrelRotatePosiY = false;
	}
}

void Shape::setMoveBarrel(int i)
{
	if (i > 0) {
		if (isBarrelRotatePosiY or isBarrelRotateNegaY)
			wait = &isBarrelmovePosiX;

		else {
			isBarrelmovePosiX = !isBarrelmovePosiX;
			isBarrelmoveNegaX = false;
		}
	}

	else if (i < 0) {
		if (isBarrelRotatePosiY or isBarrelRotateNegaY)
			wait = &isBarrelmoveNegaX;

		else {
			isBarrelmoveNegaX = !isBarrelmoveNegaX;
			isBarrelmovePosiX = false;
		}
	}

	else {
		isBarrelmoveNegaX = false;
		isBarrelmovePosiX = false;
	}
}

void Shape::setRotateArm(int i)
{
	if (i > 0) {
		isArmRotatePosiZ = !isArmRotatePosiZ;
		isArmRotateNegaZ = false;
	}

	else if (i < 0) {
		isArmRotateNegaZ = !isArmRotateNegaZ;
		isArmRotatePosiZ = false;
	}

	else {
		isArmRotatePosiZ = false;
		isArmRotateNegaZ = false;
	}
}

void Shape::Reset()
{
	pos = rot = deg = {};
}

void Shape::Update()
{
	if (isMovePosiX)
		movePos += 0.005f;

	else if (isMoveNegaX)
		movePos -= 0.005f;

	if (isTopRotatePosiY) {
		if (topRotateAngle < 90.f)
			topRotateAngle += 3.f;
	}

	else if (isTopRotateNegaY) {
		if (topRotateAngle > 0.f) {
			topRotateAngle -= 3.f;

		}
	}

	if (isBarrelRotatePosiY) {
		if (barrelRotateAngle < 90.f) {
			barrelRotateAngle += 3.f;
			correctionBarrel += 0.0019;
		}

		else if (wait) {
			*wait = !(*wait);
			wait = nullptr;
		}
	}

	else if (isBarrelRotateNegaY) {
		if (barrelRotateAngle > 0.f) {
			barrelRotateAngle -= 3.f;
			correctionBarrel -= 0.0019;
		}

		else if (wait) {
			*wait = !(*wait);
			wait = nullptr;
		}
	}

	if (isBarrelmovePosiX) {
		if (barrelMovePos < 0.195f) {
			barrelMovePos += 0.005f;
			isCombined = false;
		}

		else
			isCombined = true;
	}

	else if (isBarrelmoveNegaX) {
		if (barrelMovePos > 0.f) {
			barrelMovePos -= 0.005f;
			isCombined = false;
		}
	}

	if (isArmRotatePosiZ) {
		if (armRotateAngle > -90.f) {
			armRotateAngle -= 3.f;
			correctionArm += 0.0019;
		}
	}

	else if (isArmRotateNegaZ) {
		if (armRotateAngle < 0.f) {
			armRotateAngle += 3.f;
			correctionArm -= 0.0019;
		}
	}
}

void Shape::Draw(GLuint shaderProgram)
{
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
	}
}

void Shape::DrawPlat(GLuint shaderProgram)
{
	GLuint uLoc = glGetUniformLocation(shaderProgram, "modelTransform");

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));

		glm::mat4 SRT = T;
		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(SRT));
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
}
