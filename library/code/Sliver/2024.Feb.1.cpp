#include <bits/stdc++.h>
using namespace std;
#define ll long long

bool pair_comp_first(pair<ll, ll>& a, pair<ll, ll>& b) {
    return a.first < b.first;
}

bool pair_comp_second(pair<ll, ll>& a, pair<ll, ll>& b) {
    return a.second < b.second;
}

bool is_feasible(ll current_y, vector<pair<ll, ll>>& corners, vector<ll>& slopes) {
    vector<ll> max_slopes;
    for (auto& corner : corners) {
        max_slopes.push_back((corner.second - current_y) / corner.first);
    }
    sort(max_slopes.begin(), max_slopes.end());
    for (int i = 0; i < slopes.size(); i++){
        if (slopes[i] > max_slopes[i])
            return false;
    }
    return true;
}

ll find_min_y(vector<pair<ll, ll>>& corners4PositiveSlopes, vector<ll>& positiveSlopes) {
    sort(positiveSlopes.begin(), positiveSlopes.end());
    ll min_y = min_element(corners4PositiveSlopes.begin(),corners4PositiveSlopes.end(), pair_comp_second)->second;

    ll max_x = max_element(corners4PositiveSlopes.begin(), corners4PositiveSlopes.end(), pair_comp_first)->first;

    ll hi = min_y;
    ll lo = min_y - positiveSlopes.back() * max_x;
    while (lo < hi) {
        ll mid = (lo + hi + 1) / 2;
        if (is_feasible(mid, corners4PositiveSlopes, positiveSlopes)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

ll find_max_y(vector<pair<ll, ll>>& corners4NegativeSlopes, vector<ll>& negativeSlopes) {
    vector<pair<ll, ll>> new_corners4NegativeSlopes;
    for (auto& target : corners4NegativeSlopes) {
        new_corners4NegativeSlopes.push_back({target.first, -target.second});
    }
    vector<ll> new_positiveSlopes;
    for (auto& s: negativeSlopes) {
        new_positiveSlopes.push_back(-s);
    }
    return -find_min_y(new_corners4NegativeSlopes, new_positiveSlopes);
}

int main() {
    int T;
    cin >> T;

    while (T--) {
        int N;
        ll x1;
        cin >> N >> x1;
        vector<ll> y_coordinates(2*N);
        vector<pair<ll, ll>> corners4PositiveSlopes, corners4NegativeSlopes;
        vector<ll> slopes(4 * N);

        for (int i = 0; i < N; ++i) {
            ll y1, y2, x2;
            cin >> y1 >> y2 >> x2;
            y_coordinates[2*i] = y1;
            y_coordinates[2*i+1] = y2;
            corners4PositiveSlopes.push_back({x2, y1});
            corners4NegativeSlopes.push_back({x2, y2});
        }

        for (int i = 0; i < 4 * N; ++i) {
            cin >> slopes[i];
        }

        vector<ll> negativeSlopes, positiveSlopes;
        for (ll s : slopes) {
            if (s < 0) {
                negativeSlopes.push_back(s);
            } else {
                positiveSlopes.push_back(s);
            }
        }
        if (negativeSlopes.size() < N || positiveSlopes.size() < N) {
            cout << -1 << endl;
            continue;
        }
        sort(y_coordinates.begin(), y_coordinates.end());
        for (ll y : y_coordinates) {
            if (corners4NegativeSlopes.size() < negativeSlopes.size()) {
                corners4NegativeSlopes.push_back({x1, y});
            } else {
                corners4PositiveSlopes.push_back({x1, y});
            }
        }
        if (corners4NegativeSlopes.size() != negativeSlopes.size() ||
            corners4PositiveSlopes.size() != positiveSlopes.size() ) {
                cout << -1 << endl;
                continue;
        }

        ll y_min = find_min_y(corners4PositiveSlopes, positiveSlopes);
        ll y_max = find_max_y(corners4NegativeSlopes, negativeSlopes);

        cout << y_max - y_min << endl;
    }

    return 0;
}