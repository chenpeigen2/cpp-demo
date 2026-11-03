// 53 - <chrono>：时间库（C++20 大幅增强）
#include <print>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;    // 100ms / 3s 等字面量

int main() {
    // 三种时钟：system（挂钟，可回拨）、steady（单调，测间隔用这个）、high_resolution
    auto t0 = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(50ms);
    auto t1 = std::chrono::steady_clock::now();
    std::println("睡 50ms 实测 {:.3f}ms",
                 std::chrono::duration<double, std::milli>(t1 - t0).count());

    // duration：时长类型，可换算、可运算
    std::chrono::minutes m(90);
    std::println("90 分钟 = {} 小时 = {} 秒",
                 std::chrono::duration_cast<std::chrono::hours>(m).count(),
                 std::chrono::duration_cast<std::chrono::seconds>(m).count());
    std::println("1.5h 转成分(安全转换)={}", std::chrono::duration<double, std::ratio<60>>(1.5h).count());

    // C++20 日历 API：year_month_day 直接做日期运算
    using namespace std::chrono;
    auto today = year_month_day{floor<days>(system_clock::now())};
    std::println("今天: {} 年 {} 月 {} 日",
                 int(today.year()), unsigned(today.month()), unsigned(today.day()));
    auto next_week = year_month_day{sys_days{today} + days(7)};
    std::println("一周后: {} 日", unsigned(next_week.day()));
    std::println("2025-11-03 是 {}",
                 std::chrono::weekday{std::chrono::year{2025}/11/3});  // weekday 直接打印 Mon/Tue...

    // 时长字面量与比较
    std::println("100ms == 0.1s? {}", 100ms == 0.1s);
    std::println("2h + 30min = {} min", (2h + 30min).count());

    // time_of_day 风格：hh_mm_ss 拆分
    auto day_dur = duration_cast<seconds>(system_clock::now().time_since_epoch()) % (24h);
    hh_mm_ss hms{day_dur};
    std::println("当前时刻 {}:{:02}:{:02}",
                 hms.hours().count(), hms.minutes().count(), hms.seconds().count());
    return 0;
}
