#include"address.h"
int main() {
	chs;
	ifstream fin("Data/1.txt");
	string s;
	fin >> s;
	Address a;
	a = s;
	a.parse();
	/*
	ifstream fin("AddressData.txt");
	ofstream fout("2.txt");
	string s;
	wstring ws;
	fin >> s;
	fout << s<<endl;
	while (fin >> s) {
		fout << s << endl;
		int f = 0;
		while (fin >> s && s[0] != '#') {
			ws = UTF8ToUnicode(s);
			if (ws.back() == L'ÊÐ')
				ws.pop_back();
			s = UnicodeToUTF8(ws);
			if (f)
				fout << " " << s;
			else {
				fout << s;
				f = 1;
			}
		}
		fout <<endl<< "#" << endl;
	}
	*/
	return 0;
}