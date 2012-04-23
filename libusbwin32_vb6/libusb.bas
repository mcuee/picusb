Attribute VB_Name = "libusb"
' ******************************************* LIBusb interface code ************************************


Option Explicit

Global Const LIBUSB_PATH_MAX = 512

Global Const USB_CLASS_PER_INTERFACE = 0
Global Const USB_CLASS_AUDIO = 1
Global Const USB_CLASS_COMM = 2
Global Const USB_CLASS_HID = 3
Global Const USB_CLASS_PRINTER = 7
Global Const USB_CLASS_MASS_STORAGE = 8
Global Const USB_CLASS_HUB = 9
Global Const USB_CLASS_DATA = 10
Global Const USB_CLASS_VENDOR_SPEC = &HFF

Global Const USB_DT_DEVICE = &H1
Global Const USB_DT_CONFIG = &H2
Global Const USB_DT_STRING = &H3
Global Const USB_DT_INTERFACE = &H4
Global Const USB_DT_ENDPOINT = &H5
Global Const USB_DT_HID = &H21
Global Const USB_DT_REPORT = &H22
Global Const USB_DT_PHYSICAL = &H23
Global Const USB_DT_HUB = &H29

Global Const USB_DT_DEVICE_SIZE = 18
Global Const USB_DT_CONFIG_SIZE = 9
Global Const USB_DT_INTERFACE_SIZE = 9
Global Const USB_DT_ENDPOINT_SIZE = 7
Global Const USB_DT_ENDPOINT_AUDIO_SIZE = 9
Global Const USB_DT_HUB_NONVAR_SIZE = 7

Global Const USB_ENDPOINT_ADDRESS_MASK = &HF
Global Const USB_ENDPOINT_DIR_MASK = &H80

Global Const USB_ENDPOINT_TYPE_MASK = &H3
Global Const USB_ENDPOINT_TYPE_CONTROL = 0
Global Const USB_ENDPOINT_TYPE_ISOCHRONOUS = 1
Global Const USB_ENDPOINT_TYPE_BULK = 2
Global Const USB_ENDPOINT_TYPE_INTERRUPT = 3

Global Const USB_MAXALTSETTING = 128
Global Const USB_MAXCONFIG = 8

Global Const USB_REQ_GET_STATUS = &H0
Global Const USB_REQ_CLEAR_FEATURE = &H1
Global Const USB_REQ_SET_FEATURE = &H3
Global Const USB_REQ_SET_ADDRESS = &H5
Global Const USB_REQ_GET_DESCRIPTOR = &H6
Global Const USB_REQ_SET_DESCRIPTOR = &H7
Global Const USB_REQ_GET_CONFIGURATION = &H8
Global Const USB_REQ_SET_CONFIGURATION = &H9
Global Const USB_REQ_GET_INTERFACE = &HA
Global Const USB_REQ_SET_INTERFACE = &HB
Global Const USB_REQ_SYNCH_FRAME = &HC

Global Const USB_TYPE_STANDARD = &H0
Global Const USB_TYPE_CLASS = &H20
Global Const USB_TYPE_VENDOR = &H40
Global Const USB_TYPE_RESERVED = &H60

Global Const USB_RECIP_DEVICE = &H0
Global Const USB_RECIP_INTERFACE = &H1
Global Const USB_RECIP_ENDPOINT = &H2
Global Const USB_RECIP_OTHER = &H3

Global Const USB_ENDPOINT_IN = &H80
Global Const USB_ENDPOINT_OUT = 0

Global Const USB_ERROR_BEGIN = 500000

Global Const USB_MAXENDPOINTS = 32
Global Const USB_MAXINTERFACES = 32

Type UsbDescriptorHeader
    bLength As Byte
    bDescriptorType As Byte
End Type

Type UsbStringDescriptor
    bLength As Byte
    bDescriptorType As Byte
    wData(1) As Integer
End Type

