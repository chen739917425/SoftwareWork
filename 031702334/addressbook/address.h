#include<string>
#include<unordered_map>
#include<fstream>
#include<vector>
#include<codecvt>
#include<regex>
#include<iostream>
using namespace std;
unordered_map< wstring, vector<wstring> > Province, Municipality;
string UnicodeToUTF8(const wstring & wstr){
	string ret;
	wstring_convert< codecvt_utf8<wchar_t> > wcv;
	ret = wcv.to_bytes(wstr);
	return ret;
}
wstring UTF8ToUnicode(const string & str){
	wstring ret;
	wstring_convert< codecvt_utf8<wchar_t> > wcv;
	ret = wcv.from_bytes(str);
	return ret;
}
class Address{
	private:
		int level;
		bool isMunicipality;
		wstring adr;
		wstring province, city, area, county, road, house_number, detail;
		wstring name, phone_number;
	public:
		void init();
		Address(string s) {
			init();
			adr = UTF8ToUnicode(s);
		}
};
void Address::init() {
	ifstream fin("AddressData.txt");
	string s;
	wstring ws;
	for (int i = 0; i < 4; ++i) {
		fin >> s;
		ws = UTF8ToUnicode(s);
		while (fin >> s && s != "#")
			Municipality[ws].push_back(UTF8ToUnicode(s));
	}
	while (fin >> s) {
		ws = UTF8ToUnicode(s);
		while (fin >> s && s != "#")
			Province[ws].push_back(UTF8ToUnicode(s));
	}
}