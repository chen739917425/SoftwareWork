#include<string>
#include<unordered_map>
#include<fstream>
#include<vector>
#include<codecvt>
#include<regex>
#include<iostream>
#define chs wcout.imbue(locale("chs"));
using namespace std;
string UnicodeToUTF8(const wstring & wstr){
	string ret;
	try {
		wstring_convert< codecvt_utf8<wchar_t> > wcv;
		ret = wcv.to_bytes(wstr);
	}
	catch (const exception & e) {
		cerr << e.what() << endl;
		exit(0);
	}
	return ret;
}
wstring UTF8ToUnicode(const string & str){
	wstring ret;
	try {
		wstring_convert< codecvt_utf8<wchar_t> > wcv;
		ret = wcv.from_bytes(str);
	}
	catch (const exception & e) {
		cerr << e.what() << endl;
		exit(0);
	}
	return ret;
}
unordered_map< wstring, vector<wstring> > Province, Municipality;
class Address{
	private:
		wstring suf;
		int level;
		bool isMunicipality;
		wstring adr;
		wstring province, city, area, county, road, house_number, detail;
		wstring name, phone_number;
	public:
		void init();
		string pick_PhoneNumber(string s);	// 提取电话号码
		void get_level();					// 提取地址的级数
		void get_name();					// 提取姓名
		void get_pro();						// 提取省地址
		void get_city();					// 提取市地址
		void parse();						// 解析地址
		Address() {
			init();
		}
		Address(string s) {
			init();
			adr = UTF8ToUnicode(pick_PhoneNumber(s));
		}
		Address& operator = (const string& s) {
			this->adr = UTF8ToUnicode(pick_PhoneNumber(s));
			return *this;
		}
};
void Address::init() {
	ifstream fin("AddressData.txt");
	string s;
	wstring ws;
	fin >> s;
	suf = UTF8ToUnicode(s);
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
string Address::pick_PhoneNumber(string s) {
	regex pattern("\\d{11}");
	smatch result;
	regex_search(s, result, pattern);
	phone_number = UTF8ToUnicode(result[0]);
	s=regex_replace(s, pattern, "");
	return s;
}
void Address::get_level() {
	level = stoi(adr);
	adr = adr.substr(2);
}
void Address::get_name() {
	int pos = adr.find(L",");
	name = adr.substr(0, pos);
	adr.erase(0, pos+1);
}
void Address::get_pro() {
	for (auto i : Municipality) {
		int pos = adr.find(i.first);
		if (pos != adr.npos) {
			isMunicipality = true;
			province = adr.substr(pos, i.first.size());
			if (adr[pos + i.first.size()] == suf[1])
				adr.erase(pos, i.first.size() + 1);
			else
				adr.erase(pos, i.first.size());
		}
	}
	for (auto i : Province) {
		int pos=adr.find(i.first);
		if (pos != adr.npos) {
			isMunicipality = false;
			province = adr.substr(pos, i.first.size());
			if (province.size() <= 3)
				province.push_back(suf[0]);
			if (adr[pos + i.first.size()] == suf[0])
				adr.erase(pos, i.first.size() + 1);
			else
				adr.erase(pos, i.first.size());
			return;
		}
	}
	province = L"";
}
void Address::get_city() {
	if (isMunicipality) {
		city = province;
		city.push_back(suf[0]);
	}
	else {
		for (auto i : Province[province]) {
			int pos = adr.find(i);
			if (pos != adr.npos) {
				city = adr.substr(0, i.size());
			}
		}
	}
}
void Address::parse() {
	wcout << suf << endl;
	get_level();
	get_name();
	get_pro();
		wcout << level << " " << name << " " << province << " " << adr;
}