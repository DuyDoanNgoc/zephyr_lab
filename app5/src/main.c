#include <zephyr.h>
#include <kernel.h>
#include <misc/printk.h>

#define MY_STACKSIZE 500
#define PRIORITY1 8
#define PRIORITY2 8
#define PRIORITY3 8


struct k_thread my_thread1;
struct k_thread my_thread2;
struct k_thread my_thread3;

K_THREAD_STACK_DEFINE(thread1_stack, MY_STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, MY_STACKSIZE);
K_THREAD_STACK_DEFINE(thread3_stack, MY_STACKSIZE);
k_tid_t thread1, thread2, thread3;

struct data_item_type{
	u32_t field1;
	u32_t field2;
	u32_t field3;
};

char __aligned(4) my_msgq_buffer[10 * sizeof(struct data_item_type)];
struct k_msgq my_msgq;


void threadA(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	printk("A\n");
	struct data_item_type data;
	int i;
	for(i = 0 ; i < 12 ; i++) {
		data.field1 = (u32_t)i+1;
		data.field2 = (u32_t)i+2;
		data.field3 = (u32_t)i+3;
					
		while(k_msgq_put(&my_msgq, &data, K_NO_WAIT) != 0){
			k_msgq_purge(&my_msgq);
			printk("%d: send: %d %d %d\n", i+1, (int)data.field1, (int)data.field2, (int)data.field3);
			k_busy_wait(100000);

		}
	}
}

void threadB(void* dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	printk("B\n");
	struct data_item_type data;
	int i;
	for(i = 0 ; i < 12 ; i++) {
		k_msgq_get(&my_msgq, &data, K_NO_WAIT);
//		k_msgq_peek(&my_msgq, &data);

		printk("%d: get: %d %d %d\n", i+1, (int)data.field1, (int)data.field2, (int)data.field3);
		printk("wait..\n");
		k_busy_wait(100000);
	}
}

void threadC(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	struct data_item_type data;
	data.field1 = 1;
	data.field2 = 2;
	data.field3 = 3;

	printk("C\n");
	k_msgq_put(&my_msgq, &data, K_NO_WAIT);
/*	
	while(1){
		printk("threadC\n");
		k_busy_wait(100000);
	}
*/
}

int main(void){
//	k_sched_time_slice_set(10000,4);
	k_msgq_init(&my_msgq, my_msgq_buffer, sizeof(struct data_item_type), 10);
	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, 5);

	return 0;
}

