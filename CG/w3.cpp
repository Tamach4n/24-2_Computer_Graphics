//	���� ����Ʈ �����
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
	for (int i = 9; i >= 0; --i) {	//	10���� �����
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
	int num = 0;		//	����� �ε��� ��
	bool sorted = false;

	while (1) {
		cout << endl;
		cout << "#=====================================================================================================================#" << '\n';
		cout << "+ x y z : ����Ʈ�� �� ���� �Է�(x, y, z: ����)" << '\n';
		cout << "- : ����Ʈ�� �� ������ �����Ѵ�." << '\n';
		cout << "e x y z : ����Ʈ�� �� �Ʒ��� �Է�(��ɾ� + �� �ݴ��� ��ġ, ����Ʈ�� ����� �����Ͱ��� ���� �ö󰣴�.)" << '\n';
		cout << "d : ����Ʈ�� �� �Ʒ����� �����Ѵ�. (����Ʈ���� ������ ĭ�� ����ִ�.)" << '\n';
		cout << "l : ����Ʈ�� ����� ���� ������ ����Ѵ�." << '\n';
		cout << "c : ����Ʈ�� ����.����Ʈ�� ��� �� �ٽ� �Է��ϸ� 0������ ����ȴ�." << '\n';
		cout << "m : �������� ���� �� �Ÿ��� �ִ� ���� ��ǥ���� ����Ѵ�." << '\n';
		cout << "n : �������� ���� ����� �Ÿ��� �ִ� ���� ��ǥ���� ����Ѵ�." << '\n';
		cout << "a : �������� �Ÿ��� �����Ͽ� ������������ �����Ͽ� ����Ѵ�.�ε��� 0������ �� ĭ���� �����Ͽ� ����Ѵ�." << '\n';
		cout << "s : �������� �Ÿ��� �����Ͽ� ������������ �����Ͽ� ����Ѵ�. (�ٽ� a �Ǵ� s�� ������ ������ �ε��� ��ġ�� ����Ѵ�.)" << '\n';
		cout << "q : ���α׷��� �����Ѵ�." << '\n';
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
			if (curr < MAX_DATA - 1) {	//	-1 ~ 18 ( 0 ~ 19 �ε���)
				++curr;
				++num;

				tmp.isDeleted = false;
				list[curr] = tmp;
			}

			else if (num < MAX_DATA) {	//	�� �ε����� �߰��� �־� �� ���� ä�� �� ���� ���
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
			if (!list[0].isDeleted) {	//	�� �ؿ� �ε����� ���� ��� ���� �ű�
				if (num == MAX_DATA)	//	�� ������ ��� �н�
					break;

				for (int i = 1; i < MAX_DATA; ++i) {	//	�� �ε��� Ž��
					if (list[i].isDeleted) {
						for (int j = i; j > 0; --j) {
							swap(list[j], list[j - 1]);
						}

						break;
					}
				}

				tmp.isDeleted = false;	//	���� ����
				list[0] = tmp;
				++num;
			}

			else {	//	���� ��� �״�� ����
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
				if (num < 2)	break;	//	�ε��� 2�� �̸��̸� �н�
				
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
				if (num < 2)	break;	//	�ε��� 2�� �̸��̸� �н�

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