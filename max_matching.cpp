int n;
int adj[222][222];
int a[222][222];
const int MOD = 29989;
int powmod(int a, int b) {
    if (b==0) return 1;
    if (b&1) return a * powmod(a,b-1) % MOD;
    int rt = powmod(a,b/2);
    return rt*rt % MOD;
}
int comp_rank() {
    int r = 0;
    rep(j,0,n) {
        int k = r;
        while (k < n && !a[k][j]) ++k;
        if (k == n) continue;
        swap(a[r], a[k]);
        int inv = powmod(a[r][j], MOD - 2);
        rep(i,j,n)
            a[r][i] = a[r][i] * inv % MOD;
        rep(u,r+1,n) rep(v,j+1,n)
            a[u][v] = (a[u][v] - a[r][v] * a[u][j] % MOD + MOD) % MOD;
        ++r;
    }
    return r;
}
int max_matching() {
    rep(i,0,n) rep(j,0,i)
        if (adj[i][j]) {
            a[i][j] = rand() % (MOD - 1) + 1;
            a[j][i] = MOD - a[i][j];
        }
    return comp_rank() >> 1;
}
