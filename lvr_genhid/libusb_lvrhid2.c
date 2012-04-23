
#include <usb.h>
#include <stdio.h>

#define VERSION "0.1.0"
#define VENDOR_ID 0x0925
#define PRODUCT_ID 0x7001
#define INTERFACE 0
const static int reqIntLen=2;
const static int endpoint_Int_in=0x81; /* endpoint 0x81 address for IN */
const static int endpoint_Int_out=0x01; /* endpoint 1 address for OUT */

const static int timeout=5000; /* timeout in ms */

void bad(const char *why) {
	fprintf(stderr,"Fatal error> %s\n",why);
	exit(17);
}

usb_dev_handle *find_lvr_hid();

usb_dev_handle* setup_libusb_access() {
    usb_dev_handle *lvr_hid;
    int	retval;
    char dname[32] = {0};
    usb_set_debug(255);
    usb_init();
    usb_find_busses();
    usb_find_devices();
            
    if(!(lvr_hid = find_lvr_hid())) {
		printf("Couldn't find the USB device, Exiting\n");
		return NULL;
	}

	retval = usb_get_driver_np(lvr_hid, 0, dname, 31);
	if (!retval)
		usb_detach_kernel_driver_np(lvr_hid, 0);
	
	retval=usb_set_configuration(lvr_hid, 1);
	if ( retval < 0) {
		printf("Could not set configuration 1 : %d\n", retval);
		return NULL;
	}
	retval = retval=usb_claim_interface(lvr_hid, INTERFACE);
	if ( retval < 0) {
		printf("Could not claim interface: %d\n", retval);
		return NULL;
	}

	return lvr_hid;
}

usb_dev_handle *find_lvr_hid() 
{
    struct usb_bus *bus;
	struct usb_device *dev;

	for (bus = usb_busses; bus; bus = bus->next) {
    	for (dev = bus->devices; dev; dev = dev->next) {
   			if (dev->descriptor.idVendor == VENDOR_ID && 
		  		dev->descriptor.idProduct == PRODUCT_ID ) {
				usb_dev_handle *handle;
		  		printf("lvr_hid with Vendor Id: %x and Product Id: %x found.\n", VENDOR_ID, PRODUCT_ID);
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

/*
 void test_control_transfer(usb_dev_handle *dev)
 {
//   usb_set_altinterface(dev, 0);
   usb_release_interface(dev, 0);
 }
*/
 void test_interrupt_transfer(usb_dev_handle *dev)
 {
   int r,i;
   char answer[reqIntLen];
   char question[reqIntLen];
   for (i=0;i<reqIntLen; i++) question[i]=i;
   r = usb_interrupt_write(dev, endpoint_Int_out, question, reqIntLen, timeout);
   if( r < 0 )
   {
	  perror("USB interrupt write"); bad("USB write failed"); 
   }
   r = usb_interrupt_read(dev, endpoint_Int_in, answer, reqIntLen, timeout);
   if( r != reqIntLen )
   {
	  perror("USB interrupt read"); bad("USB read failed"); 
   }
   for (i=0;i<reqIntLen; i++) printf("%i, %i, \n",question[i],answer[i]);
//   usb_set_altinterface(dev, 0);
   usb_release_interface(dev, 0);
 }


int main( int argc, char **argv)
{
	usb_dev_handle *lvr_hid;
	if ((lvr_hid = setup_libusb_access()) == NULL) {
		exit(-1);
	} 
//	test_control_transfer(lvr_hid); //not implemented yet
	test_interrupt_transfer(lvr_hid);
	usb_close(lvr_hid);

	return 0;
}
