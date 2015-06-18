#ifndef __CONFIGURE_H
#define __CONFIGURE_H

#include "typeDefs.h"

/********************************************************************************
    network connection
********************************************************************************/
#define SSID_NAME               "HERE_AP_SIID"               /* AP SSID */
#define SECURITY_TYPE           SL_SEC_TYPE_WPA_WPA2         /* Security type (OPEN or WEP or WPA)*/
#define SECURITY_KEY            "HERE_AP_PASSWORD"           /* Password of the secured AP */



/********************************************************************************
    PLOT.LY
********************************************************************************/

#define PLOTLY_SERVER_NAME       "plot.ly"

#define PLOTLY_USER_NAME         "lk4ep"					/* Plot.ly user name */
#define PLOTLY_USER_KEY          "5qj6fclwhz"			 	/* Plot.ly API  key  */


/********************************************************************************
    HIBERNATE
********************************************************************************/

#define HIBERNATE_TIME_IN_MIN			5


/********************************************************************************
    SNTP SERVER NAME
********************************************************************************/

#define SNTP_SERVER_NAME        "ntp.inode.at"

//  ######################### list of SNTP servers #################################
//  ##
//  ##          hostname             |        IP       |       location
//  ## -----------------------------------------------------------------------------
//  ##   ntp.inode.at                | 195.58.160.5    |       Vienna
//  ##   ntp3.proserve.nl            | 212.204.198.85  |       Amsterdam
//  ##   ntp.spadhausen.com          | 109.168.118.249 |       Milano - Italy
//  ##   ntp3.tcpd.net               | 23.23.128.218   |       London, UK
//  ##   dmz0.la-archdiocese.net     | 209.151.225.100 |       Los Angeles, CA
//  ##   Optimussupreme.64bitVPS.com | 216.128.88.62   |       Brooklyn, New York
//  ##   ntp.mazzanet.id.au          | 203.206.205.83  |       Regional Victoria, Australia
//  ##   a.ntp.br                    | 200.160.0.8     |       Sao Paulo, Brazil
//  ################################################################################

/********************************************************************************
    GMT TIME ZONE
********************************************************************************/

#define GMT_TIME_HOURS          +1                                            // set gmt hours
#define GMT_TIME_MINUTS         0                                             // set gmt minuts
#define GMT_TIME_ZONE           (GMT_TIME_HOURS *3600 + GMT_TIME_MINUTS*60)   // GMT+01:00

// GMT-12:00
/* Eniwetok, Kwaialein */
// GMT-11:00
/* Midway Island,  Samoa - changes to GMT +12 on 31/12/2011 to be the same as New Zealand */
// GMT-10:00
/* Hawaii, Honolulu, Hawaiin Standard Time*/
// GMT-09:30
/* French Polynesia*/
// GMT-09:00
/* Alaska, Alaska Standard Time*/
// GMT-08:00
/* Anchorage, British Columbia Cent., Los Angeles, San Diego, San Francisco,
   Seattle, Tijuana, Vancouver, Yukon, Pacific Standard Time */
// GMT-07:00
/* Alberta, British Columbia N.E., Denver, Edmonton, Phoenix,
   Salt Lake City, Santa Fe, Saskatchewan West, Mountain Standard Time */
// GMT-06:00
/* Chicago,  Dallas, El Paso, Ft. Worth, Guatamala, Houston,
   Indiana [west], Manitoba, Memphis, New Orleans, Mexico City,
   Ontario N.W., San Antonio-Texas, Saskatchewan East, St. Louis, Winnipeg Central Standard Time */
// GMT-05:00
/* Bogota, Boston, Indiana [east], Kingston, Lima, Miami, Montreal,
  New York, Ontario, Quebec, Washington, Eastern Standard Time */
// GMT-04:30
/* Kabul */
// GMT-04:00
/* Caracas, Labrador, La Paz, Maritimes, Santiago, Atlantic Standard Time*/
// GMT-03:30
/* Newfoundland */
// GMT-03:00
/* Brazilia, Buenos Aires, Georgetown, Montevideo, Rio de Janero Standard Time [Canada] */
// GMT-02:00
/* Mid-Atlantic*/
// GMT-01:00
/* Azores, Cape Verde Is., Western Africa Time */
// GMT-00:00
/* LONDON, includes Belfast, Cardiff, Dublin, Edinburgh, Iceland,
  Rep. of Ireland, Lisbon, Monrovia, Morocco, Portugal */
// GMT+01:00
/* Amsterdam, Berlin, Bern, Bratislava, Brussells, Budapest, EUROPEAN UNION,
   Lagos, Madrid, Malta, Prague, Paris, Riga, Rome, Sarajevo, Swedish Winter Time,
   Valletta, Vienna, Warsaw, Zagreb, Middle and Central European Time */
// GMT+02:00
/* Athens, Bucharest, Cairo, Cape Town, Cyprus, Estonia, Finland, Greece,
   Harare, Helsinki, Israel, Istanbul, Latvia, Pretoria, Russian Zone 1 and Eastern Europe */
// GMT+03:00
/* Ankara, Aden, Amman, Baghdad, Bahrain, Beruit, Dammam, Kuwait, Moscow,
   Nairobi, Riyadh, St. Petersburg, Tehran Russian Zone 2 and Baghdad */
// GMT+03:30
/* Iraq */
// GMT+04:00
/* Abu Dhabi, Baku, Kabul, Kazan, Muscat, Tehran, Tbilisi, Volgograd, Russian Zone 3 */
// GMT+04:30
/* Afghanistan, Iran */
// GMT+05:00
/* Calcutta, Colombo, Islamabad, Madras, New Dehli */
// GMT+05:30
/* India */
// GMT+05:45
/* Nepal */
// GMT+06:00
/* Almaty, Dhakar, Kathmandu, Colombo, Sri Lanka */
// GMT+06:30
/* Rangoon ,Myanmar (Burma) */
// GMT+07:00
/* Bangkok, Hanoi, Jakarta, Phnom Penh, Western Australia Standard Time*/
// GMT+08:00
/* Beijing, Chongqing, Hong Kong, Kuala Lumpar, Manila,
   Perth, Singapore, Taipei, Urumqi, China Coast Time*/
// GMT+09:00
/*  Osaka, Seoul, Sapporo, Seoul, Tokyo, Yakutsk, Japan Standard Time*/
// GMT+09:30
/* Adelaide and Darwin, Central Australia*/
// GMT+10:00
/* Brisbane, Canberra, Guam, Hobart, Melbourne, Port Moresby,
   Sydney, Vladivostok, Guan Standard Time, Eastern Australia Standard Time*/
// GMT+11:00
/* Magadan, New Caledonia, Solomon Is. Samoa - changes to GMT +12 on 31/12/2011 to be the same as New Zealand */
// GMT+12:00
/* Auckland, Christchurch NZ., Fiji, Kamchatka, Marshall Is.,
   Samoa, Wellington, Suva, New Zealand Standard Time*/
// GMT+13:00
/* Samoa */

#endif
