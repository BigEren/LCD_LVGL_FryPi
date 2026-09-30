#ifndef __HEALTH_SENSOR_H__
#define __HEALTH_SENSOR_H__

#include "stdint.h"
#include "stdbool.h"
#include "health_algorithm.h"

bool HealthSensor_Read(HealthData_t *result);

#endif /*__HEALTH_SENSOR_H__*/
