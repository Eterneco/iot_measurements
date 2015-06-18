#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// simplelink includes
#include "device.h"

// driverlib includes
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "interrupt.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"
#include "timer.h"
#include "utils.h"
#include "pin_mux_config.h"
#include "system.h"


//Free_rtos/ti-rtos includes
#include "osi.h"


// common interface includes
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "gpio.h"
#include "gpio_if.h"
#include "configure.h"
#include "uartA0.h"
#include "time.h"
#include "network_if.h"
#include "udma_if.h"
#include "hibernate.h"
#include "adc12.h"
#include "mcp9700.h"



#define SLEEP_TIME              8000000
#define SPAWN_TASK_PRIORITY     9
#define OSI_STACK_SIZE          6000




// GLOBAL VARIABLES -- Start

cInt8          	acSendBuff[TX_BUFF_SIZE];	 // 1024
cInt8          	acRecvbuff[RX_BUFF_SIZE]; 	 // 1024
SlSecParams_t 	SecurityParams = {0};  		 // AP Security Parameters



/********************************************************************************
   global procedures
********************************************************************************/


static int CreateConnection(unsigned long ulDestinationIP)
{
    int iLenorError;
    SlSockAddrIn_t  sAddr;
    int iAddrSize;
    int iSockIDorError = 0;

    sAddr.sin_family = SL_AF_INET;
    sAddr.sin_port = sl_Htons(80);

    //Change the DestinationIP endianity , to big endian
    sAddr.sin_addr.s_addr = sl_Htonl(ulDestinationIP);

    iAddrSize = sizeof(SlSockAddrIn_t);

    //Create TCP socket, IPv4
    iSockIDorError = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);

    if( iSockIDorError < 0 )
    {
        DBG_PRINT("Error: Error Number = %d .\n\r", iSockIDorError );
        return iSockIDorError;
    }

    //Connect with server
    iLenorError = sl_Connect(iSockIDorError, ( SlSockAddr_t *)&sAddr, iAddrSize);
    if( iLenorError < 0 )
    {
        // error
        DBG_PRINT("Error: Error Number = %d.  \n\r", iLenorError );
        return iLenorError;
    }

    DBG_PRINT("Socket Id: %d was created.",iSockIDorError);

    return iSockIDorError;//success, connection created
}


static sInt16 Connect2AccessPoint()
{
    // Reset The state of the machine
    Network_IF_ResetMCUStateMachine();

    //
    // Start the driver
    Network_IF_InitDriver(ROLE_STA);

    // Initialize AP security params
    SecurityParams.Key = (signed char *) SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type = SECURITY_TYPE;

    // Connect to the Access Point
    return(Network_IF_ConnectAP(SSID_NAME,SecurityParams));
}


/********************************************************************************
                         Measurements
********************************************************************************/

// ntp time struct
struct
{
    int 		   iSockID;
    unsigned long  ulElapsedSec;
    short 		   isGeneralVar;
    unsigned long  ulGeneralVar;
    unsigned long  ulGeneralVar1;
    char 		   acTimeStore[30];
    char		  *pcCCPtr;
    unsigned short uisCCLen;
}g_sAppData;



//*****************************************************************************
//
//! Gets the current time from the selected SNTP server
//!
//! \brief  This function obtains the NTP time from the server.
//!
//! \return None
//!
//
//*****************************************************************************
void GetSNTPTime(long ulDestinationIP)
{

    char cDataBuf[48];
    int iRet = 0;
    SlSockAddr_t sAddr;
    SlSockAddrIn_t sLocalAddr;
    int iAddrSize;
    //
    // Send a query ? to the NTP server to get the NTP time
    //
    memset(cDataBuf, 0, sizeof(cDataBuf));
    cDataBuf[0] = '\x1b';

    sAddr.sa_family = AF_INET;
    // the source port
    sAddr.sa_data[0] = 0x00;
    sAddr.sa_data[1] = 0x7B;    // UDP port number for NTP is 123
    sAddr.sa_data[2] = (char)((ulDestinationIP>>24)&0xff);
    sAddr.sa_data[3] = (char)((ulDestinationIP>>16)&0xff);
    sAddr.sa_data[4] = (char)((ulDestinationIP>>8)&0xff);
    sAddr.sa_data[5] = (char) (ulDestinationIP&0xff);

    iRet = sl_SendTo(g_sAppData.iSockID, cDataBuf, sizeof(cDataBuf), 0, &sAddr, sizeof(sAddr));

    if (iRet != sizeof(cDataBuf))
    {
        DBG_PRINT("Could not send SNTP request\n\r\n\r");
        return;    // could not send SNTP request
    }

    //
    // Wait to receive the NTP time from the server
    //
    iAddrSize = sizeof(SlSockAddrIn_t);
    sLocalAddr.sin_family = SL_AF_INET;
    sLocalAddr.sin_port = 0;
    sLocalAddr.sin_addr.s_addr = 0;
    sl_Bind(g_sAppData.iSockID,
            (SlSockAddr_t *)&sLocalAddr,
            iAddrSize);

    iRet = sl_RecvFrom(g_sAppData.iSockID,
                       cDataBuf, sizeof(cDataBuf), 0,
                       (SlSockAddr_t *)&sLocalAddr,
                       (SlSocklen_t*)&iAddrSize);
    if (iRet <= 0)
    {
        DBG_PRINT("Did not receive\n\r");
        return;
    }

    //
    // Confirm that the MODE is 4 --> server
    //
    if ((cDataBuf[0] & 0x7) != 4)    // expect only server response
    {
        DBG_PRINT("Expecting response from Server Only!\n\r");
        return;    // MODE is not server, abort
    }
    else
    {

        // Getting the data from the Transmit Timestamp (seconds) field
        // This is the time at which the reply departed the
        // server for the client
        //
        g_sAppData.ulElapsedSec = cDataBuf[40];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[41];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[42];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[43];


        tDateTime dt;
                                                // from  1900-01-01 to 2000-01-01
        g_sAppData.ulElapsedSec -= 3155673600;  // 100 years + 28 days(leap)
        g_sAppData.ulElapsedSec +=GMT_TIME_ZONE;
        dt = TimeInStruct( g_sAppData.ulElapsedSec);

        // time print time,,,
        //sprintf(g_sAppData.acTimeStore,"%4d-%2d-%2d %2d:%2d:%2d",dt.year+2000,dt.month,dt.day,dt.hour,dt.min,dt.sec);
        TimeInText(g_sAppData.acTimeStore, dt);

        DBG_PRINT("response from server: ");
        DBG_PRINT((char *)SNTP_SERVER_NAME);
        DBG_PRINT("\n\r");
        DBG_PRINT(g_sAppData.acTimeStore);
        DBG_PRINT("\n\r\n\r");

    }
}

