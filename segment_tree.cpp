const int maxn = 50500;
const int maxl = 16; // = ceil(log_2(maxn))
const int inf = 1 << 29;
int N = 1 << maxl;

int left(int i) { return 2 * i; }
int right(int i) { return 2 * i + 1; }

struct Range {
    int max_left;
    int max_left1;
    int max_right;
    int max_right1;
    int max_mid1;
    int sum;
    int count;
};
Range NULL_RANGE = {0, -inf, 0, -inf, -inf, 0, 0};

Range operator+(const Range& l, const Range& r) {
    return {
        max(l.sum + r.max_left, l.max_left),
        max(l.sum + ((l.count > 0) ? r.max_left : r.max_left1),
            l.max_left1),

        max(r.sum + l.max_right, r.max_right),
        max(r.sum + ((r.count > 0) ? l.max_right : l.max_right1),
            r.max_right1),

        max(    l.max_mid1,
            max(r.max_mid1,
            max(l.max_right1 + r.max_left,
                l.max_right + r.max_left1))),
        l.sum + r.sum,
        l.count + r.count,
    };
}

enum ActionType { ACTION_ID = 0, ACTION_SET };
struct Action {
    ActionType type;
    int val;
    Range apply(const Range& r) {
        if (type == ACTION_ID) return r;
        int w = r.count;
        return {
            max(0, val * w),
            max(val, val * w),
            max(0, val * w),
            max(val, val * w),
            max(val, val * w),
            val * w,
            w,
        };
    }
};
Action NULL_ACTION = { ACTION_ID, 0 };
Action operator+(const Action& a, const Action& b) {
    if (b.type == ACTION_SET) return b;
    else return a;
}

struct Node {
  int lo, hi;
  Range r;
  Action action;
  Range range() { return action.apply(r); }
} t[1 << (maxl + 1)];

void maintain(int i) {
  t[i].r = t[left(i)].range() + t[right(i)].range();
  t[i].action = NULL_ACTION;
}

void update(int x, const Action& a, int ql, int qr);
void push_down(int x) {
  update(left(x), t[x].action, -inf, inf);
  update(right(x), t[x].action, -inf, inf);
}

void update(int x, const Action& a, int ql, int qr) {
  if (t[x].hi < ql || t[x].lo > qr) return;
  if (ql <= t[x].lo && t[x].hi <= qr) {
    t[x].action = t[x].action + a;
    return;
  }
  push_down(x);
  update(left(x), a, ql, qr);
  update(right(x), a, ql, qr);
  maintain(x);
}

Range query(int x, int ql, int qr) {
  if (t[x].hi < ql || t[x].lo > qr) return NULL_RANGE;
  if (ql <= t[x].lo && t[x].hi <= qr) return t[x].range();
  push_down(x);
  return query(left(x), ql, qr) + query(right(x), ql, qr);
}

// nodes reside in t[N..2N - 1], initialize those first
void init() {
    rep(i,0,N) t[N + i].lo = t[N + i].hi = i;
    for (int i = N - 1; i >= 1; --i) {
        maintain(i);
        t[i].lo = t[left(i)].lo;
        t[i].hi = t[right(i)].hi;
    }
}
