// 
// 
// 
#include "AHAS_Config.h"
#include "App_RunTime.h"

void startRuntimeMeasurement()
{
    /* Get the timestamp */
    RunTime.TimeStampStart = millis();
}

void endRuntimeMeasurement(RunTime_Type* module)
{
    /* Increment the counter */
    module->count++;
    /* Calculate the runtime */
    module->act = (unsigned int)(millis() - RunTime.TimeStampStart);
    /* Check the maximum */
    module->act>module->max ? module->max = module->act : module->max = module->max;
    /* Check the minimum */
    module->act<module->min ? module->min = module->act : module->min = module->min;
    /* Calculate the average */
    module->avg = (float)(module->avg)*((module->count - 1) / (float)module->count) + (module->act) / (float)module->count;

    /* When runtime is not zero */
    if (module->act > 1)
    {
        /* Increment the counter */
        module->countNZ++;
        /* Calculate the average when runtime is not zero*/
        module->avgNZ = (float)(module->avgNZ)*((module->countNZ - 1) / (float)module->countNZ) + (module->act) / (float)module->countNZ;
    }

}

void printRuntTime(RunTime_Type* module)
{
    Serial.print(F("Act:")); Serial.println(module->act);
    Serial.print(F("Max:")); Serial.println(module->max);
    Serial.print(F("Min:")); Serial.println(module->min);
    Serial.print(F("Avg:")); Serial.println(module->avg);
    Serial.print(F("AvgNZ:")); Serial.println(module->avgNZ);
    Serial.print(F("Count:")); Serial.println(module->count);
}

