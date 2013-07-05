// convex hull, minimum
vector<ll> M, B;
int ptr;
bool bad(int l1,int l2,int l3) {
  // use deterministic comuputation with long long if sufficient
  return (long double)(B[l3]-B[l1])*(M[l1]-M[l2])<(long double)(B[l2]-B[l1])*(M[l1]-M[l3]);
}
// insert with non-increasing m
void insert(ll m, ll b) {
  M.push_back(m);
  B.push_back(b);
  while (M.size() >= 3 && bad(M.size()-3, M.size()-2, M.size()-1)) {
    M.erase(M.end()-2);
    B.erase(B.end()-2);
  }
}
ll get(int i, ll x) {
  return M[i]*x + B[i];
}
// query with non-decreasing x
ll query(ll x) {
  ptr=min((int)M.size()-1,ptr);
  while (ptr<M.size()-1 && get(ptr+1,x)<get(ptr,x))
    ptr++;
  return get(ptr,x);
}
