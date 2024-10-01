#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WH 0.1f

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd(0.0f, 1.0f);	//	�Ǽ� ���� ���� (float or double)

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

int maxSize{};

struct Rect {
	GLfloat x1, y1, x2, y2;
};

class RectClass {
public:
	RectClass() {
		std::uniform_real_distribution<float> hani(-1.0f, 0.9f);

		float cX = hani(dre);
		float cY = hani(dre);

		rect = { cX, cY, cX + WH, cY + WH };
		draw = true;
		randomColors();
	}

	bool isClicked(bool rectangle, float mx, float my) const {
		return (mx >= rect.x1 && mx <= rect.x2 && my >= rect.y1 && my <= rect.y2);
	}

	bool isDrawn() const {
		return draw;
	}

	bool isColliding(const RectClass& other) const {
		return (rect.x1 < other.rect.x2 && rect.x2 > other.rect.x1
			&& rect.y1 < other.rect.y2 && rect.y2 > other.rect.y1);
	}

	void drawRect() const {
		glColor3f(r, g, b);
		glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	}

	void randomColors() {
		r = urd(dre);
		g = urd(dre);
		b = urd(dre);
	}

	const Rect& getRect() const {
		return rect;
	}

	void updateState(bool state) {
		draw = state;
	}

	void colorRGB(GLclampf& cR, GLclampf& cG, GLclampf& cB) {
		cR = r;
		cG = g;
		cB = b;
	}

protected:
	Rect rect;
	GLclampf r, g, b;

private:
	bool draw;
};

class EraserRect : public RectClass {
public:
	EraserRect() {
		std::uniform_real_distribution<float> hani(-1.0f, 0.8f);

		float cX = hani(dre);
		float cY = hani(dre);

		rect = { cX, cY, cX + 2 * WH, cY + 2 * WH };
		randomColors();
	}

	EraserRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
		rect = { x1, y1, x2, y2 };
		r = g = b = 0.0f;
	}

	void eraseRects(RectClass& other) {
		rect.x1 -= 0.01118f;
		rect.y1 -= 0.01118f;
		rect.x2 += 0.01118f;
		rect.y2 += 0.01118f;

		other.colorRGB(r, g, b);
		other.updateState(false);
	}

	void moveRect(float dx, float dy) {
		rect.x1 += dx;
		rect.y1 += dy;
		rect.x2 += dx;
		rect.y2 += dy;
	}
};

RectClass* rc;
EraserRect* eraser;

void Mouse(int button, int state, int x, int y);

void Motion(int x, int y);

void KeyBoard(unsigned char key, int x, int y);

void transCoord(int wx, int wy, float& ox, float& oy) {
	ox = 2.0f * wx / WINDOW_WIDTH - 1.0f;
	oy = 1.0f - 2.0f * wy / WINDOW_HEIGHT;
}

bool timer = false;
bool left_button = false;
//int selected;
int oldX, oldY;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ - ���� ���� ȣ��!
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ���� - ���� ���۸� | RGBD ���
	glutInitWindowPosition(300, 300); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("1-5"); // ������ ���� (������ �̸�)

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
	glutMotionFunc(Motion);
	glutKeyboardFunc(KeyBoard);
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(204 / 255.0f, 1.0f, 229 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	if (rc != nullptr) {
		for (int i = 0; i < maxSize; ++i) {
			if (rc[i].isDrawn())
				rc[i].drawRect();
		}
	}

	if (eraser != nullptr)
		eraser->drawRect();

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

//	���콺 ���콺 ���콺
void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {	//	Ŭ��
		float openglX, openglY;

		transCoord(x, y, openglX, openglY);		//	������ ��ǥ�� OpenGL ��ǥ�� ��ȯ

		eraser = new EraserRect(openglX - WH, openglY - WH, openglX + WH, openglY + WH);
		oldX = x;
		oldY = y;
		left_button = true;

		std::cout << "On" << '\n';
	}

	else if (state == GLUT_UP) {
		delete eraser;
		eraser = nullptr;
		left_button = false;

		std::cout << "Off" << '\n';
	}

	glutPostRedisplay();
}

//	��� ��� ���
void Motion(int x, int y)
{
	if (left_button) {
		float ox, oy;
		float nx, ny;

		transCoord(oldX, oldY, ox, oy);
		transCoord(x, y, nx, ny);

		eraser->moveRect(nx - ox, ny - oy);

		//	��ǥ ������Ʈ
		oldX = x;
		oldY = y;

		//	�浹 üũ - ����
		for (int i = maxSize - 1; i >= 0; --i) {
			if (rc[i].isDrawn() && eraser->isColliding(rc[i])) {
				eraser->eraseRects(rc[i]);
				std::cout << i << " Erased" << '\n';
				break;
			}
		}

		glutPostRedisplay();
	}
}

void KeyBoard(unsigned char key, int x, int y)
{
	if (key == 'r') {
		std::uniform_int_distribution<> uid(20, 40);

		maxSize = uid(dre);
		std::cout << "Number of Shapes: " << maxSize << '\n';

		if(rc!=nullptr)
			delete[] rc;

		rc = new RectClass[maxSize];
		glutPostRedisplay();
	}

	else if (key == 'q') {
		delete[] rc;
		glutLeaveMainLoop();
	}
}