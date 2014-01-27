template <typename T>
struct MinQueue {
  deque<T> q, mono;
  void enqueue(T x) {
    while (!mono.empty() && x <= mono.back()) mono.pop_back();
    mono.push_back(x);
    q.push_back(x);
  }
  void dequeue() {
    if (mono.front() == q.front()) mono.pop_front();
    q.pop_front();
  }
  T front() { return q.front(); }
  T min() { return mono.front(); }
};
