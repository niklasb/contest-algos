#include <ctime>
#include <cstdlib>
#include <iostream>

#define INVALID -(1<<30)
#define maxn 1000100
struct node {
	int data,priority,count,size,child[2];
} treap[maxn];
int temp,top,root;
inline void init() { // initialization
	srand(time(0));
	treap[0]=(node){0,0,0,0,{0,0}};
	top=0; root=0;
}
inline void maintain(int r) {
	treap[r].size=treap[treap[r].child[0]].size+
			treap[treap[r].child[1]].size+treap[r].count;
} // op=0 right rotate, otherwise left rotate
inline void rotate(int &r, int op) {
	temp=treap[r].child[op];
	treap[r].child[op]=treap[temp].child[!op];
	treap[temp].child[!op]=r;
	maintain(r); maintain(temp); r=temp;
} // insert an element
void insert(int &r,int d) {
	if (r==0)
		treap[r=++top]=(node){d,rand(),1,1,{0,0}};
	else if (d==treap[r].data)
		treap[r].count++,treap[r].size++;
	else {
		int dir = d>treap[r].data;
		insert(treap[r].child[dir],d);
		treap[r].size++;
		if (treap[treap[r].child[dir]].priority<treap[r].priority)
			rotate(r, dir);
	}
} // find and delete an element, return false if element is not found
bool remove(int r,int d) {
	bool res;
	if (r==0)
		res=false;
	else if (d!=treap[r].data)
			res = remove(treap[r].child[d>treap[r].data],d);
	else {
		if ((res = (treap[r].count>0)))
			treap[r].count--;
	}
	maintain(r);  	//when r == 0 , maintain does nothing
	return res;
} // count how many duplications does the given element have
int find(int r,int d) {
	while (r!=0 && treap[r].data!=d)
		r = treap[r].child[d>treap[r].data];
	return treap[r].count;
} // count how many elements <= the given element
int lessequal(int r, int d) {
	int res = 0;
	while (r!=0 && treap[r].data!=d) {
		if (d>treap[r].data)
			res+=treap[r].count+treap[treap[r].child[0]].size;
		r = treap[r].child[d>treap[r].data];
	}
	return res+treap[r].count+treap[treap[r].child[0]].size;
}// return the k-th smallest element
int kth(int r,int d) {
	if (d>treap[r].size || d<1)
			return INVALID;
	while (true)
		if (d>treap[treap[r].child[0]].size) {
			d-=treap[treap[r].child[0]].size;
			if (d<=treap[r].count)
				return treap[r].data;
			else
				d-=treap[r].count,r=treap[r].child[1];
		} else r=treap[r].child[0];
}

int main() {
	init();
	for (int i = 1; i <= 1000000; ++i)
		insert(root, i);
}
