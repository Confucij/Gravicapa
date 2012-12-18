#ifndef __SENSORS_H__
#define __SENSORS_H__


typedef struct sensor_str{
    uint64_t id;
    int16_t  period;
    int16_t  time_left;
    uint32_t value;

}sensor;


#endif
