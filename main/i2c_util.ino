#include "i2c_util.h"

static esp_err_t I2C_masterWriteSlave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


static esp_err_t I2C_masterReadSlave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/*
 * *******************************************************************
 * I2C Master Initialization
 * 
 * 
 * 
 * 
 * *******************************************************************
 */

static esp_err_t I2C_masterInit(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config((i2c_port_t)i2c_master_port, &conf);
    return i2c_driver_install((i2c_port_t)i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/*
 * *******************************************************************
 * I2C Slave initialization
 * 
 * 
 * 
 * 
 * *******************************************************************
 */
 
static esp_err_t I2C_slaveInit(void)
{
    int i2c_slave_port = I2C_SLAVE_NUM;
    i2c_config_t conf_slave;
    conf_slave.sda_io_num = (gpio_num_t)I2C_SLAVE_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = (gpio_num_t)I2C_SLAVE_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
    i2c_param_config((i2c_port_t)i2c_slave_port, &conf_slave);
    return i2c_driver_install((i2c_port_t)i2c_slave_port, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}


/* i2c test task */

static void i2c_tesTask(void *arg)
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
        ret = I2C_masterWriteSlave((i2c_port_t)I2C_MASTER_NUM, data_wr, (size_t)RW_TEST_LENGTH);
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

/* i2c Master write slave read task */

static void I2C_masterWriteSlaveRead(void *arg)
{
  static int counter = 1;
  int i = 0;
  int ret, size;

  uint32_t task_idx = (uint32_t)arg;
  uint8_t *dataMasWr = (uint8_t *)malloc(DATA_LENGTH);
  uint8_t *dataSlvRd = (uint8_t *)malloc(DATA_LENGTH);

  while(1)
  {
      Serial.printf("\n-----------------------------------------------------------------------------------------------\n");
      Serial.printf("<%s> Task[%d] Count: %d \n ", __FUNCTION__, task_idx, counter++);
          
      static int adder;
      for(i = 0; i< 10; i++)
      {
        dataMasWr[i] = i + adder;
      }
      for(int indx = i; indx < DATA_LENGTH; indx++)
      {
        dataMasWr[indx] = 0;
      }
      adder ++;
    
      xSemaphoreTake(print_mux, portMAX_DELAY);
      
      ret = I2C_masterWriteSlave((i2c_port_t)I2C_MASTER_NUM, dataMasWr, (size_t)RW_TEST_LENGTH);
      if (ret == ESP_OK) 
      {
           size = i2c_slave_read_buffer((i2c_port_t)I2C_SLAVE_NUM, dataSlvRd, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
      }
      if (ret == ESP_ERR_TIMEOUT)
      {
           //ESP_LOGE(TAG, "I2C Timeout");
           Serial.printf("<%s> I2C Timeout !!! \n");
      }
      else if (ret == ESP_OK) 
      {
           Serial.printf("*******************\n");
           Serial.printf("TASK[%d]  MASTER WRITE TO SLAVE\n", task_idx);
           Serial.printf("----TASK[%d] Master write ----\n", task_idx);
           disp_buf(dataMasWr, RW_TEST_LENGTH);
           Serial.printf("----TASK[%d] Slave read:[%d] bytes ----\n", task_idx, size);
           disp_buf(dataSlvRd, size);
           Serial.printf("*******************\n");
       } 
       else
       {
           //ESP_LOGW(TAG, "TASK[%d] %s: Master write slave error, IO not connected....\n",
           Serial.printf("<%s> TASK[%d]: %s Master write slave error, IO not connected !!! \n",
                                                                                              __FUNCTION__, task_idx, esp_err_to_name(ret));
       }
       xSemaphoreGive(print_mux);
       Serial.printf("\n-----------------------------------------------------------------------------------------------\n");
       vTaskDelay(10000 / portTICK_RATE_MS);
    }
    vSemaphoreDelete(print_mux);
    vTaskDelete(NULL);
}

/* i2c slave write master read task */

static void I2C_slaveWriteMasterRead(void *arg)
{
  static int counter = 1;
  int i = 0;
  int ret, size;

  uint32_t task_idx = (uint32_t)arg;
  uint8_t *dataSlvWr = (uint8_t *)malloc(DATA_LENGTH);
  uint8_t *dataMasRd = (uint8_t *)malloc(DATA_LENGTH);

  bool runLoopExeSts = false;
  
  while(1)
  {
      Serial.printf("\n-----------------------------------------------------------------------------------------------\n");
      Serial.printf("<%s> Task[%d] Count: %d \n ", __FUNCTION__, task_idx, counter++);

      if(runLoopExeSts == false)
      {
        int value = 0, valueCnt = 3;
        int indx;
        for(indx = 0; indx < 5; indx ++)
        {
          dataSlvWr[indx] = value + valueCnt;
          valueCnt = valueCnt + 3;
        }
        for(int b = indx; b < RW_TEST_LENGTH; b ++)
        {
          dataSlvWr[b] = 0; 
        }
      }

      if(runLoopExeSts == true)
      {
        int value = 0, valueCnt = 5;
        int indx;
        for(indx = 0; indx < 5; indx ++)
        {
          dataSlvWr[indx] = value + valueCnt;
          valueCnt = valueCnt + 5;
        }
        for(int b = indx; b < RW_TEST_LENGTH; b ++)
        {
          dataSlvWr[b] = 0; 
        }
      }
    
      xSemaphoreTake(print_mux, portMAX_DELAY);
        
      size_t d_size = i2c_slave_write_buffer((i2c_port_t)I2C_SLAVE_NUM, dataSlvWr, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
      if (d_size == 0)
      {
          //ESP_LOGW(TAG, "i2c slave tx buffer full");
          Serial.printf("<%s> i2c slave tx buffer full !!! \n");
          ret = I2C_masterReadSlave((i2c_port_t)I2C_MASTER_NUM, dataMasRd, DATA_LENGTH);
      }
      else
      {
          ret = I2C_masterReadSlave((i2c_port_t)I2C_MASTER_NUM, dataMasRd, RW_TEST_LENGTH);
      }

      if (ret == ESP_ERR_TIMEOUT) 
      {
           //ESP_LOGE(TAG, "I2C Timeout");
           Serial.printf("<%s> i2c Timeout !!! \n");
      } 
      else if (ret == ESP_OK) 
      {
          printf("*******************\n");
          printf("TASK[%d]  MASTER READ FROM SLAVE\n", task_idx);
          printf("====TASK[%d] Slave buffer data ====\n", task_idx);
          disp_buf(dataSlvWr, d_size);
          printf("====TASK[%d] Master read ====\n", task_idx);
          disp_buf(dataMasRd, d_size);
          printf("*******************\n");
      } 
      else
      {
          //ESP_LOGW(TAG, "TASK[%d] %s: Master read slave error, IO not connected...\n",
          //                                                                            task_idx, esp_err_to_name(ret));
          Serial.printf("<%s> TASK[%d] %s: Master read slave error, IO not connected !!! \n", __FUNCTION__, task_idx, esp_err_to_name(ret));
      }
      
      runLoopExeSts = !runLoopExeSts;

      xSemaphoreGive(print_mux);
      Serial.printf("\n-----------------------------------------------------------------------------------------------\n");
      vTaskDelay(10000 / portTICK_RATE_MS);
   }
   vSemaphoreDelete(print_mux);
   vTaskDelete(NULL);
}
