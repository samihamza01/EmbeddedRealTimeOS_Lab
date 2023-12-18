/*
 * smpl_list.h
 *
 * this is a simple list implementation of list.h from linux kernel
 *
 * Created on: Nov 25, 2023
 * Author: 	Valentin Lairich
 * Co-Author:	-
 * Version:	1.0
 * Updated:	Nov 25, 2023
 */

#ifndef SMPL_LIST_H_
#define SMPL_LIST_H_

#include <stddef.h>
#include <stdbool.h>

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

static inline bool list_empty(const struct list_head *head) {
    return head->next == head;
}

static inline void __list_add(struct list_head *new,
                               struct list_head *prev,
                               struct list_head *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
    __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    __list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
    next->prev = prev;
    prev->next = next;
}

static inline void __list_del_entry(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry) {
    __list_del_entry(entry);
    entry->next = entry;
    entry->prev = entry;
}

static inline void list_replace(struct list_head *old, struct list_head *new) {
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void list_replace_init(struct list_head *old, struct list_head *new) {
    list_replace(old, new);
    INIT_LIST_HEAD(old);
}

static inline void list_del_init(struct list_head *entry) {
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

static inline void list_move(struct list_head *list, struct list_head *head) {
    __list_del_entry(list);
    list_add(list, head);
}

static inline void list_move_tail(struct list_head *list, struct list_head *head) {
    __list_del_entry(list);
    list_add_tail(list, head);
}

static inline void list_splice(const struct list_head *list, struct list_head *head) {
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}

static inline void list_splice_tail(struct list_head *list, struct list_head *head) {
    if (!list_empty(list))
        __list_splice(list, head->prev, head);
}

static inline void list_splice_init(struct list_head *list, struct list_head *head) {
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}

static inline void list_splice_tail_init(struct list_head *list, struct list_head *head) {
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_HEAD(list);
    }
}

#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
    list_entry((ptr)->prev, type, member)

#define list_first_entry_or_null(ptr, type, member) ({ \
    struct list_head *head__ = (ptr); \
    struct list_head *pos__ = head__->next; \
    pos__ != head__ ? list_entry(pos__, type, member) : NULL; \
})

#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_prev_entry(pos, member) \
    list_entry((pos)->member.prev, typeof(*(pos)), member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_reverse(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

#define list_count_nodes(head) ({ \
    size_t count = 0; \
    struct list_head *pos; \
    list_for_each(pos, head) \
        count++; \
    count; \
})

#define list_empty_nodes(head) \
    list_for_each_safe(pos, n, head) { \
        list_del(pos); \
        free(pos); \
    }

#endif /* SMPL_LIST_H_ */
