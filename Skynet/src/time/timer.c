#include <string.h>

#include "timer.h"

#define TIMER_VEC_SIZE    5

struct tvec_root_s {
	struct list_head head;
	int     expire;
	time_t  min_expiring_time;
};

static struct tvec_root_s  vec[TIMER_VEC_SIZE];
static struct list_head    micro_timer;

struct timeval  now;
struct tm       tm_cur;

enum {
	max_timer_type	= 10000
};

/*用于保存定时器回调函数的地址*/
static timer_cb_func_t tcfs[max_timer_type];

static inline void add_timer(timer_struct_t* t);

static inline int  find_min_idx(time_t diff, int max_idx);
static inline void set_min_exptm(time_t exptm, int idx);

static inline timer_struct_t*
find_event(list_head_t* head, timer_cb_func_t func);
static inline timer_struct_t*
find_event_with_expire(list_head_t* head, timer_cb_func_t function, time_t expire);

void setup_timer()
{
	int i;

	renew_now();
	for (i = 0; i < TIMER_VEC_SIZE; i++) {
		INIT_LIST_HEAD(&vec[i].head);
		vec[i].expire = 1 << (i + 2);
	}
	INIT_LIST_HEAD(&micro_timer);
}

void destroy_timer()
{
	int i;
	list_head_t *l, *p;

	for (i = 0; i < TIMER_VEC_SIZE; i++) {
		list_for_each_safe(l, p, &vec[i].head) {
			timer_struct_t* t = list_entry(l, timer_struct_t, entry);
			do_remove_timer(t, 1);
		}	
	}

	list_for_each_safe(l, p, &micro_timer) {
		micro_timer_struct_t* t = list_entry(l, micro_timer_struct_t, entry);
		remove_micro_timer(t, 1);
	}
}

timer_struct_t*
add_event(list_head_t* head, timer_cb_func_t function, void* owner, void* data, time_t expire, timer_add_mode_t flag)
{
	timer_struct_t* timer;

	if (flag != timer_add_new_timer) {
		timer = find_event(head, function);
		if (!timer) {
			goto new_timer;
		}

		if ( flag == timer_replace_timer ) {			
			mod_expire_time(timer, expire);
		}
		return timer;
	}
new_timer:
	timer = g_slice_alloc(sizeof *timer);
	INIT_LIST_HEAD(&timer->sprite_list);
	INIT_LIST_HEAD(&timer->entry);
	timer->function  = function;
	timer->func_indx = 0;
	timer->expire    = expire;
	timer->owner     = owner;
	timer->data      = data;

	list_add_tail(&timer->sprite_list, head);
	add_timer(timer);
	return timer;
}

timer_struct_t*
add_event_ex(list_head_t* head, int fidx, void* owner, void* data, time_t expire, timer_add_mode_t flag)
{
	if (!tcfs[fidx]) {
		return 0;
	}

	timer_struct_t* timer = add_event(head, tcfs[fidx], owner, data, expire, flag);
	if (timer) {
		timer->func_indx = fidx;
	}
	return timer;
}

static void
scan_timer_list(const int idx)
{
	int    i;
	time_t min_exptm = 0;
	timer_struct_t *t;
	list_head_t *cur, *next;

	list_for_each_safe(cur, next, &vec[idx].head) {
		t = list_entry(cur, timer_struct_t, entry);
		if (t->function) {
			i = find_min_idx(t->expire - now.tv_sec, idx);
			if (i != idx) {
				list_del(&t->entry);
				list_add_tail(&t->entry, &vec[i].head);
				set_min_exptm(t->expire, i);
			} else if ((t->expire < min_exptm) || !min_exptm) {
				min_exptm = t->expire;
			}
		} else {
			do_remove_timer(t, 1);			
		}
 	}

	vec[idx].min_expiring_time = min_exptm;
}

void scan_seconds_timer()
{
	list_head_t *l, *p;
	timer_struct_t* t;

	list_for_each_safe(l, p, &vec[0].head) {
		t = list_entry(l, timer_struct_t, entry);
		if (!(t->function)) {
			do_remove_timer(t, 1);
		} else if (t->expire <= now.tv_sec) {
			if ( t->function(t->owner, t->data) == 0 ) {
				do_remove_timer(t, 1);
			}
		}
	}

	int i = 1;
	for (; i != TIMER_VEC_SIZE; ++i) {
		if ((vec[i].min_expiring_time - now.tv_sec) < 2) {
			scan_timer_list(i);
		}
	}
}

