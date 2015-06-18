#ifndef __TIME_H
#define __TIME_H

#include "typeDefs.h"


typedef struct 
{
      uInt8     hour;
      uInt8     min;
      uInt8     sec;
      
      uInt8     day;
      uInt8     month;
      uInt8     year;
      
}tDateTime;




extern             uInt8      TimeInStructIsOk(tDateTime dt);

extern             uInt32     TimeInSeconds(tDateTime  dt);
extern             tDateTime  TimeInStruct(uInt32  sec);
extern             void       TimeInText(cInt8 * buff, tDateTime  dt);



#endif
