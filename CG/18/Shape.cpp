#include "Shape.h"
#include <fstream>
#include <sstream>

Shape::Shape()
{
	std::cout << "Shape()" << '\n';;
	shapeAngle = 0.f;
	orbitRadius = 0.f;
	orbitSpeed = 0.f;
	rotateY = false;
	rotateX = false;
}

Shape::Shape(float r,float degree)
{
	std::cout << "Shape(float r, float degree)" << '\n';
	shapeAngle = 0.f;
	orbitRadius = r;
	orbitSpeed = 0.f;
	deg.z = degree;
	rotateY = false;
	rotateX = false;
	orbitVertex = nullptr;
}

Shape::Shape(const Shape& other)
{
	this->pos = other.pos;
	this->deg = other.deg;
	this->rot = other.rot;
	this->rotateY = other.rotateY;
	this->rotateX = other.rotateX;
	orbitVertex = nullptr;
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
	delete orbitVertex;
}

void Shape::clearBuffer()
{
	delete shapeVertex;
	delete orbitVertex;
}

void Shape::initVerts(float radius, const Position& color)
{	
	/*std::vector<glm::vec3> VAO = readOBJ("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\sphere.obj", color);*/
	std::vector<float> VAO;
	std::vector<unsigned int> VBO;
	
	std::ifstream in{ "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\sphere.obj" };

	std::vector<glm::vec3>vertex;
	std::vector<glm::vec3>vn;

	while (in) {
		std::string line;
		std::getline(in, line);
		std::stringstream ss{ line };
		std::string str;
		ss >> str;
		if (str == "v") {
			std::vector<float> v;

			for (int i = 0; i < 3; ++i) {
				std::string subStr;
				ss >> subStr;
				v.push_back(std::stof(subStr));
			}

			vertex.push_back(glm::vec3(v[0], v[1], v[2]));
		}

		else if (str == "vn") {
			std::vector<float> n;

			for (int i = 0; i < 3; ++i) {
				std::string subStr;
				ss >> subStr;
				n.push_back(std::stof(subStr));
			}

			vn.push_back(glm::vec3(n[0], n[1], n[2]));
		}
		else if (str == "f") {
			std::vector<unsigned int> temp;
			for (int i = 0; i < 3; ++i) {
				std::string substr;
				ss >> substr;
				std::stringstream subss{ substr };
				std::string vIdx;

				std::getline(subss, vIdx, '/');
				temp.push_back(std::stoi(vIdx) - 1);
				VBO.push_back(std::stoi(vIdx) - 1);
				std::cout << vIdx << " ";
				std::getline(subss, vIdx, '/'); 
				std::getline(subss, vIdx, '/');
			}
			std::cout << '\n';
			/*for (int i = 2; i >= 0; --i) {
				VBO.push_back(temp[i]);
			}*/
		}
	}

	for (int i = 0; i < vertex.size(); ++i) {
		VAO.push_back(vertex[i].x);
		VAO.push_back(vertex[i].y);
		VAO.push_back(vertex[i].z);
		VAO.push_back(color.x);
		VAO.push_back(color.y);
		VAO.push_back(color.z);
		VAO.push_back(vn[i].x);
		VAO.push_back(vn[i].y);
		VAO.push_back(vn[i].z);
	}

	std::cout << VAO.size() / 9 << " Vertices Exists." << std::endl;

	//const int LATITUDE_SEGMENTS = 20;  // 위도 분할 개수
	//const int LONGITUDE_SEGMENTS = 20;  // 경도 분할 개수

	//// 정점 데이터 생성 (위도와 경도를 따라 분할)
	//for (int i = 0; i <= LATITUDE_SEGMENTS; ++i) {
	//	float theta = i * M_PI / LATITUDE_SEGMENTS;  // 위도 각도
	//	float sinTheta = sin(theta);
	//	float cosTheta = cos(theta);

	//	for (int j = 0; j <= LONGITUDE_SEGMENTS; ++j) {
	//		float phi = j * 2.0f * M_PI / LONGITUDE_SEGMENTS;  // 경도 각도
	//		float x = cos(phi) * sinTheta;
	//		float y = cosTheta;
	//		float z = sin(phi) * sinTheta;
	//			
	//		// 정점 (x, y, z)
	//		VAO.push_back(radius * x);
	//		VAO.push_back(radius * y);
	//		VAO.push_back(radius * z);

	//		if (y == 0.5f) {
	//			VAO.push_back(1.0f);
	//			VAO.push_back(1.0f);
	//			VAO.push_back(1.0f);
	//		}
	//		else {
	//			VAO.push_back(color.x);
	//			VAO.push_back(color.y);
	//			VAO.push_back(color.z);
	//		}
	//	}
	//}

	//// 인덱스 데이터 생성 (삼각형들로 구성)
	//for (int i = 0; i < LATITUDE_SEGMENTS; ++i) {
	//	for (int j = 0; j < LONGITUDE_SEGMENTS; ++j) {
	//		int first = (i * (LONGITUDE_SEGMENTS + 1)) + j;
	//		int second = first + LONGITUDE_SEGMENTS + 1;

	//		// 삼각형 1
	//		VBO.push_back(first);
	//		VBO.push_back(second);
	//		VBO.push_back(first + 1);

	//		// 삼각형 2
	//		VBO.push_back(second);
	//		VBO.push_back(second + 1);
	//		VBO.push_back(first + 1);
	//	}
	//}

	//shapeVertex = new Vertex(data);
	shapeVertex = new Vertex(VAO, VBO);
}

