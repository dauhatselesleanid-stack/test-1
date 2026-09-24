#include <iostream>
#include <vector>

using namespace std;

int LowerBound(const vector<int>& a, int x) {
    int q = 0;
    int r = a.size();
    while (q < r) {
        int k = (q + r) / 2;
        if (x <= a[k]) {
            r = k;
        } else {
            q = k + 1;
        }
    }
    return q;
}

int UpperBound(const vector<int>& a, int x) {
    int q = 0;
    int r = a.size();
    while (q < r) {
        int k = (q + r) / 2;
        if (x < a[k]) {
            r = k;
        } else {
            q = k + 1;
        }
    }
    return q;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int k_requests;
    cin >> k_requests;

    for (int i = 0; i < k_requests; ++i) {
        int x;
        cin >> x;


        int l = LowerBound(a,x);
        int r = UpperBound(a, x);
        
        int b = (l < n && a[l] == x) ? 1 : 0;

        cout << b << " " << l << " " << r << "\n";
    }

    return 0;
}