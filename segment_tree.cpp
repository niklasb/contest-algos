// http://www.spoj.pl/problems/SEGSQRSS/
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

typedef long long LL;
#define REP(i,n) for (int i = 0; i < (n); ++i)

const int maxn = 1<<17;

enum ActionType { ACTION_ADD = 0, ACTION_SET };
struct Action {
	int z;
	ActionType type;
	Action(ActionType tt, int zz) : z(zz), type(tt) { }
	Action() : z(0), type(ACTION_ADD) {}
};
struct Node {
	// variables: x = sum of values beneath the node, y = sum of squares of values
	// x and y are always correctly updated if there is no outstanding action in the parent node
	int x, y;
	Action action; // this action needs to be lazily applied to all children of the node
} nodes[2*maxn+10];

int left(int i) { return 2*i + 1; }
int right(int i) { return 2*i + 2; }
int parent(int i) { return (i-1)/2; }

int n; // needs to be set to a power of 2!

void maintain(int x) {
	nodes[x].x = nodes[left(x)].x + nodes[right(x)].x;
	nodes[x].y = nodes[left(x)].y + nodes[right(x)].y;
}

void build() {
	for (int i = n - 2; i >= 0; --i) maintain(i);
	REP(i, 2*n-1) nodes[i].action = Action();
}

void resolve(const Action& act, int x, int a, int b) {
	if (act.type == ACTION_SET) {
		nodes[x].y = (b-a)*act.z*act.z;
		nodes[x].x = (b-a)*act.z;
		nodes[x].action = act; // mark children for update
	} else { // ACTION_ADD
		nodes[x].y += (b-a)*act.z*act.z + 2*act.z*nodes[x].x;
		nodes[x].x += (b-a)*act.z;
		nodes[x].action.z += act.z; // update saved action (in this case, incrementing
		                            // z is correct for both ACTION_SET and ACTION_ADD
	}
}

// [ia, ib) is the update interval, x is the root of the current partial tree and [a,b)
// is the key range represented by x
void range_action(const Action& act, int ia, int ib, int x, int a, int b) {
	ia = max(ia, a); ib = min(ib, b); // intersect interval
	if (ia >= ib) return;
	if (ia == a and ib == b) {
		resolve(act, x, a, b); // we can respond to the update in O(1)
		return;
	}
	int m = (a+b)/2;
	// push update to the children. We first need to push the outstanding update
	range_action(nodes[x].action, a, m, left(x), a, m);
	range_action(nodes[x].action, m, b, right(x), m, b);
	// now we can push the new update
	range_action(act, ia, ib, left(x), a, m);
	range_action(act, ia, ib, right(x), m, b);
	nodes[x].action = Action(); // reset action for the current node, because
	                            // updates have been pushed to the children
	maintain(x); // compute new values for x from the values of the children
}

LL range_query(int ia, int ib, int x, int a, int b) {
	ia = max(ia, a); ib = min(ib, b);
	if (ia >= ib) return 0;
	if (ia == a && ib == b) return nodes[x].y;
	int m = (a+b)/2;
	// we need to resolve the outstanding update for the current node before
	// recursing
	resolve(nodes[x].action, left(x), a, m);
	resolve(nodes[x].action, right(x), m, b);
	nodes[x].action = Action(); // we can delete the lazy action now because it was pushed
	                            // to the children
	return range_query(ia, ib, left(x), a, m) + range_query(ia, ib, right(x), m, b);
}

int main() {
	ios_base::sync_with_stdio(false);
	int T, Q; cin >> T;
	for (int t = 1; t <= T; ++t) {
		int N; cin >> N >> Q;
		n = 1<<(int)ceil(log(N)/log(2));
		for (int i = 0; i < N; ++i) {
			cin >> nodes[n+i-1].x;
			nodes[n+i-1].y = nodes[n+i-1].x * nodes[n+i-1].x;
		}
		build();
		cout << "Case " << t << ":\n";
		while (Q--) {
			int op; cin >> op;
			if (op == 2) {
				int a, b; cin >> a >> b;
				cout << range_query(a-1, b, 0, 0, n) << endl;
			} else if (op == 1) {
				int a, b, x; cin >> a >> b >> x;
				range_action(Action(ACTION_ADD, x), a-1, b, 0, 0, n);
			} else if (op == 0) {
				int a, b, x; cin >> a >> b >> x;
				range_action(Action(ACTION_SET, x), a-1, b, 0, 0, n);
			} else {
				// debug info
				for (int i = 0; i < 2*n-1; ++i)
					cout << nodes[i].x << "/" << nodes[i].y << "/" << nodes[i].action.type << "/" << nodes[i].action.z << " ";
				cout<<endl;
			}
		}
	}
}

int ary[maxn];
int test() {
	n = 1<<10;
	for (int i = 0; i < n; ++i) {
		ary[i] = nodes[n+i-1].x = rand() % 10 - 5;
		nodes[n+i-1].y = nodes[n+i-1].x * nodes[n+i-1].x;
	}
	build();
	int q = 1000;
	srand(time(NULL));
	//for (int j = 0; j < n; ++j) cout << ary[j] << " "; cout << endl;
	LL total = 0;
	for (int i = 0; i < q; ++i) {
		int op = rand() % 3;
		if (op == 2) {
			int a = rand() % n;
			int b = rand() % (n-a) + a + 1;
			int x = rand() % 10 - 5;
			//cout << "add " << a << " " << b << " " << x << endl;
			range_action(Action(ACTION_SET, x), a, b, 0, 0, n);
			for (int j = a; j < b; ++j) ary[j] = x;
		} if (op == 1) {
			int a = rand() % n;
			int b = rand() % (n-a) + a + 1;
			int x = rand() % 10 - 5;
			//cout << "add " << a << " " << b << " " << x << endl;
			range_action(Action(ACTION_ADD, x), a, b, 0, 0, n);
			for (int j = a; j < b; ++j) ary[j] += x;
		} else if (op == 0) {
			int a = rand() % n;
			int b = rand() % (n-a) + a + 1;
			//cout << "query " << a << " " << b << endl;
			LL res1 = range_query(a, b, 0, 0, n);
			total += res1;
			LL res2 = 0;
			for (int j = a; j < b; ++j) res2 += ary[j] * ary[j];
			//for (int j = 0; j < n; ++j) cout << ary[j] << " "; cout << endl;
			if (res2 && res1 != res2)
				cout << "ERROR " << res1 << " " << res2 << endl;
		}
	}
	cout << total << endl;
}
