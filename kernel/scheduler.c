#include <kernel/scheduler.h>

void init_scheduler(void){
    thread_t *main_thread = (thread_t *)kmalloc(sizeof(thread_t));
    main_thread->tid = 0;
    main_thread->state = RUNNING;
    main_thread->stack_bottom = NULL;

    enqueue_thread(main_thread);
    current_thread = main_thread;

    create_thread(&idle_thread_entry, 0x1000);
}

uint64_t scheduler_handler(uint64_t old_rsp){
    if (!ready_list_head || ready_list_head->next == ready_list_head) return old_rsp;

    thread_t *old_thread = current_thread;

    if (old_thread->state != DEAD){
        old_thread->rsp = old_rsp;
        old_thread->state = READY;
    } else dequeue_thread(old_thread);

    thread_t *next_thread = (old_thread->state == DEAD) ? ready_list_head : old_thread->next;

    while (next_thread->state == DEAD){
        next_thread = next_thread->next;
        if (next_thread == ready_list_head && next_thread->state == DEAD) return old_rsp;
    }

    next_thread->state = RUNNING;
    current_thread = next_thread;

    return next_thread->rsp;
}