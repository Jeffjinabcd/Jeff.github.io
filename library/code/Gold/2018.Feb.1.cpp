#include <bits/stdc++.h>
using namespace std;

struct Tile {
    int depth;
    int id;
};

struct Query {
    int s, d;
    int id;
};

bool cmpTile(const Tile& a, const Tile& b) {
    return a.depth > b.depth;
}

bool cmpQuery(const Query& a, const Query& b) {
    return a.s > b.s;
}

int L[100005], R[100005];
int ans[100005];
Tile tiles[100005];
Query queries[100005];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    freopen("snowboots.in", "r", stdin);
    freopen("snowboots.out", "w", stdout);

    int N, B;
    if (!(cin >> N >> B)) return 0;

    for (int i = 1; i <= N; ++i) {
        cin >> tiles[i].depth;
        tiles[i].id = i;
    }

    // Sort tiles by depth descending, but we need to keep track of their original positions
    // Actually, we process tiles by depth. The tiles array will store (depth, original_index).
    // But we need to remove them from the linked list which is based on original indices.
    // So sorting `tiles` array is correct.
    sort(tiles + 1, tiles + N + 1, cmpTile);

    for (int i = 0; i < B; ++i) {
        cin >> queries[i].s >> queries[i].d;
        queries[i].id = i;
    }

    sort(queries, queries + B, cmpQuery);

    // Initialize linked list
    // L[i] is the valid tile to the left of i
    // R[i] is the valid tile to the right of i
    for (int i = 1; i <= N; ++i) {
        L[i] = i - 1;
        R[i] = i + 1;
    }
    
    // Initially all tiles are valid (conceptually infinite snow boot depth)
    // The max gap is 1 because all adjacent tiles are reachable from each other
    int max_gap = 1;
    int tile_idx = 1; // Pointer to the sorted tiles array

    for (int i = 0; i < B; ++i) {
        int s = queries[i].s;
        int d = queries[i].d;

        // Remove tiles that have depth > s
        while (tile_idx <= N && tiles[tile_idx].depth > s) {
            int pos = tiles[tile_idx].id;
            
            // We can't remove the first or last tile as per problem statement f_1 = f_N = 0
            // and s >= 0. But the loop condition tiles[tile_idx].depth > s handles it 
            // because f_1=0 and f_N=0 will never be > s (since s >= 0).
            // Wait, if s < 0? Problem says s_i >= 0. So 0 > s is false.
            // So pos will never be 1 or N.

            // Update neighbors
            int left = L[pos];
            int right = R[pos];
            
            R[left] = right;
            L[right] = left;

            // Update max_gap
            // The new gap created is between 'left' and 'right'
            max_gap = max(max_gap, right - left);

            tile_idx++;
        }

        if (max_gap <= d) {
            ans[queries[i].id] = 1;
        } else {
            ans[queries[i].id] = 0;
        }
    }

    for (int i = 0; i < B; ++i) {
        cout << ans[i] << "\n";
    }

    return 0;
}
