#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

#define SSSIZE 30

struct DATA {
	int startX, startY;
	int endX, endY;
};

struct SHAPE {
	DATA main;
	DATA temp{};
	bool isSplitted = false;
};

void swap(int& a, int& b) {
	int t = a;
	a = b;
	b = t;
}

void init(SHAPE& shape) {
	cout << "Input coord value(1 ~ 30): ";
	cin >> shape.main.startX >> shape.main.startY >> shape.main.endX >> shape.main.endY;

	if (shape.main.startX > shape.main.endX)
		swap(shape.main.startX, shape.main.endX);

	if (shape.main.startY > shape.main.endY)
		swap(shape.main.startY, shape.main.endY);

	cout << endl;
}

void printBoard(int board[SSSIZE + 1][SSSIZE + 1], const SHAPE& s1,  const SHAPE& s2);

int main()
{
	int board[SSSIZE + 1][SSSIZE + 1]{};

	SHAPE sh1, sh2;
	init(sh1);
	init(sh2);

	while (1) {
		printBoard(board, sh1, sh2);

		cout << "Input Order: ";
		char ans = _getch();
		cout << endl;

		switch (ans) {
		case 'x':	//	x방향 1칸 이동
			if (!sh1.isSplitted) {
				++sh1.main.startX;
				++sh1.main.endX;

				if (sh1.main.endX > SSSIZE) {
					sh1.isSplitted = true;
					sh1.temp = { 1, sh1.main.startY, 1, sh1.main.endY };
				}
			}

			else {
				++sh1.main.startX;
				++sh1.temp.endX;

				if (sh1.main.startX > SSSIZE) {
					sh1.isSplitted = false;
					sh1.main = sh1.temp;
				}
			}

			break;

		case 'X':
			if (!sh1.isSplitted) {
				--sh1.main.startX;
				--sh1.main.endX;

				if (sh1.main.startX == 0) {
					sh1.isSplitted = true;
					sh1.temp = { SSSIZE, sh1.main.startY, SSSIZE, sh1.main.endY };
				}
			}

			else {
				--sh1.main.endX;
				--sh1.temp.startX;

				if (sh1.main.endX == 0) {
					sh1.isSplitted = false;
					sh1.main = sh1.temp;
				}
			}

			break;

		case 'y':	//	y방향 1칸 이동
			if (!sh1.isSplitted) {
				++sh1.main.startY;
				++sh1.main.endY;

				if (sh1.main.endY > SSSIZE) {
					sh1.isSplitted = true;
					sh1.temp = { sh1.main.startX, 1, sh1.main.endX, 1 };
				}
			}

			else {
				++sh1.main.startY;
				++sh1.temp.endY;

				if (sh1.main.startY > SSSIZE) {
					sh1.isSplitted = false;
					sh1.main = sh1.temp;
				}
			}

			break;

		case 'Y':
			if (!sh1.isSplitted) {
				--sh1.main.startY;
				--sh1.main.endY;

				if (sh1.main.startY == 0) {
					sh1.isSplitted = true;
					sh1.temp = { sh1.main.startX, SSSIZE, sh1.main.endX, SSSIZE };
				}
			}

			else {
				--sh1.main.endY;
				--sh1.temp.startY;

				if (sh1.main.endY == 0) {
					sh1.isSplitted = false;
					sh1.main = sh1.temp;
				}
			}

			break;

		case 's':
			if (sh1.main.endX - sh1.main.startX > 0 && sh1.main.endY - sh1.main.startY > 0) {
				--sh1.main.endX;
				--sh1.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;

		case 'S':
			if (sh1.main.endX < SSSIZE && sh1.main.endY < SSSIZE) {
				++sh1.main.endX;
				++sh1.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;

		case 'w':	//	x방향 1칸 이동
			if (!sh2.isSplitted) {
				++sh2.main.startX;
				++sh2.main.endX;

				if (sh2.main.endX > SSSIZE) {
					sh2.isSplitted = true;
					sh2.temp = { 1, sh2.main.startY, 1, sh2.main.endY };
				}
			}

			else {
				++sh2.main.startX;
				++sh2.temp.endX;

				if (sh2.main.startX > SSSIZE) {
					sh2.isSplitted = false;
					sh2.main = sh2.temp;
				}
			}

			break;

		case 'W':
			if (!sh2.isSplitted) {
				--sh2.main.startX;
				--sh2.main.endX;

				if (sh2.main.startX == 0) {
					sh2.isSplitted = true;
					sh2.temp = { SSSIZE, sh2.main.startY, SSSIZE, sh2.main.endY };
				}
			}

			else {
				--sh2.main.endX;
				--sh2.temp.startX;

				if (sh2.main.endX == 0) {
					sh2.isSplitted = false;
					sh2.main = sh2.temp;
				}
			}

			break;

		case 'a':	//	y방향 1칸 이동
			if (!sh2.isSplitted) {
				++sh2.main.startY;
				++sh2.main.endY;

				if (sh2.main.endY > SSSIZE) {
					sh2.isSplitted = true;
					sh2.temp = { sh2.main.startX, 1, sh2.main.endX, 1 };
				}
			}

			else {
				++sh2.main.startY;
				++sh2.temp.endY;

				if (sh2.main.startY > SSSIZE) {
					sh2.isSplitted = false;
					sh2.main = sh2.temp;
				}
			}

			break;

		case 'A':
			if (!sh2.isSplitted) {
				--sh2.main.startY;
				--sh2.main.endY;

				if (sh2.main.startY == 0) {
					sh2.isSplitted = true;
					sh2.temp = { sh2.main.startX, SSSIZE, sh2.main.endX, SSSIZE };
				}
			}

			else {
				--sh2.main.endY;
				--sh2.temp.startY;

				if (sh2.main.endY == 0) {
					sh2.isSplitted = false;
					sh2.main = sh2.temp;
				}
			}

			break;

		case 'd':
			if (sh2.main.endX - sh2.main.startX > 0 && sh2.main.endY - sh2.main.startY > 0) {
				--sh2.main.endX;
				--sh2.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;

		case 'D':
			if (sh2.main.endX < SSSIZE && sh2.main.endY < SSSIZE) {
				++sh2.main.endX;
				++sh2.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;


		case 'r':	//	완전 리셋
			init(sh1);
			init(sh2);
			break;

		case 'q':
			exit(0);
			break;
		}
	}
}

void printBoard(int board[SSSIZE + 1][SSSIZE + 1], const SHAPE& s1, const SHAPE& s2) {
	for (int i = 0; i < SSSIZE; ++i) {
		for (int j = 0; j < SSSIZE; ++j) {
			cout.width(3);

			if (!s1.isSplitted && !s2.isSplitted) {
				if (j >= s1.main.startX - 1 && j < s1.main.endX && i >= s1.main.startY - 1 && i < s1.main.endY) {
					if (j >= s2.main.startX - 1 && j < s2.main.endX && i >= s2.main.startY - 1 && i < s2.main.endY) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						cout << "#";
					}

					else
						cout << "O";
				}

				else if (j >= s2.main.startX - 1 && j < s2.main.endX && i >= s2.main.startY - 1 && i < s2.main.endY)
					cout << "X";

				else
					cout << "  ·";
			}

			else {
				if ((j >= s1.main.startX - 1 && j < s1.main.endX && i >= s1.main.startY - 1 && i < s1.main.endY) ||
					(j >= s1.temp.startX - 1 && j < s1.temp.endX && i >= s1.temp.startY - 1 && i < s1.temp.endY)) {
					if ((j >= s2.main.startX - 1 && j < s2.main.endX && i >= s2.main.startY - 1 && i < s2.main.endY) ||
						(j >= s2.temp.startX - 1 && j < s2.temp.endX && i >= s2.temp.startY - 1 && i < s2.temp.endY)) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						cout << "#";
					}

					else
						cout << "0";
				}

				else if ((j >= s2.main.startX - 1 && j < s2.main.endX && i >= s2.main.startY - 1 && i < s2.main.endY) ||
					(j >= s2.temp.startX - 1 && j < s2.temp.endX && i >= s2.temp.startY - 1 && i < s2.temp.endY))
					cout << "X";

				else
					cout << "  ·";
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}

		cout << '\n';
	}
}