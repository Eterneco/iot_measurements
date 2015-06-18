#include "time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const uInt8  monthLength[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 

 
uInt8   TimeInStructIsOk(tDateTime dt)
{  
   uInt8 l_leap = 0;
  
   if(dt.hour  > 23)          return 0;
   if(dt.min   > 59)          return 0;
   if(dt.sec   > 59)          return 0;

   if(dt.year  > 99)          return 0;
   if(dt.month > 12)          return 0;
   
   if(dt.month == 2)     
    if(0 == (dt.year % 4))    l_leap = 1;

   
   if(dt.day > (monthLength[dt.month -1] + l_leap) )   return 0;
   
   return 1;
}

uInt32  TimeInSeconds(tDateTime  dt)
{
  uInt32   l_result;
  
  uInt16   l_days  =   dt.day-1;
  
  uInt8    l_month =   dt.month-1;
  uInt8    l_year  =   dt.year;


    if((0 == (l_year % 4)) && (l_month > 1))     ++ l_days; 

    while (l_month)
    {
            --l_month;
            l_days +=  monthLength[l_month];
    }
    
    while (l_year)
    {
            --l_year;
            l_days += 365;
            if(0 == (l_year % 4)) ++l_days;
    }

   l_result    = (uInt32)l_days   * 86400;
   l_result   += (uInt32)dt.hour  *  3600;
   l_result   += (uInt32)dt.min   *    60;
   l_result   += (uInt32)dt.sec;

   return l_result;
}

tDateTime  TimeInStruct(uInt32  sec)
{
   tDateTime dt;
   uInt8     idx; 
   uInt16    l_aux16;
   uInt32    l_aux32;

   //sec in day (60*60*24)
   l_aux32 = (sec % 86400);                           

   dt.hour  = (l_aux32  / 3600);
   dt.min   = ((l_aux32 % 3600)/60);
   dt.sec   = (l_aux32  % 60); 

   //days from 2000-01-01
   l_aux32= (sec / 86400);

   dt.year  =   0;
   l_aux16  = 366;              // days in leap year
   while (l_aux32 >= l_aux16)
   {
          l_aux32 -= l_aux16;
          dt.year ++;
          
          l_aux16 = 365;
          if(0 == (dt.year % 4))  l_aux16 = 366;
    }
   
    idx = 0;
    dt.month = 0;
    l_aux16 = monthLength[idx];
    while (l_aux32 >= l_aux16)
    {
            l_aux32 -= l_aux16;
            dt.month ++;
            
            l_aux16 = monthLength[++ idx];
            if (idx == 1 && (0 == (dt.year % 4))) l_aux16 += 1;
    }
       
    dt.month += 1;
    dt.day    = l_aux32 + 1;	

   return dt;
}

void  TimeInText(cInt8 * buff, tDateTime  dt)
{
    cInt8  l_tab[5];
    sInt16 l_index;
    
    // year
    l_index = sprintf(buff,"%d",dt.year+2000);
    buff[l_index ++] = '-';
    
    // month
    if(sprintf(l_tab,"%d",dt.month) == 1)
    {
        buff[l_index ++] = '0';
        buff[l_index ++] = l_tab[0];
    } 
    else  
    {  
        buff[l_index ++] = l_tab[0];
        buff[l_index ++] = l_tab[1];
    }
    buff[l_index ++] = '-';   
    
    // day
    if(sprintf(l_tab,"%d",dt.day) == 1)
    {
        buff[l_index ++] = '0';
        buff[l_index ++] = l_tab[0];
    } 
    else  
    {  
        buff[l_index ++] = l_tab[0];
        buff[l_index ++] = l_tab[1];
    }
    buff[l_index ++] = ' ';   
   
    // hour
    if(sprintf(l_tab,"%d",dt.hour) == 1)
    {
        buff[l_index ++] = '0';
        buff[l_index ++] = l_tab[0];
    } 
    else  
    {  
        buff[l_index ++] = l_tab[0];
        buff[l_index ++] = l_tab[1];
    }
    buff[l_index ++] = ':';   
    
    // min
    if(sprintf(l_tab,"%d",dt.min) == 1)
    {
        buff[l_index ++] = '0';
        buff[l_index ++] = l_tab[0];
    } 
    else  
    {  
        buff[l_index ++] = l_tab[0];
        buff[l_index ++] = l_tab[1];
    }
    buff[l_index ++] = ':'; 
    
    // sec
    if(sprintf(l_tab,"%d",dt.sec) == 1)
    {
        buff[l_index ++] = '0';
        buff[l_index ++] = l_tab[0];
    } 
    else  
    {  
        buff[l_index ++] = l_tab[0];
        buff[l_index ++] = l_tab[1];
    }
    buff[l_index ++] = 0;   
}
