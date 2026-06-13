#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
struct node {
	int x, y;
};
int dx[4] = { -1, 1, 0, 0 }, dy[4] = { 0, 0, -1, 1 };
int N, D, ans;
int dis[1002][1002];
int w[1002][1002];
char ch[1002][1002];

int main()
{
	scanf("%d%d", &N, &D);
	for (int i = 1; i <= N; i++)
		scanf("%s", ch[i] + 1);
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			dis[i][j] = (ch[i][j] == '#' ? 0 : INF);

	// 预处理，对于每个空格, 它距离最近的障碍格的曼哈顿距离的最小值
	// TODO

	queue<node> q;
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
		{
			for (int i = 1; i <= N; i++) {
				for (int j = 2; j <= N; j++)
					dis[i][j] = min(dis[i][j], dis[i][j - 1] + 1);
				for (int j = N - 1; j > 0; j--)
					dis[i][j] = min(dis[i][j], dis[i][j + 1] + 1);
			}
		}

	// 通过BFS求出, 对于地图上的每个空地(x, y), 想要把菱形的中心移动到(x, y),
	// 最少需要几次移动操作
	while (!q.empty())
	{
		node cur = q.front();
		q.pop();
		int x = cur.x, y = cur.y;
		// TODO
	}z

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++) 
		{
			if (w[i][j] == -1)
				dis[i][j] = 0;
		}

	// 以(i, j)为中心的最大的不撞上障碍物的菱形
	// TODO

    for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			if (dis[i][j])
				ans++;
	printf("%d\n", ans);
	return 0;
}