void mod_expire_time(timer_struct_t* t, time_t expiretime)
{
	if (!t->expire) return;

	time_t diff = expiretime - now.tv_sec;
	int j = 0;

	t->expire = expiretime;
	for ( ; j != (TIMER_VEC_SIZE - 1); ++j ) {
		if ( diff <= vec[j].expire ) break;
	}

	list_del_init(&t->entry);
	list_add_tail(&t->entry, &vec[j].head);
	set_min_exptm(t->expire, j);
}

micro_timer_struct_t*
add_micro_event(timer_cb_func_t func, const struct timeval* tv, void* owner, void* data)
{
	micro_timer_struct_t* timer = g_slice_alloc(sizeof *timer);
	INIT_LIST_HEAD(&timer->entry);
	timer->function  = func;
	timer->func_indx = 0;
	timer->tv        = *tv;
	timer->owner     = owner;
	timer->data      = data;

	list_add_tail(&timer->entry, &micro_timer);
	return timer;
}

micro_timer_struct_t*
add_micro_event_ex(int fidx, const struct timeval* tv, void* owner, void* data)
{
	micro_timer_struct_t* timer = add_micro_event(tcfs[fidx], tv, owner, data);
	if (timer) {
		timer->func_indx = fidx;
	}
	return timer;
}

void
scan_microseconds_timer()
{
	list_head_t *l, *p;
	micro_timer_struct_t* t;

	list_for_each_safe(l, p, &micro_timer) {
		t = list_entry(l, micro_timer_struct_t, entry);
		if (!(t->function)) {
			remove_micro_timer(t, 1);
		} else if (now.tv_sec > t->tv.tv_sec || (now.tv_sec == t->tv.tv_sec && now.tv_usec > t->tv.tv_usec)) {
			if (t->function(t->owner, t->data) == 0) {
				remove_micro_timer(t, 1);
			}
		}
	}
}

void remove_micro_timers(void* owner)
{
	list_head_t *l, *p;
	micro_timer_struct_t* t;

	list_for_each_safe(l, p, &micro_timer) {
		t = list_entry(l, micro_timer_struct_t, entry);
		if (t->owner == owner) {
			remove_micro_timer(t, 0);
		}
	}
}

/*----------------------------------------------
  *  inline utilities
  *----------------------------------------------*/
static inline void
add_timer(timer_struct_t *t)
{
	int i, diff;

	diff = t->expire - now.tv_sec;
	for (i = 0; i != (TIMER_VEC_SIZE - 1); ++i) {
		if (diff <= vec[i].expire)
			break;
	}

	list_add_tail(&t->entry, &vec[i].head);
	set_min_exptm(t->expire, i);
}

static inline int
find_min_idx(time_t diff, int max_idx)
{
	while (max_idx && (vec[max_idx - 1].expire >= diff)) {
		--max_idx;
	}
	return max_idx;
}

static inline void
set_min_exptm(time_t exptm, int idx)
{
	if ((exptm < vec[idx].min_expiring_time) || (vec[idx].min_expiring_time == 0)) {
		vec[idx].min_expiring_time = exptm;
	}
}

static inline timer_struct_t *
find_event(list_head_t* head, timer_cb_func_t function)
{
	timer_struct_t* t;

	list_for_each_entry(t, head, sprite_list) {
		if (t->function == function)
			return t;
	}

	return NULL;
}

static inline timer_struct_t *
find_event_with_expire(list_head_t* head, timer_cb_func_t function, time_t expire)
{
	timer_struct_t* t;

	list_for_each_entry(t, head, sprite_list) {
		if (t->function == function && t->expire == expire)
			return t;
	}

	return NULL;
}

/*根据定时器的类型ID登记回调函数的地址*/
int register_timer_callback(int nbr, timer_cb_func_t cb)
{
	if (nbr <= 0 || nbr >= max_timer_type) {
		return -1;
	}
	if (tcfs[nbr]) {
		return -1;
	}
	tcfs[nbr] = cb;
	return 0;
}

void unregister_timers_callback()
{
	memset(tcfs, 0, sizeof(tcfs));
}

/*重新加载text.so后，回调函数的地址发生变化，更新已启动的定时器回调函数地址*/
void refresh_timers_callback()
{
	int i;
	for (i = 0; i < TIMER_VEC_SIZE; i++) {
		timer_struct_t* t;
		list_for_each_entry (t, &vec[i].head, entry) {
			t->function = tcfs[t->func_indx];
		}	
	}

	micro_timer_struct_t* mt;
	list_for_each_entry (mt, &micro_timer, entry) {
		mt->function = tcfs[mt->func_indx];
	}
}

