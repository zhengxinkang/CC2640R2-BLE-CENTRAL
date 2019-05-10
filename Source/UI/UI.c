/*
 * UI.c
 *
 *  Created on: 2018Äê12ÔÂ17ÈÕ
 *      Author: zheng
 */
#include "BF_Util.h"
#include "UI.h"
#include "Hal_buzz.h"
#include "Hal_oled.h"

void UI(UI_TYPE uiType, uint32_t data, uint8_t *str, uint32_t dataReason)
{
    switch(uiType)
    {
        case UI_TYPE_READY:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, 000);          Hal_oled_strChange(OLED_STR_TYPE_ITEM, "             ");
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "       ");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "READY  ");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
//            BF_taskSleepMs(50);                                                            Hal_oled_dataChange(OLED_DATA_TYPE_REASON, 000);
            break;
        }
        case UI_TYPE_SUCCESS:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, 000);          Hal_oled_strChange(OLED_STR_TYPE_ITEM, "-finish      ");
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "       ");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "PASS   ");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, dataReason);
//            BF_taskSleepMs(50);
            Buzz_action(1000, 500, 2);
            break;
        }
        case UI_TYPE_FAIL:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, 000);        /*  Hal_oled_strChange(OLED_STR_TYPE_ITEM, "-finish  ");*/
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "       ");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "NG     ");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, data);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, dataReason);
//            BF_taskSleepMs(50);
            Buzz_action(200, 100, 7);
            break;
        }
        case UI_TYPE_TESTED_PASS:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, 000);          Hal_oled_strChange(OLED_STR_TYPE_ITEM, "             ");
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "good   ");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "TESTED ");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, 000);
//            BF_taskSleepMs(50);
            Buzz_action(1000, 0, 1);
            break;
        }
        case UI_TYPE_TESTED_FAIL:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, 000);          Hal_oled_strChange(OLED_STR_TYPE_ITEM, "             ");
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "bad    ");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, data);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "TESTED ");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, 000);
//            BF_taskSleepMs(50);
            Buzz_action(200, 100, 3);
            break;
        }
        case UI_TYPE_QUIT:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, 000);          Hal_oled_strChange(OLED_STR_TYPE_ITEM, "             ");
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "       ");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "READY  ");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, 000);
//            BF_taskSleepMs(50);
            Buzz_action(200, 0, 1);
            break;
        }
        case UI_TYPE_PROCESS:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, data);         Hal_oled_strChange(OLED_STR_TYPE_ITEM, str);
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "-------");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "-------");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, 000);
//            BF_taskSleepMs(50);
            Buzz_action(30, 0, 1);
            break;
        }

        case UI_TYPE_CURRENT_OK:
        {
//            Hal_oled_dataChange(OLED_DATA_TYPE_ITEM, data);         Hal_oled_strChange(OLED_STR_TYPE_ITEM, str);
//            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "-------");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
//            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "-------");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, dataReason);
//            BF_taskSleepMs(50);
            Buzz_action(500, 0, 1);
            break;
        }
        case UI_TYPE_CURRENT_MONITOR:
        {
            Hal_oled_dataChange(OLED_DATA_TYPE_CURRENT, data);       /*  Hal_oled_strChange(OLED_STR_TYPE_ITEM, "stop");*/
            Hal_oled_strChange(OLED_STR_TYPE_LASTTIME, "-------");  Hal_oled_dataChange(OLED_DATA_TYPE_LASTTIME, 000);
            Hal_oled_strChange(OLED_STR_TYPE_THISTIME, "-------");  Hal_oled_dataChange(OLED_DATA_TYPE_THISTIME, 000);
                                                                        Hal_oled_dataChange(OLED_DATA_TYPE_REASON, 000);
//            BF_taskSleepMs(50);
            Buzz_action(30, 0, 1);
            break;
        }
    }
}

