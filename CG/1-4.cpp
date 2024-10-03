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

	void moveDiagonal() {		
		moveRect(dx, dy);	//	이동

		if (isColliding() == -1) {	//	충돌 체크
			dx = -dx;
		}

		if (isColliding() == 1) {
			dy = -dy;
		}
	}

private:
	GLfloat x, y;
	Rect rect;
	Rect* manu = nullptr;
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
		glutTimerFunc(100, Timer, key);
		break;

	case '2':
		t2 = !t2;
		glutTimerFunc(200, Timer, key);
		break;

	case '3':
		t3 = !t3;
		glutTimerFunc(200, Timer, key);
		break;

	case '4':
		t4 = !t4;
		glutTimerFunc(200, Timer, key);
		break;

	case 's':
		t1 = t2 = t3 = t4 = false;
		glutPostRedisplay();
		break;

	case 'm':

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
	if (t1 && t2 && t3 && t4)	return;

	switch (key) {
	case 49:	//	1
		for (int i = 0; i < SZ; ++i) {
			rc[i].moveDiagonal();
		}

		break;

	case 50:
		for (int i = 0; i < SZ; ++i) {
			//rc[i]
		}

		break;

	case 51:
		for (int i = 0; i < SZ; ++i) {
			rc[i].randomResize(0.05f);
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