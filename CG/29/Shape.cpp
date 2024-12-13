#include "Shape.h"
#include "Scene.h"
#include <fstream>
#include <sstream>

Shape::Shape(Scene* scene)
{
	std::cout << "Shape()" << '\n';
    Reset();
	mScene = scene;
}

void Shape::Init()
{

}

void Shape::readOBJ(const std::string& fileName, std::vector<float>& vao, std::vector<unsigned int>& vbo)
{
    std::ifstream file(fileName);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;

    std::vector<float> vertexData;
    std::unordered_map<std::string, unsigned int> uniqueVertices;
    unsigned int currentIndex = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            // Read vertex positions
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vn") {
            // Read vertex normals
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "vt") {
            // Read texture coordinates
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (prefix == "f") {
            // Read face data
            for (int i = 0; i < 3; ++i) { // OBJ typically defines triangles
                std::string vertex;
                ss >> vertex;

                if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
                    // Parse vertex/texCoord/normal indices
                    std::stringstream vertexStream(vertex);
                    std::string index;
                    unsigned int vIndex = 0, tIndex = 0, nIndex = 0;

                    std::getline(vertexStream, index, '/');
                    vIndex = std::stoi(index) - 1;

                    if (std::getline(vertexStream, index, '/') && !index.empty()) {
                        tIndex = std::stoi(index) - 1;
                    }

                    if (std::getline(vertexStream, index, '/') && !index.empty()) {
                        nIndex = std::stoi(index) - 1;
                    }

                    // Add unique vertex data
                    glm::vec3 position = positions[vIndex];
                    glm::vec2 texCoord = tIndex < texCoords.size() ? texCoords[tIndex] : glm::vec2(0.0f);
                    glm::vec3 normal = nIndex < normals.size() ? normals[nIndex] : glm::vec3(0.0f);

                    vertexData.push_back(position.x);
                    vertexData.push_back(position.y);
                    vertexData.push_back(position.z);
                    vertexData.push_back(normal.x);
                    vertexData.push_back(normal.y);
                    vertexData.push_back(normal.z);
                    vertexData.push_back(texCoord.x);
                    vertexData.push_back(texCoord.y);

                    uniqueVertices[vertex] = currentIndex++;
                }

                // Add the index for this vertex
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    file.close();

    vao = std::move(vertexData);    //  소유권 이동, 복사X -> 빠르고 효율적, 가독성 향상 -> 디버깅 도움
    vbo = std::move(indices);
}

void Shape::setActive()
{
	shapeVertex->setActive();
}

void Shape::setPosition(glm::vec3 p)
{
	pos = p;
}

void Shape::setRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

void Shape::setDirection(float x, float y, float z)
{
	dir.x = x;
	dir.y = y;
	dir.z = z;
}

void Shape::Reset()
{
    pos = deg = dir = rot = glm::vec3(0.f);
    mScale = 1.f;
}

void Shape::Update()
{
	pos += dir;
	deg += rot;

	/*if (mOrbit)
		setOrbitPos(pos);*/
}

void Shape::Draw(const Shader* shader)
{
	std::cout << "Shape::Draw(const Shader* shader)\n";
}
