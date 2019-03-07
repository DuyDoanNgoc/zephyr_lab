#include <zephyr.h>
#include <kernel.h>
#include <misc/printk.h>
#include <kernel_structs.h>

#define MY_STACKSIZE 500
#define PRIORITY1 8
#define PRIORITY2 8
//#define PRIORITY3 8


struct k_thread my_thread1;
struct k_thread my_thread2;
//struct k_thread my_thread3;

K_THREAD_STACK_DEFINE(thread1_stack, MY_STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, MY_STACKSIZE);
//K_THREAD_STACK_DEFINE(thread3_stack, MY_STACKSIZE);

struct k_mutex my_mutex;
k_tid_t thread1, thread2;//, thread3;

void threadA(void *dummy1, void *dummy2, void *dummy3){
	int i = 0;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
//	printk("thread A\nyield\n");
//	k_yield();
	printk("thread A start!!\n");
	if(k_mutex_lock(&my_mutex, K_FOREVER) == 0){
		while(1){
			i++;
			printk("thread A running!!!\n");
			k_busy_wait(100000);
			printk("%d\n",(int)(thread2->base.thread_state));
			if(i == 5){
//				k_thread_suspend(thread2);
//				k_thread_abort(thread1);
//				printk("yield?\n");
				k_sleep(1);
			}
			if(i==10){
				k_mutex_unlock(&my_mutex);
				
			}
		}
	}
//	else{
//		printk("Connot lock\n");
//	}	
}

void threadB(void* dummy1, void *dummy2, void *dummy3){
	int i = 0;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	k_mutex_lock(&my_mutex, K_FOREVER);
//	k_thread_abort(thread1);
	while(1){
//		i++;
		printk("thread B running!!!\n");
		printk("threadB's prio: %d\n",(int)k_thread_priority_get(k_current_get()));
//		printk("%d\n",(int)(thread1->base.thread_state));
//		k_thread_suspend(thread1);
//		printk("threadB\n");
		k_busy_wait(100000);
//		if(i==5) k_thread_resume(thread1);
	}
//	k_mutex_unlock(&my_mutex);
}
/*
void threadC(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	while(1){
		printk("threadC\n");
		k_busy_wait(100000);
	}
}
*/
int main(void){
	k_mutex_init(&my_mutex);

	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, K_NO_WAIT);

	return 0;
}

