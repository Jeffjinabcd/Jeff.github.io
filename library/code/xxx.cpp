#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>


using namespace std;

typedef long long ll;

const ll INF = 4e18; // Larger than any possible coordinate difference

struct Interval {
  ll l, r;
  int id;
};

struct Event {
  ll pos;
  int type;   // 0 for start, 1 for end
  int is_cow; // 1 for cow, 0 for package
  int id;

  bool operator<(const Event &other) const {
    if (pos != other.pos)
      return pos < other.pos;
    return type < other.type; // Process Start (0) before End (1)
  }
};

ll M;
int N, P;
vector<Interval> cow_intervals;
vector<Interval> pkg_intervals;

// Helper for integer division
ll floor_div(ll a, ll b) {
  return a / b - ((a % b != 0) && ((a < 0) ^ (b < 0)));
}

ll ceil_div(ll a, ll b) {
  return a / b + ((a % b != 0) && ((a > 0) ^ (b > 0)));
}

// Count points in [L, R] with phase phi modulo M
ll count_points(ll L, ll R, ll phi) {
  if (L > R)
    return 0;
  // k * M + phi >= L  =>  k * M >= L - phi  => k >= ceil((L-phi)/M)
  // k * M + phi <= R  =>  k * M <= R - phi  => k <= floor((R-phi)/M)
  ll k_min = ceil_div(L - phi, M);
  ll k_max = floor_div(R - phi, M);
  if (k_max < k_min)
    return 0;
  return k_max - k_min + 1;
}

