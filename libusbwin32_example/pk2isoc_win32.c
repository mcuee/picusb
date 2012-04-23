#include <usb.h>

#define VERSION "0.1.0"
#define VENDOR_ID 0x04D8
#define PRODUCT_ID 0x0080
#define INTERFACE 0
#define BUFFER_SIZE 65

usb_dev_handle *find_pickit2_isoc();

usb_dev_handle* setup_libusb_access() {
   usb_dev_handle *pickit2_isoc;

       usb_set_debug(255);
   usb_init();
   usb_find_busses();
   usb_find_devices();

   if(!(pickit2_isoc = find_pickit2_isoc())) {
               printf("Couldn't find the mouse, Exiting\n");
               return NULL;
       }

       if (usb_set_configuration(pickit2_isoc, 1) < 0) {
               printf("Could not set configuration 1 : %s\n");
               return NULL;
       }

       if (usb_claim_interface(pickit2_isoc, INTERFACE) < 0) {
               printf("Could not claim interface: %s\n");
               return NULL;
       }

       return pickit2_isoc;
}

usb_dev_handle *find_pickit2_isoc()
{
   struct usb_bus *bus;
       struct usb_device *dev;

       for (bus = usb_busses; bus; bus = bus->next) {
       for (dev = bus->devices; dev; dev = dev->next) {
                       if (dev->descriptor.idVendor == VENDOR_ID &&
                               dev->descriptor.idProduct == PRODUCT_ID ) {
                               usb_dev_handle *handle;
                               printf("pickit2_isoc with Vendor Id: %x and Product Id: %x
found.\n", VENDOR_ID, PRODUCT_ID);
                               if (!(handle = usb_open(dev))) {
                                       printf("Could not open USB device\n");
                                       return NULL;
                               }

                               return handle;
                       }

               }
       }

       return NULL;
}

 void test_isochronous_async(usb_dev_handle *dev)
 {
  unsigned char buf0[640];
  unsigned char buf1[640];
  unsigned char buf2[640];

  int i;

  /* use three contexts for this example (more can be used) */
  void *context0 = NULL;
  void *context1 = NULL;
  void *context2 = NULL;

  /* write transfer (stream) */
/*
  usb_isochronous_setup_async(dev, &context0, 0x01);
  usb_isochronous_setup_async(dev, &context1, 0x01);
  usb_isochronous_setup_async(dev, &context2, 0x01);

  usb_submit_async(context0, buf0, sizeof(buf0));
  usb_submit_async(context1, buf1, sizeof(buf1));
  usb_submit_async(context2, buf2, sizeof(buf2));

  for(i = 0; i < 10; i++)
    {
      usb_reap_async(context0, 5000);
      usb_submit_async(context0, buf0, sizeof(buf0));

      usb_reap_async(context1, 5000);
      usb_submit_async(context1, buf1, sizeof(buf1));

      usb_reap_async(context2, 5000);
      usb_submit_async(context2, buf2, sizeof(buf2));
    }

  usb_reap_async(context0, 5000);
  usb_reap_async(context1, 5000);
  usb_reap_async(context2, 5000);

  usb_free_async(&context0);
  usb_free_async(&context1);
  usb_free_async(&context2);

*/

  /* read transfer (stream) */
  usb_isochronous_setup_async(dev, &context0, 0x81,64);
  usb_isochronous_setup_async(dev, &context1, 0x81,64);
  usb_isochronous_setup_async(dev, &context2, 0x81,64);

  usb_submit_async(context0, buf0, sizeof(buf0));
  usb_submit_async(context1, buf1, sizeof(buf1));
  usb_submit_async(context2, buf2, sizeof(buf2));

  for(i = 0; i < 10; i++)
    {
      usb_reap_async(context0, 5000);
      usb_submit_async(context0, buf0, sizeof(buf0));

      usb_reap_async(context1, 5000);
      usb_submit_async(context1, buf1, sizeof(buf1));

      usb_reap_async(context2, 5000);
      usb_submit_async(context2, buf2, sizeof(buf2));
    }

  for(i = 0; i < 640; i++) {
               printf(" %02x, %02x, %02x ", buf0[i],buf1[i],buf2[i]);
//              printf(" %02x, %02x, %02x ", buf0[i]);
               }

  usb_reap_async(context0, 5000);
  usb_reap_async(context1, 5000);
  usb_reap_async(context2, 5000);
  usb_free_async(&context0);
  usb_free_async(&context1);
  usb_free_async(&context2);

  /* release interface */
  usb_set_altinterface(dev, 0);
  usb_release_interface(dev, 0);
 }


int main(void)
{
       usb_dev_handle *pickit2_isoc;
       if ((pickit2_isoc = setup_libusb_access()) == NULL) {
               exit(-1);
       }
       test_isochronous_async(pickit2_isoc);
       usb_close(pickit2_isoc);

       return 0;
}
