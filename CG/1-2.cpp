#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

#define WINDOW_WIDTH 800.f
#define WINDOW_HEIGHT 600.f

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd(0.0f, 1.0f);	//	�Ǽ� ���� ���� (float or double)

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

struct Rect {
	GLfloat x1, y1, x2, y2;
};

class QuadRect {
public:
	QuadRect() {
		rect = { 0, 0, 0, 0 };
		bg = { 0, 0, 0, 0 };
		randomColors();
	}

	QuadRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
		rect = { x1, y1, x2, y2 };
		bg = { x1, y1, x2, y2 };
		randomColors();
	}

	bool isClicked(bool rectangle, float mx, float my) const {
		if(rectangle)
			return (mx >= rect.x1 && mx <= rect.x2 && my >= rect.y1 && my <= rect.y2);

		else
			return (mx >= bg.x1 && mx <= bg.x2 && my >= bg.y1 && my <= bg.y2);
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

	void resizeRect(float size) {
		if (size >= 0 || (!(rect.x2 - rect.x1 < 0.2 || rect.y2 - rect.y1 < 0.2))) {
			rect.x1 -= size;
			rect.y1 -= size;
			rect.x2 += size;
			rect.y2 += size;
		}
	}

	void printCoord() const {
		std::cout << rect.x1 << " " << rect.y1 << " " << rect.x2 << " " << rect.y2 << '\n';
	}

private:
	Rect rect, bg;
	GLclampf r, g, b;
};

QuadRect qr[4] = { QuadRect(-1, 0, 0, 1), QuadRect(0, 0, 1, 1), QuadRect(-1, -1, 0, 0), QuadRect(0, -1, 1, 0) };

void Mouse(int button, int state, int x, int y);
void KeyBoard(unsigned char key, int x, int y);
void RandomColors(int i);
void transCoord(int wx, int wy, float& ox, float& oy) {
	ox = 2.0f * wx / WINDOW_WIDTH - 1.0f;
	oy = 1.0f - 2.0f * wy / WINDOW_HEIGHT;
	std::cout << "OX: " << ox << " OY: " << oy << '\n';
}

GLclampf r{}, g{}, b{};

bool timer = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ - ���� ���� ȣ��!
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ���� - ���� ���۸� | RGBD ���
	glutInitWindowPosition(300, 300); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("1-2"); // ������ ���� (������ �̸�)

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
	g = 1.0f;
	b = 229 / 255.0f;

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

	for (int i = 0; i < 4; ++i) {
		qr[i].drawRect();
	}
	
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {	//	Ŭ��
		bool changed = false;
		float openglX, openglY;

		transCoord(x, y, openglX, openglY);	//	������ ��ǥ�� OpenGL ��ǥ�� ��ȯ

		if (button == GLUT_LEFT_BUTTON) {	//	��Ŭ��	
			for (int i = 0; i < 4; ++i) {
				if (qr[i].isClicked(1, openglX, openglY)) {
					std::cout << i << '\n';
					qr[i].printCoord();

					qr[i].randomColors();
					changed = true;
					break;
				}
			}

			if (!changed)
				RandomColors(static_cast<int>(changed));
		}

		else if (button == GLUT_RIGHT_BUTTON) {		//	��Ŭ��
			for (int i = 0; i < 4; ++i) {
				if (qr[i].isClicked(1, openglX, openglY)) {		//	�簢�� Ŭ��
					std::cout << i << '\n';
					qr[i].printCoord();

					qr[i].resizeRect(-0.02);
					changed = true;
					break;
				}

				else if (qr[i].isClicked(0, openglX, openglY)) {	//	��� Ŭ��
					std::cout << i << '\n';
					qr[i].printCoord();

					qr[i].resizeRect(0.02);
					changed = true;
					break;
				}
			}

			if (!changed) {

			}
		}

		glutPostRedisplay();
	}
}

void KeyBoard(unsigned char key, int x, int y)
{
	if (key == 'q')	
		glutLeaveMainLoop();
}

void RandomColors(int i)
{
	std::uniform_real_distribution<float> uid(0.0f, 1.0f);

	r = uid(dre);
	g = uid(dre);
	b = uid(dre);

	glutPostRedisplay();
}