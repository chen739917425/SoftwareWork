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
unordered_map< wstring, bool > isCity;
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
		string toJson();					// 以json格式输出
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
	ifstream fin;
	fin.open("AddressData.txt");
	if (!fin.is_open()) {
		cout << "AddressData file open failed!\nymzsb!";
		exit(0);
	}
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
	fin.close();
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
			province = i.first;
			if (province.size() <= 3)
				province.push_back(suf[0]);
			if (adr[pos + i.first.size()] == suf[0])
				adr.erase(pos, i.first.size() + 1);
			else
				adr.erase(pos, i.first.size());
			return;
		}
	}
	isMunicipality = false;
	province = L"";
}
void Address::get_city() {
	if (isMunicipality) {
		city = province;
		city.push_back(suf[1]);
	}
	else {
		if (province.size()) {
			wstring pro = province;
			if (pro.back() == suf[0])
				pro.pop_back();
			for (auto i : Province[pro]) {
				wstring tmp = i;
				if (tmp.back() == suf[1])
					tmp.pop_back();
				int pos = adr.find(tmp);
				if (pos != adr.npos) {
					city = i;
					if (adr[pos + tmp.size()] == suf[1])
						adr.erase(pos, tmp.size() + 1);
					else
						adr.erase(pos, tmp.size());
					return;
				}
			}
			city = L"";
		}
		else {
			for (auto i:Province)
				for (auto j : i.second) {
					wstring tmp = j;
					if (tmp.back() == suf[1])
						tmp.pop_back();
					int pos = adr.find(tmp);
					if (pos != adr.npos) {
						city = j;
						if (adr[pos + tmp.size()] == suf[1])
							adr.erase(pos, tmp.size() + 1);
						else
							adr.erase(pos, tmp.size());
						return;
					}
				}
			city = L"";
		}
	}
}
void Address::parse() {
	get_level();
	get_name();
	get_pro();
	get_city();
	wcout << level << " " << name << " " << province << " " << city << "\n" << adr;
}
string Address::toJson() {
	string res;
	return res;
}