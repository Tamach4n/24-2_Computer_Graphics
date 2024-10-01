//	저장 리스트 만들기
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
//#include <cstdio>

using namespace std;

#define MAX_DATA 20

struct DATA {
	int x = '\0', y = '\0', z = '\0';
	bool isDeleted = true;
};

void printList(DATA list[]) {
	for (int i = 9; i >= 0; --i) {	//	10개만 출력함
		cout.width(2);
		cout << i + 1 << " : (" << list[i].x << ", " << list[i].y << ", " << list[i].z << ")" << '\n';
	}
}

int sq(int x) {
	return x * x;
}

int main()
{
	DATA list[MAX_DATA + 1];
	DATA manu_list[MAX_DATA + 1];

	int curr = -1;
	int num = 0;		//	저장된 인덱스 수
	bool sorted = false;

	while (1) {
		cout << endl;
		cout << "#=====================================================================================================================#" << '\n';
		cout << "+ x y z : 리스트의 맨 위에 입력(x, y, z: 숫자)" << '\n';
		cout << "- : 리스트의 맨 위에서 삭제한다." << '\n';
		cout << "e x y z : 리스트의 맨 아래에 입력(명령어 + 와 반대의 위치, 리스트에 저장된 데이터값이 위로 올라간다.)" << '\n';
		cout << "d : 리스트의 맨 아래에서 삭제한다. (리스트에서 삭제된 칸이 비어있다.)" << '\n';
		cout << "l : 리스트에 저장된 점의 개수를 출력한다." << '\n';
		cout << "c : 리스트를 비운다.리스트를 비운 후 다시 입력하면 0번부터 저장된다." << '\n';
		cout << "m : 원점에서 가장 먼 거리에 있는 점의 좌표값을 출력한다." << '\n';
		cout << "n : 원점에서 가장 가까운 거리에 있는 점의 좌표값을 출력한다." << '\n';
		cout << "a : 원점과의 거리를 정렬하여 오름차순으로 정렬하여 출력한다.인덱스 0번부터 빈 칸없이 저장하여 출력한다." << '\n';
		cout << "s : 원점과의 거리를 정렬하여 내림차순으로 정렬하여 출력한다. (다시 a 또는 s를 누르면 원래의 인덱스 위치에 출력한다.)" << '\n';
		cout << "q : 프로그램을 종료한다." << '\n';
		cout << "#=====================================================================================================================#" << '\n';
		cout << "> ";

		DATA tmp{};
		char command, ans[20];
		cin.getline(ans, 20);
		cout << endl;

		if (ans[1] != '\0')
			sscanf(ans, "%c %d %d %d", &command, &tmp.x, &tmp.y, &tmp.z);

		else
			command = ans[0];

		switch (command) {
		case '+':
			if (curr < MAX_DATA - 1) {	//	-1 ~ 18 ( 0 ~ 19 인덱스)
				++curr;
				++num;

				tmp.isDeleted = false;
				list[curr] = tmp;
			}

			else if (num < MAX_DATA) {	//	빈 인덱스가 중간에 있어 맨 위에 채울 수 없는 경우
				for (int i = MAX_DATA - 1; i >= 0; --i) {
					if (list[i].isDeleted) {
						tmp.isDeleted = false;
						list[curr] = tmp;
						++num;
						break;
					}
				}
			}

			printList(list);
			break;

		case '-':
			if (curr >= 0) {
				list[curr] = { '\0','\0','\0', true };
				--curr;
				--num;
			}

			printList(list);
			break;

		case 'e':
			if (!list[0].isDeleted) {	//	맨 밑에 인덱스가 있을 경우 위로 옮김
				if (num == MAX_DATA)	//	꽉 차있을 경우 패스
					break;

				for (int i = 1; i < MAX_DATA; ++i) {	//	빈 인덱스 탐색
					if (list[i].isDeleted) {
						for (int j = i; j > 0; --j) {
							swap(list[j], list[j - 1]);
						}

						break;
					}
				}

				tmp.isDeleted = false;	//	최종 삽입
				list[0] = tmp;
				++num;
			}

			else {	//	없을 경우 그대로 삽입
				tmp.isDeleted = false;
				list[0] = tmp;
				++num;
			}

			printList(list);
			break;

		case 'd':
			for (int i = 0; i < MAX_DATA; ++i) {
				if (!list[i].isDeleted) {
					list[i] = { '\0', '\0', '\0', true };
					--num;
					break;
				}
			}

			printList(list);
			break;

		case 'l':
			cout << "Length of Lists: " << num << '\n';
			break;

		case 'c':
			for (int i = curr; i >= 0; --i) {
				list[i] = { '\0','\0','\0', true };
			}

			curr = -1;
			num = 0;

			printList(list);
			break;

		case 'm':
		{
			int farthest = -1;

			for (int i = 0; i < MAX_DATA; ++i) {
				if (sq(list[farthest].x) + sq(list[farthest].y) + sq(list[farthest].z) < sq(list[i].x) + sq(list[i].y) + sq(list[i].z))
					farthest = i;
			}

			cout << "Farthest: " << list[farthest].x << ", " << list[farthest].y << ", " << list[farthest].z << '\n';
			break;
		}

		case 'n':
		{
			int closest = -1;

			for (int i = 0; i < MAX_DATA; ++i) {
				if (sq(list[closest].x) + sq(list[closest].y) + sq(list[closest].z) > sq(list[i].x) + sq(list[i].y) + sq(list[i].z))
					closest = i;
			}

			cout << "Nearest: " << list[closest].x << ", " << list[closest].y << ", " << list[closest].z << '\n';
			break;
		}

		case 'a':
			if (!sorted) {
				if (num < 2)	break;	//	인덱스 2개 미만이면 패스
				
				for (int i = 0; i < curr + 1; ++i) {
					manu_list[i] = list[i];
				}

				for (int i = 0; i < curr; ++i) {
					manu_list[i] = list[i];
					int close = i;

					for (int j = i; j <= curr; ++j) {
						if (sq(list[close].x) + sq(list[close].y) + sq(list[close].z) > sq(list[j].x) + sq(list[j].y) + sq(list[j].z))
							close = j;
					}

					swap(list[i], list[close]);
				}
			}

			else {
				for (int i = 0; i < curr + 1; ++i) {
					list[i] = manu_list[i];
				}
			}

			sorted = !sorted;
			printList(list);
			break;

		case 's':
			if (!sorted) {
				if (num < 2)	break;	//	인덱스 2개 미만이면 패스

				for (int i = 0; i < curr + 1; ++i) {
					manu_list[i] = list[i];
				}

				for (int i = 0; i < curr; ++i) {
					int far = i;

					for (int j = i; j <= curr; ++j) {
						if (sq(list[far].x) + sq(list[far].y) + sq(list[far].z) < sq(list[j].x) + sq(list[j].y) + sq(list[j].z))
							far = j;
					}

					swap(list[i], list[far]);
					printList(list);
				}
			}

			else {
				for (int i = 0; i < curr + 1; ++i) {
					list[i] = manu_list[i];
				}
			}

			sorted = !sorted;
			printList(list);
			break;

		case 'q':
			exit(0);
			break;
		}
	}
}