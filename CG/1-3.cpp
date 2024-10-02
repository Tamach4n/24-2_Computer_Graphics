#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

#define WINDOW_WIDTH 800.f
#define WINDOW_HEIGHT 600.f
#define SZ 10

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
	RectClass() {
		std::uniform_real_distribution<float> hani(-1.0f, 0.8f);

		float wh = 0.2f;
		float cX = hani(dre);
		float cY = hani(dre);

		rect = { cX, cY, cX + wh, cY + wh };
		draw = true;
		randomColors();
	}

	RectClass(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
		rect = { x1, y1, x2, y2 };
		draw = true;
		randomColors();
	}

	bool isClicked(float mx, float my) const {
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

	void moveRect(float dx, float dy) {
		rect.x1 += dx;
		rect.y1 += dy;
		rect.x2 += dx;
		rect.y2 += dy;
	}	

	void mergeRects(RectClass& other) {
		Rect temp{};

		temp.x1 = (rect.x1 < other.rect.x1) ? rect.x1 : other.rect.x1;
		temp.y1 = (rect.y1 < other.rect.y1) ? rect.y1 : other.rect.y1;
		temp.x2 = (rect.x2 > other.rect.x2) ? rect.x2 : other.rect.x2;
		temp.y2 = (rect.y2 > other.rect.y2) ? rect.y2 : other.rect.y2;

		rect = temp;
		other.draw = false;
	}

	void randomColors() {
		r = urd(dre);
		g = urd(dre);
		b = urd(dre);
	}

private:
	Rect rect;
	GLclampf r, g, b;
	bool draw;
};

RectClass* rc;

void Mouse(int button, int state, int x, int y);

void Motion(int x, int y);

void KeyBoard(unsigned char key, int x, int y);

void RandomColors(int i);

void transCoord(int wx, int wy, float& ox, float& oy) {
	ox = 2.0f * wx / WINDOW_WIDTH - 1.0f;
	oy = 1.0f - 2.0f * wy / WINDOW_HEIGHT;
	std::cout << "OX: " << ox << " OY: " << oy << '\n';
}

GLclampf r{}, g{}, b{};

bool timer = false;
bool left_button = false;
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

	r = 204 / 255.0f;
	g = 1;
	b = 229 / 255.0f;

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
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
			if(rc[i].isDrawn())
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
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {	//	Ŭ��
		float openglX, openglY;

		transCoord(x, y, openglX, openglY);		//	������ ��ǥ�� OpenGL ��ǥ�� ��ȯ

		for (int i = SZ - 1; i >= 0; --i) {		//	���߿� ��µ� �簢�� ���� �νĵǰ� �ϱ� ����
			if (rc[i].isDrawn() && rc[i].isClicked(openglX, openglY)) {
				std::cout << i << " Clicked" << '\n';

				oldX = x;
				oldY = y;
				selected = i;
				left_button = true;
				break;
			}
		}
	}

	else if (state == GLUT_UP)
		left_button = false;
	
	glutPostRedisplay();
}

//	��� ��� ���
void Motion(int x, int y)	
{
	if (left_button) {
		//float dx, dy;
		//transCoord((x - oldX), (y - oldY), dx, dy);	������ ��ǥ���� �ٷ� ��ȯ�ϸ� ��ȭ���� �ʹ� �۾� �̻��� ���� ����
		float ox, oy;
		float nx, ny;

		transCoord(oldX, oldY, ox, oy);
		transCoord(x, y, nx, ny);

		rc[selected].moveRect(nx - ox, ny - oy);
		 
		//	��ǥ ������Ʈ
		oldX = x;
		oldY = y;

		//	�浹 üũ - ����
		for (int i = SZ - 1; i >= 0; --i) {
			if (i == selected)
				continue;

			if (rc[i].isDrawn() && rc[selected].isColliding(rc[i])) {
				rc[selected].mergeRects(rc[i]);
				std::cout << "Collided" << '\n';
				break;
			}
		}

		glutPostRedisplay();
	}
}

void KeyBoard(unsigned char key, int x, int y)
{
	if (key == 'a') {
		if (rc != nullptr)
			delete[] rc;

		rc = new RectClass[SZ];
		glutPostRedisplay();
	}

	else if (key == 'q') {
		delete[] rc;
		glutLeaveMainLoop();
	}
}

void RandomColors(int i)
{
	r = urd(dre);
	g = urd(dre);
	b = urd(dre);

	glutPostRedisplay();
}