#pragma once
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

class MoveTriangle
{
	// 삼각형을 속성을 가지는 객체로 생각
private:
	float posX, posY;

	float moveSpeedX;
	float moveSpeedY;

	int lookIndex;	// 삼각형의 방향 (0~3 : 위, 아래, 왼, 오)

	int boundSignX;
	int boundSignY;

	int degree;
	int edge;
	float zz;
	bool moveYY;

public:
	MoveTriangle();

	void update(int mode);
	void draw(GLuint shaderID, GLuint* VAO) const;
	void setMode(int mode);
};

