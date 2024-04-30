#ifndef __LIST_H__
#define __LIST_H__
struct dl_list {
	struct dl_list *next;
	struct dl_list *prev;
};

typedef long os_time_t;
struct os_time {
	os_time_t sec;
	os_time_t usec;
};

#ifndef os_memcpy
#define os_memcpy(d, s, n) memcpy((d), (s), (n))
#endif
#ifndef os_memmove
#define os_memmove(d, s, n) memmove((d), (s), (n))
#endif
#ifndef os_memset
#define os_memset(s, c, n) memset(s, c, n)
#endif
#ifndef os_memcmp
#define os_memcmp(s1, s2, n) memcmp((s1), (s2), (n))
#endif

#ifndef os_strlen
#define os_strlen(s) strlen(s)
#endif

#define os_time_before(a, b) \
    ((a)->sec < (b)->sec || \
     ((a)->sec == (b)->sec && (a)->usec < (b)->usec))


#define os_time_sub(a, b, res) do { \
    (res)->sec = (a)->sec - (b)->sec; \
    (res)->usec = (a)->usec - (b)->usec; \
    if ((res)->usec < 0) { \
        (res)->sec--; \
        (res)->usec += 1000000; \
    } \
} while (0)

static inline void dl_list_init(struct dl_list *list)
{
	list->next = list;
	list->prev = list;
}

static inline void dl_list_add(struct dl_list *list, struct dl_list *item)
{
	item->next = list->next;
	item->prev = list;
	list->next->prev = item;
	list->next = item;
}

static inline void dl_list_add_tail(struct dl_list *list, struct dl_list *item)
{
	dl_list_add(list->prev, item);
}

static inline void dl_list_del(struct dl_list *item)
{
	item->next->prev = item->prev;
	item->prev->next = item->next;
	item->next = NULL;
	item->prev = NULL;
}

static inline int dl_list_empty(struct dl_list *list)
{
	return list->next == list;
}

static inline unsigned int dl_list_len(struct dl_list *list)
{
	struct dl_list *item;
	int count = 0;

	for(item = list->next; item != list; item = item->next)
		count++;

	return count;
}

#ifndef offsetof
#define offsetof(type, member) ((long) &((type *) 0)->member)
#endif

#define dl_list_entry(item, type, member) \
    ((type *) ((char *) item - offsetof(type, member)))

#define dl_list_first(list, type, member) \
    (dl_list_empty((list)) ? NULL : \
     dl_list_entry((list)->next, type, member))

#define dl_list_last(list, type, member) \
    (dl_list_empty((list)) ? NULL : \
     dl_list_entry((list)->prev, type, member))

#define dl_list_for_each(item, list, type, member) \
    for (item = dl_list_entry((list)->next, type, member); \
            &item->member != (list); \
            item = dl_list_entry(item->member.next, type, member))

#define dl_list_for_each_safe(item, n, list, type, member) \
    for (item = dl_list_entry((list)->next, type, member), \
            n = dl_list_entry(item->member.next, type, member); \
            &item->member != (list); \
            item = n, n = dl_list_entry(n->member.next, type, member))

#define dl_list_for_each_reverse(item, list, type, member) \
    for (item = dl_list_entry((list)->prev, type, member); \
            &item->member != (list); \
            item = dl_list_entry(item->member.prev, type, member))

#define DEFINE_DL_LIST(name) \
    struct dl_list name = { &(name), &(name) }

#endif
