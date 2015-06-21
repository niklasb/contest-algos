struct Node {
	int val, prio, size;
	Node* child[2];
	void apply() { // apply lazy actions and push them down
	}
	void maintain() {
		size = 1;
		rep(i,0,2) size += child[i] ? child[i]->size : 0;
	}
};
pair<Node*, Node*> split(Node* n, int val) { // returns (< val, >= val)
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
Node* insert(Node* n, int val) {
	auto sub = split(n, val);
	Node* x = new Node { val, rand(), 1 };
	return merge(merge(sub.fst, x), sub.snd);
}
Node* remove(Node* n, int val) {
	if (!n) return 0;
	n->apply();
	if (val == n->val)
		return merge(n->child[0], n->child[1]);
	Node*& c = n->child[val > n->val];
	c = remove(c, val);
	n->maintain();
	return n;
}
