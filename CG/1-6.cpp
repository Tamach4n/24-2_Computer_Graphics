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

int maxSize{};

struct Rect {
	GLfloat x1, y1, x2, y2;

	void shrinkRect(GLfloat size) {
		x1 += size;
		y1 += size;
		x2 -= size;
		y2 -= size;
	}

	void moveRect(GLfloat dx, GLfloat dy) {
		x1 += dx;
		y1 += dy;
		x2 += dx;
		y2 += dy;
	}
};
//	�ڽ� Ŭ���� 2�� ���� �ϸ� �� ���� �� ������ �ƽ������
class RectClass {
public:
	RectClass();

	bool isClicked(float mx, float my) const {
		return (mx >= rect.x1 && mx <= rect.x2 && my >= rect.y1 && my <= rect.y2);
	}

	int isVanishing() const {
		return vanish;
	}

	int getLengthChibi() const {
		return lengthChibi;
	}

	const Rect& getRect() const {
		return rect;
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

	void updateState(int state) {
		vanish = state;
	}

	void colorRGB(GLclampf cR, GLclampf cG, GLclampf cB) {
		r = cR;
		g = cG;
		b = cB;
	}

	void moveRect(GLfloat dx, GLfloat dy) {
		rect.x1 += dx;
		rect.y1 += dy;
		rect.x2 += dx;
		rect.y2 += dy;
	}

	int vanishRect();

	void moveFourWays();

	void moveDiagonal();

	void moveOneWays();

	void moveEightWays();

	RectClass* chibi{};	//	����� �� ��Ÿ�� ���� �簢���� ������

private:
	Rect rect;
	GLclampf r, g, b;
	int lengthChibi{};
	int vanish; 	//	0 : ���, 1 : ������� ��, 2 : �����
};

RectClass::RectClass()
{
	//	���� x y ���� �غ� :D
	std::uniform_real_distribution<float> size(0.1f, 0.4f);

	float sizeX = size(dre);
	float sizeY = size(dre);

	std::uniform_real_distribution<float> haniX(-1.0f, 1.0f - sizeX);
	std::uniform_real_distribution<float> haniY(-1.0f, 1.0f - sizeY);

	float cX = haniX(dre);
	float cY = haniY(dre);

	rect = { cX, cY, cX + sizeX, cY + sizeY };
	vanish = 0;
	randomColors();
}

int RectClass::vanishRect()
{
	vanish = 1;

	std::uniform_int_distribution<> uid(0, 3);

	int anime = uid(dre);

	if (anime == 0) {
		lengthChibi = 4;
		chibi = new RectClass[lengthChibi];

		chibi[0].rect = { rect.x1, (rect.y2 + rect.y1) / 2, (rect.x2 + rect.x1) / 2, rect.y2 };
		chibi[1].rect = { (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2, rect.x2, rect.y2 };
		chibi[2].rect = { rect.x1, rect.y1, (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2 };
		chibi[3].rect = { (rect.x2 + rect.x1) / 2, rect.y1, rect.x2, (rect.y2 + rect.y1) / 2 };

		for (int i = 0; i < lengthChibi; ++i)
			chibi[i].colorRGB(r, g, b);
	}

	else if (anime == 1) {
		lengthChibi = 4;
		chibi = new RectClass[lengthChibi];

		chibi[0].rect = { rect.x1, (rect.y2 + rect.y1) / 2, (rect.x2 + rect.x1) / 2, rect.y2 };
		chibi[1].rect = { (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2, rect.x2, rect.y2 };
		chibi[2].rect = { rect.x1, rect.y1, (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2 };
		chibi[3].rect = { (rect.x2 + rect.x1) / 2, rect.y1, rect.x2, (rect.y2 + rect.y1) / 2 };

		for (int i = 0; i < lengthChibi; ++i)
			chibi[i].colorRGB(r, g, b);
	}

	else if (anime == 2) {
		lengthChibi = 4;
		chibi = new RectClass[lengthChibi];

		chibi[0].rect = { rect.x1, (rect.y2 + rect.y1) / 2, (rect.x2 + rect.x1) / 2, rect.y2 };
		chibi[1].rect = { (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2, rect.x2, rect.y2 };
		chibi[2].rect = { rect.x1, rect.y1, (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2 };
		chibi[3].rect = { (rect.x2 + rect.x1) / 2, rect.y1, rect.x2, (rect.y2 + rect.y1) / 2 };

		for (int i = 0; i < lengthChibi; ++i)
			chibi[i].colorRGB(r, g, b);
	}

	else if (anime == 3) {
		lengthChibi = 8;
		chibi = new RectClass[lengthChibi];

		float w = (rect.x2 - rect.x1) / 4.0f;
		float h = (rect.y2 - rect.y1) / 4.0f;

		chibi[0].rect = { rect.x1, (rect.y2 + rect.y1) / 2, rect.x1 + w, rect.y2 };
		chibi[1].rect = { rect.x1 + w, (rect.y2 + rect.y1) / 2, (rect.x2 + rect.x1) / 2, rect.y2 };
		chibi[2].rect = { (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2, rect.x2 - w, rect.y2 };
		chibi[3].rect = { rect.x2 - w, (rect.y2 + rect.y1) / 2, rect.x2, rect.y2 };

		chibi[4].rect = { rect.x1, rect.y1, rect.x1 + w, (rect.y2 + rect.y1) / 2 };
		chibi[5].rect = { rect.x1 + w, rect.y1, (rect.x2 + rect.x1) / 2, (rect.y2 + rect.y1) / 2 };
		chibi[6].rect = { (rect.x2 + rect.x1) / 2, rect.y1, rect.x2 - w, (rect.y2 + rect.y1) / 2 };
		chibi[7].rect = { rect.x2 - w, rect.y1, rect.x2, (rect.y2 + rect.y1) / 2 };

		for (int i = 0; i < lengthChibi; ++i)
			chibi[i].colorRGB(r, g, b);
	}

	return anime;
}

void RectClass::moveFourWays()
{
	float dx = 20.0f / WINDOW_WIDTH;
	float dy = 20.0f / WINDOW_HEIGHT;

	chibi[0].rect.moveRect(-dx, 0);
	chibi[1].rect.moveRect(0, dy);
	chibi[2].rect.moveRect(0, -dy);
	chibi[3].rect.moveRect(dx, 0);

	chibi[0].rect.shrinkRect(0.01f);
	chibi[1].rect.shrinkRect(0.01f);
	chibi[2].rect.shrinkRect(0.01f);
	chibi[3].rect.shrinkRect(0.01f);

	if (abs(chibi[0].rect.x2 - chibi[0].rect.x1) * 8.f <= abs(rect.x2 - rect.x1)
		|| abs(chibi[0].rect.y2 - chibi[0].rect.y1) * 8.f <= abs(rect.y2 - rect.y1)) {
		vanish = 2;
		lengthChibi = 0;
		delete[] chibi;
		std::cout << " Vanished" << '\n';
	}
}

void RectClass::moveDiagonal()
{
	float dx = 20.0f / WINDOW_WIDTH;
	float dy = 20.0f / WINDOW_HEIGHT;

	chibi[0].rect.moveRect(-dx, dy);
	chibi[1].rect.moveRect(dx, dy);
	chibi[2].rect.moveRect(-dx, -dy);
	chibi[3].rect.moveRect(dx, -dy);

	chibi[0].rect.shrinkRect(0.01f);
	chibi[1].rect.shrinkRect(0.01f);
	chibi[2].rect.shrinkRect(0.01f);
	chibi[3].rect.shrinkRect(0.01f);

	if (abs(chibi[0].rect.x2 - chibi[0].rect.x1) * 8.f <= abs(rect.x2 - rect.x1)
		|| abs(chibi[0].rect.y2 - chibi[0].rect.y1) * 8.f <= abs(rect.y2 - rect.y1)) {
		vanish = 2;
		lengthChibi = 0;
		delete[] chibi;
		std::cout << " Vanished" << '\n';
	}
}

void RectClass::moveOneWays()
{
	float d = 20.f / WINDOW_HEIGHT;

	chibi[0].rect.moveRect(d, 0);
	chibi[1].rect.moveRect(d, 0);
	chibi[2].rect.moveRect(d, 0);
	chibi[3].rect.moveRect(d, 0);

	chibi[0].rect.shrinkRect(0.01f);
	chibi[1].rect.shrinkRect(0.01f);
	chibi[2].rect.shrinkRect(0.01f);
	chibi[3].rect.shrinkRect(0.01f);

	if (abs(chibi[0].rect.x2 - chibi[0].rect.x1) * 8.f <= abs(rect.x2 - rect.x1) ||
		abs(chibi[0].rect.y2 - chibi[0].rect.y1) * 8.f <= abs(rect.y2 - rect.y1)) {
		vanish = 2;
		lengthChibi = 0;
		delete[] chibi;
		std::cout << " Vanished" << '\n';
	}
}

void RectClass::moveEightWays()
{
	float dx = 20.0f / WINDOW_WIDTH;
	float dy = 20.0f / WINDOW_HEIGHT;

	chibi[0].rect.moveRect(-dx, 0);
	chibi[1].rect.moveRect(-dx, dy);
	chibi[2].rect.moveRect(0, dy);
	chibi[3].rect.moveRect(dx, dy);
	chibi[4].rect.moveRect(-dx, -dy);
	chibi[5].rect.moveRect(0, -dy);
	chibi[6].rect.moveRect(dx, -dy);
	chibi[7].rect.moveRect(dx, 0);

	for (int i = 0; i < lengthChibi; ++i)
		chibi[i].rect.shrinkRect(0.005f);

	if (abs(chibi[0].rect.x2 - chibi[0].rect.x1) * 8.f <= abs(rect.x2 - rect.x1) ||
		abs(chibi[0].rect.y2 - chibi[0].rect.y1) * 8.f <= abs(rect.y2 - rect.y1)) {
		vanish = 2;
		lengthChibi = 0;
		delete[] chibi;
		std::cout << " Vanished" << '\n';
	}
}

RectClass* rc{};

void Mouse(int button, int state, int x, int y);

void KeyBoard(unsigned char key, int x, int y);

void transCoord(int wx, int wy, float& ox, float& oy) {
	ox = 2.0f * wx / WINDOW_WIDTH - 1.0f;
	oy = 1.0f - 2.0f * wy / WINDOW_HEIGHT;
}

void VanishRect(int i);

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ - ���� ���� ȣ��!
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ���� - ���� ���۸� | RGBD ���
	glutInitWindowPosition(300, 300); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("1-6"); // ������ ���� (������ �̸�)

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
	glClearColor(204 / 255.0f, 1.0f, 229 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	if (rc != nullptr) {
		for (int i = 0; i < maxSize; ++i) {
			if (rc[i].isVanishing() == 0)
				rc[i].drawRect();

			else if (rc[i].isVanishing() == 1) {
				for (int j = 0; j < rc[i].getLengthChibi(); ++j) {
					rc[i].chibi[j].drawRect();
				}
			}
		}
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {	//	Ŭ��
		float openglX, openglY;

		transCoord(x, y, openglX, openglY);		//	������ ��ǥ�� OpenGL ��ǥ�� ��ȯ

		for (int i = maxSize - 1; i >= 0; --i) {
			if (rc[i].isVanishing() == 0 && rc[i].isClicked(openglX, openglY)) {
				//rc[i].vanishRect();
				glutTimerFunc(200, VanishRect, i);

				std::cout << i << " Clicked" << '\n';
				break;
			}
		}
	}

	glutPostRedisplay();
}

void KeyBoard(unsigned char key, int x, int y)
{
	if (key == 'r') {
		std::uniform_int_distribution<> uid(5, 10);

		maxSize = uid(dre);
		std::cout << "Number of Shapes: " << maxSize << '\n';

		if (rc != nullptr)
			delete[] rc;

		rc = new RectClass[maxSize];
		glutPostRedisplay();
	}

	else if (key == 'q') {
		delete[] rc;
		glutLeaveMainLoop();
	}
}

void VanishRect(int i)	//	Ÿ�̸� �Լ�
{
	static int anime;

	if (0 == rc[i].isVanishing())
		anime = rc[i].vanishRect();	

	switch (anime) {
	case 0:
		rc[i].moveFourWays();
		break;

	case 1:
		rc[i].moveDiagonal();
		break;

	case 2:
		rc[i].moveOneWays();
		break;

	case 3:
		rc[i].moveEightWays();
		break;
	}

	glutPostRedisplay();

	if (2 != rc[i].isVanishing())
		glutTimerFunc(200, VanishRect, i);
}