void Shape::setActive(Shader* shader)
{
	shapeVertex->setActive();
}

void  Shape::setRotateY() {
	rotateY = !rotateY;
}

void  Shape::setRotateZ()
{
	rotateX = !rotateX;
}

void Shape::setMoveX(int st)
{
	moveX = st;
}

void Shape::setMoveY(int st)
{
	moveY = st;
}

void Shape::setMoveZ(int st)
{
	moveZ = st;
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

void Shape::Reset()
{
	pos = rot = deg = {};
}

void Shape::Update(const Position& center)
{
	if (moveX) {
		if (moveX == 1)
			dir.x += 0.01f;

		else
			dir.x -= 0.01f;
	}

	if (moveY) {
		if (moveY == 1)
			dir.y += 0.01f;

		else
			dir.y -= 0.01f;
	}

	if (moveZ) {
		if (moveZ == 1)
			dir.z += 0.01f;

		else
			dir.z -= 0.01f;
	}

	shapeAngle += orbitSpeed;
	pos.x = orbitRadius * cos(shapeAngle) + dir.x;
	pos.y = dir.y;
	pos.z = orbitRadius * sin(shapeAngle) + dir.z;
	//pos += dir;

	if (rotateY) {
		deg.y += 5.f;
	}

	if (rotateX) {
		deg.x += 5.f;
	}
}

void Shape::Draw(GLuint shaderProgram, const Position& center)
{
}

std::vector<glm::vec3> Shape::readOBJ(std::string fileName, const Position& color)
{
	std::ifstream in{ fileName };
	if (!in) {
		std::cout << fileName << " file read failed\n";
		exit(1);
	}

	// c++ stream --> input output을 해주는 흐름?

	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> vcolor;
	std::vector<glm::vec3> normal;
	std::vector<glm::ivec3> vindex;
	std::vector<glm::ivec3> nindex;
	while (in) {
		std::string line;
		std::getline(in, line);
		std::stringstream ss{ line };
		std::string str;
		ss >> str;
		if (str == "v") {
			glm::vec3 v;
			for (int i = 0; i < 3; ++i) {
				std::string subStr;
				ss >> subStr;
				v[i] = std::stof(subStr);
			}
			vertex.push_back(v);
			vcolor.push_back(glm::vec3(color.x, color.y, color.z));
		}
		else if (str == "vn") {
			glm::vec3 n;
			for (int i = 0; i < 3; ++i) {
				std::string subStr;
				ss >> subStr;
				n[i] = std::stof(subStr);
			}
			normal.push_back(n);
		}
		else if (str == "f") {
			glm::ivec3 fv;
			glm::ivec3 fn;
			for (int i = 0; i < 3; ++i) {
				std::string substr;
				ss >> substr;
				std::stringstream subss{ substr };
				std::string vIdx;
				std::getline(subss, vIdx, '/');
				fv[i] = std::stoi(vIdx) - 1;
				std::getline(subss, vIdx, '/');
				// 텍스처 건너뛰기
				std::getline(subss, vIdx, '/');
				fn[i] = std::stoi(vIdx) - 1;
			}
			vindex.push_back(fv);
			nindex.push_back(fn);
		}
	}

	std::vector<glm::vec3> data;
	for (int i = 0; i < vindex.size(); ++i) {
		std::cout << vertex[vindex[i][0]].x << " " << vertex[vindex[i][0]].y << " " << vertex[vindex[i][0]].z << '\n';
		std::cout << vertex[vindex[i][1]].x << " " << vertex[vindex[i][1]].y << " " << vertex[vindex[i][1]].z << '\n';
		std::cout << vertex[vindex[i][2]].x << " " << vertex[vindex[i][2]].y << " " << vertex[vindex[i][2]].z << '\n';
		data.push_back(vertex[vindex[i][0]]);
		data.push_back(vcolor[vindex[i][0]]);
		data.push_back(normal[nindex[i][0]]);
		data.push_back(vertex[vindex[i][1]]);
		data.push_back(vcolor[vindex[i][1]]);
		data.push_back(normal[nindex[i][1]]);
		data.push_back(vertex[vindex[i][2]]);
		data.push_back(vcolor[vindex[i][2]]);
		data.push_back(normal[nindex[i][2]]);
	}

	std::cout << fileName << " File Read, " << data.size() / 3 << " Vertices Exists." << std::endl;
	return data;
}