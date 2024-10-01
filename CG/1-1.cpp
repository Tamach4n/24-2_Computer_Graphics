#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

std::random_device rd;
std::default_random_engine dre(rd());

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void KeyBoard(unsigned char key, int x, int y);
void RandomColors(int i);

float r{}, g{}, b{};
bool timer = false;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화 - 제일 먼저 호출!
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정 - 더블 버퍼링 | RGBD 모드
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("1-1"); // 윈도우 생성 (윈도우 이름)

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
	glutKeyboardFunc(KeyBoard);
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(r, g, b, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void KeyBoard(unsigned char key, int x, int y)
{
	std::uniform_int_distribution<> uid(0, 255);

	switch (key) {
	case 'c':
		r = 0;
		g = 1;
		b = 1;
		break;

	case 'm':
		r = 1;
		g = 0;
		b = 1;
		break;

	case 'y':
		r = 1;
		g = 1;
		b = 0;
		break;

	case 'a':
		r = uid(dre) / 255.0f;
		g = uid(dre) / 255.0f;
		b = uid(dre) / 255.0f;
		break;

	case 'w':
		r = g = b = 1;
		break;

	case 'k':
		r = g = b = 0;
		break;

	case 't':
		timer = true;
		glutTimerFunc(100, RandomColors, 1);
		break;

	case 's':
		timer = false;
		break;

	case 'q':
		glutLeaveMainLoop();
		break;

	default:
		std::cout << "clicked" << '\n';
		break;
	}

	glutPostRedisplay();
}

void RandomColors(int i)
{
	std::uniform_int_distribution<> uid(0, 255);

	r = uid(dre) / 255.0f;
	g = uid(dre) / 255.0f;
	b = uid(dre) / 255.0f;

	glutPostRedisplay();

	if (timer)
		glutTimerFunc(100, RandomColors, 1);
}