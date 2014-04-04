#include <cstdio>
#include <iostream>
#include <algorithm>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using namespace std;

struct Node { int size, maxi; };
template<typename nci, typename ni, typename cmp, typename alloc>
#define POL detail::branch_policy<nci, ni, alloc>
struct tree_policy : private POL {
    typedef Node metadata_type;
    void operator() (ni it, nci null) const {
        auto& n = (Node&)it.get_metadata();
        n.size = 1;
        n.maxi = (*it)->second;
        for (auto& c : { it.get_l_child(), it.get_r_child() }) {
            if (c != null) {
                n.size += c.get_metadata().size;
                n.maxi = max(n.maxi, c.get_metadata().size);
            }
        }
    }
    typename nci::value_type select(size_t k) const {
        auto it = node_begin(), null = node_end();
        while (it != null) {
            auto l = it.get_l_child();
            auto o = (l == null)? 0 : l.get_metadata().size;
            if (k == o) return *it;
            else if (k < o) it = l;
            else k -= o + 1, it = it.get_r_child();
        }
        return POL::end_iterator();
    }
    virtual nci node_begin() const = 0;
    virtual nci node_end() const = 0;
};
typedef tree<int,int,less<int>,splay_tree_tag,tree_policy> Tree;
Tree* other;
typedef Tree::iterator ti;
namespace std {
    template<> iterator_traits<ti>::difference_type distance<ti>(ti a, ti b) {
        assert(other->begin() == a && other->end() == b);
        return other->node_begin().get_metadata().size;
    }
}
void split(Tree& a, Tree& b, int x) { other=&b; a.split(x, b); }
Tree::const_iterator select(Tree& a, int k) {
    auto it = a.select(k);
    a.find(it->first); // splay if necessary
    return it;
}
int rank_of(Tree& a, int key) {
    auto it = a.find(key);
    auto l = (Tree::node_iterator){it.m_p_nd}.get_l_child();
    return l == a.node_end() ? 0 : l.get_metadata().size;
}

Tree A;
int arr[1000100];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int t;
        scanf("%d", &t);
        A.insert(make_pair(i, t));
        arr[i] = t;
    }
    for (int i = 1; i <= n; ++i) {
        assert(select(A,i-1)->second == arr[i]);
        assert(rank_of(A,i)==i-1);
    }
    assert(n==A.node_begin().get_metadata().size);
    //return 0;
    int m;
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
    {
        char t;
        int x, y;
        scanf(" %c %d %d", &t, &x, &y);
        if (t == 'u')
        {
            A.erase(x);
            A.insert(make_pair(x, y));
        }
        else
        {
            Tree B, C;
            split(A, C, y);
            split(A, B, x-1);
            printf("%d\n", B.node_begin().get_metadata().maxi);
            A.join(B);
            A.join(C);
        }
    }
}

