#include "usb.h"
#include <errno.h>
#include <string.h>
#include <windows.h>

void  __stdcall vb_usb_init(void);
void  __stdcall vb_usb_set_debug(int level);
usb_dev_handle * __stdcall vb_usb_open(int index, int vid, int pid);
int __stdcall vb_usb_close(usb_dev_handle *dev);
int __stdcall vb_usb_get_string_descriptor(usb_dev_handle *dev, 
                                           int index, int langid, 
                                           char *buf, int buflen);
int __stdcall vb_usb_get_string_simple(usb_dev_handle *dev, int index, 
                                       char *buf, int buflen);
int __stdcall vb_usb_get_descriptor(usb_dev_handle *udev, int type,
                                    int index, void *buf, int size);
int __stdcall vb_usb_get_device_descriptor(usb_dev_handle *udev, void *buf);
int __stdcall vb_usb_get_configuration_descriptor(usb_dev_handle *udev, 
                                           int config_index, void *buf);
int __stdcall vb_usb_get_interface_descriptor(usb_dev_handle *udev, 
                                              int config_index, 
                                              int interface_index,
                                              int alt_index,
                                              void *buf);
int __stdcall vb_usb_get_endpoint_descriptor(usb_dev_handle *udev, 
                                             int config_index, 
                                             int interface_index,
                                             int alt_index,
                                             int endpoint_index,
                                              void *buf);
int __stdcall vb_usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, 
                                int size, int timeout);
int __stdcall vb_usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, 
                               int size, int timeout);
int __stdcall vb_usb_interrupt_write(usb_dev_handle *dev, int ep,
                                     char *bytes, int size, int timeout);
int __stdcall vb_usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes,
                                    int size, int timeout);
int __stdcall vb_usb_control_msg(usb_dev_handle *dev, int requesttype,
                                 int request, int value, int index, 
                                 char *bytes, int size, int timeout);
int __stdcall vb_usb_set_configuration(usb_dev_handle *dev, 
                                       int configuration);
int __stdcall vb_usb_claim_interface(usb_dev_handle *dev, int interface);
int __stdcall vb_usb_release_interface(usb_dev_handle *dev, int interface);
int __stdcall vb_usb_set_altinterface(usb_dev_handle *dev, int alternate);
int __stdcall vb_usb_resetep(usb_dev_handle *dev, unsigned int ep);
int __stdcall vb_usb_clear_halt(usb_dev_handle *dev, unsigned int ep);
int __stdcall vb_usb_reset(usb_dev_handle *dev);



void __stdcall vb_usb_init(void)
{
  usb_init();
}

void __stdcall vb_usb_set_debug(int level)
{
  usb_set_debug(level);
}

usb_dev_handle * __stdcall vb_usb_open(int index, int vid, int pid)
{
  struct usb_bus *bus;
  struct usb_device *dev;
  
  usb_find_busses();
  usb_find_devices();

  for(bus = usb_get_busses(); bus; bus = bus->next) 
    {
      for(dev = bus->devices; dev; dev = dev->next) 
        {
          if((dev->descriptor.idVendor == vid || vid < 0)
             && (dev->descriptor.idProduct == pid || pid < 0))
            {
              if(!index)
                return usb_open(dev);
              else 
                index--;
            }
        }
    }

  return NULL;
}

int __stdcall vb_usb_close(usb_dev_handle *dev)
{
  return usb_close(dev);
}

int __stdcall vb_usb_get_string_descriptor(usb_dev_handle *dev, int index,
                                           int langid, char *buf, int buflen)
{
  return usb_get_string(dev, index, langid, buf, buflen);
}

int __stdcall vb_usb_get_string_simple(usb_dev_handle *dev, int index, 
                                       char *buf, int buflen)
{
  return usb_get_string_simple(dev, index, buf, buflen);
}

int __stdcall vb_usb_get_descriptor(usb_dev_handle *udev, int type,
                                    int index, void *buf, int size)
{
  return usb_get_descriptor(udev, (unsigned char)type, (unsigned char)index, 
                            buf, size);
}

int __stdcall vb_usb_get_device_descriptor(usb_dev_handle *udev, void *buf)
{
  struct usb_device *dev;
  if(!udev || !buf)
    return FALSE;
  dev = usb_device(udev);
  memcpy(buf, &dev->descriptor, USB_DT_DEVICE_SIZE);
  return TRUE;
}

