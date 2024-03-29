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
		wstring suf;						//suf="省市区县镇乡街道号路街巷"
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
		void get_county();					// 提取区/县/县级市
		void get_area();					// 提取街道/镇/乡
		void get_detail();					// 提取详细地址
		void parse();						// 解析地址
		string toJson();					// 以json格式输出
		Address() {
			init();
		}
		Address(string s) {
			init();
			adr = UTF8ToUnicode(pick_PhoneNumber(s));
			parse();
		}
		Address& operator = (const string& s) {
			this->adr = UTF8ToUnicode(pick_PhoneNumber(s));
			parse();
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
		if (pos != adr.npos&&pos == 0) {
			isMunicipality = true;
			province = adr.substr(pos, i.first.size());
			if (adr[pos + i.first.size()] == suf[1])
				adr.erase(pos, i.first.size() + 1);
			else
				adr.erase(pos, i.first.size());
			return;
		}
	}
	for (auto i : Province) {
		int pos=adr.find(i.first);
		if (pos != adr.npos&&pos == 0) {
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
				if (pos != adr.npos&&pos == 0) {
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
void Address::get_county() {
	int pos = min(adr.find(suf[1]), min(adr.find(suf[2]), adr.find(suf[3])));
	if (pos != adr.npos) {
		county = adr.substr(0, pos + 1);
		adr.erase(0, pos + 1);
	}
	else
		county = L"";
}
void Address::get_area() {
	int pos;
	pos = adr.find(suf.substr(6, 2));
	if (pos != adr.npos) {
		area = adr.substr(0, pos + 2);
		adr.erase(0, pos + 2);
		return;
	}
	pos = min(adr.find(suf[4]), adr.find(suf[5]));
	if (pos != adr.npos) {
		area = adr.substr(0, pos + 1);
		adr.erase(0, pos + 1);
		return;
	}
	area = L"";
}
void Address::get_detail() {
	if (level == 1) {
		detail = adr;
		detail.pop_back();
	}
	else {
		int ed = adr.find(suf[8]); 
		if (ed != adr.npos) {
			int st = ed-1;
			while (st >= 0 && iswdigit(adr[st]))
				--st;
			road = adr.substr(0, st + 1);
			house_number = adr.substr(st + 1, ed - st);
			adr.erase(0, ed + 1);
		}
		else {
			house_number = road = L"";
			for (int i = 9; i <= 11; ++i) {
				int pos = adr.find(suf[i]);
				if (pos != adr.npos) {
					road = adr.substr(0, pos + 1);
					adr.erase(0, pos + 1);
					break;
				}
			}
		}
		detail = adr;
		detail.pop_back();
	}
}
void Address::parse() {
	get_level();
	get_name();
	get_pro();
	get_city();
	get_county();
	get_area();
	get_detail();
}
string Address::toJson() {
	wstring res = L"{\"姓名\":\"" + name + L"\",\"手机\":\"" + phone_number +
		L"\",\"地址\":[\"" + province + L"\"," + L"\"" + city + L"\"," + L"\"" + county + L"\"," + L"\"" + area + L"\",";
	if (level > 1)
		res += L"\"" + road + L"\"," + L"\"" + house_number + L"\",";
	res += L"\"" + detail + L"\"]}";
	return UnicodeToUTF8(res);
}