#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ - ���� ���� ȣ��!
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ���� - ���� ���۸� | RGBD ���
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("1-1"); // ������ ���� (������ �̸�)

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
	glutKeyboardFunc(KeyBoard);
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(r, g, b, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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