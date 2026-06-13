#include <iostream>
#include <vector>
#include <string>

using namespace std;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= 1000000007;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % 1000000007;
        base = (base * base) % 1000000007;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n) {
    return power(n, 1000000007 - 2);
}

long long nCr(int n, int r, const vector<long long>& fact, const vector<long long>& invFact) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % 1000000007 * invFact[n - r] % 1000000007;
}

int main() {
    int K;
    int N;
    long long L;
    if (!(cin >> K >> N >> L)) return 0;
    
    string T;
    cin >> T;

    // Precompute factorials for nCr
    // Max needed N is 10^6, but we might need more if Os accumulate?
    // Actually, at any point, max Os we can have is bounded?
    // Worst case: T is O...OM...M. All Os then all Ms.
    // Max Os is N. So we need factorials up to N.
    
    vector<long long> fact(N + 1);
    vector<long long> invFact(N + 1);
    fact[0] = 1;
    invFact[0] = 1;
    for (int i = 1; i <= N; i++) {
        fact[i] = (fact[i - 1] * i) % 1000000007;
        invFact[i] = modInverse(fact[i]);
    }

    long long waysT = 1;
    int current_Os = 0;

    // Process T from right to left
    for (int i = N - 1; i >= 0; i--) {
        if (T[i] == 'O') {
            current_Os++;
        } else if (T[i] == 'M') {
            if (current_Os < K) {
                // This should not happen based on problem guarantee for S,
                // and our deduction that T must be balanced.
                // However, if T is NOT balanced but S is valid, that's tricky.
                // But S = T^L. If T has net negative Os, S will eventually fail.
                // If T has net positive Os, S will eventually fail or have leftovers.
                // Given "guaranteed that the number of decompositions of S is nonzero",
                // T must be balanced.
                waysT = 0;
                break;
            }
            long long combinations = nCr(current_Os, K, fact, invFact);
            waysT = (waysT * combinations) % 1000000007;
            current_Os -= K;
        }
    }

    cout << power(waysT, L) << endl;

    return 0;
}
