#include "hibernate.h"

#include "hw_types.h"
#include "utils.h"
#include "prcm.h"
#include "uartA0.h"
#include "rom_map.h"
#include "configure.h"
#include "wdt_if.h"



#define SLOW_CLK_FREQ           32768


void HibernateInit()
{
    //
    // Configure the HIB module RTC wake time
    //
    MAP_PRCMHibernateIntervalSet(HIBERNATE_TIME_IN_MIN * 60 * SLOW_CLK_FREQ);

    //
    // Enable the HIB RTC
    //
    MAP_PRCMHibernateWakeupSourceEnable(PRCM_HIB_SLOW_CLK_CTR);

}

void HibernateEnter()
{

    //
    // Enter debugger info
    //
    DBG_PRINT("HIB: Entering HIBernate...\n\r");
    MAP_UtilsDelay(80000);

    //
    // Enter HIBernate mode
    //
    MAP_PRCMHibernateEnter();
}

void HibernateWatchdog()
{
    MAP_PRCMHibernateIntervalSet(SLOW_CLK_FREQ);
    MAP_PRCMHibernateWakeupSourceEnable(PRCM_HIB_SLOW_CLK_CTR);

    DBG_PRINT("WDT: Entering HIBernate...\n\r");
    MAP_UtilsDelay(80000);
    MAP_PRCMHibernateEnter();
}
