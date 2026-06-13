#include <bits/stdc++.h>
using namespace std;

using namespace std;

struct Point
{
    int x, y;
};

double distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

const int MAX_N = 150;
const double INF = 1e20;
int n;
int field[MAX_N] = {0};
double diameter[MAX_N + 1] = {0};
double dist[MAX_N][MAX_N];

void dfs(int i, int id)
{
    field[i] = id;
    for (int j = 0; j < n; ++j)
        if (!field[j] && dist[i][j] < INF)
            dfs(j, id);
}

int main()
{
    int i, j, k;
    Point a[MAX_N];
    scanf("%d\n", &n);
    for (i = 0; i < n; ++i)
        scanf("%d %d\n", &a[i].x, &a[i].y);
    
    char s[MAX_N + 1];
    for (i = 0; i < n; ++i)
    {
        scanf("%s", s);
        for (j = 0; j < n; ++j)
        if (s[j] == '1' || i == j)
            dist[i][j] = distance(a[i], a[j]);
        else
            dist[i][j] = INF;
    }

    // 1. DFS
    int id = 0;
    for (i = 0; i < n; ++i)
        if (!field[i])
            dfs(i, ++id);
    
    // 2. Floyd-Warshall
    for (k = 0; k < n; ++k)
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];

    // 3.4. max_sp: find max shortest path and diameter;
    double max_sp[MAX_N];
    for (i = 0; i < n; ++i)
    {
        max_sp[i] = 0.0;
        for (j = 0; j < n; ++j)
            if (dist[i][j] < INF)
                max_sp[i] = max(max_sp[i], dist[i][j]);
        diameter[field[i]] = max(diameter[field[i]], max_sp[i]);
    }

    // 5. find answer
    double min_d = INF, max_d;
    for (i = 0; i < n; ++i)
        for (j = i + 1; j < n; ++j)
            if (field[i] != field[j])
            {
                max_d = max(max(diameter[field[i]], diameter[field[j]]),
                            max_sp[i] + distance(a[i], a[j]) + max_sp[j]);
                min_d = min(min_d, max_d);
            }

    printf("%.6f\n", min_d);

    return 0;
}