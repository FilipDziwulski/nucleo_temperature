/**
 * @file temps.h
 *
 * Runs the computation of running averages
 * Two alterante implementations:
 *      1. Cascading buffers
 *      2. No buffer, fading memory average implentation (memory efficient approximation)
 */


#ifndef TEMPS_H
#define TEMPS_H

#include <stdlib.h> 
#include <stdint.h>
#include <stdbool.h> 


#define IMPLEMENTATION_CHOICE       1



#define FIVE_S_IN_MS                (5000)              // 5s in ms
#define SIXTY_S_IN_MS               (60000)             // 60s in ms 
#define ONE_HR_IN_MS                (SIXTY_S_IN_MS*60)  // 1hr in ms  
#define ONE_DAY_IN_MS               (ONE_HR_IN_MS*24)   // 24hrs in ms

#define MINUTES_IN_1HR              (60)
#define HRS_IN_1DAY                 (24)


#define TEMP_SAMPLE_INTERVAL_MS     (FIVE_S_IN_MS)
#define BUFFER_SIZE_60S             (SIXTY_S_IN_MS / TEMP_SAMPLE_INTERVAL_MS)   // buffer size 12
#define BUFFER_SIZE_1HR             (MINUTES_IN_1HR)                            // buffer size 60
#define BUFFER_SIZE_24HR            (HRS_IN_1DAY)                               // buffer size 24


#define SAMPLES_IN_60S              (SIXTY_S_IN_MS/TEMP_SAMPLE_INTERVAL_MS)
#define SAMPLES_IN_1HR              (ONE_HR_IN_MS/TEMP_SAMPLE_INTERVAL_MS)
#define SAMPLES_IN_24HRS            (ONE_DAY_IN_MS/TEMP_SAMPLE_INTERVAL_MS)


#if IMPLEMENTATION_CHOICE == 1
/* Buffer typedef for various duration buffers */
typedef struct temp_buffer_s {
    uint8_t size;         
    float *buffer;
    uint8_t index;
    bool filled;
    float average;
} temp_buffer_t;

#elif IMPLEMENTATION_CHOICE == 2
/* Buffer typedef for various duration accumulators */
typedef struct temp_accumulator_s {
    float accumulator;     
    uint32_t count;
    uint32_t maxSamples;
} temp_accumulator_t;

#else
  #error Unsupported Implementation Setting
#endif


bool initTemperatures();
void runAverages(float newTemp);
float getLatest();
float get60sAverage();
float get1hrAverage();
float get24hrAverage();


#endif /* TEMPS_H */
