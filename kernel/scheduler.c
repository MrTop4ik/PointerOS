#include <kernel/scheduler.h>

void init_scheduler(void){
    thread_t *main_thread = (thread_t *)kmalloc(sizeof(thread_t));
    main_thread->tid = 0;
    main_thread->state = RUNNING;
    main_thread->rsp = read_rsp();
    main_thread->stack_bottom = NULL;

    enqueue_thread(main_thread);
    current_thread = main_thread;

    create_thread(&idle_thread_entry, 0x1000);
}

uint64_t scheduler_handler(uint64_t old_rsp){
    if (!ready_list_head || ready_list_head->next == ready_list_head) return old_rsp;

    thread_t *old_thread = current_thread;

    if (old_thread->state == DEAD){
        dequeue_thread(old_thread);
        old_thread->next = dead_list_head;
        dead_list_head = old_thread;
    } else {
        old_thread->rsp = old_rsp;
        old_thread->state = READY;
    }

    thread_t *starting_point = (old_thread->state == DEAD) ? ready_list_head : old_thread->next;
    thread_t *next_thread = starting_point;

    if (!next_thread) return old_rsp;

    while (next_thread->state != READY && next_thread->state != RUNNING){
        next_thread = next_thread->next;
        if (next_thread == starting_point){
            if (old_thread->state == DEAD) for(;;) kprintf("Panic");
            old_thread->state = RUNNING;
            return old_rsp;
        }
        
    }

    next_thread->state = RUNNING;
    current_thread = next_thread;

    return next_thread->rsp;
}