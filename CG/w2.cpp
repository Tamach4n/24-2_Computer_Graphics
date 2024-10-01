//	파일에서 문자열 읽기
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

		if (is.is_open())	break;	//	열렸을 경우

		cerr << Fname << " 찾을 수 없음." << '\n';	//	cerr - 표준 에러 메세지 출력 스트림: 출력 시간 짧음
		is.clear();
	}

	string line, word;
	vector<string> lines;
	vector<int> wordCount;

	while (getline(is, line)) {		//	문장 단위로 읽기
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
		cout << "c: 대문자로 시작되는 단어를 찾아 그 단어를 다른 색으로 출력하고, 몇 개 있는지를 계산하여 출력한다. 다시 누르면 원래대로 출력한다." << '\n';
		cout << "d: 문장 한 줄 전체를 거꾸로 출력하기. 다시 누르면 원래대로 출력한다." << '\n';
		cout << "e: 3문자 후 2개의 @ 문자 삽입하기. 다시 누르면 원래대로 출력한다." << '\n';
		cout << "f: 공백을 기준으로 모든 단어들을 거꾸로 출력하기. 다시 누르면 원래대로 출력한다." << '\n';
		cout << "g: 문자 내부의 특정 문자를 다른 문자로 바꾸기 (바꿀 문자와 새롭게 입력할 문자 입력 받음) 다시 누르면 원래대로 출력한다." << '\n';
		cout << "h: 각 줄의 단어의 개수를 출력한다." << '\n';
		cout << "r: 한 문장의 단어의 개수에 따라 오름차순으로 정렬하여 문장들을 출력한다. 다시 누르면 내림차순으로 출력한다. 다시 누르면 원래대로 출력한다. (즉, 오름차순 → 내림차순 → 원래대로 → 오름차순 …)" << '\n';
		cout << "s: 명령어와 단어를 입력하면, 입력받은 문장들을 모두 출력하면서 그 단어를 찾아 다른 색으로 출력하고, 몇 개 있는지를 계산하여 출력한다. (대소문자 구분하지 않는다)" << '\n';
		cout << "q: 프로그램 종료" << '\n';
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
					temp.push_back(a);	//	원본 복사

					int pos = 2;

					while (pos < a.size()) {
						a.insert(pos + 1, 2, '@');	//	pos+1 위치에 @를 2번 삽입
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
						if (word.back() == '.')	//	마지막 온점은 그대로 둠
							reverse(word.begin(), word.end() - 1);

						else
							reverse(word.begin(), word.end());

						vWord.push_back(word);	//	반전된 단어 벡터에 삽입
					}

					string result;

					for (const auto& a : vWord)
						result += a + ' ';		//	단어와 공백 합쳐 문장 만들기

					if (!result.empty())
						result.pop_back();		//	문장 끝 공백 제거

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
				cout << "무엇을: ";
				cin >> A;
				cout << "무엇으로: ";
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

				//	선택 정렬
				for (int i = 0; i < lines.size() - 1; ++i) {
					int min = i;	//	최소

					for (int j = i; j < lines.size(); ++j) {
						if (wordCount[j] < wordCount[min])
							min = j;
					}

					swap(lines[i], lines[min]);
					swap(wordCount[i], wordCount[min]);		//	문장과 단어 순서도 변경
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

			cout << "바꿀 단어: ";
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
	if (str1.size() != str2.size())	//	크기가 다르면 false
		return false;
		
	for (int i = 0; i < str1.size(); ++i) {
		if (tolower(str1[i]) != tolower(str2[i])) //	문자가 다르면 false
			return false;
		
	}

	return true;
}