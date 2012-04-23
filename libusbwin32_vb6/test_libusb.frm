VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   7470
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7995
   LinkTopic       =   "Form1"
   ScaleHeight     =   7470
   ScaleWidth      =   7995
   StartUpPosition =   3  'Windows-Standard
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   255
      Left            =   240
      TabIndex        =   1
      Top             =   120
      Width           =   1215
   End
   Begin VB.TextBox textbox 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   6495
      Left            =   240
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Beides
      TabIndex        =   0
      Text            =   "test_libusb.frx":0000
      Top             =   720
      Width           =   7575
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim my_descriptor As UsbDeviceDescriptor
Dim dev_config As UsbConfigDescriptor
Dim my_interface As UsbInterfaceDescriptor
Dim my_endpoint As UsbEndPointDescriptor

Private Sub Command1_Click()
    scan_bus
End Sub
Private Sub Form_Activate()
    UsbInit
    UsbSetDebug (255)
    scan_bus
End Sub

Private Sub scan_bus()
    Dim buffer(0 To 255) As Byte
    Dim i, x As Long
    Dim dev As Long
    textbox.Text = ""
    i = 0
    dev = UsbOpen(i, -1, -1)
    Do While dev
        If UsbGetDeviceDescriptor(dev, my_descriptor) Then
            msg "Device " & i
            msg "-- VID             : " & Hex$(my_descriptor.idVendor)
            msg "-- PID             : " & Hex$(my_descriptor.idProduct) & " "
            
            If (my_descriptor.iManufacturer) Then
                If (UsbGetStringSimple(dev, my_descriptor.iManufacturer, buffer(0), UBound(buffer))) Then
                    msg "-- Manufacturer    : " & StrConv(buffer, vbUnicode)
                    msg " "
                End If
            Else
                msg "-- Manufacturer    : not specified"
            End If
                        
            If (my_descriptor.iProduct) Then
                If (UsbGetStringSimple(dev, my_descriptor.iProduct, buffer(0), UBound(buffer))) Then
                    msg "-- Product         : " & StrConv(buffer, vbUnicode)
                    msg " "
                End If
            Else
                    msg "-- Product         : not specified"
            End If
            
            If (my_descriptor.iSerialNumber) Then
                If (UsbGetStringSimple(dev, my_descriptor.iSerialNumber, buffer(0), UBound(buffer))) Then
                    msg "- Serial  nmbr    : " & StrConv(buffer, vbUnicode)
                    msg " "
                End If
            Else
                msg "-- Serial  nmbr    : not specified"
            End If
            
            ' retrieve the USB version
            usbver = Hex$(my_descriptor.bcdUSB)
            Mid$(usbver, 3, 1) = Mid$(usbver, 2, 1)
            Mid$(usbver, 2, 1) = "."
            msg "-- USB version     : " & usbver
            
            msg "-- Device Class    : " & my_descriptor.bDeviceClass
            msg "-- Subclass        : " & my_descriptor.bDeviceSubClass
'           msg "-- Max Packet size : " & my_descriptor.bMaxPacketSize0
'           msg "-- Protocol        : " & my_descriptor.bDeviceProtocol
            For x = 0 To my_descriptor.bNumConfigurations - 1
                print_configuration dev, x
            Next x
        End If
        UsbClose (dev)
        i = i + 1
        dev = UsbOpen(i, -1, -1)
    Loop
    
End Sub

Sub print_configuration(handle As Long, index)
    Dim x
    If UsbGetConfigurationDescriptor(handle, index, dev_config) Then
        msg "--- Configuration   : " & index
        msg "--- Total Length    : " & dev_config.wTotalLength
        msg "--- Num interfaces  : " & dev_config.bNumInterfaces
        msg "--- Config. Value   : " & dev_config.bConfigurationValue
        msg "--- Configuration   : " & dev_config.iConfiguration
        msg "--- Attributes      : " & Hex$(dev_config.bmAttributes)
        msg "--- Max Power       : " & dev_config.MaxPower
    End If
    For x = 0 To dev_config.bNumInterfaces - 1
        print_interface handle, index, x
    Next x
End Sub
            
Sub print_interface(handle As Long, config_index, interface_index)
    Dim x
    Dim alt
    
    alt = 0

    Do While UsbGetInterfaceDescriptor(handle, config_index, interface_index, alt, my_interface)
       msg "---- Interface         : " & interface_index & "/" & alt
       msg "---- Alternate setting : " & my_interface.bAlternateSetting
       msg "---- NumEndpoints      : " & my_interface.bNumEndpoints
       msg "---- InterfaceClass    : " & my_interface.bInterfaceClass
       msg "---- InterfaceSubClass : " & my_interface.bInterfaceSubClass
       msg "---- InterfaceProtocol : " & my_interface.bInterfaceProtocol
       msg "---- Interface         : " & my_interface.iInterface
       
       For x = 0 To my_interface.bNumEndpoints - 1
           print_endpoint handle, config_index, interface_index, alt, x
       Next x
       
       alt = alt + 1
    Loop
End Sub


Sub print_endpoint(handle As Long, config_index, interface_index, alt_setting, index)
    If UsbGetEndpointDescriptor(handle, config_index, interface_index, alt_setting, index, my_endpoint) Then
       msg "----- Endpoint    : " & index
       msg "----- Address     : " & Hex$(my_endpoint.bEndpointAddress)
       msg "----- Attributes  : " & Hex$(my_endpoint.bmAttributes)
       msg "----- Packetsize  : " & Hex$(my_endpoint.wMaxPacketSize)
       msg "----- Interval    : " & Hex$(my_endpoint.bInterval)
       msg "----- Refresh     : " & Hex$(my_endpoint.bRefresh)
       msg "----- Syncaddress : " & Hex$(my_endpoint.bSynchAddress)
    End If
End Sub

Sub msg(msg)
   textbox.Text = textbox.Text & msg & vbNewLine
End Sub

