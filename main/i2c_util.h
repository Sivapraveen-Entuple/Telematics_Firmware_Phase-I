#ifndef __I2C_UTIL_H__
#define __I2C_UTIL_H__

#include "driver/i2c.h"
#include "i2c_util.h"

/* i2c definitions */
#define DATA_LENGTH 1024
#define RW_TEST_LENGTH 10
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000

#define I2C_SLAVE_SCL_IO 22
#define I2C_SLAVE_SDA_IO 21
#define I2C_SLAVE_NUM 0
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)

#define ESP_SLAVE_ADDR 0x28

#define I2C_MASTER_SCL_IO 19
#define I2C_MASTER_SDA_IO 18
#define I2C_MASTER_NUM 1
#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0

#define WRITE_BIT I2C_MASTER_WRITE
#define READ_BIT I2C_MASTER_READ
#define ACK_CHECK_EN 0x1
#define ACK_CHECK_DIS 0x0
#define ACK_VAL I2C_MASTER_ACK
#define NACK_VAL I2C_MASTER_NACK

static esp_err_t I2C_masterReadSlave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size);
static esp_err_t I2C_masterWriteSlave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size);
static esp_err_t I2C_masterInit();
static esp_err_t I2C_slaveInit();

#endif
