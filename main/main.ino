/*
 * *******************************************************************
 * File: main.ino
 * 
 * Author: sivapraveen@entuplemobility.com
 * 
 * Date: June 2021
 * 
 * Copyright (C) Entuple Emobility Pvt Ltd
 * All rights reserved. 
 * *******************************************************************
 */

/* includes */
#include <stdio.h>
#include "errorkeys.h"
#include "driver/i2c.h"
#include "freertos/task.h"
#include "i2c_util.h"

/* Semaphore util */
SemaphoreHandle_t print_mux = NULL;

/* definitions */
#define VERBOSE_EN 

static void disp_buf(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        Serial.printf("%02x ", buf[i]);
        if ((i + 1) % 16 == 0) {
            Serial.printf("\n");
        }
    }
    Serial.printf("\n");
}

static void vTaskDelayTaskImp(int *param_p)
{
  Serial.printf("vTaskDelayTaskImp Holds ... \n");
  vTaskDelay(10000 / portTICK_RATE_MS);
}

static void i2c_test_task(void *arg)
{
    static int adc1, adc2;
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t)arg;
    uint8_t *data = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t *data_wr = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t *data_rd = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;
    int cnt = 0;
    while (1) {
        Serial.printf("loop task: %d \n", ++adc1);
        ESP_LOGI(TAG, "TASK[%d] test cnt: %d", task_idx, cnt++);
  
        int size;
        //for (i = 0; i < DATA_LENGTH; i++) {
        //    data_wr[i] = i + 10;
        //}
       // memset(data_wr, 0, 128*sizeof(data_wr[0]));
        static int adder;
        for(i = 0; i< 10; i++)
        {
            data_wr[i] = i + adder;
        }
        for(int indx = i; indx < DATA_LENGTH; indx++)
        {
            data_wr[indx] = 0;
        }
        adder++;
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //we need to fill the slave buffer so that master can read later
        ret = i2c_master_write_slave((i2c_port_t)I2C_MASTER_NUM, data_wr, (size_t)RW_TEST_LENGTH);
        if (ret == ESP_OK) {
            size = i2c_slave_read_buffer((i2c_port_t)I2C_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        }
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            Serial.printf("*******************\n");
            Serial.printf("TASK[%d]  MASTER WRITE TO SLAVE Count-2\n", task_idx);
            Serial.printf("*******************\n");
            Serial.printf("----TASK[%d] Master writeCount-2 ----\n", task_idx);
            disp_buf(data_wr, RW_TEST_LENGTH);
            Serial.printf("----TASK[%d] Slave read:Count-2 [%d] bytes ----\n", task_idx, size);
            disp_buf(data, size);
        } else {
            ESP_LOGW(TAG, "TASK[%d] %s: Master write slave error, IO not connected....\n",
                     task_idx, esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(10000 / portTICK_RATE_MS);
    }
    vSemaphoreDelete(print_mux);
    vTaskDelete(NULL);
}



/* declarations */
int verbose = 1;

/*
 * *******************************************************************
 * 
 * 
 * 
 * 
 * 
 * *******************************************************************
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  print_mux = xSemaphoreCreateMutex();
  //i2c_slave_init();
  //i2c_master_init();
  ESP_ERROR_CHECK(i2c_slave_init());
  ESP_ERROR_CHECK(i2c_master_init());
  xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
  //xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void *)1, 10, NULL);

  Serial.printf("Setup completed ... \n");  
}

/*
 * *******************************************************************
 * 
 * 
 * 
 * 
 * 
 * *******************************************************************
 */
 
void loop() {
  // put your main code here, to run repeatedly

  Serial.printf("Loop exutes ... \n");
  delay(10000);
  
}
