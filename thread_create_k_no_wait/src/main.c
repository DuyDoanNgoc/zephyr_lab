#include <zephyr.h>
#include <kernel.h>
#include <misc/printk.h>
#include <kernel_structs.h>

#define MY_STACKSIZE 500
#define PRIORITY1 7
#define PRIORITY2 8
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
	
	printk("thread A start!!!\n");
	printk("thread A: %d\n", (int)(thread1->base.thread_state));
	printk("thread B: %d\n", (int)(thread2->base.thread_state));
//	printk("thread C: %d\n", (int)(thread3->base.thread_state));
	printk("suspend self!!!\n");
	k_thread_suspend(thread1);	
	while(1){
		printk("thread A running!!!\n");
		printk("thread A: %d\n", (int)(thread1->base.thread_state));
		printk("thread B: %d\n", (int)(thread2->base.thread_state));
//		printk("thread C: %d\n", (int)(thread3->base.thread_state));
//		k_sleep(10);
		k_busy_wait(100000);
	}
}

void threadB(void* dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	printk("thread B start!!!\n");
	printk("thread A: %d\n", (int)(thread1->base.thread_state));
	printk("thread B: %d\n", (int)(thread2->base.thread_state));
//	printk("thread C: %d\n", (int)(thread3->base.thread_state));
	int i = 0;
	while(1){
		i++;
		printk("thread B running!!!\n");
		if(i==5){
//			printk("thread A aborting!!!\n");
//			k_thread_abort(thread1);
			printk("thread A resume!!!\n");
			k_thread_priority_set(thread1,8);
			printk("%d\n",k_thread_priority_get(thread1));
			k_thread_resume(thread1);
		}
//		if(i==10){
//			printk("thread B restart!!!!\n");
//			k_thread_start(thread1);
//		}
		printk("thread A: %d\n", (int)(thread1->base.thread_state));
		printk("thread B: %d\n", (int)(thread2->base.thread_state));
//		printk("thread C: %d\n", (int)(thread3->base.thread_state));

		k_busy_wait(100000);
	}
}

void threadC(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	printk("thread C start!!!\n");
	printk("thread A: %d\n", (int)(thread1->base.thread_state));
	printk("thread B: %d\n", (int)(thread2->base.thread_state));
	printk("thread C: %d\n", (int)(thread3->base.thread_state));
	
	while(1){
		printk("thread C running!!!\n");
		printk("thread A: %d\n", (int)(thread1->base.thread_state));
		printk("thread B: %d\n", (int)(thread2->base.thread_state));
		printk("thread C: %d\n", (int)(thread3->base.thread_state));

		k_busy_wait(100000);
	}
}

int main(void){

	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, K_NO_WAIT);

	return 0;
}