void  SetPlotLyData(int iSockID, float value, char * fileName, char * plotTitle, char * xAxisTitle, char * yAxisTitle  )
{
    int iTXStatus;
    int iRXDataStatus;
    uInt16     l_contentLen;
    char*      pcBufLocation;

    // prefix data
    const char prefBuf1[]   = "POST /clientresp HTTP/1.1\r\nHost: 54.209.176.66\r\nContent-Length: ";
    const char prefBuf2[]   = "\r\n\r\n"; // calculate and set content length

    // postfix data
    const char postBuf01[]  = "version=2.3&origin=plot&platform=cc3200";
    const char postBuf02[]  = "&un=";               // SET user name
    const char postBuf03[]  = "&key=";              // SET user api key
    // args
    const char postBuf04[]  = "&args=[{\"x\": [\""; // SET value x ( time )
    const char postBuf05[]  = "\"], \"y\": [";      // SET value y ( measured value )
    const char postBuf06[]  = "],\"type\": \"scatter\",";        // plot: type
    const char postBuf07[]	= "\"mode\": \"lines+markers\", \"name\": \"value\",";                 // line: type and name
    const char postBuf08[]  = "\"line\": {\"shape\": \"linear\",\"color\":\"green\",\"size\":2},"; // line: properties
    const char postBuf09[]  = "\"marker\": {  \"color\": \"blue\",  \"size\": 6}}]";               // marker: properties
    // kwargs
    const char postBuf10[]  = "&kwargs={\"fileopt\": \"extend\",\"world_readable\": true,";        // data file: access type
    const char postBuf11[]  = "\"filename\": \"";               // SET data file: name
    const char postBuf12[]  = "\", \"layout\":{\"title\": \"";  // SET   plot title
    const char postBuf13[]  = "\", \"xaxis\": {\"title\": \"";  // SET x axis title
    const char postBuf14[]  = "\"},\"yaxis\": {\"title\": \"";  // SET y axis title
    const char postBuf15[]  = "\" }}}\r\n";

    l_contentLen = 0;
    memset(acRecvbuff, 0, sizeof(acRecvbuff));

    // calculate content length
    l_contentLen  = strlen(postBuf01) + strlen(postBuf02) + strlen(postBuf03) + strlen(postBuf04) + strlen(postBuf05);
    l_contentLen += strlen(postBuf06) + strlen(postBuf07) + strlen(postBuf08) + strlen(postBuf09) + strlen(postBuf10);
    l_contentLen += strlen(postBuf11) + strlen(postBuf12) + strlen(postBuf13) + strlen(postBuf14) + strlen(postBuf15);

    l_contentLen += strlen(PLOTLY_USER_NAME) + strlen(PLOTLY_USER_KEY);
    l_contentLen += strlen(fileName) + strlen(plotTitle) + strlen(xAxisTitle) + strlen(yAxisTitle);
    l_contentLen += 19; 							   // time  x=1,
    l_contentLen += sprintf(acSendBuff,"%1.f",value);  // value y


    // Puts together the HTTP POST string

    pcBufLocation = acSendBuff;

    // prefix
    pcBufLocation += sprintf(pcBufLocation,"%s%d%s",prefBuf1,l_contentLen,prefBuf2);

    // postfix
    pcBufLocation += sprintf(pcBufLocation,"%s%s%s%s%s",postBuf01,postBuf02,PLOTLY_USER_NAME,postBuf03,PLOTLY_USER_KEY);
    // args: x & y
    pcBufLocation +=sprintf(pcBufLocation,"%s%s%s%.1f",postBuf04,g_sAppData.acTimeStore,postBuf05,value);
    // args: plot type, line type, line properties, marker properties
    pcBufLocation += sprintf(pcBufLocation,"%s%s%s%s",postBuf06,postBuf07,postBuf08,postBuf09);
    // kwargs: data file access type, data file name
    pcBufLocation += sprintf(pcBufLocation,"%s%s%s",postBuf10,postBuf11,fileName);
    // kwargs: plot title, x axis title, y axis title
    pcBufLocation += sprintf(pcBufLocation,"%s%s%s%s%s%s",postBuf12,plotTitle,postBuf13,xAxisTitle,postBuf14,yAxisTitle);
    // finish
    pcBufLocation += sprintf(pcBufLocation,"%s",postBuf15);

    *pcBufLocation = 0;
    DBG_PRINT("Data To SEND \n\r");
    DBG_PRINT(acSendBuff);

    //
    // Send the HTTP POST string to the open TCP/IP socket.
    //
    DBG_PRINT("Sent HTTP POST request. \n\r");
    iTXStatus = sl_Send(iSockID, acSendBuff, strlen(acSendBuff), 0);
    MAP_UtilsDelay(80000000 *2);
    DBG_PRINT("Return value: %d \n\r", iTXStatus);

    //
    // Store the reply from the server in buffer.
    //
    DBG_PRINT("Received HTTP POST response data. \n\r");
    iRXDataStatus = sl_Recv(iSockID, &acRecvbuff[0], sizeof(acRecvbuff), 0);
    DBG_PRINT("Return value: %d \n\r", iRXDataStatus);
    acRecvbuff[sizeof(acRecvbuff) - 1] = 0;

    DBG_PRINT(acRecvbuff);
}


