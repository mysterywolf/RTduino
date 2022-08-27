/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-07     Meco Man     first version
 */

#include <Arduino.h>

void setup(void)
{
    Serial.begin();
}

void loop(void)
{
    Serial.println("Hello world!");
    delay(500);
}