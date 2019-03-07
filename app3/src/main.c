#include <zephyr.h>
#include <kernel.h>
#include <misc/printk.h>

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

struct k_mutex my_mutex;
k_tid_t thread1, thread2;//, thread3;

void threadA(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	printk("thread A\n");
	if(k_mutex_lock(&my_mutex, 1) == 0){
		while(1){
			printk("threadA\n");
			k_busy_wait(100000);
		}
	}
	else{
		printk("Connot lock\n");
	}
	k_mutex_unlock(&my_mutex);
}

void threadB(void* dummy1, void *dummy2, void *dummy3){
	int i = 0;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	k_mutex_lock(&my_mutex, K_FOREVER);
	while(1){
		i++;
		printk("threadB's prio: %d\n",(int)k_thread_priority_get(k_current_get()));
		k_busy_wait(100000);
//		k_mutex_unlock(&my_mutex);
		if(i == 5) k_mutex_unlock(&my_mutex);
	}
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

	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, 1);
	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, K_NO_WAIT);

	return 0;
}

