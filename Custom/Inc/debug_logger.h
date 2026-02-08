#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <stddef.h>
#include <stdbool.h>

struct rcl_publisher_s;

void debug_logger_set_publisher(struct rcl_publisher_s *publisher);
void debug_logger_on_write(const char *data, size_t len);
void debug_logger_flush(void);
bool debug_logger_publish(const char *msg);

#endif