int __stdcall vb_usb_get_configuration_descriptor(usb_dev_handle *udev, 
                                                  int config_index, void *buf)
{
  struct usb_device *dev;
  if(!udev || !buf)
    return FALSE;
  dev = usb_device(udev);
  if(config_index >= dev->descriptor.bNumConfigurations)
    return FALSE;
  memcpy(buf, &dev->config[config_index], USB_DT_CONFIG_SIZE);
  return TRUE;
}

int __stdcall vb_usb_get_interface_descriptor(usb_dev_handle *udev, 
                                              int config_index, 
                                              int interface_index,
                                              int alt_index,
                                              void *buf)
{
  struct usb_device *dev;
  if(!udev || !buf)
    return FALSE;
  dev = usb_device(udev);
  if(config_index >= dev->descriptor.bNumConfigurations)
    return FALSE;
  if(interface_index >= dev->config[config_index].bNumInterfaces)
    return FALSE;
  if(alt_index >= dev->config[config_index]
     .interface[interface_index].num_altsetting)
    return FALSE;

  memcpy(buf, &dev->config[config_index]
         .interface[interface_index].altsetting[alt_index], 
         USB_DT_INTERFACE_SIZE);
  return TRUE;
}

int __stdcall vb_usb_get_endpoint_descriptor(usb_dev_handle *udev, 
                                             int config_index, 
                                             int interface_index,
                                             int alt_index,
                                             int endpoint_index,
                                             void *buf)
{
  struct usb_device *dev;
  if(!udev || !buf)
    return FALSE;
  dev = usb_device(udev);
  if(config_index >= dev->descriptor.bNumConfigurations)
    return FALSE;
  if(interface_index >= dev->config[config_index].bNumInterfaces)
    return FALSE;
  if(alt_index >= dev->config[config_index]
     .interface[interface_index].num_altsetting)
    return FALSE;
  if(endpoint_index >= dev->config[config_index]
     .interface[interface_index].altsetting[alt_index].bNumEndpoints)
    return FALSE;
  memcpy(buf, &dev->config[config_index]
         .interface[interface_index].altsetting[alt_index]
         .endpoint[endpoint_index], USB_DT_ENDPOINT_SIZE);
  return TRUE;
}

int __stdcall vb_usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, 
                                int size, int timeout)
{
  return usb_bulk_write(dev, ep, bytes, size, timeout);
}

int __stdcall vb_usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, 
                               int size, int timeout)
{
  return usb_bulk_read(dev, ep, bytes, size, timeout);
}

int __stdcall vb_usb_interrupt_write(usb_dev_handle *dev, int ep,
                                     char *bytes, int size, int timeout)
{
  return usb_interrupt_write(dev, ep, bytes, size, timeout);
}

int __stdcall vb_usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes,
                                    int size, int timeout)
{
  return usb_interrupt_read(dev, ep, bytes, size, timeout);
}

int __stdcall vb_usb_control_msg(usb_dev_handle *dev, int requesttype,
                                 int request, int value, int index, 
                                 char *bytes, int size, int timeout)
{
  return usb_control_msg(dev, requesttype, request, value, index, 
                                 bytes, size, timeout);
}

int __stdcall vb_usb_set_configuration(usb_dev_handle *dev, 
                                       int configuration)
{
  return usb_set_configuration(dev, configuration);
}

int __stdcall vb_usb_claim_interface(usb_dev_handle *dev, int interface)
{
  return usb_claim_interface(dev, interface);
}

int __stdcall vb_usb_release_interface(usb_dev_handle *dev, int interface)
{
  return usb_release_interface(dev, interface);
}

int __stdcall vb_usb_set_altinterface(usb_dev_handle *dev, int alternate)
{
  return usb_set_altinterface(dev, alternate);
}

int __stdcall vb_usb_resetep(usb_dev_handle *dev, unsigned int ep)
{
  return usb_resetep(dev, ep);
}

int __stdcall vb_usb_clear_halt(usb_dev_handle *dev, unsigned int ep)
{
  return usb_clear_halt(dev, ep);
}

int __stdcall vb_usb_reset(usb_dev_handle *dev)
{
  return usb_reset(dev);
}


