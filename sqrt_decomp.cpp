const int blocksz = 310;
const int blockcnt = (maxn+blocksz-1)/blocksz;
struct Block {
  int xx[blocksz];
  int min, zeroes;
  int offset;
} blocks[blockcnt];

void init() { rep(i,0,blockcnt) blocks[i].zeroes = blocksz; }

void rewrite(Block& b) {
  int m = 1<<30;
  int zeroes=0;
  rep(i,0,blocksz) {
    b.xx[i]+=b.offset;
    m = min(m,b.xx[i]);
    zeroes += b.xx[i]==0;
  }
  b.offset=0;
  b.zeroes = zeroes;
  b.min = m;
}

int inc(Block& b) {
  if (b.min==0) b.zeroes=0;
  b.min++;
  b.offset++;
  return 0;
}

int dec(Block& b) {
  b.offset--;
  b.min--;
  if (b.min==0) rewrite(b);
  return 0;
}

typedef int BlockFunc(Block& b);
typedef int ItemFunc(Block& b, int i);
// perform an action on the interval [l,r]
int sqrt_action(int l, int r, BlockFunc f, ItemFunc g) {
  r++;
  int lb = l / blocksz;
  int rb = r / blocksz;
  int res=0;
  if (rb == lb) {
    for (int i = l%blocksz; i < r%blocksz; ++i) res += g(blocks[lb], i);
    rewrite(blocks[lb]);
    return res;
  }
  for (int i = lb+1; i < rb; ++i) res += f(blocks[i]);
  for (int i = l%blocksz; i < blocksz; ++i) res += g(blocks[lb],i);
  for (int i = 0; i < r%blocksz; ++i) res += g(blocks[rb],i);
  rewrite(blocks[lb]);
  rewrite(blocks[rb]);
  return res;
}

int inci(Block& b, int i) { return b.xx[i]++; }
void add(int l, int r) { sqrt_action(l,r,inc,inci); }

int deci(Block& b, int i) { return b.xx[i]--; }
void remove(int l, int r) { sqrt_action(l,r,dec,deci); }

int get(Block& b) { return blocksz-b.zeroes; }
int geti(Block& b, int i) { return !!(b.xx[i]+b.offset); }
// return number of non-zero elements in [l,r]
int query(int l, int r) { return sqrt_action(l,r,get,geti); }
