#include<string>
#include<codecvt>
#include<regex>
using namespace std;
class Address{
	private:
		int level;
		wstring adr;
		wstring province, city, area, county, road, house_number, detail;
		wstring name, phone_number;
	public:
		Address(){}
		Address(string s) {
			adr = UTF8ToUnicode(s);
		}

};
string UnicodeToUTF8(const wstring & wstr)
{
	string ret;
	wstring_convert< codecvt_utf8<wchar_t> > wcv;
	ret = wcv.to_bytes(wstr);
	return ret;
}
wstring UTF8ToUnicode(const string & str)
{
	wstring ret;
	wstring_convert< codecvt_utf8<wchar_t> > wcv;
	ret = wcv.from_bytes(str);
	return ret;
}