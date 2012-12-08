/**
 *============================================================
 *  @file      timer.h
 *  @brief     定时器函数，有秒级和毫秒级两种精度的接口。用于设定某一时刻调用某个函数。需要glib支持。
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *============================================================
 */

#ifndef ant_TIMER_H_
#define ant_TIMER_H_

// Since C89
#include <stdlib.h>
#include <time.h>
// Since C99
#include <stdint.h>
// Posix
#include <sys/time.h>
// Glib
#include <glib.h>
// User-defined
#include "kernel_ds/list.h"

// export now and tm_cur
extern struct timeval now;
extern struct tm      tm_cur;

/**
 * @typedef timer_cb_func_t
 * @brief   回调函数的类型。如果回调函数返回0，则表示定时器到期时要删除该定时器，反之，则不删除。
 */
typedef int (*timer_cb_func_t)(void*, void*);

/**
 * @typedef timer_struct_t
 * @brief   timer_struct的typedef。
 */

/**
 * @struct  timer_struct
 * @brief   秒级定时器。
 */
typedef struct timer_struct {
	struct list_head	entry;
	struct list_head	sprite_list;
	/*! 定时器的触发时刻，亦即调用回调函数的时间。*/
	time_t				expire;
	void*				owner;
	void*				data;
	timer_cb_func_t		function;
	int					func_indx;
} timer_struct_t ;

/**
 * @typedef micro_timer_struct_t
 * @brief   micro_timer_struct的typedef。
 */

/**
 * @struct  micro_timer_struct
 * @brief   微秒级定时器。
 */
typedef struct micro_timer_struct {
	struct list_head	entry;
	/*! 定时器的触发时刻，亦即调用回调函数的时间。*/
	struct timeval		tv;
	void*				owner;
	void*				data;
	timer_cb_func_t		function;
	int					func_indx;
} micro_timer_struct_t ;

/**
 * @typedef timer_add_mode_t
 * @brief   timer_add_mode的typedef。
 */

/**
 * @enum   timer_add_mode
 * @brief  定时器添加模式。用于add_event函数。
 * @see    add_event
 */
typedef enum timer_add_mode {
	/*! 添加一个新的定时器。*/
	timer_add_new_timer,
	/*! 替换一个定时器。如果找不到符合条件的定时器，则创建一个新的定时器。*/
	timer_replace_timer,
	// for backward compatibility only
	ADD_EVENT = timer_add_new_timer,
	ADD_EVENT_REPLACE_UNCONDITIONALLY = timer_replace_timer
} timer_add_mode_t;

/**
 * @brief  初始化定时器功能。必须调用了这个函数，才能使用定时器功能。
 * @see    destroy_timer
 */
void setup_timer();
/**
 * @brief  销毁所有定时器（包括秒级和微秒级的定时器），并释放内存。
 * @see    setup_timer
 */
void destroy_timer();

void scan_seconds_timer();
void scan_microseconds_timer();
static inline void renew_now();

/**
 * @brief  扫描定时器列表，调用到期了的定时器的回调函数，并根据回调函数的返回值决定是否需要把该定时器删除掉。
 *         如果回调函数返回0，则表示要删除该定时器，反之，则不删除。必须定期调用该函数才能调用到期了的定时器的回调函数。
 */
static inline void
handle_timer()
{
	static time_t last = 0;

	renew_now();
	//second timer
	if (last != now.tv_sec) {
		last = now.tv_sec;
		scan_seconds_timer();
	}
	//microseconds timer
	scan_microseconds_timer();
}

/**
 * @brief  添加/替换一个秒级定时器，该定时器的到期时间是expire，到期时回调的函数是func。
 * @param  head 链头，新创建的定时器会被插入到该链表中。
 * @param  func 定时器到期时调用的回调函数。
 * @param  owner 传递给回调函数的第一个参数。
 * @param  data 传递给回调函数的第二个参数。
 * @param  expire 定时器到期时间（从Epoch开始的秒数）。
 * @param  flag 指示add_event添加/替换定时器。如果flag==timer_replace_unconditionally，
 *         那么add_event将在head链表中搜索出第一个回调函数==func的定时器，
 *         然后把这个定时器的到期时间修改成expire。如果找不到符合条件的定时器，则新建一个定时器。
 *         建议只有当head链表中所有定时器的回调函数都各不相同的情况下，才使用timer_replace_unconditionally。
 *         注意：绝对不能在定时器的回调函数中修改该定时器的到期时间！
 * @return 指向新添加/替换的秒级定时器的指针。
 * @see    ADD_TIMER_EVENT, REMOVE_TIMER, remove_timers, REMOVE_TIMERS
 */
timer_struct_t*
add_event(list_head_t* head, timer_cb_func_t func, void* owner, void* data, time_t expire, timer_add_mode_t flag);

