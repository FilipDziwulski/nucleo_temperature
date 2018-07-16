/**
 * @file temps.c
 *
 * Runs the computation of running averages
 * Two alterante implementations:
 *      1. Cascading buffers
 *      2. No buffer, fading memory average implentation (memory efficient approximation through a low pass filter)
 */

#include "temps.h"
#include "serial_io.h"


/*
*
*   FIRST IMPLEMENTATION
*
*   Buffers of averages. 
*   In order to not store massive buffers, these buffers cascade into eachother. 
*   Each longer duration average buffer uses the shorter average durations buffers average result as entries into its buffer.
*
*/
#if IMPLEMENTATION_CHOICE == 1
    struct Temps {
        float latestTemp;

        temp_buffer_t temps_60s;
        temp_buffer_t temps_1hr;
        temp_buffer_t temps_24hr;

    };    

    static struct Temps temps;


    /*****************************************************************************
    * Private Declarations
    ******************************************************************************/
    static inline bool insertIntoBuffer(temp_buffer_t *temps, float newTemp);

    /*****************************************************************************
    * Public Definitions
    ******************************************************************************/

    bool initTemperatures()
    {
        bool success = false;


        temps.latestTemp = 0.0;

        temps.temps_60s.size = BUFFER_SIZE_60S;
        temps.temps_60s.buffer = malloc(BUFFER_SIZE_60S * sizeof(float));
        temps.temps_60s.index = 0;
        temps.temps_60s.filled = false;
        temps.temps_60s.average = 0.0;

        temps.temps_1hr.size = BUFFER_SIZE_1HR;
        temps.temps_1hr.buffer = malloc(BUFFER_SIZE_1HR * sizeof(float));
        temps.temps_1hr.index = 0;
        temps.temps_1hr.index = false;
        temps.temps_1hr.average = 0.0;

        temps.temps_24hr.size = BUFFER_SIZE_24HR;
        temps.temps_24hr.buffer = malloc(BUFFER_SIZE_24HR * sizeof(float));
        temps.temps_24hr.index = 0;
        temps.temps_24hr.index = false;
        temps.temps_24hr.average = 0.0;



        if(temps.temps_60s.buffer != NULL &&
            temps.temps_1hr.buffer != NULL &&
            temps.temps_24hr.buffer != NULL)
        {
            success = true;
        }

        return success;
    }


    void runAverages(float newTemp)
    {
        temps.latestTemp = newTemp;

        // cascading buffers. If a buffer for its time duration loops, it returns true. 
        // This causes the next buffer in the heirarchy to recieve a new entry.
        if(insertIntoBuffer(&(temps.temps_60s), temps.latestTemp))
        {
            if(insertIntoBuffer(&(temps.temps_1hr), temps.temps_60s.average))
            {
                insertIntoBuffer(&(temps.temps_24hr), temps.temps_1hr.average);

            }
        }
    }


    float getLatest()
    {
        return temps.latestTemp;
    }

    float get60sAverage()
    {
        return temps.temps_60s.average;
    }

    float get1hrAverage()
    {
        return temps.temps_1hr.average;
    }

    float get24hrAverage()
    {
        return temps.temps_24hr.average;
    }


    /*****************************************************************************
    * Private Defintions
    ******************************************************************************/

    static inline bool insertIntoBuffer(temp_buffer_t *temps, float newTemp)
    {
        bool cascade = false;

        temps->buffer[temps->index] = newTemp;
        
        /* compute new averages */
        int numValues = temps->filled ? (temps->size) : (temps->index + 1);
        float sum = 0;

        for(int i = 0; i < numValues; i++)
        {
            sum += temps->buffer[i];
        }

        temps->average = sum/numValues;

        if((temps->index == 0) && (temps->filled == false))
        {
            cascade = true;
        }

        /* increment the index and adjust for boundaries. Also detect circular buffer wrap around */
        temps->index++;
        if(temps->index == temps->size)
        {
            temps->index = 0;
            temps->filled = true;
            cascade = true;
        }

        return cascade;

    }


/*
*
*   SECOND IMPLEMENTATION
*   Fading memory accumulator. Basically a low pass filter. 
*   Performance isn't as good over long durations, but it has a very low memory footprint.
*
*/
#elif IMPLEMENTATION_CHOICE == 2
    struct Temps {
        float latestTemp;

        temp_accumulator_t avg_60s;
        temp_accumulator_t avg_1hr;
        temp_accumulator_t avg_24hrs;

    };

    static struct Temps temps;

    /*****************************************************************************
    * Private Declarations
    ******************************************************************************/
    static inline void insertIntoAccumulator(temp_accumulator_t *temp, float newTemp);

    /*****************************************************************************
    * Public Definitions
    ******************************************************************************/

    bool initTemperatures()
    {

        temps.avg_60s.accumulator = 0.0;
        temps.avg_60s.count = 0;
        temps.avg_60s.maxSamples = SAMPLES_IN_60S;

        temps.avg_1hr.accumulator = 0.0;
        temps.avg_1hr.count = 0;
        temps.avg_1hr.maxSamples = SAMPLES_IN_1HR;

        temps.avg_24hrs.accumulator = 0.0;
        temps.avg_24hrs.count = 0;
        temps.avg_24hrs.maxSamples = SAMPLES_IN_24HRS;

        return true;
    }


    void runAverages(float newTemp)
    {
        temps.latestTemp = newTemp;

        insertIntoAccumulator(&(temps.avg_60s), newTemp);
        insertIntoAccumulator(&(temps.avg_1hr), newTemp);
        insertIntoAccumulator(&(temps.avg_24hrs), newTemp);
    }


    float getLatest()
    {
        return temps.latestTemp;
    }

    float get60sAverage()
    {
        return temps.avg_60s.accumulator;
    }

    float get1hrAverage()
    {
        return temps.avg_1hr.accumulator;
    }

    float get24hrAverage()
    {
        return temps.avg_24hrs.accumulator;
    }


    /*****************************************************************************
    * Private Defintions
    ******************************************************************************/
    static inline void insertIntoAccumulator(temp_accumulator_t *temp, float newTemp)
    {

        if(0 == temp->count) // if this our first sample, set the accumulator to that value
        {
            temp->accumulator = newTemp;
            temp->count++;  
        }
        else  // otherwise compute weighted average using sample count (which is limited above to the maxSamples in this time window)
        {
            /* increment sample count */
            if(temp->count < temp->maxSamples)
            {
                temp->count++;
            }

            float tempCount_f = (float)temp->count;

            temp->accumulator = (temp->accumulator * ((tempCount_f - 1)/tempCount_f)) + (newTemp * (1/tempCount_f));
        }

    }


#else
  #error Unsupported Implementation Setting
#endif






