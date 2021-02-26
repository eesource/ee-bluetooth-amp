/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <string.h>
#include "esp_log.h"
#include "i2c_bus.h"
//#include "driver/i2c.h"
#include "driver/gpio.h"
#include "board.h"

#include "new_codec.h"

static const char *TAG = "new_codec";

static bool codec_init_flag;

static i2c_bus_handle_t i2c_handle;

audio_hal_func_t AUDIO_NEW_CODEC_DEFAULT_HANDLE = {
    .audio_codec_initialize = new_codec_init,
    .audio_codec_deinitialize = new_codec_deinit,
    .audio_codec_ctrl = new_codec_ctrl_state,
    .audio_codec_config_iface = new_codec_config_i2s,
    .audio_codec_set_mute = new_codec_set_voice_mute,
    .audio_codec_set_volume = new_codec_set_voice_volume,
    .audio_codec_get_volume = new_codec_get_voice_volume,
};

bool new_codec_initialized()
{
    return codec_init_flag;
}

esp_err_t new_codec_init(audio_hal_codec_config_t *cfg)
{
    ESP_LOGI(TAG, "new_codec init");
    esp_err_t ret = ESP_OK;
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .master.clk_speed = 100000
    };
    ret |= get_i2c_pins(I2C_NUM_1, &config);
    i2c_handle = i2c_bus_create(I2C_NUM_1, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Fail to init new codec");
    }
    return ret;
}

esp_err_t new_codec_deinit(void)
{
    return ESP_OK;
}

esp_err_t new_codec_ctrl_state(audio_hal_codec_mode_t mode, audio_hal_ctrl_t ctrl_state)
{
    return ESP_OK;
}

//#define CODEC_ADDR (0x38 >> 1)
#define CODEC_ADDR (0x38)
esp_err_t new_codec_config_i2s(audio_hal_codec_mode_t mode, audio_hal_codec_i2s_iface_t *iface)
{
    uint8_t reg_val;
    uint8_t reg_addr;

    ESP_LOGI(TAG, "new_codec config");
    esp_err_t ret = ESP_OK;
    for(uint8_t i = 0; i<0x30; i++)
    {
      reg_val = 0;
      ret |= i2c_bus_read_bytes(i2c_handle, CODEC_ADDR, &i, 1, &reg_val, 1);
      ESP_LOGI(TAG, "[%02x]: %02x, ret %x", i, reg_val, ret);
    }
    reg_val = 0xdc;
    reg_addr = 0x5;
    ret |= i2c_bus_write_bytes(i2c_handle, CODEC_ADDR, &reg_addr, 1, &reg_val, 1);
    reg_val = 0x00;
    reg_addr = 0x7;
    ret |= i2c_bus_write_bytes(i2c_handle, CODEC_ADDR, &reg_addr, 1, &reg_val, 1);
    return ret;
}

esp_err_t new_codec_set_voice_mute(bool mute)
{
    return ESP_OK;
}

esp_err_t new_codec_set_voice_volume(int volume)
{
    return ESP_OK;
}

esp_err_t new_codec_get_voice_volume(int *volume)
{
    return ESP_OK;
}
