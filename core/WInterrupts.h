/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-07     Meco Man     first version
 */

#ifndef WInterrupts_h
#define WInterrupts_h

#include <rtthread.h>
#include <rthw.h>

class noInterrupts
{
private:
    rt_base_t interrupt_nesting;
public:
    noInterrupts()
    {
        interrupt_nesting = rt_hw_interrupt_disable();
        rt_kprintf("noInterrupts\n");
    }

    ~noInterrupts()
    {
        rt_hw_interrupt_enable(interrupt_nesting);
        rt_kprintf("exit noInterrupts\n");
    }

    noInterrupts(noInterrupts&&) = delete;
    noInterrupts& operator=(noInterrupts&&) = delete;
};

#endif /* WInterrupts_h */