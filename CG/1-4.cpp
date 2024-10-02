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
std::uniform_real_distribution<float> urd(0.0f, 1.0f);	//	실수 난수 생성 (float or double)

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

struct Rect {
	GLfloat x1, y1, x2, y2;

	void move(GLfloat dx, GLfloat dy) {
		x1 += dx;
		y1 += dy;
		x2 += dx;
		y2 += dy;
	}
};

class RectClass {
public:
	RectClass() {
		std::uniform_real_distribution<float> hani(-1.0f, 0.8f);

		float cX = hani(dre);
		float cY = hani(dre);

		rect = { cX, cY, cX + WH, cY + WH };
		draw = true;
		randomColors();
	}

	RectClass(GLfloat x, GLfloat y) :x(x), y(y)
	{
		rect = { x - WH, y - WH, x + WH, y + WH };
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


	void setRect(GLfloat x, GLfloat y) {
		rect = { x - WH, y - WH, x + WH, y + WH };
	}

	void moveRect(float dx, float dy) {
		rect.x1 += dx;
		rect.y1 += dy;
		rect.x2 += dx;
		rect.y2 += dy;
	}

	void randomColors() {
		r = urd(dre);
		g = urd(dre);
		b = urd(dre);
	}

	void resize(GLfloat size) {
		std::uniform_real_distribution<float> urdSize(0.05f, 0.3f);

		float wh = urdSize(dre);

		rect = { x - wh, y - wh, x + wh, y + wh };
	}

	void moveDiagonal() {
		float dx = 20.f / WINDOW_WIDTH;
		float dy = 20.f / WINDOW_HEIGHT;

		
	}

private:
	GLfloat x, y;
	Rect rect;
	Rect* manu = nullptr;
	GLclampf r, g, b;
	bool draw;
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

bool timer = false;
int selected;
int oldX, oldY;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화 - 제일 먼저 호출!
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정 - 더블 버퍼링 | RGBD 모드
	glutInitWindowPosition(300, 300); // 윈도우의 위치 지정
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("1-3"); // 윈도우 생성 (윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";
	
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(KeyBoard);
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	if (rc != nullptr) {
		for (int i = 0; i < SZ; ++i) {
			rc[i].drawRect();
		}			
	}

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

//	마우스 마우스 마우스
void Mouse(int button, int state, int x, int y)
{
	if (rc == nullptr && state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {	//	클릭
		float openglX, openglY;

		transCoord(x, y, openglX, openglY);		//	윈도우 좌표를 OpenGL 좌표로 변환

		std::uniform_real_distribution<float> randX(openglX - 0.5f, openglX + 0.5f);
		std::uniform_real_distribution<float> randY(openglY - 0.5f, openglY + 0.5f);

		rc = new RectClass[SZ]({ randX(dre), randY(dre) }, 
			{ randX(dre), randY(dre) }, { randX(dre), randY(dre) }, 
			{ randX(dre), randY(dre) }, { randX(dre), randY(dre) });

		glutPostRedisplay();
	}
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
	case '2':
	case '3':
	case '4':
		timer = true;
		glutTimerFunc(200, Timer, key);
		break;

	case 's':
		timer = false;
		break;

	case 'm':

		break;

	case 'r':
		timer = false;
		delete[] rc;
		rc = nullptr;
		break;

	case 'q':
		delete[] rc;
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

void Timer(int key)
{
	if (!timer)	return;
	switch (key) {
	case 49:	//	1
		for (int i = 0; i < SZ; ++i) {
			//rc[i]
		}

		break;

	case 50:
		for (int i = 0; i < SZ; ++i) {
			//rc[i]
		}

		break;

	case 51:
		for (int i = 0; i < SZ; ++i) {
			rc[i].resize(0.05f);
		}

		break;

	case 52:
		for (int i = 0; i < SZ; ++i) {
			rc[i].randomColors();
		}

		break;
	}

	glutPostRedisplay();

	glutTimerFunc(200, Timer, key);
}