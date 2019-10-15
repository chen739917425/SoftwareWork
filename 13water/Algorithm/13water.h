#include<iostream>
#include<vector>
#define sz(x) int(x.size())
#define fi first
#define se second
#define dd(x) cout<<#x<<" = "<<x<<" "
#define de(x) cout<<#x<<" = "<<x<<"\n"
using namespace std;
typedef vector<pair<int,int>> cards;
bool MostDragon(cards& card){
	for (int i=1;i<sz(card);++i)
		if (card[i].se!=card[i-1].se)
			return 0;
	return 1;
}
bool Dragon(cards card){
	sort(card.begin(),card.end());
	for (int i=1;i<sz(card);++i)
		if (card[i].fi-card[i-1].fi!=1)
			return 0;
	return 1;
}
bool TwelveRoyal(cards& card){
	int cnt=0;
	for (auto i:card)
		cnt+=i.fi>=10;
	return cnt==12;
}
bool AllBig(card& card){
	for (auto i:card)
		if (i.fi<8)
			return 0;
	return 1;
}
bool AllSmall(card& card){
	for (auto i:card)
		if (i.fi>8)
			return 0;
	return 1;
}
bool TripleBoom(card& card){
	int cnt[20]={0};
	for (auto i:card)
		cnt[i.fi]++;
	for (int i=2;i<=14;++i)
		cnt[0]+=cnt[i]==4;
	return cnt[0]==3;
}
bool OneColor(card& card){
	int cnt[2]={0};
	for (auto i:card)
		cnt[i.fi<=2]++;
	return !cnt[0]||!cnt[1];
}
void cal(cards card){
	
