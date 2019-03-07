#include <zephyr.h>
#include <kernel.h>
#include <misc/printk.h>
#include <string.h>

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

struct k_mbox my_mailbox;

void threadA(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	printk("A\n");

	struct k_mbox_msg send_msg;
	char buffer[10];
	int buffer_bytes_used;
	char source[100] = "ab";
//buffer와 source의 크기에 따라 CPU page fault error가 발생하는데 이유를 모르겠음..
//소스 코드를 봐도 모르겠음..
	while(1){
		buffer_bytes_used = 11;
		memcpy(buffer, source, buffer_bytes_used);
	
		
//		u32_t random_value = sys_rand32_get();
//		send_msg.info = random_value;
//		send_msg.size = 0;
//		send_msg.tx_data = NULL;
		
		send_msg.info = buffer_bytes_used;
		send_msg.size = buffer_bytes_used;
		send_msg.tx_data = buffer;

		send_msg.tx_block.data = NULL;
		send_msg.tx_target_thread = K_ANY;
//		send_msg.rx_source_thread = thread1;

//		printk("A\n");
		
		k_mbox_put(&my_mailbox, &send_msg, K_NO_WAIT);
		printk("thread A: %s\n",buffer);

		k_mbox_put(&my_mailbox, &send_msg, K_NO_WAIT);
		printk("thread A: %s\n",buffer);

		k_busy_wait(100000);
//		printk("A\n");

		if(send_msg.size < buffer_bytes_used){
			printk("some message data dropped during transfer!\n");
			printk("receiver only had room for %d bytes\n", send_msg.info);
		}
	}
}

void threadB(void* dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	printk("B\n");
	
	struct k_mbox_msg recv_msg;
	char buffer[100];
	int i, total;	
/*
	while(1){
		recv_msg.info = 100;
		recv_msg.size = 100;
		recv_msg.rx_source_thread = K_ANY;
		
		k_mbox_get(&my_mailbox, &recv_msg, buffer, K_FOREVER);
		
		printk("thread B: %s\n",buffer);	

		if(recv_msg.info != recv_msg.size){
			printk("some message data dropped during transfer!\n");
			printk("sender tried to send %d bytes\n", recv_msg.info);
		}
		
		total = 0;
		for(i = 0; i<recv_msg.size; i++){
			total += buffer[i];
		}
	}
*/
}

void threadC(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	printk("C\n");
	while(1){
		printk("threadC\n");
		k_busy_wait(100000);
	}
}

int main(void){
	k_mbox_init(&my_mailbox);
	thread1 = k_thread_create(&my_thread1, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack), threadA, NULL, NULL, NULL, PRIORITY1, 0, K_NO_WAIT);
	thread2 = k_thread_create(&my_thread2, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack), threadB, NULL, NULL, NULL, PRIORITY2, 0, K_NO_WAIT);
//	thread3 = k_thread_create(&my_thread3, thread3_stack, K_THREAD_STACK_SIZEOF(thread3_stack), threadC, NULL, NULL, NULL, PRIORITY3, 0, 5);

	return 0;
}

