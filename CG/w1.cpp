//	행렬 다루기
#include <iostream>
#include <random>

using namespace std;

random_device rd;
default_random_engine dre(rd());

#define MAX 4

void initMatrix(int matrix[][MAX]);
void printMatrix(int matrix[][MAX]);
void printEMatrix(int matrix[][MAX], bool turn);
int determinant(int matrix[][MAX], int n);

int main()
{
	int matrix1[MAX][MAX];
	int matrix2[MAX][MAX];

	initMatrix(matrix1);
	initMatrix(matrix2);

	printMatrix(matrix1);
	printMatrix(matrix2);

	while (true) {
		cout << "#=====================================================================================================================#" << '\n';
		cout << "m : 행렬의 곱셈" << '\n';
		cout << "a : 행렬의 덧셈" << '\n';
		cout << "d : 행렬의 뺄셈" << '\n';
		cout << "r : 행렬식의 값(Determinant) → 입력한 2개의 행렬의 행렬식 값을 모두 출력한다." << '\n';
		cout << "t : 전치 행렬(Transposed matrix)과 그 행렬식의 값 → 입력한 2개의 행렬에 모두 적용한다." << '\n';
		cout << "e : 행렬의 값 중 짝수만 출력한다.→ 다시 누르면 홀수만 출력한다.→ 다시 누르면 원래대로 출력한다." << '\n';
		cout << "s : 행렬의 값을 새로 랜덤하게 설정한다." << '\n';
		cout << "숫자(1~9) : 입력한 숫자를 행렬에 곱한다." << '\n';
		cout << "q : 프로그램 종료" << '\n';
		cout << "#=====================================================================================================================#" << '\n';
		cout << "> ";

		char ans;
		cin >> ans;

		switch (ans) {
		case 'm':	//	곱
		{
			for (int i = 0; i < MAX; ++i) {
				for (int j = 0; j < MAX; ++j) {
					int mul = 0;

					for (int k = 0; k < MAX; ++k) {
						mul += matrix1[i][j] * matrix2[j][k];
					}

					matrix1[i][j] = mul;
				}
			}

			printMatrix(matrix1);
			break;
		}

		case 'a':	//	합
			for (int i = 0; i < MAX; ++i) {
				for (int j = 0; j < MAX; ++j) {
					matrix1[i][j] += matrix2[i][j];
				}
			}

			printMatrix(matrix1);
			break;

		case 'd':	//	차
			for (int i = 0; i < MAX; ++i) {
				for (int j = 0; j < MAX; ++j) {
					matrix1[i][j] -= matrix2[i][j];
				}
			}

			printMatrix(matrix1);
			break;

		case 'r':	//	det
		{
			double det = determinant(matrix1, MAX);
			cout << det << '\n';
			det = determinant(matrix2, MAX);
			cout << det << '\n';
			break;
		}

		case 't':	//	전치 + det
		{
			int matrix4[MAX][MAX]{};
			int matrix3[MAX][MAX]{};

			for (int i = 0; i < MAX; ++i) {
				for (int j = 0; j < MAX; ++j) {
					matrix3[j][i] = matrix1[i][j];
					matrix4[j][i] = matrix2[i][j];
				}
			}

			printMatrix(matrix3);
			double det = determinant(matrix3, MAX);
			cout << det << '\n' << '\n';

			printMatrix(matrix4);
			det = determinant(matrix4, MAX);
			cout << det << '\n' << '\n';
			break;
		}
			
		case 'e':	//	 짝 홀 전체
		{
			static int order = -1;
			++order;

			if (order == 0)
				printEMatrix(matrix1, 1);

			else if (order == 1)
				printEMatrix(matrix1, 0);

			else {
				order = 0;
				printMatrix(matrix1);
			}				

			break;
		}

		case 's':	//	reset
			initMatrix(matrix1);
			initMatrix(matrix2);

			printMatrix(matrix1);
			printMatrix(matrix2);
			break;

		case 'q':
			exit(0);
			break;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			for (int i = 0; i < MAX; ++i) {
				for (int j = 0; j < MAX; ++j) {
					matrix1[i][j] = (ans - 48) * matrix1[i][j];
				}
			}

			printMatrix(matrix1);
			break;

		default:
			break;
		}
	}
}

void initMatrix(int matrix[][MAX])
{
	int count2 = 2;		//	2 최대 개수

	for (int i = 0; i < MAX; ++i) {
		for (int j = 0; j < MAX; ++j) {
			uniform_int_distribution<> uid(0, 2);

			do {
				matrix[i][j] = uid(dre);

				if (matrix[i][j] == 2)					//	2인 경우 1 감소
					--count2;
			} while (matrix[i][j] == 2 && count2 < 0);	//	2가 2미 2개 있으면 재설정
		}
	}
}

void printMatrix(int matrix[][MAX]) 
{
	for (int i = 0; i < MAX; ++i) {
		for (int j = 0; j < MAX; ++j) {
			cout.width(2);
			cout << matrix[i][j] << " ";
		}

		cout << '\n';
	}

	cout << '\n';
}

void printEMatrix(int matrix[][MAX], bool turn)
{
	if (turn) {
		for (int i = 0; i < MAX; ++i) {
			for (int j = 0; j < MAX; ++j) {
				if (~matrix[i][j] % 2)
					cout << matrix[i][j] << " ";

				else
					cout << "  ";
			}

			cout << '\n';
		}

		cout << '\n';
	}

	else {
		for (int i = 0; i < MAX; ++i) {
			for (int j = 0; j < MAX; ++j) {
				if (matrix[i][j] % 2)
					cout << matrix[i][j] << " ";

				else
					cout << "  ";
			}

			cout << '\n';
		}

		cout << '\n';
	}
}

int determinant(int matrix[][MAX], int n)
{
	int det = 0, p, h, k, i, j, temp[MAX][MAX];
	if (n == 1) {
		return matrix[0][0];
	}
	else if (n == 2) {
		det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
		return det;
	}
	else {
		for (p = 0; p < n; p++) {
			h = 0;
			k = 0;
			for (i = 1; i < n; i++) {
				for (j = 0; j < n; j++) {
					if (j == p) {
						continue;
					}
					temp[h][k] = matrix[i][j];
					k++;
					if (k == n - 1) {
						h++;
						k = 0;
					}
				}
			}
			det = det + matrix[0][p] * pow(-1, p) * determinant(temp, n - 1);
		}
		return det;
	}
}