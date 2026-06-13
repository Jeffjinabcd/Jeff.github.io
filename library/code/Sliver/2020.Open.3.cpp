#include <bits/stdc++.h>
using namespace std;

pair<int, int> a[100001];
int N, cnt;
int minh[100001];
int maxh[100001];
int main()
{
	freopen("moop.in", "r", stdin);
	freopen("moop.out", "w", stdout);
	scanf("%d", &N);
	for (int i = 1; i <= N; i++)
		scanf("%d%d", &a[i].first, &a[i].second);
	sort(a + 1, a + N + 1);

	for (int i = 1; i <= N; i++)
	{
		cnt++;
		minh[cnt] = a[i].second;
		maxh[cnt] = a[i].second;
		while (cnt > 1 && maxh[cnt] >= minh[cnt - 1]) {
			maxh[cnt - 1] = max(maxh[cnt], maxh[cnt - 1]);
			minh[cnt - 1] = min(minh[cnt], minh[cnt - 1]);
			cnt--;
		}
	}
	printf("%d\n", cnt);
	return 0;
}