Type UsbHIDDescriptor
    bLength As Byte
    bDescriptorType As Byte
    bcdHID As Integer
    bCountryCode As Byte
    bNumDescriptors As Byte
End Type

Type UsbEndPointDescriptor
  bLength As Byte
  bDescriptorType As Byte
  bEndpointAddress As Byte
  bmAttributes As Byte
  wMaxPacketSize As Integer
  bInterval As Byte
  bRefresh As Byte
  bSynchAddress As Byte
End Type

Type UsbInterfaceDescriptor
  bLength As Byte
  bDescriptorType As Byte
  bInterfaceNumber As Byte
  bAlternateSetting As Byte
  bNumEndpoints As Byte
  bInterfaceClass As Byte
  bInterfaceSubClass As Byte
  bInterfaceProtocol As Byte
  iInterface As Byte
End Type

Type UsbConfigDescriptor
  bLength As Byte
  bDescriptorType As Byte
  wTotalLength As Integer
  bNumInterfaces As Byte
  bConfigurationValue As Byte
  iConfiguration As Byte
  bmAttributes As Byte
  MaxPower As Byte
End Type

Type UsbDeviceDescriptor
  bLength As Byte
  bDescriptorType As Byte
  bcdUSB As Integer
  bDeviceClass As Byte
  bDeviceSubClass As Byte
  bDeviceProtocol As Byte
  bMaxPacketSize0 As Byte
  idVendor As Integer
  idProduct As Integer
  bcdDevice As Integer
  iManufacturer As Byte
  iProduct As Byte
  iSerialNumber As Byte
  bNumConfigurations As Byte
End Type

Type UsbCtrlSetup
  bRequestType As Byte
  bRequest As Byte
  wValue As Integer
  wIndex As Integer
  wLength As Integer
End Type

Type UsbVersion
  dll_major As Long
  dll_minor As Long
  dll_micro As Long
  dll_nano As Long
  driver_major As Long
  driver_minor As Long
  driver_micro As Long
  driver_nano As Long
End Type

' ********************************************* USBinit ***************************************************
' Before beeing able to do any USB operations the engine needs to be started. USBinit does exactly that.
'       Example :  x = UsbInit
'
Declare Sub UsbInit Lib "libusbvb0.dll" _
  Alias "vb_usb_init" ()

' ********************************************* USBsetdebug ***********************************************

Declare Sub UsbSetDebug Lib "libusbvb0.dll" _
  Alias "vb_usb_set_debug" ( _
  ByVal level As Long)

' ********************************************* USB Open **************************************************
' Prior to be able to perform operations with a device you need to open it by calling USB_Open
' USB_Open returns a magic number called a 'handle' that will be used for subsequent operations
' You need to store that handle in a variable. the handle is a LONG datatype variable
' There are 2 possible ways of opening a device
'    1) By Index
'       Example :       dim my_handle as long
'                       my_index=3
'                       my_handle = USBOpen(my_index,-1,-1)
'                       if my_handle = 0 then debug.print " No device found "
'       The above code would open the device 'my_index' on the usb bus.
'       You need to specify -1 for both VID and VIP. This forces libusb to open by index number.
'       You need to test the returning handle to contain a magic number.
'       If the handle is 0 there was no device at the specified index.
'
'       You can use this method to 'walk' the entire USB bus
'
'       Example :      dim my_handle as long
'                      dim my_index as long
'                      my_index=0
'                      do
'                        my_handle = USBOpen(my_index,-1,-1)
'                        usbclose my_handle
'                        my_index = my_index + 1
'                      loop until my_handle=0
'                      debug.print " There are " & index & " Devices on the USB bus"
'
'    2) Opening by VID / VIP
'       The second mechanism is to open a device directly by VID and VIP
'       Lets assume we have a device where the VID is &hDD8 and hte PID is &hC100 ( this is an example )
'       The following code would open this device ;
'       Example:       Dim my_handle
'                      my_handle= USBopen (0,&hDD8,&hC100)
'                      if my_handle=0 then debug.print " No such device "
'
'       Again , the handle returned needs to be checked. if the handle is 0 this means there is no such device attached.
'       Let's assume we actually have multiple devices with the same VID and PID.
'       It is possible to actually check how many devices there are with this specific VID and PId combination
'
'       Example:       Dim my_handle , my_index as long
'                      Const my_vid = &hdd8
'                      const my_pid = &hc100
'                      my_index=0
'                      do
'                         my_handle= USBopen (my_index,my_vid,my_pid)
'                         usbclose my_handle
'                         my_index=my_index+1
'                      loop until my_handle=0
'                      Debug.Print " There are " & my_index & " devices with VID " & hex$(my_vid) & " PID " & hex$(my_pid)

