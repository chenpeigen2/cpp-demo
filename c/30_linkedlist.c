// 30 - 实战：手写双向链表（哨兵节点）+ 侵入式链表（container_of）
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// ---- 普通版：值携带节点 ----
typedef struct Node {
    int val;
    struct Node *prev, *next;
} Node;

typedef struct {
    Node head;                    // 哨兵：不存数据，让"空表/边界"统一处理
} List;

static void list_init(List *L) { L->head.next = L->head.prev = &L->head; }

static void list_push_back(List *L, int val) {
    Node *n = malloc(sizeof *n);
    *n = (Node){val, L->head.prev, &L->head};
    L->head.prev->next = n;       // 原最后一个指向新节点
    L->head.prev = n;             // 哨兵.prev 指向新节点
}

static _Bool list_remove_val(List *L, int val) {
    for (Node *p = L->head.next; p != &L->head; p = p->next)
        if (p->val == val) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            free(p);
            return 1;
        }
    return 0;
}

static void list_print(const List *L, const char *tag) {
    printf("%s 正向: ", tag);
    for (const Node *p = L->head.next; p != &L->head; p = p->next) printf("%d ", p->val);
    printf("| 反向: ");
    for (const Node *p = L->head.prev; p != &L->head; p = p->prev) printf("%d ", p->val);
    putchar('\n');
}

// 就地反转：交换每个节点的 prev/next，最后交换哨兵的
static void list_reverse(List *L) {
    Node *h = &L->head;
    for (Node *p = h->next; p != h; ) {
        Node *nx = p->next;
        Node *t = p->prev;
        p->prev = p->next;
        p->next = t;
        p = nx;
    }
    Node *t = h->prev;
    h->prev = h->next;
    h->next = t;
}

// ---- 侵入式版：节点嵌在数据结构里（内核链表的思路）----
typedef struct Link { struct Link *prev, *next; } Link;
typedef struct { int id; Link link; } Item;

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))   // 成员地址反推宿主地址

int main(void) {
    List L;
    list_init(&L);
    for (int i = 1; i <= 5; i++) list_push_back(&L, i);
    list_print(&L, "构建后");

    list_remove_val(&L, 3);
    list_print(&L, "删除 3");

    list_reverse(&L);
    list_print(&L, "反转后");

    // 释放剩余节点
    Node *p = L.head.next;
    while (p != &L.head) { Node *nx = p->next; free(p); p = nx; }

    // ---- 侵入式演示：同一个 Item 里的 Link 挂到链上，用 container_of 取回宿主 ----
    Item a = {10, {0, 0}}, b = {20, {0, 0}}, c = {30, {0, 0}};
    Link head = {&head, &head};                    // 循环哨兵
    Link *links[] = {&a.link, &b.link, &c.link};
    for (int i = 0; i < 3; i++) {                  // 头插
        links[i]->next = head.next;
        links[i]->prev = &head;
        head.next->prev = links[i];
        head.next = links[i];
    }
    printf("侵入式（头插所以是 30 20 10）: ");
    for (Link *l = head.next; l != &head; l = l->next)
        printf("%d ", container_of(l, Item, link)->id);
    putchar('\n');
    printf("container_of 原理：成员地址 - 成员偏移(%zu) = 宿主地址\n", offsetof(Item, link));
    return 0;
}
