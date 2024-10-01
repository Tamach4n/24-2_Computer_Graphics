#include <iostream>
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
	bool Xcut = false;
	bool Ycut = false;
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

void printBoard(int board[SSSIZE + 1][SSSIZE + 1], const SHAPE& shape);

int main()
{
	int board[SSSIZE + 1][SSSIZE + 1]{};

	SHAPE shape;
	init(shape);

	while (1) {
		printBoard(board, shape);

		cout << "Input Order: ";
		char ans;
		cin >> ans;
		cout << endl;

		switch (ans) {
		case 'x':	//	x방향 1칸 이동
			if (!shape.Xcut&&!shape.Ycut) {
				++shape.main.startX;
				++shape.main.endX;

				if (shape.main.endX > SSSIZE) {
					shape.Xcut = true;
					shape.temp = { 1, shape.main.startY, 1, shape.main.endY };
				}
			}

			else {
				if (shape.Ycut) {
					++shape.main.startX;
					++shape.temp.startX;
					++shape.main.endX;
					++shape.temp.endX;
				}

				else {
					++shape.main.startX;
					++shape.temp.endX;
				}

				if (shape.main.startX > SSSIZE) {
					shape.Xcut = false;
					shape.main = shape.temp;
				}
			}

			break;

		case 'X':
			if (!shape.Xcut && !shape.Ycut) {
				--shape.main.startX;
				--shape.main.endX;

				if (shape.main.startX == 0) {
					shape.Xcut = true;
					shape.temp = { SSSIZE, shape.main.startY, SSSIZE, shape.main.endY };
				}
			}

			else {
				if (shape.Ycut) {
					--shape.main.endX;
					--shape.temp.endX;
					--shape.main.startX;
					--shape.temp.startX;
				}

				else{
					--shape.main.endX;
					--shape.temp.startX;				
				}

				if (shape.main.endX == 0) {
					shape.Xcut = false;
					shape.main = shape.temp;
				}
			}

			break;

		case 'y':	//	y방향 1칸 이동
			if (!shape.Xcut && !shape.Ycut) {
				++shape.main.startY;
				++shape.main.endY;

				if (shape.main.endY > SSSIZE) {
					shape.Ycut = true;
					shape.temp = { shape.main.startX, 1, shape.main.endX, 1 };
				}
			}

			else {
				if (shape.Xcut) {
					++shape.main.startY;
					++shape.temp.startY;
					++shape.main.endY;
					++shape.temp.endY;
				}

				else {
					++shape.main.startY;
					++shape.temp.endY;
				}

				if (shape.main.startY > SSSIZE) {
					shape.Ycut = false;
					shape.main = shape.temp;
				}
			}

			break;

		case 'Y':
			if (!shape.Xcut && !shape.Ycut) {
				--shape.main.startY;
				--shape.main.endY;

				if (shape.main.startY == 0) {
					shape.Xcut = true;
					shape.temp = { shape.main.startX, SSSIZE, shape.main.endX, SSSIZE };
				}
			}

			else {
				if (shape.Xcut) {
					--shape.main.startY;
					--shape.temp.startY;
					--shape.main.endY;
					--shape.temp.endY;
				}

				else {
					--shape.main.endY;
					--shape.temp.startY;
				}

				if (shape.main.endY == 0) {
					shape.Xcut = false;
					shape.main = shape.temp;
				}
			}

			break;

		case 's':
			if (shape.main.endX - shape.main.startX > 0 && shape.main.endY - shape.main.startY > 0) {
				--shape.main.endX;
				--shape.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;

		case 'S':
			if (shape.main.endX < SSSIZE && shape.main.endY < SSSIZE) {
				++shape.main.endX;
				++shape.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;

		case 'i':	//	x축 크기 변경
			if (shape.main.endX - shape.main.startX > 0)
				--shape.main.endX;

			else
				cout << "CANT!" << '\n';

			break;

		case 'I':
			if (shape.main.endX < SSSIZE)
				++shape.main.endX;

			else
				cout << "CANT!" << '\n';

			break;

		case 'j':	//	y축 크기 변경
			if (shape.main.endY - shape.main.startY > 0)
				--shape.main.endY;

			else
				cout << "CANT!" << '\n';

			break;

		case 'J':
			if (shape.main.endY < SSSIZE)
				++shape.main.endY;

			else
				cout << "CANT!" << '\n';

			break;

		case 'a':
			if (shape.main.endX - shape.main.startX > 0 && shape.main.endY < SSSIZE) {
				--shape.main.endX;
				++shape.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;

		case 'A':
			if (shape.main.endX < SSSIZE && shape.main.endY - shape.main.startY > 0) {
				++shape.main.endX;
				--shape.main.endY;
			}

			else
				cout << "CANT!" << '\n';

			break;

		case 'm':	//	면적 출력
			cout << "Area: " << (shape.main.endX - shape.main.startX + 1) * (shape.main.endY - shape.main.startY + 1) << '\n';
			break;

		case 'n':	//	전체 면적에 대한 비율 출력
		{
			int area = (shape.main.endX - shape.main.startX + 1) * (shape.main.endY - shape.main.startY + 1);
			cout << "Total Area: " << SSSIZE * SSSIZE << ", Square Area: " << area
				<< ", Area Ratio: " << static_cast<float>(area) / (SSSIZE * SSSIZE) * 100.0 << "%" << '\n';
				break;
		}

		case 'r':	//	완전 리셋
			init(shape);
			break;

		case 'q':
			exit(0);
			break;
		}
	}
}

void printBoard(int board[SSSIZE + 1][SSSIZE + 1], const SHAPE& shape) {
	if (!shape.Xcut && !shape.Ycut) {
		for (int i = 0; i < SSSIZE; ++i) {
			for (int j = 0; j < SSSIZE; ++j) {
				cout.width(3);

				if (j >= shape.main.startX - 1 && j < shape.main.endX && i >= shape.main.startY - 1 && i < shape.main.endY)
					cout << "0";

				else
					cout << "  ·";
			}

			cout << '\n';
		}
	}

	else {
		for (int i = 0; i < SSSIZE; ++i) {
			for (int j = 0; j < SSSIZE; ++j) {
				cout.width(3);

				if ((j >= shape.main.startX - 1 && j < shape.main.endX && i >= shape.main.startY - 1 && i < shape.main.endY) ||
					(j >= shape.temp.startX - 1 && j < shape.temp.endX && i >= shape.temp.startY - 1 && i < shape.temp.endY))
					cout << "0";

				else
					cout << "  ·";
			}

			cout << '\n';
		}
	}
}
