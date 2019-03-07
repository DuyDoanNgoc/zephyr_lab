#include <zephyr.h>
#include <kernel.h>
#include <misc/printk.h>
#include <kernel_structs.h>

#define MY_STACKSIZE 500
#define PRIORITY1 7
#define PRIORITY2 8
//#define PRIORITY3 8


struct k_thread my_thread1;
struct k_thread my_thread2;
//struct k_thread my_thread3;

K_THREAD_STACK_DEFINE(thread1_stack, MY_STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, MY_STACKSIZE);
//K_THREAD_STACK_DEFINE(thread3_stack, MY_STACKSIZE);

k_tid_t thread1, thread2;//, thread3;

void threadC(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	while(1){
		printk("threadC\n");
		k_busy_wait(100000);
	}
}

void threadA(void *dummy1, void *dummy2, void *dummy3){
//	int i = 0;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
//	printk("threadA\n");
//	printk("%d\n",(int)k_current_get());
	printk("%d\n",(int)thread1->base.thread_state);
//	printk("%d\n",(int)(thread1->base.thread_state & _THREAD_DEAD));
//printk("%d\n",(int)(thread1->base.thread_state & _THREAD_DUMMY));
//printk("%d\n",(int)(thread1->base.thread_state & _THREAD_PENDING));
//printk("%d\n",(int)(thread1->base.thread_state & _THREAD_PRESTART));
//printk("%d\n",(int)(thread1->base.thread_state & _THREAD_SUSPENDED));
//printk("%d\n",(int)(thread1->base.thread_state & _THREAD_QUEUED));
//	printk("%d\n",(int)(thread2->base.thread_state & _THREAD_PRESTART));
//	printk("%d\n",(int)thread2->base.thread_state);
//		printk("threadA\n");
	
//	k_sleep(10);
	printk("threadA\n");
//	while(1){
//		k_busy_wait(100000);
//		k_thread_abort(k_current_get());
//		if(i==0)k_sleep(1000);
//		i++;
//	}

}


void threadB(void* dummy1, void *dummy2, void *dummy3){
//	int i = 0;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
//	printk("threadB\n");
//	printk("%d\n",(int)k_current_get());
//	printk("make threadA\n");
//	k_thread_start(thread);
//	k_thread_abort(thread1);
//	k_sched_lock();
//	printk("%d\n",(int)(thread1->base.thread_state & _THREAD_DEAD));
//	if((thread1->base.thread_state & _THREAD_DEAD) == 8){
//		printk("thread A is DEAD\n");
//	}
	printk("%d\n",(int)thread1->base.thread_state);
	while(1){
		printk("threadB\n");
		k_busy_wait(100000);
//		k_thread_start(thread1);
//		if(i==0) k_sleep(1000);
//		i++;
	}
//	k_sched_unlock();
}

int main(void){
//	k_sched_time_slice_set(1000, 4);	

	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);

	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);

//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, K_NO_WAIT);



//	printk("main: %d\n",(int)(k_thread_priority_get(k_current_get())));


	return 0;
}



//K_THREAD_DEFINE(thread_a, MY_STACKSIZE, threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
//K_THREAD_DEFINE(thread_b, MY_STACKSIZE, threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
