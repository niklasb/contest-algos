struct SuffixAutomaton { // can be used for Aho-Corasick, LCS and others
    struct State {
        int depth, id;
        State *go[128], *suffix;
    } *root = new State {0}, *sink = root;
    void append(const string& str, int offset=0) { // O(|str|)
        for (int i = 0; i < str.size(); ++i) {
            int a = str[i];
            State *cur = sink, *sufState;
            sink = new State { sink->depth + 1, offset + i, {0}, 0 };
            while (cur && !cur->go[a]) {
                cur->go[a] = sink;
                cur = cur->suffix;
            }
            if (!cur) sufState = root;
            else {
                State *q = cur->go[a];
                if (q->depth == cur->depth + 1)
                    sufState = q;
                else {
                    State *r = new State(*q);
                    r->depth = cur->depth + 1;
                    q->suffix = sufState = r;
                    while (cur && cur->go[a] == q) {
                        cur->go[a] = r;
                        cur = cur->suffix;
                    }
                }
            }
            sink->suffix = sufState;
        }
    }
    int walk(const string& str) { // O(|str|) returns LCS with automaton string
        int tmp = 0;
        State *cur = root;
        int ans = 0;
        for (int i = 0; i < str.size(); ++i) {
            int a = str[i];
            if (cur->go[a]) {
                tmp++;
                cur = cur->go[a];
            } else {
                while (cur && !cur->go[a])
                    cur = cur->suffix;
                if (!cur) {
                    cur = root;
                    tmp = 0;
                } else {
                    tmp = cur->depth + 1;
                    cur = cur->go[a];
                }
            }
            ans = max(ans, tmp);  // i - tmp + 1 is start of match
        }
        return ans;
    }
};
