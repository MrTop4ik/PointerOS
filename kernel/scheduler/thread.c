#include <kernel/scheduler/thread.h>

uint64_t next_thread_id = 0;

thread_t *current_thread = NULL;
thread_t *ready_list_head = NULL;
thread_t *dead_list_head = NULL;

thread_t *create_thread(void (*entry_point)(void), size_t stack_size){
    thread_t *t = (thread_t *)kmalloc(sizeof(thread_t));
    if (!t) return NULL;

    void *stack_mem = alloc_stack(stack_size);
    if (!stack_mem) {
        kfree(t);
        return NULL;
    }

    t->stack_bottom = stack_mem;

    uint64_t *stack_top = (uint64_t*)((uint64_t)stack_mem + stack_size);
    stack_top = (uint64_t*)((uint64_t)stack_top & ~15UL);

    stack_top--; *stack_top = (uint64_t)&thread_exit;

    uint64_t *clean_stack_top = stack_top;

    stack_top--; *stack_top = 0x10;
    stack_top--; *stack_top = (uint64_t)clean_stack_top;
    stack_top--; *stack_top = 0x202;
    stack_top--; *stack_top = 0x08;
    stack_top--; *stack_top = (uint64_t)entry_point;

    for (int i = 0; i < 15; i++){
        stack_top--; *stack_top = 0;
    }

    t->rsp = (uint64_t)stack_top;
    t->tid = next_thread_id++;
    t->state = READY;
    t->next = NULL;
    t->prev = NULL;

    enqueue_thread(t);
    return t;
}

void destroy_thread(thread_t *t){
    if (t){
        if (t->stack_bottom) kfree(t->stack_bottom);
        kfree(t);
    }
}

void thread_exit(void){
    cli();
    current_thread->state = DEAD;
    yield();
}

void enqueue_thread(thread_t *t){
    if (!ready_list_head){
        t->next = t;
        t->prev = t;
        ready_list_head = t;
    } else {
        thread_t *tail = ready_list_head->prev;
        tail->next = t;
        t->prev = tail;
        t->next = ready_list_head;
        ready_list_head->prev = t;
    }
}

void dequeue_thread(thread_t *t){
    if (t->next == t) ready_list_head = NULL;
    else {
        t->prev->next = t->next;
        t->next->prev = t->prev;
        if (ready_list_head == t) ready_list_head = t->next;
    } 
    t->next = t->prev = NULL;
}

void cleanup_dead_threads(void){
    cli();

    thread_t *t = dead_list_head;
    dead_list_head = NULL;
    
    sti();

    while (t){
        thread_t *next_dead = t->next;
        destroy_thread(t);
        t = next_dead;
    }
}

void idle_thread_entry(void){
    while (1){
        cleanup_dead_threads();
        hlt();
    }
}

void third_thread(void){
    for (int i = 0; i < 5; i++) kprintf("Thread 3\n");
}