/**
 * @brief  添加/替换一个秒级定时器，该定时器的到期时间是expire，回调函数是register_timer函数根据定时器类型登记。
 * @param  head 链头，新创建的定时器会被插入到该链表中。
 * @param  fidx 定时器类型。
 * @param  owner 传递给回调函数的第一个参数。
 * @param  data 传递给回调函数的第二个参数。
 * @param  expire 定时器到期时间（从Epoch开始的秒数）。
 * @param  flag 指示add_event添加/替换定时器。如果flag==timer_replace_unconditionally，
 *         那么add_event将在head链表中搜索出第一个回调函数==func的定时器，
 *         然后把这个定时器的到期时间修改成expire。如果找不到符合条件的定时器，则新建一个定时器。
 *         建议只有当head链表中所有定时器的回调函数都各不相同的情况下，才使用timer_replace_unconditionally。
 *         注意：绝对不能在定时器的回调函数中修改该定时器的到期时间！
 * @return 指向新添加/替换的秒级定时器的指针。
 * @see    ADD_TIMER_EVENT, REMOVE_TIMER, remove_timers, REMOVE_TIMERS
 */
timer_struct_t*
add_event_ex(list_head_t* head, int fidx, void* owner, void* data, time_t expire, timer_add_mode_t flag);

/**
 * @def    ADD_TIMER_EVENT
 * @brief  创建一个新的秒级定时器，该定时器的到期时间是exptm_，到期时调用的回调函数是func_，传递给回调函数
 *         func_的第一个参数是owner_，第二个参数是data_。如果你要传递给回调函数的第一个参数是一个结构体，
 *         并且该结构体里面有一个名为timer_list的list_head_t类型的成员变量，那么你就可以使用这个宏来简化创建新定时器的操作。
 * @param  owner_ 传递给回调函数的第一个参数。指针类型，指向一个结构体，并且这个结构体里面必须有一个名为
 *         timer_list的list_head_t类型的成员变量。新创建的定时器会被插入到owner_下的timer_list链表中。
 * @param  func_ 定时器到期时调用的回调函数。
 * @param  data_ 传递给回调函数的第二个参数。
 * @param  exptm_ 定时器的到期时间（从Epoch开始的秒数）。
 * @return 指向新创建的秒级定时器的指针。
 * @see    add_event
 */
#define ADD_TIMER_EVENT(owner_, func_, data_, exptm_) \
		add_event( &((owner_)->timer_list), (func_), (owner_), (data_), (exptm_), timer_add_new_timer)

/**
 * @def    ADD_TIMER_EVENT_EX
 * @brief  创建一个新的秒级定时器，该定时器的到期时间是exptm_，回调函数是register_timer根据根据定时器类型登记的，
 *         传递给回调函数的第一个参数是owner_，第二个参数是data_。如果你要传递给回调函数的第一个参数是一个结构体，
 *         并且该结构体里面有一个名为timer_list的list_head_t类型的成员变量，那么你就可以使用这个宏来简化创建新定时器的操作。
 * @param  owner_ 传递给回调函数的第一个参数。指针类型，指向一个结构体，并且这个结构体里面必须有一个名为
 *         timer_list的list_head_t类型的成员变量。新创建的定时器会被插入到owner_下的timer_list链表中。
 * @param  fidx_ 定时器类型。
 * @param  data_ 传递给回调函数的第二个参数。
 * @param  exptm_ 定时器的到期时间（从Epoch开始的秒数）。
 * @return 指向新创建的秒级定时器的指针。
 * @see    add_event
 */
#define ADD_TIMER_EVENT_EX(owner_, fidx_, data_, exptm_) \
		add_event_ex( &((owner_)->timer_list), (fidx_), (owner_), (data_), (exptm_), timer_add_new_timer)

/**
 * @brief  修改秒级定时器tmr的到期时间。注意：绝对不能在定时器的回调函数中修改该定时器的到期时间！
 * @param  tmr 需要修改到期时间的定时器。
 * @param  exptm 将tmr的到期时间修改成exptm（从Epoch开始的秒数）。
 * @see    add_event, ADD_TIMER_EVENT
 */
void mod_expire_time(timer_struct_t* tmr, time_t exptm);

static inline void
do_remove_timer(timer_struct_t* t, int freed)
{
	if (t->sprite_list.next != 0) {
		list_del(&t->sprite_list);
	}
	if (freed) {
		list_del(&t->entry);
		g_slice_free1(sizeof *t, t);
	} else {
		t->function = 0;
		t->func_indx = 0;
	}
}

/**
 * @brief  删除链表head中所有的定时器，并释放内存。用于删除秒级定时器。
 * @param  head 定时器链表的链头。
 * @see    add_event, ADD_TIMER_EVENT
 */
static inline void
remove_timers(list_head_t* head)
{
	timer_struct_t *t;
	list_head_t *l, *m;

	list_for_each_safe (l, m, head) {
		t = list_entry (l, timer_struct_t, sprite_list);
		do_remove_timer(t, 0);
	}
}

/**
 * @def    REMOVE_TIMER
 * @brief  删除秒级定时器timer_。
 * @param  timer_ 调用add_event创建定时器时返回的指针。
 * @see    add_event, ADD_TIMER_EVENT, remove_timers
 */
