#include "aht20.h"

#define AHT20_ADDRESS 0x70

uint8_t readerBuffer[6]={0};

// 发送
void AHT20_Init()
{
    uint8_t readerBuffer;
    HAL_Delay(40);
    HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readerBuffer, 1, HAL_MAX_DELAY);
    if ((readerBuffer & 0x08) == 0x00)
    {
        uint8_t sendBuffer[3] = {0xBE, 0x08, 0x00};
        HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
    }
}

// 接受
void AHT20_Read(float* Temperature, float* Humidity)
{
    uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};
    uint8_t readerBuffer[0];
    HAL_I2C_Master_Transmit_IT(&hi2c1, AHT20_ADDRESS, sendBuffer, 3);
    HAL_Delay(75);
    HAL_I2C_Master_Receive_IT(&hi2c1, AHT20_ADDRESS, readerBuffer, 6);
    if ((readerBuffer[0] & 0x80) == 0x00)
    {
        uint32_t data = 0;
        data = ((uint32_t)readerBuffer[3] >> 4) + ((uint32_t)readerBuffer[2] << 4) + ((uint32_t)readerBuffer[1] << 12);
        *Humidity = data * 100.0f / (1 << 20);

        data = (((uint32_t)readerBuffer[3] & 0x0F) << 16) + ((uint32_t)readerBuffer[4] << 8) + ((uint32_t)readerBuffer[5]);
        *Temperature = data * 200.0f / (1 << 20) - 50;
    }
}

void AHT20_Measure()
{
    static uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};
    HAL_I2C_Master_Transmit_DMA(&hi2c1, AHT20_ADDRESS, sendBuffer, 3);
}

void AHT20_Get()
{
    HAL_I2C_Master_Receive_DMA(&hi2c1, AHT20_ADDRESS, readerBuffer, 6);
}

void AHT20_Analysis(float* Temperature, float* Humidity)
{
    if ((readerBuffer[0] & 0x80) == 0x00)
    {
        uint32_t data = 0;
        data = ((uint32_t)readerBuffer[3] >> 4) + ((uint32_t)readerBuffer[2] << 4) + ((uint32_t)readerBuffer[1] << 12);
        *Humidity = data * 100.0f / (1 << 20);

        data = (((uint32_t)readerBuffer[3] & 0x0F) << 16) + ((uint32_t)readerBuffer[4] << 8) + ((uint32_t)readerBuffer[5]);
        *Temperature = data * 200.0f / (1 << 20) - 50;
    }
}