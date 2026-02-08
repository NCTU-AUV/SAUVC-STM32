#include "micro_ros_configuration.h"

#include <stdbool.h>
#include <stdio.h>

#include <rmw_microros/rmw_microros.h>
#include <rcl/allocator.h>
#include <rcutils/error_handling.h>
#include "debug_logger.h"
#include <stdio.h>

bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);

static void configure_transport(UART_HandleTypeDef *ros_transport_huart)
{
    rmw_uros_set_custom_transport(
    true,
    (void *) ros_transport_huart,
    cubemx_transport_open,
    cubemx_transport_close,
    cubemx_transport_write,
    cubemx_transport_read);
}

static void configure_allocator()
{
    rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
    freeRTOS_allocator.allocate = microros_allocate;
    freeRTOS_allocator.deallocate = microros_deallocate;
    freeRTOS_allocator.reallocate = microros_reallocate;
    freeRTOS_allocator.zero_allocate =  microros_zero_allocate;

    if (!rcutils_set_default_allocator(&freeRTOS_allocator)) {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "micro_ros_configuration: failed to set default FreeRTOS allocator: %s\n",
                         rcutils_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcutils_reset_error();
    }
}

void configure_micro_ros(UART_HandleTypeDef *ros_transport_huart)
{
    configure_transport(ros_transport_huart);
    configure_allocator();
}
