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
#include "errorkeys.h"
#include "driver/i2c.h"
#include "freertos/task.h"



/* definitions */
#define VERBOSE_EN 


/* declarations */
int verbose = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}


void loop() {
  // put your main code here, to run repeatedly:


}
