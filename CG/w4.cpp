#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <random>
#include <Windows.h>

using namespace std;

//random_device rd;
//default_random_engine dre(rd());

default_random_engine dre;

#define SIZE 5		//	보드 크기
#define COUNT 25	//	진행 가능 횟수

struct DATA {
	int index = 0;
	bool isPresented = false;
};

void printBoard(DATA board[SIZE][SIZE]);	//	출력

void shuffleCards(DATA board[SIZE][SIZE]);	//	초기화

int convertColumn(const char c);	//	문자행을 숫자로 변환하는 함수

void findJoker(float& point, int index, int x, int y, DATA board[SIZE][SIZE]);

float getPoints(int card) {
	if (card >= 1 && card < 4)
		return 1;

	else if (card >= 4 && card < 7)
		return 1.3;

	else if (card >= 7 && card < 10)
		return 1.7;

	else if (card >= 10 && card < 13)
		return 2;
}

void matchCards(DATA(&board)[SIZE][SIZE], const char* c1, const char* c2, float& point);

int main()
{
	DATA board[SIZE][SIZE]{};

	float point = 0;
	int count = COUNT;

	shuffleCards(board);	
	printBoard(board);

	while (1) {
		char ans[10]{}, card1[3]{}, card2[3]{};

		cout << "Input: ";
		cin.getline(ans, 10);
		cout << '\n';

		if (ans[0] == 'q')	//	종료
			exit(0);

		else if (ans[0] == 'r') {	//	리셋
			shuffleCards(board);
			point = 0;
			count = COUNT;
		}

		else {
			sscanf(ans, "%s %s", &card1, &card2);

			matchCards(board, card1, card2, point);

			--count;	//	잘못된 입력에도 횟수 차감됨 -> 인생은 실전.

			if (count == 0)
				break;
		}

		printBoard(board);
	}

	cout << "** Game Over. **" << '\n';
	cout << "Your Point: " << point << '\n';
}

void printBoard(DATA board[SIZE][SIZE])
{
	cout << "  ※  a  b  c  d  e" << '\n';

	for (int i = 0; i < SIZE; ++i) {
		cout.width(3);
		cout << i + 1;

		for (int j = 0; j < SIZE; ++j) {
			cout.width(3);
			if (board[i][j].isPresented) {	//	플레이어가 고른 칸만 보이게 함
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), board[i][j].index);	//	색은 고정

				switch (board[i][j].index) {
				case -1:		//	조커
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					cout << "  ♥";
					break;

				case 1:
					cout << "A";
					break;

				case 2:
					cout << "B";
					break;

				case 3:
					cout << "C";
					break;

				case 4:
					cout << "D";
					break;

				case 5:
					cout << "E";
					break;

				case 6:
					cout << "F";
					break;

				case 7:
					cout << "G";
					break;

				case 8:
					cout << "H";
					break;

				case 9:
					cout << "W";
					break;

				case 10:
					cout << "X";
					break;

				case 11:
					cout << "K";
					break;

				case 12:
					cout << "L";
					break;
				}
			}

			else	cout << "*";

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}

		cout << '\n';
	}
}

void shuffleCards(DATA board[SIZE][SIZE])
{
	uniform_int_distribution<> uid(0, SIZE - 1);

	if (board[0][0].index != 0) {	//	인덱스가 0이 아니다 -> 이미 섞여 있다.
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				board[0][0] = { 0, false };
			}
		}
	}

	int index[] = { -1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12 };

	for (int i = 0; i < SIZE * SIZE; ++i) {
		int x, y;

		do {
			x = uid(dre);
			y = uid(dre);
		} while (board[y][x].index != 0);

		board[y][x].index = index[i];
	}
}

int convertColumn(const char c) 
{
	if (c > 90)
		return c - 97;

	else
		return c - 65;
}

void findJoker(float& point, int index, int x, int y, DATA board[SIZE][SIZE]) {	//	A 찾기
	cout << "** Find the Joker **" << endl;
	cout << "Get +5 Points" << '\n' << '\n';
	point += 5;

	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (j != x && i != y && board[i][j].index == index)
				board[i][j].isPresented = true;
		}
	}
}

void matchCards(DATA(&board)[SIZE][SIZE], const char* c1, const char* c2, float& point)
{
	int x1 = convertColumn(c1[0]);
	int y1 = c1[1] - '1';
	int x2 = convertColumn(c2[0]);
	int y2 = c2[1] - '1';

	if (x1 < 0 || x1 > 5 || x2 < 0 || x2 > 5 || y1 < 0 || y1 > 5 || y2 < 0 || y2 > 5)
		return;

	board[y1][x1].isPresented = true;
	board[y2][x2].isPresented = true;

	if (board[y1][x1].index == board[y2][x2].index) {
		int earned = getPoints(board[y1][x1].index);

		cout << "** Matched **" << endl;
		cout << "Get +" << earned << " Points." << '\n' << '\n';
		point += earned;
	}

	else {
		if (board[y1][x1].index == -1) {
			findJoker(point, board[y2][x2].index, x1, y1, board);
			printBoard(board);
			system("pause");
			cout << '\n';
		}

		else if (board[y2][x2].index == -1) {
			findJoker(point, board[y1][x1].index, x2, y2, board);
			printBoard(board);
			system("pause");
			cout << '\n';
		}

		else {
			cout << "** Not Matched **" << endl << '\n';
			printBoard(board);
			board[y1][x1].isPresented = false;
			board[y2][x2].isPresented = false;
			system("pause");
			cout << '\n';
		}
	}
}