// Distance from phase p to nearest phase in set phases
ll get_min_dist(ll p, const multiset<ll> &phases) {
  if (phases.empty())
    return INF;
  auto it = phases.lower_bound(p);
  ll d = INF;
  if (it != phases.end()) {
    d = min(d, *it - p);
  } else {
    d = min(d, *phases.begin() + M - p);
  }

  if (it != phases.begin()) {
    d = min(d, p - *prev(it));
  } else {
    d = min(d, p - (*phases.rbegin() - M));
  }
  return d;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  if (!(cin >> M >> N >> P))
    return 0;

  cow_intervals.resize(N);
  for (int i = 0; i < N; ++i) {
    cin >> cow_intervals[i].l >> cow_intervals[i].r;
    cow_intervals[i].id = i;
  }

  pkg_intervals.resize(P);
  for (int i = 0; i < P; ++i) {
    cin >> pkg_intervals[i].l >> pkg_intervals[i].r;
    pkg_intervals[i].id = i;
  }

  vector<Event> events;
  for (int i = 0; i < N; ++i) {
    events.push_back({cow_intervals[i].l, 0, 1, i});
    events.push_back({cow_intervals[i].r, 1, 1, i});
  }
  for (int i = 0; i < P; ++i) {
    events.push_back({pkg_intervals[i].l, 0, 0, i});
    events.push_back({pkg_intervals[i].r, 1, 0, i});
  }

  sort(events.begin(), events.end());

  ll total_cost = 0;

  // State for sweep line
  multiset<ll> active_cow_phases;
  vector<int> active_pkgs; // List of IDs
  int active_cow_count = 0;

  // For Outside Gaps
  struct Fragment {
    ll l, r;
  };
  vector<Fragment> current_gap_fragments;
  ll last_cow_end = -INF; // Rightmost end of previous cow zone
  bool in_gap = true;     // Initially in gap before first cow

  // We need to track the "merged cow zones" to define gaps correctly.
  // A gap is when active_cow_count == 0.
  // However, we only update cost when moving.

  // Iterate through events
  for (size_t i = 0; i < events.size();) {
    ll curr_pos = events[i].pos;

    // Process all events at this position
    // But wait, we need to process the interval [prev_pos, curr_pos] first.
    // Actually, standard sweep line: process interval, then update state.
    // But the first event is at start.

    static ll prev_pos = events[0].pos;

    // If we moved, process the interval [prev_pos, curr_pos]
    if (curr_pos > prev_pos) {
      // Interval [prev_pos, curr_pos - 1] ?
      // No, the events are inclusive points.
      // If we have event at 10 and 20.
      // The interval is [10, 20].
      // But wait, if event at 10 is START, then 10 is included.
      // If event at 20 is END, then 20 is included.
      // So the range is [10, 20].
      // But we need to be careful about "between" logic.
      // Let's assume the state is valid for [prev_pos, curr_pos].
      // But events at curr_pos change the state.
      // So the state was valid for [prev_pos, curr_pos] EXCLUDING the effect of
      // events at curr_pos? No. Standard: State is valid for (prev_event_pos,
      // curr_event_pos). But we have closed intervals. Let's use: process
      // [prev_pos, curr_pos]. But we need to handle the exact points. Actually,
      // since coordinates are large, we just treat [prev_pos, curr_pos] as a
      // range. If active_cow_count > 0:
      //   It's an Inside zone.
      //   Calculate cost for all active packages in [prev_pos, curr_pos].
      //   Wait, if we process [prev_pos, curr_pos], we might double count the
      //   endpoints? We should process [prev_pos, curr_pos - 1] ? Or [prev_pos,
      //   curr_pos]? Let's look at the events. Start at L: active from L. End
      //   at R: active until R. So if we have Start at 10, End at 20. Events:
      //   (10, Start), (20, End). i=0: pos=10. prev=10. Loop doesn't run.
      //   Update state: active=1. i=1: pos=20. prev=10. Loop runs for [10, 20].
      //   active=1.
      //        Cost for [10, 20].
      //        Update state: active=0.
      //   This seems correct. The interval [10, 20] is fully covered.

      //   What if Start at 10, Start at 15?
      //   i=0: pos=10. active=1.
      //   i=1: pos=15. Loop [10, 15]. active=1.
      //        Update state: active=2.
      //   i=2: ...
      //   We processed [10, 15].
      //   Next we process [15, ...].
      //   We double counted 15?
      //   Yes.
      //   We should process [prev_pos, curr_pos].
      //   But we need to avoid double counting.
      //   The issue is discrete points.
      //   We can process [prev_pos, curr_pos - 1] if prev_pos < curr_pos.
      //   But then we miss curr_pos?
      //   Or we process [prev_pos + 1, curr_pos]?
      //   Let's just be precise.
      //   The state applies to the open interval (prev, curr).
      //   What about the points prev and curr?
      //   Events at prev determined the state starting at prev.
      //   So the state includes prev.
      //   Events at curr will change the state starting at curr.
      //   So the state applies to [prev_pos, curr_pos - 1].
      //   And what about curr_pos?
      //   It will be handled in the NEXT interval as the start?
      //   No, [curr, next-1].
      //   So yes, [prev, curr-1] is safe.
      //   But wait, if prev=10, curr=20. We do [10, 19].
      //   Next [20, ...].
      //   This covers everything.
      //   BUT, the event at 20 (End) means the interval includes 20.
      //   If we process [10, 19], we miss 20.
      //   So we should process [prev_pos, curr_pos] ?
      //   If we do [10, 20], and next is [20, 30]. We count 20 twice.
      //   We need to handle points carefully.
      //   Maybe [prev_pos, curr_pos - 1] is correct, but we need to handle the
      //   single point curr_pos? Or just use half-open intervals [L, R+1)? The
      //   problem uses closed intervals [L, R]. Let's convert everything to
      //   half-open [L, R+1). Cow: [L, R] -> [L, R+1). Pkg: [L, R] -> [L, R+1).
      //   Events: L (Start), R+1 (End).
      //   Then we process [prev, curr).
      //   This avoids double counting and handles adjacency correctly.
      //   Example: Cow [10, 20]. Events 10, 21.
      //   Process [10, 21).
      //   Points 10...20. Correct.
      //   Example: Cow [10, 10]. Events 10, 11.
      //   Process [10, 11). Point 10. Correct.

      //   So strategy: Convert to half-open.

      ll L_range = prev_pos;
      ll R_range = curr_pos; // Exclusive

      if (active_cow_count > 0) {
        // Inside Zone
        // If we were accumulating gap fragments, we need to stop?
        // No, if active_cow_count > 0, we are NOT in a gap.
        // But wait, if we just transitioned from 0 to 1?
        // Then the gap ended at prev_pos.
        // We should have processed the gap already.
        // We can do this by checking state BEFORE processing interval.
        // But active_cow_count is the state DURING [prev, curr).

        // Calculate Inside Cost
        for (int pkg_idx : active_pkgs) {
          ll phi = pkg_intervals[pkg_idx].l % M;
          ll dist = get_min_dist(phi, active_cow_phases);
          if (dist != INF) {
            // Count points in [L_range, R_range - 1] intersecting [PkgL, PkgR]
            ll start = max(L_range, pkg_intervals[pkg_idx].l);
            ll end = min(R_range - 1, pkg_intervals[pkg_idx].r);
            ll cnt = count_points(start, end, phi);
            total_cost += cnt * dist;
          }
        }

        // Update last_cow_end
        // The current interval [L, R) is covered by cows.
        // So the rightmost cow point is R_range - 1.
        // Actually, we just need to know where the cow zone ends.
        // It ends when active_cow_count drops to 0.
        last_cow_end = R_range - 1; // Tentative, will be updated as we go

      } else {
        // Outside Zone (Gap)
        // Collect fragments
        for (int pkg_idx : active_pkgs) {
          ll start = max(L_range, pkg_intervals[pkg_idx].l);
          ll end = min(R_range - 1, pkg_intervals[pkg_idx].r);
          if (start <= end) {
            current_gap_fragments.push_back({start, end});
          }
        }
      }
    }

    prev_pos = curr_pos;

    // Process events at curr_pos
    while (i < events.size() && events[i].pos == curr_pos) {
      int idx = events[i].id;
      if (events[i].is_cow) {
        if (events[i].type == 0) { // Start
          // If we were in a gap (count 0), the gap ends at curr_pos.
          if (active_cow_count == 0) {
            // Process the collected gap fragments
            // Gap is between last_cow_end and curr_pos (which is start of new
            // cow) Wait, curr_pos is start of cow interval. So the gap is
            // (last_cow_end, curr_pos). The fragments are in this range.

            // Logic for gap cost
            if (!current_gap_fragments.empty()) {
              // Sort fragments by coordinate (using l)
              // Actually, fragments might overlap?
              // Yes.
              // But for the "split" logic, we just need the set of points.
              // Actually, we have intervals.
              // We need to assign each interval to Left or Right.
              // Sort by center? Or just L?
              // If we split at index k, we assign first k to Left, rest to
              // Right. But "first k" implies an order. Order by position.
              sort(current_gap_fragments.begin(), current_gap_fragments.end(),
                   [](const Fragment &a, const Fragment &b) {
                     return a.l < b.l;
                   });

              ll gap_cost = INF;

              // Precompute suffix mins for Right cost
              // Right cost = L_next - min(a_i).
              // L_next is curr_pos.
              // We need min(a_i) for i > k.
              int m = current_gap_fragments.size();
              vector<ll> suffix_min_l(m + 1, INF);
              for (int j = m - 1; j >= 0; --j) {
                suffix_min_l[j] =
                    min(suffix_min_l[j + 1], current_gap_fragments[j].l);
              }

              // Iterate split point
              // k = -1 (all right) to m-1 (all left)
              ll current_max_r = -INF;

              for (int k = -1; k < m; ++k) {
                ll cost_left = 0;
                if (k >= 0) {
                  current_max_r =
                      max(current_max_r, current_gap_fragments[k].r);
                  // Cost is max_r - last_cow_end
                  // If last_cow_end is -INF (no left cow), cost is infinite?
                  // Yes, if no left cow, we MUST go right.
                  if (last_cow_end == -INF)
                    cost_left = INF;
                  else
                    cost_left = current_max_r - last_cow_end;
                }

                ll cost_right = 0;
                if (k < m - 1) {
                  ll min_l = suffix_min_l[k + 1];
                  // Cost is curr_pos - min_l
                  // If curr_pos is effectively infinite (no right cow)?
                  // But here we are at a Cow Start, so curr_pos is valid.
                  cost_right = curr_pos - min_l;
                }

                gap_cost = min(gap_cost, cost_left + cost_right);
              }
              total_cost += gap_cost;
              current_gap_fragments.clear();
            }
          }

          active_cow_count++;
          active_cow_phases.insert(cow_intervals[idx].l % M);
        } else { // End
          active_cow_count--;
          active_cow_phases.erase(
              active_cow_phases.find(cow_intervals[idx].l % M));
          if (active_cow_count == 0) {
            // Gap starts.
            // last_cow_end is curr_pos - 1.
            last_cow_end = curr_pos - 1;
          }
        }
      } else {                     // Package
        if (events[i].type == 0) { // Start
          // Add to active packages
          // We can use a vector and just push_back.
          // Removal is tricky.
          // Since N, P small, maybe just linear scan to remove?
          // Or use a boolean flag array?
          // P <= 20000. Vector of IDs is fine.
          active_pkgs.push_back(idx);
        } else { // End
          // Remove from active packages
          for (size_t j = 0; j < active_pkgs.size(); ++j) {
            if (active_pkgs[j] == idx) {
              active_pkgs[j] = active_pkgs.back();
              active_pkgs.pop_back();
              break;
            }
          }
        }
      }
      i++;
    }
  }

  // Handle trailing gap if any
  if (!current_gap_fragments.empty()) {
    // No right cow. All must go Left.
    if (last_cow_end == -INF) {
      // No cows at all? Problem says N >= 1.
      // But maybe all packages are to the left of first cow.
      // Handled in the loop when first cow starts.
      // This block is for packages to the RIGHT of last cow.
    }

    ll max_r = -INF;
    for (const auto &f : current_gap_fragments)
      max_r = max(max_r, f.r);

    if (last_cow_end != -INF) {
      total_cost += max_r - last_cow_end;
    } else {
      // Should not happen if N >= 1
    }
  }

  cout << total_cost << endl;

  return 0;
}
