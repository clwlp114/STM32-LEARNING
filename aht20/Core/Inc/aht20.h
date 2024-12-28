//
// Created by clwlp on 24-12-26.
//

#ifndef AHT20_H
#define AHT20_H

#include "i2c.h"

void AHT20_Init();

void AHT20_Read(float *temperature, float *humidity);

void AHT20_Measure();

void AHT20_Get();

void AHT20_Analysis(float* Temperature, float* Humidity);

#endif //AHT20_H
