// Treap with multiset functionality
struct Node {
	ll val, cnt, prio, size;
	Node* child[2];
	void apply() { // apply lazy actions and push them down
	}
	void maintain() {
		size = cnt;
		rep(i,0,2) size += child[i] ? child[i]->size : 0;
	}
};
pair<Node*, Node*> split(Node* n, ll val) { // returns (< val, >= val)
	if (!n) return {0,0};
	n->apply();
	Node*& c = n->child[val > n->val];
	auto sub = split(c, val);
	if (val > n->val) { c = sub.fst; n->maintain(); return mk(n, sub.snd); }
	else              { c = sub.snd; n->maintain(); return mk(sub.fst, n); }
}
Node* merge(Node* l, Node* r) {
	if (!l || !r) return l ? l : r;
	if (l->prio > r->prio) {
		l->apply();
		l->child[1] = merge(l->child[1], r);
		l->maintain();
		return l;
	} else {
		r->apply();
		r->child[0] = merge(l, r->child[0]);
		r->maintain();
		return r;
	}
}
Node* insert(Node* n, ll val, ll num=1) {
	auto l = split(n, val);
	auto r = split(l.second, val + 1);
	if (r.first) {
		r.first->apply();
		r.first->cnt += num;
	}
	else {
		r.first = new Node { val, num, rand(), num };
	}
	r.first->maintain();
	return merge(merge(l.fst, r.fst), r.second);
}
Node* remove(Node* n, ll val, ll num=1) { // DANGER UNTESTED SO FAR
	if (!n) return 0;
	n->apply();
	if (val == n->val) {
		if (num == n->cnt)
			return merge(n->child[0], n->child[1]);
		else {
      n->apply();
			n->cnt -= num;
			n->maintain();
			return n;
		}
	}
	Node*& c = n->child[val > n->val];
	c = remove(c, val, num);
	n->maintain();
	return n;
}

void debug(Node* n, int ind=0) {
	if (!n) {
		rep(_,0,ind) cout << "  ";
		cout << "NIL" << endl;
		return;
	}
	rep(_,0,ind) cout << "  "; cout << n->val << "*" << n->cnt << " : " << n->size << endl;
	debug(n->child[0], ind+1);
	debug(n->child[1], ind+1);
}

Node* copy(Node* from, Node* to) { // insert everything from one treap into another
	if (!from) return to;
	return copy(from->child[0], copy(from->child[1], insert(to, from->val, from->cnt)));
}
