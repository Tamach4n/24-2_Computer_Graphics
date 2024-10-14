#pragma once
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

class MoveTriangle
{
	// �ﰢ���� �Ӽ��� ������ ��ü�� ����
private:
	float posX, posY;

	float moveSpeedX;
	float moveSpeedY;

	int lookIndex;	// �ﰢ���� ���� (0~3 : ��, �Ʒ�, ��, ��)

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

