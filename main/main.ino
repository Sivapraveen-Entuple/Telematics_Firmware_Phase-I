/*
 * *******************************************************************
 * File: main.ino
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

/* queue util */
QueueHandle_t Q1;


/* definitions */
#define VERBOSE_EN 

/*
 * *******************************************************************
 * 
 * 
 * 
 * *******************************************************************
 */

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



/* declarations */
int verbose = 1;

/*
 * *******************************************************************
 * 
 *
 *
 * *******************************************************************
 */
void setup() {
  
  Serial.begin(115200);
  print_mux = xSemaphoreCreateMutex();
  ESP_ERROR_CHECK(i2c_slave_init());
  ESP_ERROR_CHECK(i2c_master_init());
  
  /* Queue Initialize */
  Q1 = xQueueCreate(10, sizeof(int));
  
  /* creating a task */
  
  //xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
  //xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void *)1, 10, NULL);  
  xTaskCreate(I2C_masterWriteSlaveRead, "I2C - Master Write Slave Read", 1024*2, (void*)0, 10, NULL);
  xTaskCreate(I2C_slaveWriteMasterRead, "I2C - Slave Write Master Read", 1024*2, (void*)1, 10, NULL);
  
  Serial.printf("Setup completed ... \n");  
}

/*
 * *******************************************************************
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
