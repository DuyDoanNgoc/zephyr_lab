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
struct k_fifo my_fifo;
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
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while(1){
		printk("thread A\n");
		k_busy_wait(100000);
	}

}


void threadB(void* dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while(1){
		printk("threadB\n");
		k_busy_wait(100000);
	}
}

int main(void){
	k_fifo_init(&my_fifo);
	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);

	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);

//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, K_NO_WAIT);

	return 0;
}



//K_THREAD_DEFINE(thread_a, MY_STACKSIZE, threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
//K_THREAD_DEFINE(thread_b, MY_STACKSIZE, threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
