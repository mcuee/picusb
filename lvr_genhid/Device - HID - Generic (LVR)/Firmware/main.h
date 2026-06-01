

/********************************************************************
 File Description:

 Change History:
  Rev   Date        Description
  0.9   1/11/2013    Original version 

********************************************************************/
                     
#ifndef USER_H
#define USER_H

#define OUTPUT_REPORT &hid_report_out[0]

#if defined(USER_GET_REPORT_HANDLER) 
	#define FEATURE_REPORT &hid_report_feature[0]
    #define SET_REPORT_PFUNC & USER_GET_REPORT_HANDLER
#endif


/** P U B L I C  P R O T O T Y P E S *****************************************/

void UserInit(void);
void ProcessIO(void);
BYTE ReportSupported(void);
     
#endif //USER_H