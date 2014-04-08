struct Node {
	int val, prio, size;
	Node* child[2];
};
void maintain(Node* n) {
	if (!n) return;
	n->size = 1;
	rep(i,0,2) n->size += n->child[i] ? n->child[i]->size : 0;
}
pair<Node*, Node*> split(Node* n, int val) {
	if (!n) return {0,0};
	Node*& c = n->child[val > n->val];
	auto sub = split(c, val);
	if (val > n->val) { c = sub.fst; maintain(n); return mk(n, sub.snd); }
	else              { c = sub.snd; maintain(n); return mk(sub.fst, n); }
}
Node* merge(Node* l, Node* r) {
	if (!l || !r) return l ? l : r;
	if (l->prio > r->prio) {
		l->child[1] = merge(l->child[1], r); maintain(l); return l;
	} else {
		r->child[0] = merge(l, r->child[0]); maintain(r); return r;
	}
}
Node* insert(Node* n, int val) {
	auto sub = split(n, val);
	Node* x = new Node { val, rand(), 1 };
	return merge(merge(sub.fst, x), sub.snd);
}