#define REMOVE_TIMER(timer_) \
		do_remove_timer((timer_), 0)

/**
 * @brief  添加一个微秒级定时器，该定时器的到期时间是tv，到期时回调的函数是func。
 * @param  func 定时器到期时调用的回调函数。
 * @param  tv 定时器到期时间。
 * @param  owner 传递给回调函数的第一个参数。
 * @param  data 传递给回调函数的第二个参数。
 * @return 指向新添加的微秒级定时器的指针。
 * @see    REMOVE_MICRO_TIMER, remove_micro_timers, REMOVE_TIMERS
 */
micro_timer_struct_t*
add_micro_event(timer_cb_func_t func, const struct timeval* tv, void* owner, void* data);

/**
 * @brief  添加一个微秒级定时器，该定时器的到期时间是tv，回调函数是register_timer函数根据定时器类型登记的。
 * @param  fidx 定时器类型。
 * @param  tv 定时器到期时间。
 * @param  owner 传递给回调函数的第一个参数。
 * @param  data 传递给回调函数的第二个参数。
 * @return 指向新添加的微秒级定时器的指针。
 * @see    REMOVE_MICRO_TIMER, remove_micro_timers, REMOVE_TIMERS
 */
micro_timer_struct_t*
add_micro_event_ex(int fidx, const struct timeval* tv, void* owner, void* data);

static inline void
remove_micro_timer(micro_timer_struct_t *t, int freed)
{
	if (freed) {
		list_del_init(&t->entry);
		g_slice_free1(sizeof *t, t);
	} else {
		t->function = 0;
		t->func_indx = 0;
	}
}

/**
 * @brief  删除传递给回调函数的第一个参数==owner的所有微秒级定时器，并释放内存。
 * @param  owner 传递给回调函数的第一个参数。
 * @see    add_micro_event, REMOVE_MICRO_TIMER, REMOVE_TIMERS
 */
void remove_micro_timers(void* owner);

/**
 * @def    REMOVE_MICRO_TIMER
 * @brief  删除微秒级定时器timer_，并释放内存。
 * @param  timer_ 调用add_micro_event创建定时器时返回的指针。
 * @see    add_micro_event, remove_micro_timers, REMOVE_TIMERS
 */
#define REMOVE_MICRO_TIMER(timer_) \
		remove_micro_timer((timer_), 0)
/**
 * @def    REMOVE_TIMERS
 * @brief  删除owner_指向的结构体中timer_list成员变量中的所有定时器（对应秒级定时器），并释放内存；
 *         删除传递给回调函数的第一个参数==owner_的所有微秒级定时器，并释放内存。
 * @param  owner_ 指向一个结构体，并且该结构体里面必须有一个名为timer_list的list_head_t类型的成员变量。
 * @see    ADD_TIMER_EVENT, remove_timers, add_micro_event, remove_micro_timers
 */
#define REMOVE_TIMERS(owner_) \
		remove_timers(&((owner_)->timer_list)), remove_micro_timers((owner_))

/**
 * @brief 更新当前时间。
 * @see get_now_tv, get_now_tm
 */
static inline void
renew_now()
{
	gettimeofday(&now, 0);
	localtime_r(&now.tv_sec, &tm_cur);
}

/**
 * @brief 对于对实时性要求不会太高的程序这种循环处理客户端发过来的数据包的程序，
 *        可以先调用一次renew_now，然后处理N个数据包，然后再调用一次renew_now，接着再处理N个数据包，如此反复。
 *        这样在处理数据包的函数里就可以直接使用get_now_tv来获取不太精确的当前时间，从而能稍微提升程序的效率。
 * @return 不太精确的当前时间。
 * @see renew_now, get_now_tm
 */
static inline const struct timeval*
get_now_tv()
{
	return &now;
}

/**
 * @brief 对于对实时性要求不会太高的程序，比如摩尔庄园的Online Server这种循环处理客户端发过来的数据包的程序，
 *        可以先调用一次renew_now，然后处理N个数据包，然后再调用一次renew_now，接着再处理N个数据包，如此反复。
 *        这样在处理数据包的函数里就可以直接使用get_now_tm来获取不太精确的当前时间，从而能稍微提升程序的效率。
 * @return 不太精确的当前时间。
 * @see renew_now, get_now_tv
 */
static inline const struct tm*
get_now_tm()
{
	return &tm_cur;
}

/**
 * @brief 登记定时器类型，将定时器类型id与回调函数的对应关系保存在一个固定大小的数组中
 * @param nbr,定时器的类型；
 * @param cb,回调函数；
 * @return 0，成功；-1，失败。
 */
int register_timer_callback(int nbr, timer_cb_func_t cb);

/**
 * @brief 删除登记过的所有定时器类型
 */
void unregister_timers_callback();

/**
 * @brief 程序在线加载text.so时，由于定时器回调函数的地址会发生变化，需要更新定时器类型id与回调函数的关系对应表
 */ 
void refresh_timers_callback();

#endif // ant_TIMER_H_
