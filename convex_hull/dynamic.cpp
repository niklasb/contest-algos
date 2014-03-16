const ll is_query = -(1LL<<62);
struct HullDynamic { // will maintain upper hull for maximum
    struct Line {
        ll m, b;
        mutable function<const Line*()> succ;
        bool operator<(const Line& rhs) const {
            if (rhs.b != is_query) return m < rhs.m;
            const Line* s = succ();
            if (!s) return 0;
            ll x = rhs.m;
            return b - s->b < (s->m - m) * x;
        }
    };
    multiset<Line> lines;
    typedef multiset<Line>::iterator It;
    bool bad(It y) {
        It z = next(y);
        if (y == begin(lines)) {
            if (z == end(lines)) return 0;
            return y->m == z->m && y->b <= z->b;
        }
        It x = prev(y);
        if (z == end(lines)) return y->m == x->m && y->b <= x->b;
        return (x->b - y->b)*(z->m - y->m) >= (y->b - z->b)*(y->m - x->m);
    }
    void insert(ll m, ll b) {
        It y = lines.insert({ m, b });
        y->succ = [=] { return next(y) == lines.end() ? 0 : &*next(y); };
        if (bad(y)) { lines.erase(y); return; }
        while (next(y) != end(lines) && bad(next(y))) lines.erase(next(y));
        while (y != begin(lines) && bad(prev(y))) lines.erase(prev(y));
    }
    bool empty() { return lines.empty(); }
    ll eval(ll x) {
        auto l = *lines.lower_bound((Line) { x, is_query });
        return l.m * x + l.b;
    }
};
