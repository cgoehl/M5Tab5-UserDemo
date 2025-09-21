/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../hal_desktop.h"
#include "../hal_config.h"
#include "hal/hal.h"
#include <mooncake_log.h>
#include <lvgl.h>
#include <mutex>
#include <thread>
#include <assets/assets.h>
// https://github.com/lvgl/lv_port_pc_vscode/blob/master/main/src/main.c

static const std::string _tag = "lvgl";
static std::mutex _lvgl_mutex;

void HalDesktop::lvgl_init()
{
    mclog::tagInfo(_tag, "lvgl init");

    lv_init();

    lv_group_set_default(lv_group_create());

    auto display = lv_x11_window_create("LVGL Simulator", HAL_SCREEN_WIDTH, HAL_SCREEN_HEIGHT);
    lv_display_set_default(display);
		
		LV_IMAGE_DECLARE(mouse_cursor); /*Declare the image file.*/
		lv_x11_inputs_create(display, &mouse_cursor);

#if not defined(__APPLE__) && not defined(__MACH__)
    std::thread([]() {
        while (!hal::Check()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        while (true) {
            GetHAL()->lvglLock();
            auto time_till_next = lv_timer_handler();
            GetHAL()->lvglUnlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(time_till_next));
        }
    }).detach();
#endif
}

void HalDesktop::lvglLock()
{
    _lvgl_mutex.lock();
}

void HalDesktop::lvglUnlock()
{
    _lvgl_mutex.unlock();
}
