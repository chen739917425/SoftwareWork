#include"address.h"
string toJsonArray(vector<string>& a) {
	string res="[";
	for (int i = 0; i < int(a.size()); ++i) {
		res += a[i];
		if (i + 1 < int(a.size()))
			res += ",";
	}
	res+="]";
	return res;
}
int main(int argv, char** argc)
{
	if (argv < 3)
		return 0;
	ifstream infile;
	infile.open("1.txt");
	ofstream outfile;
	outfile.open("2.txt");
	string s;
	Address adr;
	vector<string> res;
	while (infile >> s) {
		adr = s;
		res.push_back(adr.toJson());
	}
	outfile << toJsonArray(res);
	infile.close();
	outfile.close();
	return 0;
}