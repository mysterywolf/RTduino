/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 * 2022-05-01     Meco Man     support hardware timer
 * 2022-05-20     Meco Man     support no setup-loop structure
 */

#include <RTduino.h>
#include "wiring_private.h"

#define DBG_TAG    "RTduino.thread"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static void rtduino_sketch_entry(void *parameter)
{
    rtduino_loader_t loader = (rtduino_loader_t)parameter;
    loader->setup();
    while(1)
    {
        loader->loop();
        rt_schedule();
    }
}

rtduino_loader_t rtduino_sketch_loader_create_prio(const char* name, void (*setup)(void), void (*loop)(void), rt_uint32_t stack_size, rt_uint8_t priority)
{
    rtduino_loader_t loader;

    if (name == RT_NULL || setup == RT_NULL || loop == RT_NULL)
    {
        return RT_NULL;
    }

    loader = (rtduino_loader_t)rt_malloc(sizeof(rtduino_loader));
    if (loader == RT_NULL)
    {
        return RT_NULL;
    }

    loader->name =  name;
    loader->setup = setup;
    loader->loop = loop;
    loader->thread = RT_NULL;

    rt_thread_t thread = rt_thread_create(name, rtduino_sketch_entry, (void *)loader, stack_size, priority, 20);
    if (thread != RT_NULL)
    {
        loader->thread = thread;
        rt_thread_startup(thread);
    }
    else
    {
        rt_free(loader);
        loader = RT_NULL;
    }

    return loader;
}

rtduino_loader_t rtduino_sketch_loader_create(const char* name, void (*setup)(void), void (*loop)(void))
{
    return rtduino_sketch_loader_create_prio(name, setup, loop, RTDUINO_THREAD_SIZE, RTDUINO_THREAD_PRIO);
}

rt_err_t rtduino_sketch_loader_delete(rtduino_loader_t loader)
{
    rt_err_t err;

    if(loader == RT_NULL)
    {
        return -RT_EINVAL;
    }

    err = rt_thread_delete(loader->thread);
    if(err == RT_EOK)
    {
        rt_free(loader);
    }

    return err;
}

#ifdef RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME
static void hwtimer_init(void)
{
    rt_device_t hwtimer_device;

    hwtimer_device = rt_device_find(RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME);
    if(hwtimer_device != RT_NULL)
    {
        rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD; /* periodic */
        rt_uint32_t freq = 1000000; /* 1Mhz, 1us */
        rt_hwtimerval_t val = {.sec=1, .usec=0}; /* callback interval */

        if(rt_device_open(hwtimer_device, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
        {
            LOG_E("Failed to open hardware timer!");
            return;
        }
        rt_device_control(hwtimer_device, HWTIMER_CTRL_FREQ_SET, &freq); /* set hwtimer prescaler frequency */
        rt_device_control(hwtimer_device, HWTIMER_CTRL_MODE_SET, &mode); /* set hwtimer mode */
        if(rt_device_write(hwtimer_device, 0, &val, sizeof(val)) == 0)
        {
            LOG_E("Failed to start (write) hardware timer!");
        }
    }
    else
    {
        LOG_W("Cannot find a hardware timer. Some functions cannot be used.");
    }
}
#endif /* RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME */

/* initialization for BSP; maybe a blank function  */
rt_weak void initVariant(void) {}

static int rtduino_init(void)
{
#ifdef RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME
    hwtimer_init();
#endif /* RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME */

    initVariant();

#ifndef RTDUINO_NO_SETUP_LOOP
    rtduino_sketch_loader_create("RTduino", setup, loop);
#endif /* RTDUINO_NO_SETUP_LOOP */
    return 0;
}
INIT_COMPONENT_EXPORT(rtduino_init);
