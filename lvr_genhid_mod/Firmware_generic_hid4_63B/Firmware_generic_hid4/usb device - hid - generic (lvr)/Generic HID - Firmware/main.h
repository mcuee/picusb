/********************************************************************
 FileName:      main.c
 Dependencies:  See INCLUDES section
 Processor:		PIC18 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM?FS USB Demo Board, 
 				The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Compiler:  	Microchip C18 (for PIC18)
 Company:		Jan Axelson

 Software License Agreement:

Licensor grants any person obtaining a copy of this software ("You") 
a worldwide, royalty-free, non-exclusive license, for the duration of 
the copyright, free of charge, to store and execute the Software in a 
computer system and to incorporate the Software or any portion of it 
in computer programs You write.   
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


********************************************************************
 File Description:

 Change History:
  Rev   Date        Description
  1.0   8/6/2009    Original version 

********************************************************************/
                     
#ifndef USER_H
#define USER_H

#define SET_REPORT_PFUNC &USB_HID_SET_REPORT_HANDLER
#define INPUT_REPORT1 &hid_report_in[0]
#define OUTPUT_REPORT2 &hid_report_out[0]
#define FEATURE_REPORT3 &hid_report_feature[0]
#define FEATURE_REPORT4 &hid_report_feature1[0]

#define LSB(a)          ((a).v[0])
#define MSB(a)          ((a).v[1])

/** P U B L I C  P R O T O T Y P E S *****************************************/

void UserInit(void);
void ProcessIO(void);
void mySetReportHandler(void);
BYTE ReportSupported(void);
     
#endif //USER_H