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

k_tid_t refree, player1, player2;

struct data_item_type{
	int item;
};

char __aligned(4) my_msgq_buffer1[sizeof(struct data_item_type)];
char __aligned(4) my_msgq_buffer2[sizeof(struct data_item_type)];
struct k_msgq my_msgq1;
struct k_msgq my_msgq2;

struct k_mutex my_mutex;

int player1_win, player2_win, draw;

void threadA(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	struct data_item_type data1;
	struct data_item_type data2;
	
	while(1){
		printk("I'm refree, waiting from player 1...\n");
		k_msgq_get(&my_msgq1, &data1, K_FOREVER);
		printk("I'm refree, received message from player 1: %d\n\n",data1.item);		
		
		printk("I'm refree, waiting from player 2...\n");
		k_msgq_get(&my_msgq2, &data2, K_FOREVER);
		printk("I'm refree, received message from player 2: %d\n\n",data2.item);

		//0: rock, 1: scissors, 2: paper
		printk("\nI'm refree, compare two player..\n");
//		if(k_mutex_lock(&my_mutex, K_FOREVER) == 0){
			if(data1.item == data2.item){
				draw++;
				printk("I'm refree, result: draw!!\n");
				//0: draw, 1: win, 2: lose
				data1.item = 0;
				data2.item = 0;
				k_msgq_put(&my_msgq1, &data1.item, K_NO_WAIT);
				k_msgq_put(&my_msgq2, &data2.item, K_NO_WAIT);
			}
			else if(data1.item == 0 && data2.item == 2) {
				player2_win++;
				printk("I'm refree, result: player2 win!!\n");

				data1.item = 2;
				data2.item = 1;
				k_msgq_put(&my_msgq1, &data1.item, K_NO_WAIT);
				k_msgq_put(&my_msgq2, &data2.item, K_NO_WAIT);			
			}
			else if(data1.item == 2 && data2.item == 0){
				player1_win++;
				printk("I'm refree, result: player1 win!!\n");

				data1.item = 1;
				data2.item = 2;
				k_msgq_put(&my_msgq1, &data1.item, K_NO_WAIT);
				k_msgq_put(&my_msgq2, &data2.item, K_NO_WAIT);
			}
			else{
				if(data1.item > data2.item){
					player2_win++;
					printk("I'm refree, result: player2 win!!\n");

					data1.item = 2;
					data2.item = 1;
					k_msgq_put(&my_msgq1, &data1.item, K_NO_WAIT);
					k_msgq_put(&my_msgq2, &data2.item, K_NO_WAIT);
				}
				else{
					player1_win++;
					printk("I'm refree, result: player1 win!!\n");

					data1.item = 1;
					data2.item = 2;
					k_msgq_put(&my_msgq1, &data1.item, K_NO_WAIT);
					k_msgq_put(&my_msgq2, &data2.item, K_NO_WAIT);
				}
			}
//		}
//		k_mutex_unlock(&my_mutex);
		k_busy_wait(100000);
	}
}


void threadB(void* dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	struct data_item_type data;

	while(1){
		data.item = sys_rand32_get() % 3;
		printk("I'm player1, put message..\n");
		//pending에서 돌아 올 때 priority가 변하면 어떻게 되는가?
//		k_thread_priority_set(refree,8);
		k_msgq_put(&my_msgq1,&data,K_NO_WAIT);

		printk("I'm player1, waiting for result...\n");
		k_msgq_get(&my_msgq1, &data, K_FOREVER);

		switch(data.item){
			case 0:
				printk("I'm player1, draw!!\n");
				break;
			case 1:
				printk("I'm player1, win!!\n");
				break;
			case 2:
				printk("I'm player1, lose!!\n");
		}
		k_busy_wait(100000);
	}
}

void threadC(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	struct data_item_type data;

	while(1){
		data.item = sys_rand32_get() % 3;
		printk("I'm player2, put message..\n");
		k_msgq_put(&my_msgq2,&data,K_NO_WAIT);

		printk("I'm player2, waiting for result...\n");
		k_msgq_get(&my_msgq2, &data, K_FOREVER);

		switch(data.item){
			case 0:
				printk("I'm player2, draw!!\n");
				break;
			case 1:
				printk("I'm player2, win!!\n");
				break;
			case 2:
				printk("I'm player2, lose!!\n");
		}
		k_busy_wait(100000);
	}
}

int main(void){
	k_msgq_init(&my_msgq1, my_msgq_buffer1, sizeof(struct data_item_type),1);
	k_msgq_init(&my_msgq2, my_msgq_buffer2, sizeof(struct data_item_type),1);
	
	k_mutex_init(&my_mutex);

	refree = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
	player1 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
	player2 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, K_NO_WAIT);

	return 0;
}

