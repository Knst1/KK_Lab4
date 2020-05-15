#include <iostream>
#include "parser.h"

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	cout << "¬ведите выражение:" << endl;
	string str;
	getline(cin, str);
	try
	{
		Parser p(str);
		p.handle();
	}
	catch (const exception& err) {
		cerr << err.what() << endl;
	}
	system("pause");
	return 0;
}
