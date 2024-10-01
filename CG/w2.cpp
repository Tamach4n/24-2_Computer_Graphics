//	���Ͽ��� ���ڿ� �б�
#include <iostream>
#include <windows.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

void printResult(vector<string> vec);
bool compareWord(const string& str1, const string& str2);

int main()
{
	string Fname;
	ifstream is;

	while (true)
	{
		cout << "File Name: ";
		cin >> Fname;
		cout << endl;

		is.open(Fname);

		if (is.is_open())	break;	//	������ ���

		cerr << Fname << " ã�� �� ����." << '\n';	//	cerr - ǥ�� ���� �޼��� ��� ��Ʈ��: ��� �ð� ª��
		is.clear();
	}

	string line, word;
	vector<string> lines;
	vector<int> wordCount;

	while (getline(is, line)) {		//	���� ������ �б�
		cout << line << '\n';

		lines.push_back(line);

		istringstream iss(line);
		int wc = 0;

		while (iss >> word) {
			++wc;
		}

		wordCount.push_back(wc);
	}

	vector<string> temp;
	vector<int> int_temp;
	bool C = false;
	bool E = false;
	bool F = false;
	int R = 0;
	bool G = false;

	while (true) {
		cout << endl;
		cout << "#=====================================================================================================================#" << '\n';
		cout << "c: �빮�ڷ� ���۵Ǵ� �ܾ ã�� �� �ܾ �ٸ� ������ ����ϰ�, �� �� �ִ����� ����Ͽ� ����Ѵ�. �ٽ� ������ ������� ����Ѵ�." << '\n';
		cout << "d: ���� �� �� ��ü�� �Ųٷ� ����ϱ�. �ٽ� ������ ������� ����Ѵ�." << '\n';
		cout << "e: 3���� �� 2���� @ ���� �����ϱ�. �ٽ� ������ ������� ����Ѵ�." << '\n';
		cout << "f: ������ �������� ��� �ܾ���� �Ųٷ� ����ϱ�. �ٽ� ������ ������� ����Ѵ�." << '\n';
		cout << "g: ���� ������ Ư�� ���ڸ� �ٸ� ���ڷ� �ٲٱ� (�ٲ� ���ڿ� ���Ӱ� �Է��� ���� �Է� ����) �ٽ� ������ ������� ����Ѵ�." << '\n';
		cout << "h: �� ���� �ܾ��� ������ ����Ѵ�." << '\n';
		cout << "r: �� ������ �ܾ��� ������ ���� ������������ �����Ͽ� ������� ����Ѵ�. �ٽ� ������ ������������ ����Ѵ�. �ٽ� ������ ������� ����Ѵ�. (��, �������� �� �������� �� ������� �� �������� ��)" << '\n';
		cout << "s: ��ɾ�� �ܾ �Է��ϸ�, �Է¹��� ������� ��� ����ϸ鼭 �� �ܾ ã�� �ٸ� ������ ����ϰ�, �� �� �ִ����� ����Ͽ� ����Ѵ�. (��ҹ��� �������� �ʴ´�)" << '\n';
		cout << "q: ���α׷� ����" << '\n';
		cout << "#=====================================================================================================================#" << '\n';
		cout << "> ";
		char ans;

		cin >> ans;
		cout << endl;

		switch (ans) {
		case 'c':
			if (!C) {
				C = !C;

				for (auto& a : lines) {
					istringstream iss(a);
					vector<string> vWord;
					int count = 0;

					temp.push_back(a);

					while (iss >> word) {
						if (isupper(word[0])) {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
							++count;
						}

						else	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

						
						cout << word << ' ';
					}

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					cout << " " << count << '\n';
				}
			}

			else {
				C = !C;
				printResult(lines);
			}
						
			break;
		

		case 'd':
			for (auto& a : lines) {
				reverse(a.begin(), a.end());
				cout << a << '\n';
			}

			break;

		case 'e':
			if (!E) {	//	ON
				for (auto& a : lines) {
					temp.push_back(a);	//	���� ����

					int pos = 2;

					while (pos < a.size()) {
						a.insert(pos + 1, 2, '@');	//	pos+1 ��ġ�� @�� 2�� ����
						pos += 3 + 2;
					}
				}

				E = !E;
			}

			else {	//	OFF
				lines = temp;
				temp.clear();

				E = !E;				
			}

			printResult(lines);
			break;


		case 'f':
		{
			if (!F) {	//	ON
				for (auto& a : lines) {
					istringstream iss(a);
					vector<string> vWord;

					temp.push_back(a);

					while (iss >> word) {
						if (word.back() == '.')	//	������ ������ �״�� ��
							reverse(word.begin(), word.end() - 1);

						else
							reverse(word.begin(), word.end());

						vWord.push_back(word);	//	������ �ܾ� ���Ϳ� ����
					}

					string result;

					for (const auto& a : vWord)
						result += a + ' ';		//	�ܾ�� ���� ���� ���� �����

					if (!result.empty())
						result.pop_back();		//	���� �� ���� ����

					a = result;

					F = !F;
				}
			}

			else {	//	OFF
				lines = temp;
				temp.clear();

				F = !F;
			}

			printResult(lines);
			break;
		}

		case 'g':
			if (!G) {
				temp = lines;
				G = !G;

				char A, B;	//	change A to B
				cout << "������: ";
				cin >> A;
				cout << "��������: ";
				cin >> B;

				for (auto& a : lines) {
					for (int i = 0; i < a.size(); ++i) {
						if (a[i] == A) 
							a[i] = B;						
					}
				}
			}

			else {
				lines = temp;
				G = !G;
			}

			printResult(lines);
			break;

		case 'h':
			for (int i = 0; i < lines.size(); ++i) 
				cout << lines[i] << " (" << wordCount[i] << " words)" << '\n';

			break;

		case 'r':
			++R;

			if (1 == R % 3) {	//	1 4 7 ...
				temp = lines;
				int_temp = wordCount;

				//	���� ����
				for (int i = 0; i < lines.size() - 1; ++i) {
					int min = i;	//	�ּ�

					for (int j = i; j < lines.size(); ++j) {
						if (wordCount[j] < wordCount[min])
							min = j;
					}

					swap(lines[i], lines[min]);
					swap(wordCount[i], wordCount[min]);		//	����� �ܾ� ������ ����
				}
			}

			else if (2 == R % 3) {	//	2 5 8 ...
				reverse(lines.begin(), lines.end());
				reverse(wordCount.begin(), wordCount.end());
			}

			else {	//	0 3 6 ...
				lines = temp;
				wordCount = int_temp;
				temp.clear();
				int_temp.clear();
			}

			printResult(lines);
			cout << R << '\n';
			break;

		case 's':
		{
			string sWord;
			int count = 0;

			cout << "�ٲ� �ܾ�: ";
			cin >> sWord;

			for (auto& a : lines) {
				istringstream iss(a);
				
				while (iss >> word) {
					if (compareWord(sWord, word)) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
						++count;
					}

					else	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
										
					cout << word << ' ';
				}

				cout << '\n';
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << "Total: " << count << " (" << sWord << ")" << '\n';
			break;
		}

		case 'q':
			exit(0);
			break;
		}
	}
}

void printResult(vector<string> vec)
{
	for (const auto& a : vec)
		cout << a << '\n';
}

bool compareWord(const string& str1, const string& str2)
{
	if (str1.size() != str2.size())	//	ũ�Ⱑ �ٸ��� false
		return false;
		
	for (int i = 0; i < str1.size(); ++i) {
		if (tolower(str1[i]) != tolower(str2[i])) //	���ڰ� �ٸ��� false
			return false;
		
	}

	return true;
}