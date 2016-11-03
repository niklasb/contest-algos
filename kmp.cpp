#include <bits/stdc++.h>
using namespace std;

// f[i] = maximum length of suffix of s[1..i] that is prefix of s
void kmp(const char *s, int *f, int n) {
    int t = -1;
    for (int i = 0; i < n; ++i) {
        while (t >= 0 && s[t] != s[i]) t = t>=1 ? f[t-1] : -1;
        f[i] = ++t;
    }
}

int main() {
	int a[100];
	const char * s = "bbabbabb";
	int n = strlen(s);
	kmp(s, a, n);
	for (int i = 0; i < n; ++i)
		cout << i << " " << s[i] << " " << a[i] << endl;
}
