
///Application 1 - Do not use any messaging techniques available in Zephyr, only synchronisation mechanisms
#include <zephyr/sys/printk.h>
//#include <sys/__assert.h>
#include <zephyr/random/random.h>
#include <string.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY 7
#define SLEEP_TIME_MS 1000
#define CHECK_INTERVAL_MS 10000
#define MAX_BUFFER_SIZE 1024
#define CHUNK_SIZE 512

K_THREAD_STACK_DEFINE(thread_stack_area_1, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_area_2, STACK_SIZE);

struct k_thread thread_data_1;
struct k_thread thread_data_2;

static uint8_t data_buffer[MAX_BUFFER_SIZE];
static size_t buffer_size = 0;
static struct k_mutex buffer_mutex;

void random_data_thread(void)
{
    while (1)
    {
        k_msleep(SLEEP_TIME_MS);

        k_mutex_lock(&buffer_mutex, K_FOREVER);

        // Generate random number of bytes (up to 50 bytes)
        size_t num_bytes = sys_rand32_get() % 51;

        // Ensure there's enough space in the buffer
        if (buffer_size + num_bytes <= MAX_BUFFER_SIZE)
        {
            for (size_t i = 0; i < num_bytes; i++)
            {
                data_buffer[buffer_size + i] = sys_rand32_get() % 256;
            }
            buffer_size += num_bytes;
            printk("Added %zu bytes, buffer size: %zu\n", num_bytes, buffer_size);
        }

        k_mutex_unlock(&buffer_mutex);
    }
}

void print_data_thread(void)
{
    while (1)
    {
        k_msleep(CHECK_INTERVAL_MS);

        k_mutex_lock(&buffer_mutex, K_FOREVER);

        if (buffer_size >= CHUNK_SIZE)
        {
            printk("Buffer size before printing: %zu\n", buffer_size);
            printk("Latest %d bytes (hex): ", CHUNK_SIZE);

            // Print the latest CHUNK_SIZE bytes in hex
            for (size_t i = buffer_size - CHUNK_SIZE; i < buffer_size; i++)
            {
                printk("%02x", data_buffer[i]);
            }
            printk("\n");

            // Remove the printed bytes
            buffer_size -= CHUNK_SIZE;

            // Shift remaining data to the front of the buffer
            memmove(data_buffer, data_buffer + CHUNK_SIZE, buffer_size);

            printk("Buffer size after printing: %zu\n", buffer_size);
        }
        else
        {
            printk("Not enough data to print. Buffer size: %zu\n", buffer_size);
        }

        k_mutex_unlock(&buffer_mutex);
    }
}

void main(void)
{
    k_mutex_init(&buffer_mutex);

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


  Assumptions:

The application is built and run on the native POSIX port of Zephyr.
Only synchronization mechanisms (mutexes) are used for thread safety.
The sys_rand32_get() function is available for generating random numbers.
The main goal is to demonstrate synchronization techniques without using Zephyr's messaging techniques.
