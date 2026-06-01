The fx2hid example is an adaptation of the Cypress example FX2_Hid_Keyboard, which was in turn adapted from Cypress's bulkloop.c. FX2_Hid_Keyboard is available on request from Cypress.

The code runs on the Cypress FX2 EZ-USB chip.

Both this fx2hid code and Windows applications that communicate with devices running the code are available from:
 
   www.Lvr.com/hidpage.htm

The device is a generic (vendor-defined) HID that waits to receive an Output report from the host, then returns the same data in an input report to the host.

The code uses the Cypress frameworks code and requires the full (not evaluation) version of the Keil compiler.

The changes to the FX2_Hid_Keyboard code are mainly in the report descriptors in dscr.a51 and in TD_Poll() in fx2hid.c. 

Send comments, questions, bug reports, etc. to jan@Lvr.com



