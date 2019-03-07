#include <zephyr.h>
#include <kernel.h>
#include <misc/printk.h>
#include <kernel_structs.h>

#define MY_STACKSIZE 500
#define PRIORITY1 8
#define PRIORITY2 7
#define PRIORITY3 8


struct k_thread my_thread1;
struct k_thread my_thread2;
struct k_thread my_thread3;

K_THREAD_STACK_DEFINE(thread1_stack, MY_STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, MY_STACKSIZE);
K_THREAD_STACK_DEFINE(thread3_stack, MY_STACKSIZE);
k_tid_t thread1, thread2, thread3;

void threadA(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	int i = 0;
	printk("thread B's state: %d\n",(int)(thread2->base.thread_state));
	k_thread_start(thread2);
//	k_thread_suspend(thread2);
	while(1){
		i++;
//		printk("threa B's state: %d\n",(int)(thread2->base.thread_state & _THREAD_SUSPENDED));
		printk("threa B's state: %d\n",(int)(thread2->base.thread_state & _THREAD_DEAD));
		printk("thread A\n");
		k_busy_wait(100000);
//		if(i == 5)k_sleep(10);
//		if(i==8)k_thread_resume(thread2);
	}
}

void threadB(void* dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	while(1){
		printk("thread B\n");
		k_busy_wait(100000);
	}
}

void threadC(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
}

int main(void){
	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, 5);

	return 0;
}

