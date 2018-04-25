// 
// 
// 
#include "AHAS_Config.h"
#include "App_M590.h"
#include "M590.h"
#include "App_Var.h"

#ifdef USE_M590

/* Global variables*/
M590 gsm;
M590_SignalQuality_ResultType sq;
M590_USSDResponse_ResultType ussd;
M590_SMS_ResultType sms;
M590_SMSList_ResultType smsList;
M590_IP_ResultType ip;

const String adminPhoneNumber = "+40745662769";
const String commandKeyword = "@AHAS:";
const String statusCommand = "STATUS";


void settingsCbk(ResponseStateType response)
{
    if (response == M590_RESPONSE_SUCCESS)
    {
        Serial.println(F("#GSM: SMS Textmode and GSM Charset was set"));
    }
    else
    {
        Serial.print(F("#GSM: CMGF/CSCS error: ")); Serial.println(response);
    }
}

void sqCbk(ResponseStateType response)
{
    if (response == M590_RESPONSE_SUCCESS)
    {
        Serial.print(F("#GSM: RSSI: ")); Serial.print(sq.rssi); Serial.print(F(" BER: ")); Serial.println(sq.ber);
    }
    else
    {
        Serial.print(F("#GSM: CSQ error: ")); Serial.println(response);
    }
}

void sendSmsCbk(ResponseStateType response)
{
    if (response == M590_RESPONSE_SUCCESS)
    {
        Serial.println(F("#GSM: Sms Sent"));
    }
    else
    {
        Serial.print(F("#GSM: CMGS error: ")); Serial.println(response);
    }
}

void readSMSCbk(ResponseStateType response)
{
    if (response == M590_RESPONSE_SUCCESS)
    {
        Serial.println(F("#GSM: SMS Read completed "));
        /* Print Out SMS */
        Serial.print(" Status: "); Serial.println(sms.status);
        Serial.print(" Sender: "); Serial.println(sms.sender);
        Serial.print(" Date: ");   Serial.println(sms.date);
        Serial.print(" Text: ");   Serial.println(sms.text);

        /* Check for the admin`s phonenumber */
        if (sms.sender.equals(adminPhoneNumber))
        {
            /* Check for AHAS command keyword */
            if (sms.text.startsWith(commandKeyword))
            {
                /* Remove command keyword from the beginning */
                sms.text.remove(0, commandKeyword.length());

                /* Remove also leading/trailing white spaces */
                sms.text.trim();

                if (sms.text.startsWith(statusCommand))
                {
                    /* Send back the AHAS status */

                    char responseCharArray[150]; 
                    String response;

                    response += "Date: " + String(DataPool.DS1302_Year) + "-" + String(DataPool.DS1302_Month) + "-" + String(DataPool.DS1302_Day) + " " + String(DataPool.DS1302_Hour) + ":" + String(DataPool.DS1302_Minute);
                    response += " Temp:" + String(DataPool.DHT11_Temperature);
                    response.toCharArray(responseCharArray, 150, 0);

                    gsm.sendSMS("+40745662769", responseCharArray, &sendSmsCbk);
                }
            }
        }

    }
    else
    {
        Serial.print(F("#GSM: CMGR error: ")); Serial.println(response);
    }
}

void newSmsNotificationEnableCbk(ResponseStateType response)
{
    if (response == M590_RESPONSE_SUCCESS)
    {
        Serial.println(F("#GSM: Sms notification enabled"));
    }
    else
    {
        Serial.print(F("#GSM: CNMI error: ")); Serial.println(response);
    }
}

void ussdCbk(ResponseStateType response)
{
    if (response == M590_RESPONSE_SUCCESS)
    {
        Serial.print(F("#GSM: USSD Response: ")); Serial.println(ussd);
    }
    else
    {
        Serial.print(F("#GSM: USSD error: ")); Serial.println(response);
    }
}

void deleteSMSCbk(ResponseStateType response)
{
    if (response == M590_RESPONSE_SUCCESS)
    {
        Serial.println(F("#GSM: SMS delete completed"));
    }
    else
    {
        Serial.print(F("#GSM: CMGD error: ")); Serial.println(response);
    }
}

void newSmsNotificationCbk(byte index)
{
    Serial.println("\nNew SMS arrived!");
    /* Initialize sms buffer */

    sms.date = "";
    sms.sender = "";
    sms.status = "";
    sms.text = "";

    gsm.readSMS(index, &sms, &readSMSCbk);

    gsm.deleteSMS(index, M590_SMS_DEL_REC_NR, &deleteSMSCbk);
}

void init_M590()
{
    Serial.begin(115200);

    Serial.print(F("#INIT: M590 => "));

    gsm.enableDebugSerial(&Serial);
    
    if (!gsm.init(115200, &Serial1, PIN_M590_ONOFF, "0000"))
    {
        Serial.println(F("FAILED")); 
    }
    else
    {
        Serial.println(F("DONE"));

        Serial.print(F(" GSM Network =>  "));
        if (! gsm.waitForNetWork())
        {
            Serial.println(F("Failed to register")); 
        }
        else
        {
            Serial.println(F("Registered"));

            gsm.setSMSTextModeCharSetGSM(&settingsCbk);

            gsm.deleteSMS(0, M590_SMS_DEL_ALL, &deleteSMSCbk);

            gsm.getSignalStrength(&sq, &sqCbk);

            gsm.sendSMS("+40745662769", "AHAS Startup", &sendSmsCbk);

            gsm.sendUSSD("*133#", &ussd, &ussdCbk);

            gsm.enableNewSMSNotification(&newSmsNotificationCbk, &newSmsNotificationEnableCbk);
        }
    }
}

void cyclicM590()
{
  gsm.process();
}

#endif