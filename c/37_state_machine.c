// 37 - 实战：数据驱动状态机 —— 表驱动 + 函数指针动作（协议解析的骨架）
#include <stdio.h>
#include <string.h>

typedef enum { ST_LOCKED, ST_UNLOCKED, ST_COUNT } State;
typedef enum { EV_COIN, EV_PUSH, EV_RESET, EV_COUNT } Event;

static int coins_inserted = 0;
static int passes = 0;

// 动作就是普通函数；真实项目里这里做日志/网络包/硬件操作
static void act_coin_locked(void) {
    printf("  收币（第%d枚）→ 开锁\n", ++coins_inserted);
}
static void act_push_unlocked(void) {
    printf("  推门通过（第%d人）→ 上锁\n", ++passes);
}
static void act_noop(void) { printf("  （忽略）\n"); }

typedef struct {
    State next;
    void (*action)(void);
} Transition;

// 状态转移表：整个状态机的全部逻辑，一眼看完
// 行=状态，列=事件；改需求只改表，不改控制流
static const Transition table[ST_COUNT][EV_COUNT] = {
    /*            EV_COIN                EV_PUSH                EV_RESET            */
    /* LOCKED   */ {{ST_UNLOCKED, act_coin_locked}, {ST_LOCKED, act_noop}, {ST_LOCKED, act_noop}},
    /* UNLOCKED */ {{ST_UNLOCKED, act_noop},           {ST_LOCKED, act_push_unlocked}, {ST_LOCKED, act_noop}},
};

static const char *state_names[] = {"锁定", "解锁"};
static const char *event_names[] = {"投币", "推门", "复位"};

int main(void) {
    State s = ST_LOCKED;
    printf("初始状态: %s\n", state_names[s]);

    // 一段事件序列驱动状态机
    Event script[] = {EV_PUSH, EV_COIN, EV_COIN, EV_PUSH, EV_PUSH, EV_RESET, EV_COIN, EV_PUSH};
    for (unsigned long i = 0; i < sizeof script / sizeof script[0]; i++) {
        Event e = script[i];
        const Transition *t = &table[s][e];
        printf("收到 [%s]（状态=%s）：", event_names[e], state_names[s]);
        t->action();
        s = t->next;
    }

    printf("\n统计: 收币 %d 枚, 通过 %d 人, 最终状态 %s\n",
           coins_inserted, passes, state_names[s]);

    // 优点：状态数×事件数的矩阵就是文档；非法转移一目了然（全是 act_noop）
    // 变体：分层状态机(嵌套 switch)、状态对象表(带 entry/exit 钩子)、UML 风格守卫条件
    printf("扩展点：守卫条件=在 action 前查谓词；entry/exit 钩子=进出状态时固定调用\n");
    return 0;
}
