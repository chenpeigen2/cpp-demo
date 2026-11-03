// 44 - C++23 平坦容器与适配器：flat_map / stack / queue / priority_queue
#include <print>
#include <flat_map>
#include <stack>
#include <queue>
#include <string>

int main() {
    // std::flat_map (C++23)：底层是有序 vector 的 map
    // —— 缓存友好，遍历/查找常数更小，插入 O(n)
    std::flat_map<std::string, int> fm;
    fm["b"] = 2;
    fm["a"] = 1;
    fm.emplace("c", 3);
    std::println("flat_map(自动按键排序): {}", fm);
    std::println("fm.contains('a')={} fm['b']={}", fm.contains("a"), fm["b"]);
    std::println("keys: {}", fm.keys());     // C++23 可直接观察底层序列

    // stack：LIFO 适配器（默认基于 deque）
    std::stack<int> st;
    st.push(1); st.push(2); st.push(3);
    std::println("stack top={} pop={} 再top={}", st.top(), [&]{ int t = st.top(); st.pop(); return t; }(), st.top());

    // queue：FIFO 适配器
    std::queue<std::string> q;
    q.push("first"); q.push("second");
    std::println("queue front={} size={}", q.front(), q.size());
    q.pop();
    std::println("pop 后 front={}", q.front());

    // priority_queue：堆，top 永远是最大值（默认大根堆）
    std::priority_queue<int> pq;
    pq.push(3); pq.push(1); pq.push(4);
    std::println("priority top={} size={}", pq.top(), pq.size());

    // 小根堆：传入比较器和底层容器
    std::priority_queue<int, std::vector<int>, std::greater<int>> minq;
    minq.push(3); minq.push(1); minq.push(4);
    std::println("min-heap top={}", minq.top());
    return 0;
}