Declare Function UsbOpen Lib "libusbvb0.dll" _
  Alias "vb_usb_open" ( _
  ByVal index As Long, _
  ByVal vid As Long, _
  ByVal pid As Long) As Long

' ********************************************* USB Close **************************************************
' When you no longer need access to a USB device you have to release it.
' USB_close does exactly that. Just pass it the handle you got from UsbOpen.
' USB_close returns a '1' if succesful.

Declare Function UsbClose Lib "libusbvb0.dll" _
  Alias "vb_usb_close" ( _
  ByVal dev As Long) As Long

' ********************************************* USB GetDescriptor*******************************************
' Any USB devices contains a set of data called 'descriptors' that provide information about the device
' usbgetdescriptors allows you to retrieve this dataset.
' The returning dataset is a complex structure. In order to make delaing with this data easier a number of predefined
' types have been defined that will directly format the returning data.
'
' Example:  dim my_handle as long
'           dim my_descriptor as USBdeviceDescriptor
'           my_handle=usbopen(0,-1,-1)     ' we open the first device attached
'           if (usbgetdescriptor(my_handle, USB_DT_DEVICE,0,my_descriptor,USB_DT_DEVICE_SIZE) <> USB_DT_DEVICE_SIZE) then
'              debug.print " no descriptor found "
'           end if
'
' USBgetDescriptor returns 0 if the call failed, so you need to check this.
' The retrieved data is loaded into the provided variable ( in this case my_descriptor)
' USBgetdescriptors can get a number of different descriptors. USB provides a Device , Config and interface descriptor.
' The important thing is to call usbgetdescriptors with the correct information :
' Example : dim my_handle as long
'           dim my_descriptor as USBDeviceDescriptor
'           dim my_config     as USBconfigdescriptor

'           usbgetdescriptor (my_handle, USB_DT_DEVICE   ,0,my_descriptor,USB_DT_DEVICE_SIZE)
'           usbgetdescriptor (my_handle, USB_DT_CONFIG   ,0,my_config    ,USB_DT_CONFIG_SIZE)

Declare Function UsbGetDescriptor Lib "libusbvb0.dll" _
  Alias "vb_usb_get_descriptor" ( _
  ByVal dev As Long, _
  ByVal dtype As Long, _
  ByVal index As Long, _
  ByRef buf As Any, _
  ByVal size As Long) As Long

Declare Function UsbGetDeviceDescriptor Lib "libusbvb0.dll" _
  Alias "vb_usb_get_device_descriptor" ( _
  ByVal dev As Long, _
  ByRef descriptor As UsbDeviceDescriptor) As Boolean

Declare Function UsbGetConfigurationDescriptor Lib "libusbvb0.dll" _
  Alias "vb_usb_get_configuration_descriptor" ( _
  ByVal dev As Long, _
  ByVal config_index As Long, _
  ByRef descriptor As UsbConfigDescriptor) As Boolean

Declare Function UsbGetInterfaceDescriptor Lib "libusbvb0.dll" _
  Alias "vb_usb_get_interface_descriptor" ( _
  ByVal dev As Long, _
  ByVal config_index As Long, _
  ByVal interface_index As Long, _
  ByVal alt_index As Long, _
  ByRef descriptor As UsbInterfaceDescriptor) As Boolean

