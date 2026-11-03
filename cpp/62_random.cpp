// 62 - <random>：随机数引擎 + 分布（比 srand/rand 强大得多）
#include <print>
#include <random>
#include <map>

int main() {
    // 引擎决定"怎么生成原始随机数"；分布决定"映射成什么形状"
    std::mt19937 gen(42);                      // 梅森旋转引擎，固定种子=可复现

    // 均匀整数：掷骰子
    std::uniform_int_distribution<int> die(1, 6);
    std::print("骰子10次: ");
    for (int i = 0; i < 10; i++) std::print("{} ", die(gen));
    std::println("");

    // 均匀实数 [0,1)
    std::uniform_real_distribution<double> unit(0.0, 1.0);
    std::println("unit(0,1): {:.3f} {:.3f} {:.3f}", unit(gen), unit(gen), unit(gen));

    // 正态分布：均值0 标准差1（模拟测量误差/股价波动）
    std::normal_distribution<double> gauss(0.0, 1.0);
    std::println("normal: {:.3f} {:.3f} {:.3f}", gauss(gen), gauss(gen), gauss(gen));

    // 伯努利：真假硬币
    std::bernoulli_distribution coin(0.5);
    std::println("硬币: {} {} {}", coin(gen), coin(gen), coin(gen));

    // 离散分布：按权重抽样（10% 20% 70%）
    std::discrete_distribution<int> weighted({10, 20, 70});
    std::map<int, int> histogram;
    for (int i = 0; i < 10000; i++) histogram[weighted(gen)]++;
    std::println("加权抽样统计: A={}% B={}% C={}%",
                 histogram[0] / 100, histogram[1] / 100, histogram[2] / 100);

    // 验证均匀性：掷骰子 60000 次，每面约 10000
    std::mt19937 g2(7);
    std::uniform_int_distribution<int> d6(1, 6);
    std::map<int, int> rolls;
    for (int i = 0; i < 60000; i++) rolls[d6(g2)]++;
    std::print("均匀性: ");
    for (auto [face, n] : rolls) std::print("{}:{} ", face, n);
    std::println("");

    // random_device：非确定性真随机（用作种子，不适合高频调用）
    std::random_device rd;
    std::println("random_device 样本: {} {}", rd(), rd());
    return 0;
}
