#include <deque>
using namespace std;

template <typename T>
struct MinQueue {
  deque<pair<int, T> > q, mono;
  int cnt = 0;
  void enqueue(T x) {
    while (!mono.empty() && x <= mono.back().second) mono.pop_back();
    mono.push_back(make_pair(cnt,x));
    q.push_back(make_pair(cnt,x));
    cnt++;
  }
  void dequeue() {
    if (mono.front().first == q.front().first) mono.pop_front();
    q.pop_front();
  }
  T front() { return q.front().second; }
  T min() { return mono.front().second; }
};
