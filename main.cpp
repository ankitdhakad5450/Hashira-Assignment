#include <bits/stdc++.h>
using namespace std;

// Convert number in given base to decimal
long long baseToDecimal(string value, int base) {
    long long num = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else digit = 10 + (tolower(c) - 'a'); // for hex/base>10
        num = num * base + digit;
    }
    return num;
}

// Lagrange Interpolation to find f(0)
long long lagrangeInterpolation(vector<pair<long long,long long>> shares) {
    long double secret = 0;
    int n = shares.size();

    for (int i = 0; i < n; i++) {
        long double xi = shares[i].first;
        long double yi = shares[i].second;
        long double li = 1;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                long double xj = shares[j].first;
                li *= (0 - xj) / (xi - xj);
            }
        }
        secret += yi * li;
    }
    return llround(secret);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line, json;
    while (getline(cin, line)) {
        json += line;
    }

    // Remove spaces for easier parsing
    json.erase(remove_if(json.begin(), json.end(), ::isspace), json.end());

    // Extract n and k
    size_t pos_n = json.find("\"n\":");
    size_t pos_k = json.find("\"k\":");
    int n = stoi(json.substr(pos_n + 4, json.find(",", pos_n) - (pos_n + 4)));
    int k = stoi(json.substr(pos_k + 4, json.find("}", pos_k) - (pos_k + 4)));

    vector<pair<long long,long long>> shares;

    // Parse shares manually
    for (int id = 1; id <= n; id++) {
        string key = "\"" + to_string(id) + "\":{";
        size_t pos = json.find(key);
        if (pos == string::npos) continue;

        size_t basePos = json.find("\"base\":\"", pos) + 8;
        size_t baseEnd = json.find("\"", basePos);
        int base = stoi(json.substr(basePos, baseEnd - basePos));

        size_t valPos = json.find("\"value\":\"", baseEnd) + 9;
        size_t valEnd = json.find("\"", valPos);
        string value = json.substr(valPos, valEnd - valPos);

        long long decValue = baseToDecimal(value, base);
        shares.push_back({id, decValue});
    }

    // Pick first k shares
    vector<pair<long long,long long>> subset;
    for (int i = 0; i < k && i < shares.size(); i++) {
        subset.push_back(shares[i]);
    }

    cout << "Reconstructed Secret Code (c) = "
         << lagrangeInterpolation(subset) << "\n";

    return 0;
}