void DataUpdateTask()
{
    int     iSocketDesc;
    sInt16  apConnection;
    float   temperature;

	long ulStatus;
	unsigned long ulDestinationIP;

    DBG_PRINT("Update Data Begin \n\r");


    while(1)
    {
        /********************* Connect to specific AP ********************************/
        apConnection = Connect2AccessPoint();

        if(apConnection < 0)
        {
            DBG_PRINT("can't connect to %s AP",SSID_NAME);
            break;
        }
        else
        {
            DBG_PRINT("connected to %s AP",SSID_NAME);
        }


        /********************* GET TEMPERATURE *************************************/

		Adc12Enable();
		temperature = Mcp9700Temperature();
		Adc12Disable();
		DBG_PRINT("temperature %.1f \n\r",temperature);


        /********************* GET SNTP Time ***************************************/

        // Get the serverhost IP address using the DNS lookup

        ulStatus = Network_IF_GetHostIP(SNTP_SERVER_NAME,&ulDestinationIP);
        if(ulStatus < 0)
        {
            DBG_PRINT("NTP DNS lookup failed. \n\r");
            break;
        }

        //
        // Create UDP socket
        iSocketDesc = sl_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(iSocketDesc < 0)
        {
            UART_PRINT("Socket create failed\n\r");
            break;
        }

        GetSNTPTime(ulDestinationIP);

        // Close the socket
        close(iSocketDesc);
        DBG_PRINT("\n\rSocket SNTP closed\n\r");


        /********************* SET PLOTLY DATA *************************************/


        // Get the serverhost IP address using the DNS lookup
        ulStatus = Network_IF_GetHostIP(PLOTLY_SERVER_NAME,&ulDestinationIP);
        if(ulStatus < 0)
        {
            DBG_PRINT("PLOT.LY DNS lookup failed. \n\r");
            break;
        }

        // Create a TCP connection to the server
        iSocketDesc = CreateConnection( ulDestinationIP );
        if(iSocketDesc < 0)
        {
            DBG_PRINT("Socket creation failed.\n\r");
            break ;
        }


        // upload temperature value
        SetPlotLyData(iSocketDesc,temperature,"iot-temperature","temperature  measurements","time [s]","temperature [C]");

        // Close the socket
        close(iSocketDesc);
        DBG_PRINT("\n\rSocket PLOT.LY closed\n\r");
        break;
    }

    // Stop the driver
    Network_IF_DeInitDriver();
    DBG_PRINT("Update Data End\n\r");

}



//--------------------------------------------------------------------------------

void SystemTaskManager(void *pvParameters)
{

    while(1)
    {
    	   DataUpdateTask();
    	   HibernateEnter();
    }

}

void SystemTaskCreate()
{
    // Initializing DMA
    UDMAInit();


    // Start the SimpleLink Host
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

    //
    // Start the GetWeather task
    //
    osi_TaskCreate(SystemTaskManager,
                    (const signed char *)"system",
                    OSI_STACK_SIZE,
                    NULL,
                    1,
                    NULL );

}
