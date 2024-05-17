///Use available messaging techniques in Zephyr for getting all the bytes printed without using globally accessible data structure.

#include <zephyr.h>
#include <sys/printk.h>
#include <random/rand32.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY 7
#define SLEEP_TIME_MS 1000
#define CHECK_INTERVAL_MS 10000
#define MAX_CHUNK_SIZE 50
#define MSGQ_MAX_MSGS 16

K_THREAD_STACK_DEFINE(thread_stack_area_1, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_area_2, STACK_SIZE);

struct k_thread thread_data_1;
struct k_thread thread_data_2;

typedef struct {
    uint8_t data[MAX_CHUNK_SIZE];
    size_t length;
} message_t;

K_MSGQ_DEFINE(my_msgq, sizeof(message_t), MSGQ_MAX_MSGS, 4);

void random_data_thread(void)
{
    while (1)
    {
        k_msleep(SLEEP_TIME_MS);

        message_t msg;
        msg.length = sys_rand32_get() % (MAX_CHUNK_SIZE + 1);

        for (size_t i = 0; i < msg.length; i++)
        {
            msg.data[i] = sys_rand32_get() % 256;
        }

        if (k_msgq_put(&my_msgq, &msg, K_NO_WAIT) != 0)
        {
            printk("Message queue is full. Dropping message.\n");
        }
    }
}

void print_data_thread(void)
{
    while (1)
    {
        k_msleep(CHECK_INTERVAL_MS);

        message_t msg;
        size_t total_bytes = 0;

        printk("Printing data:\n");
        while (k_msgq_get(&my_msgq, &msg, K_NO_WAIT) == 0)
        {
            for (size_t i = 0; i < msg.length; i++)
            {
                printk("%02x ", msg.data[i]);
            }
            total_bytes += msg.length;
        }

        if (total_bytes == 0)
        {
            printk("No data to print.\n");
        }
        else
        {
            printk("\nTotal bytes printed: %zu\n", total_bytes);
        }
    }
}

void main(void)
{
    k_thread_create(&thread_data_1, thread_stack_area_1,
                    K_THREAD_STACK_SIZEOF(thread_stack_area_1),
                    random_data_thread,
                    NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_NO_WAIT);

    k_thread_create(&thread_data_2, thread_stack_area_2,
                    K_THREAD_STACK_SIZEOF(thread_stack_area_2),
                    print_data_thread,
                    NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_NO_WAIT);
}

## prj.conf:
CONFIG_MAIN_STACK_SIZE=1024
CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE=1024
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y
CONFIG_PRINTK=y

Assumptions:
The application is built and run on the native POSIX port of Zephyr.
Messaging techniques (message queues) are used for inter-thread communication.
No globally accessible data structures are used.
The main goal is to demonstrate the use of messaging techniques in Zephyr.
