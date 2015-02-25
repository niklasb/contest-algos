struct Node {
	int val, prio, size;
	Node* child[2];
	Node* maintain() {
		size = 1 + (child[0] ? child[i]->size : 0) + (child[1] ? child[1]->size : 0);
		return this;
	}
};
pair<Node*, Node*> split(Node* n, int val) { // returns subtrees (< val, >= val)
	if (!n) return {0,0};
	Node*& c = n->child[val > n->val];
	auto sub = split(c, val);
	if (val > n->val) { c = sub.fst; n->maintain(); return mk(n, sub.snd); }
	else              { c = sub.snd; n->maintain(); return mk(sub.fst, n); }
}
Node* merge(Node* l, Node* r) {
	if (!l || !r) return l ? l : r;
	if (l->prio > r->prio) {
		l->child[1] = merge(l->child[1], r);
		return l->maintain();
	} else {
		r->child[0] = merge(l, r->child[0]);
		return r->maintain();
	}
}
Node* insert(Node* n, int val) {
	auto sub = split(n, val);
	Node* x = new Node { val, rand(), 1 };
	return merge(merge(sub.fst, x), sub.snd);
}
Node* remove(Node* n, int val) {
	if (!n)            return 0;
	if (val == n->val) return merge(n->child[0], n->child[1]);
	Node*& c = n->child[val > n->val];
	c = remove(c, val);
	return n->maintain();
}
Node* kth(Node* n, int k) {
	int leftsz = n->child[0].size;
	if (k == leftsz) return n;
	if (k < leftsz)  return kth(n->child[0], k);
	else             return kth(n->child[1], k - leftsz - 1);
}

int main() {
}
