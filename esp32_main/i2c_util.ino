#include "i2c_util.h"

/*
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
*/

/*
 * *******************************************************************
 * I2C Master Initialization
 * 
 * 
 * 
 * 
 * *******************************************************************
 */
/*
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
}*/
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

static void I2C_slaveRead(void *arg)
{
  int ret; 
  
  uint32_t task_idx = (uint32_t)arg;
  uint8_t *dataRecFromControlBoard = (uint8_t *)malloc(DATA_LENGTH);

  int size;
  while(1)
  {
        size = i2c_slave_read_buffer((i2c_port_t)I2C_SLAVE_NUM, dataRecFromControlBoard, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        Serial.printf("Loop running size: %d \n", size);
        if(size > 1)
        {
          Serial.printf("Data Received \n"); 
          Serial.printf("----TASK[%d] Slave read:[%d] bytes ----\n", task_idx, size);
          disp_buf(dataRecFromControlBoard, size);
          size = 0;
        }
  }
  vTaskDelete(NULL); 
}
