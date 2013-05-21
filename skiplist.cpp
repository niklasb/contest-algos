#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
using namespace std;

typedef int T;
const int maxh = 15; // should be around log2(n)

struct Node {
	Node* nxt[maxh];
	int h, dst[maxh];
	T val;
	Node(T val) : h(0), val(val) {
		memset(nxt, 0, sizeof nxt);
		fill(dst, dst+maxh, 1);
	}
	~Node() { if (nxt[0]) delete nxt[0]; }
} *head=0;
void init() {
	if (head) delete head;
	head = new Node(T());
}

int pos[maxh+1];
Node* pred[maxh];

Node* find(T val) {
	Node* n = head;
	pos[maxh] = 0;
	for (int lvl = maxh-1; lvl >= 0; --lvl) {
		pos[lvl] = pos[lvl+1];
		while (n->nxt[lvl] && n->nxt[lvl]->val < val) {
			pos[lvl] += n->dst[lvl];
			n = n->nxt[lvl];
		}
    pred[lvl] = n;
	}
	return (n && n->val == val) ? n : 0;
}

Node* insert(T val) {
	find(val);
	Node* n = new Node(val);
	int lvl = 0;
	for (int lvl = 0; lvl < maxh; lvl++)
		pred[lvl]->dst[lvl]++;
	do {
		n->nxt[lvl] = pred[lvl]->nxt[lvl];
		pred[lvl]->nxt[lvl] = n;
		n->dst[lvl] = pred[lvl]->dst[lvl] - pos[0] + pos[lvl] - 1;
		pred[lvl]->dst[lvl] = pos[0] - pos[lvl] + 1;
		lvl++;
	} while (lvl < maxh && rand() % 2);
	n->h = lvl-1;
	return n;
}

void remove(T val) {
	Node* rem = find(val);
	if (!rem) return;
	for (int lvl = 0; lvl < maxh; ++lvl) {
		if (lvl <= rem->h) {
			pred[lvl]->nxt[lvl] = rem->nxt[lvl];
			pred[lvl]->dst[lvl] += rem->dst[lvl] - 1;
		} else pred[lvl]->dst[lvl]--;
	}
}

int lessthan(T val) {
	find(val);
	return pos[0];
}

Node* kth(int k) {
	Node* n = head;
	for (int lvl = maxh-1; lvl >= 0; --lvl) {
		while (n->dst[lvl] <= k) {
			k -= n->dst[lvl];
			n = n->nxt[lvl];
		}
	}
	return n;
}

int main() {
	ios_base::sync_with_stdio(false);
	srand(0x123123);
	int T; cin >> T;
	for (int tc = 1; tc <= T; ++tc) {
		int tcc, n; cin >> tcc >> n;
		init();
		vector<int> res;
		for (int i = 0; i < n; ++i) {
			int x; cin >> x;
			insert(x);
			if (i%2 == 0)
				res.push_back(kth(i/2 + 1)->val);
		}
		cout << tcc << " " << res.size() << endl;
		for (int i = 0; i < res.size(); ++i) {
			if (i > 0 && i % 10 == 0) cout << "\n";
			if (i % 10) cout << " ";
			cout << res[i];
		}
		cout << endl;
	}
}
