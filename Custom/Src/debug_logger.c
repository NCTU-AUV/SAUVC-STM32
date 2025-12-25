#include "debug_logger.h"

#include <stdbool.h>
#include <string.h>

#include <rcl/rcl.h>
#include <std_msgs/msg/string.h>
#include <rosidl_runtime_c/string.h>

// Buffer to accumulate outgoing debug logs from printf. Keep it short to avoid blocking.
#define DEBUG_LOGGER_BUFFER_CAPACITY 256

static struct rcl_publisher_s *debug_log_publisher = NULL;
static char debug_log_buffer[DEBUG_LOGGER_BUFFER_CAPACITY];
static size_t debug_log_buffer_len = 0;


void debug_logger_set_publisher(struct rcl_publisher_s *publisher)
{
    debug_log_publisher = publisher;
}


static void publish_debug_log_buffer(void)
{
    if (debug_log_publisher == NULL || debug_log_buffer_len == 0) {
        debug_log_buffer_len = 0;
        return;
    }

    debug_log_buffer[debug_log_buffer_len] = '\0';

    std_msgs__msg__String msg;
    msg.data.data = debug_log_buffer;
    msg.data.size = debug_log_buffer_len;
    msg.data.capacity = DEBUG_LOGGER_BUFFER_CAPACITY;

    (void)rcl_publish(debug_log_publisher, &msg, NULL);

    debug_log_buffer_len = 0;
}


void debug_logger_on_write(const char *data, size_t len)
{
    if (data == NULL || len == 0) {
        return;
    }

    for (size_t i = 0; i < len; ++i) {
        char c = data[i];

        if (debug_log_buffer_len >= DEBUG_LOGGER_BUFFER_CAPACITY - 1) {
            publish_debug_log_buffer();
        }

        debug_log_buffer[debug_log_buffer_len++] = c;

        if (c == '\n') {
            publish_debug_log_buffer();
        }
    }
}


void debug_logger_flush(void)
{
    publish_debug_log_buffer();
}
