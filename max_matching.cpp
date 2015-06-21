const int N=200, MOD=1000000007, I=10;
int n, adj[N][N], a[N][N];
int rank() {
    int r = 0;
    rep(j,0,n) {
        int k = r;
        while (k < n && !a[k][j]) ++k;
        if (k == n) continue;
        swap(a[r], a[k]);
        int inv = powmod(a[r][j], MOD - 2);
        rep(i,j,n)
            a[r][i] = 1LL * a[r][i] * inv % MOD;
        rep(u,r+1,n) rep(v,j,n)
            a[u][v] = (a[u][v] - 1LL * a[r][v] * a[u][j] % MOD + MOD) % MOD;
        ++r;
    }
    return r;
}
// failure probability = (n / MOD)^I
int max_matching() {
    int ans = 0;
    rep(_,0,I) {
        rep(i,0,n) rep(j,0,i)
            if (adj[i][j]) {
                a[i][j] = rand() % (MOD - 1) + 1;
                a[j][i] = MOD - a[i][j];
            }
        ans = max(ans, rank()/2);
    }
    return ans;
}
