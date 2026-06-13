#include <bits/stdc++.h>
using namespace std;
#define ll long long

int n, len1, gx, gy;
struct node {
    ll x, y;
    int cnt;
} a[45];
ll ans[45], all[21];
vector<node> p1, p2, cur;

bool cmp(const node &a, const node &b) {
    if(a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

	cin>>n>>gx>>gy;
	for(int i=1; i<=n; i++) {
		cin >> a[i].x >> a[i].y;
	}
    len1 = n / 2;
    p1.push_back({0, 0, 0});
    for (int i = 1; i <= len1; ++i){
        cur.clear();
        for (auto p : p1){
            cur.push_back({p.x + a[i].x, p.y + a[i].y, p.cnt + 1});
        }

        for (auto c : cur){
            p1.push_back(c);
        }
    }
	// ×ó°ë±ßËùÓÐ×´Ì¬
    // TODO

    p2.push_back({gx, gy, 0});

    for (int i = len1 + 1; i <= n; ++i){
        cur.clear();
        for (auto p : p2){
            cur.push_back({p.x - a[i].x, p.y - a[i].y, p.cnt + 1});
        }

        for (auto c : cur){
            p2.push_back(c);
        }
    }
    //ÓÒ°ë±ßËùÓÐ×´Ì¬
    // TODO

    sort(p1.begin(), p1.end(), cmp);
    sort(p2.begin(), p2.end(), cmp);
    node pre = {-1, -1, 0};
    int pos = 0;
    for (auto p : p1) {
        if (p.x != pre.x || p.y != pre.y) {
            memset(all, 0, sizeof(all));
            for (; pos < p2.size() && (p2[pos].x < p.x || (p2[pos].x == p.x && p2[pos].y < p.y)); pos++) {}
            for (; pos < p2.size() && p2[pos].x == p.x && p2[pos].y == p.y; pos++) {
                all[p2[pos].cnt]++;
            }
        }
        for (int i = 0; i <= 20; i++) { // Ã¶¾ÙÓÒ°ë±ßÖ¸ÁîÊý
            ans[p.cnt + i] += all[i];
        }
        pre = p;
    }

    for (int i = 1; i <= n; i++) {
		cout<<ans[i]<<endl;
	}

	return 0;
}