Declare Function UsbGetEndpointDescriptor Lib "libusbvb0.dll" _
  Alias "vb_usb_get_endpoint_descriptor" ( _
  ByVal dev As Long, _
  ByVal config_index As Long, _
  ByVal interface_index As Long, _
  ByVal alt_index As Long, _
  ByVal endpoint_index As Long, _
  ByRef descriptor As UsbEndPointDescriptor) As Boolean

Declare Function UsbGetStringDescriptor Lib "libusbvb0.dll" _
  Alias "vb_usb_get_string_descriptor" ( _
  ByVal dev As Long, _
  ByVal index As Long, _
  ByVal langid As Long, _
  ByRef buf As Any, _
  ByVal size As Long) As Long

Declare Function UsbGetStringSimple Lib "libusbvb0.dll" _
  Alias "vb_usb_get_string_simple" ( _
  ByVal dev As Long, _
  ByVal index As Long, _
  ByRef buf As Any, _
  ByVal size As Long) As Long

Declare Function UsbBulkWrite Lib "libusbvb0.dll" _
  Alias "vb_usb_bulk_write" ( _
  ByVal dev As Long, _
  ByVal ep As Long, _
  ByRef buf As Any, _
  ByVal size As Long, _
  ByVal timeout As Long) As Long

Declare Function UsbBulkRead Lib "libusbvb0.dll" _
  Alias "vb_usb_bulk_read" ( _
  ByVal dev As Long, _
  ByVal ep As Long, _
  ByRef buf As Any, _
  ByVal size As Long, _
  ByVal timeout As Long) As Long

Declare Function UsbInterruptWrite Lib "libusbvb0.dll" _
  Alias "vb_usb_interrupt_write" ( _
  ByVal dev As Long, _
  ByVal ep As Long, _
  ByRef buf As Any, _
  ByVal size As Long, _
  ByVal timeout As Long) As Long

Declare Function UsbInterruptRead Lib "libusbvb0.dll" _
  Alias "vb_usb_interrupt_read" ( _
  ByVal dev As Long, _
  ByVal ep As Long, _
  ByRef buf As Any, _
  ByVal size As Long, _
  ByVal timeout As Long) As Long

Declare Function UsbControlMsg Lib "libusbvb0.dll" _
  Alias "vb_usb_control_msg" ( _
  ByVal dev As Long, _
  ByVal requesttype As Long, _
  ByVal request As Long, _
  ByVal value As Long, _
  ByVal index As Long, _
  ByVal buf As Any, _
  ByVal size As Long, _
  ByVal timeout As Long) As Long

Declare Function UsbSetConfiguration Lib "libusbvb0.dll" _
  Alias "vb_usb_set_configuration" ( _
  ByVal dev As Long, _
  ByVal configuration As Long) As Long

Declare Function UsbClaimInterface Lib "libusbvb0.dll" _
  Alias "vb_usb_claim_interface" ( _
  ByVal dev As Long, _
  ByVal interface As Long) As Long

Declare Function UsbReleaseInterface Lib "libusbvb0.dll" _
  Alias "vb_usb_release_interface" ( _
  ByVal dev As Long, _
  ByVal interface As Long) As Long

Declare Function UsbSetAltinterface Lib "libusbvb0.dll" _
  Alias "vb_usb_set_altinterface" ( _
  ByVal dev As Long, _
  ByVal alternate As Long) As Long

Declare Function UsbResetEp Lib "libusbvb0.dll" _
  Alias "vb_usb_resetep" ( _
  ByVal dev As Long, _
  ByVal ep As Long) As Long

Declare Function UsbClearHalt Lib "libusbvb0.dll" _
  Alias "vb_usb_clear_halt" ( _
  ByVal dev As Long, _
  ByVal ep As Long) As Long

Declare Function UsbReset Lib "libusbvb0.dll" _
  Alias "vb_usb_reset" ( _
  ByVal dev As Long) As Long
