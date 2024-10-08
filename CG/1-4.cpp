#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

#define WINDOW_WIDTH 800.f
#define WINDOW_HEIGHT 600.f
#define SZ 5
#define WH 0.2f

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd(0.0f, 1.0f);	//	�Ǽ� ���� ���� (float or double)

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

struct Rect {
	GLfloat x1, y1, x2, y2;
};

class RectClass {
public:
	RectClass(GLfloat x, GLfloat y) :x(x), y(y)
	{
		rect = { x - WH, y - WH, x + WH, y + WH };
		manu = rect;

		if (rect.x1 < -1.f) moveRect(-1.f - rect.x1, 0);
		if (rect.y1 < -1.f)	moveRect(0, -1.f - rect.y1);
		if (rect.x2 > 1.f)	moveRect(1.f - rect.x2, 0);
		if (rect.y2 > 1.f)	moveRect(0, 1.f - rect.y2);

		std::cout << rect.x1 << " " << rect.y1 << " " << rect.x2 << " " << rect.y2 << '\n';

		draw = true;
		randomColors();
	}

	int isColliding() const {
		if (rect.x1 <= -1.f || rect.x2 >= 1.f)
			return -1;

		else if (rect.y1 <= -1.f || rect.y2 >= 1.f)
			return 1;

		else return 0;
	}

	void drawRect() const {
		glColor3f(r, g, b);
		glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	}

	void setRect(GLfloat x, GLfloat y) {
		rect = { x - WH, y - WH, x + WH, y + WH };
	}

	void moveRect(float dx, float dy) {
		x += dx;
		y += dy;
		rect.x1 += dx;
		rect.y1 += dy;
		rect.x2 += dx;
		rect.y2 += dy;

		std::cout << "x: " << x << ", y: " << y << '\n';
	}

	void randomColors() {
		r = urd(dre);
		g = urd(dre);
		b = urd(dre);
	}

	void randomResize(GLfloat size) {
		std::uniform_real_distribution<float> urdSize(0.1f, 0.25f);

		float wh = urdSize(dre);

		rect = { x - wh, y - wh, x + wh, y + wh };
	}

	void setMove() {
		std::uniform_int_distribution<> uid_dir(0, 3);

		int dir = uid_dir(dre);

		if (dir == 0)
			dx = -dx;

		else if (dir == 2) {
			dx = -dx;
			dy = -dy;
		}

		else if (dir == 3)
			dy = -dy;
	}

	void checkColision() {
		if (isColliding() == -1)	//	�浹 üũ
			dx = -dx;

		if (isColliding() == 1)
			dy = -dy;
	}

	void moveDiagonal() {		
		moveRect(dx, dy);	//	�̵�
		checkColision();
	}

	void moveZigzag() {
		static bool phase = false;
		float tempY;

		if (!phase) {
			tempY = 0;
			phase = 1;
		}

		else {
			tempY = dy;
			phase = 0;
		}

		moveRect(dx, tempY);	//	�̵�
		checkColision();
	}

	void overWrite() {
		rect = manu;
	}

private:
	GLfloat x, y;
	Rect rect;
	Rect manu;
	GLclampf r, g, b;
	bool draw;

	float dx = 10.f / WINDOW_WIDTH;
	float dy = 10.f / WINDOW_HEIGHT;
};

RectClass* rc;

void Mouse(int button, int state, int x, int y);

void KeyBoard(unsigned char key, int x, int y);

void Timer(int key);

void transCoord(int wx, int wy, float& ox, float& oy) {
	ox = 2.0f * wx / WINDOW_WIDTH - 1.0f;
	oy = 1.0f - 2.0f * wy / WINDOW_HEIGHT;
	std::cout << "OX: " << ox << " OY: " << oy << '\n';
}

GLclampf r = 46 / 255.f, g = 46 / 255.f, b = 46 / 255.f;

bool t1{}, t2{}, t3{}, t4{};
int selected;
int oldX, oldY;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ - ���� ���� ȣ��!
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ���� - ���� ���۸� | RGBD ���
	glutInitWindowPosition(300, 300); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("1-3"); // ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";
	
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(KeyBoard);
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	if (rc != nullptr) {
		for (int i = 0; i < SZ; ++i) {
			rc[i].drawRect();
		}			
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

//	���콺 ���콺 ���콺
void Mouse(int button, int state, int x, int y)
{
	if (rc == nullptr && state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {	//	Ŭ��
		float openglX, openglY;

		transCoord(x, y, openglX, openglY);		//	������ ��ǥ�� OpenGL ��ǥ�� ��ȯ

		std::uniform_real_distribution<float> randX(openglX - 0.4f, openglX + 0.4f);
		std::uniform_real_distribution<float> randY(openglY - 0.4f, openglY + 0.4f);

		rc = new RectClass[SZ]({ randX(dre), randY(dre) }, 
			{ randX(dre), randY(dre) }, { randX(dre), randY(dre) }, 
			{ randX(dre), randY(dre) }, { randX(dre), randY(dre) });

		glutPostRedisplay();
	}
}

void KeyBoard(unsigned char key, int x, int y)
{
	if (rc == nullptr)	return;

	switch (key) {
	case '1':
		for (int i = 0; i < SZ; ++i)
			rc[i].setMove();

		t1 = !t1;
		glutTimerFunc(50, Timer, 1);
		break;

	case '2':
		for (int i = 0; i < SZ; ++i)
			rc[i].setMove();

		t2 = !t2;
		glutTimerFunc(50, Timer, 2);
		break;

	case '3':
		t3 = !t3;
		glutTimerFunc(200, Timer, 3);
		break;

	case '4':
		t4 = !t4;
		glutTimerFunc(200, Timer, 4);
		break;

	case 's':
		t1 = t2 = t3 = t4 = false;
		glutPostRedisplay();
		break;

	case 'm':
		for (int i = 0; i < SZ; ++i)
			rc[i].overWrite();

		glutPostRedisplay();
		break;

	case 'r':
		t1 = t2 = t3 = t4 = false;
		delete[] rc;
		rc = nullptr;
		glutPostRedisplay();
		break;

	case 'q':
		t1 = t2 = t3 = t4 = false;
		delete[] rc;
		glutLeaveMainLoop();
		break;
	}
}

void Timer(int key)
{
	if (t1 == 0 && t2 == 0 && t3 == 0 && t4 == 0)	return;		//	Ÿ�̸� Ȱ��ȭ ������ ��츸 ����

	switch (key) {
	case 1:	//	1
		for (int i = 0; i < SZ; ++i) 
			rc[i].moveDiagonal();
		
		break;

	case 2:
		for (int i = 0; i < SZ; ++i) 
			rc[i].moveZigzag();
		
		break;

	case 3:
		for (int i = 0; i < SZ; ++i) 
			rc[i].randomResize(0.05f);
		
		break;

	case 4:
		for (int i = 0; i < SZ; ++i) 
			rc[i].randomColors();
		
		break;
	}

	glutPostRedisplay();

	glutTimerFunc(60, Timer, key);
}