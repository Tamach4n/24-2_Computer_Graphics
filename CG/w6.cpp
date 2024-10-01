#include <iostream>
#include <conio.h>
#include <random>

using namespace std;

random_device rd;
default_random_engine dre(rd());

char ways[30][30];	// y, x

bool isObject;
int posX, posY;

void display()
{
	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 30; ++j) {
			if (isObject and i == posY and j == posX) {
				cout << "\033[0;31m*\033[0m ";				// https://80000coding.oopy.io/d6099e6e-0be6-482d-8438-8513319332ef 참고
			}

			else if (ways[i][j] == 'X')
				cout << "\033[0;34mX\033[0m ";

			else {
				cout << ways[i][j] << ' ';
			}
		}
		cout << endl;
	}
}

void resetWay()
{
	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 30; ++j) {
			ways[i][j] = '1';
		}
	}
}

void makeWay()
{
	uniform_int_distribution<> uid_dir(0, 3);
	uniform_int_distribution<> uid_step(1, 4);

	// 장애물 만들어주기
	int obstacle = 0;
	while (obstacle < 30) {
		int rx = rand() % 30;
		int ry = rand() % 30;

		if (ways[ry][rx] == '1') {
			ways[ry][rx] = 'X';
			++obstacle;
		}
	}

	int x = 0, y = 0;	// 경로 시작 위치
	int kousa = 0;

	ways[y][x] = '0';	

	int befDir = 0;		// 이전과 다른 방향으로 가야 함

	while (not (x == 29 and y == 29)) {	// 경로 끝에 다다를 때까지
		int step = uid_step(dre);	// 1 ~ 5칸
		int newDir = uid_dir(dre);

		while (newDir == befDir)	// 이전 경로와 다른 방향으로
			newDir = uid_dir(dre);

		int dx = 0, dy = 0;

		switch (newDir)
		{
		case 0:	// 상
			dx = 0;
			dy = -1;
			break;
		case 1:	// 하
			dx = 0;
			dy = 1;
			break;
		case 2:	// 좌
			dx = -1;
			dy = 0;
			break;
		case 3:	// 우
			dx = 1;
			dy = 0;
			break;
		}

		int k1 = 0;

		for (int i = 0; i < step; ++i) {
			int newX = x + dx;
			int newY = y + dy;

			if (newX < 0 or newX > 29 or newY < 0 or newY > 29)
				break;

			if (ways[newY][newX] == '0') {
				kousa++;
				k1++;
			}

			//if (kousa > 100) {
			//	resetWay();
			//	makeWay();
			//	break;
			//}

			if (ways[newY][newX] == 'X' or k1 > 2)	// 갈수 없는 곳이면 길을 만들지 않는다
				break;

			befDir = newDir;		// 진행이 가능하다면 이전 경로를 현재 경로로 초기화

			x = newX;
			y = newY;
			ways[y][x] = '0';
		}
	}
}

int main()
{
	while (true) {
		char cmd = _getch();		// 키입력 받아주는 함수 (conin.h)
		switch (cmd)
		{
		case '\r':	// Enter
			resetWay();
			makeWay();
			break;
		case 'r':
		case 'R':
			isObject = true;
			posX = posY = 0;
			break;
		case 'w':
			if (isObject) {
				int nx = posX;
				int ny = posY - 1;
				if (ny >= 0) {
					if (ways[ny][nx] == '0') {
						posX = nx;
						posY = ny;
					}
				}
			}
			break;
		case 's':
			if (isObject) {
				int nx = posX;
				int ny = posY + 1;
				if (ny < 30) {
					if (ways[ny][nx] == '0') {
						posX = nx;
						posY = ny;
					}
				}
			}
			break;
		case 'a':
			if (isObject) {
				int nx = posX - 1;
				int ny = posY;
				if (nx >= 0) {
					if (ways[ny][nx] == '0') {
						posX = nx;
						posY = ny;
					}
				}
			}
			break;
		case 'd':
			if (isObject) {
				int nx = posX + 1;
				int ny = posY;
				if (nx < 30) {
					if (ways[ny][nx] == '0') {
						posX = nx;
						posY = ny;
					}
				}
			}
			break;
		case 'q':
			exit(0);
			break;
		}

		system("cls");
		display();
	}
}
