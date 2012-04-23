#include <usb.h>
#include <stdio.h>

#define VERSION "0.1.0"
#define VENDOR_ID 0x0925
#define PRODUCT_ID 0x1456 
#define INTERFACE 0
#define WINUSB_REQUEST_1     0x01
#define WINUSB_REQUEST_2     0x02
#define LIBUSB_REQUEST_TYPE_VENDOR (0x02 << 5)
#define	LIBUSB_RECIPIENT_INTERFACE 0x01
#define CTRL_IN		(LIBUSB_REQUEST_TYPE_VENDOR | USB_ENDPOINT_IN | LIBUSB_RECIPIENT_INTERFACE)
#define CTRL_OUT	(LIBUSB_REQUEST_TYPE_VENDOR | USB_ENDPOINT_OUT | LIBUSB_RECIPIENT_INTERFACE)
const static int reqCtlLen=2;
const static int reqIntLen=2;
const static int reqBulkLen=64;
const static int endpoint_Int_in=0x82; /* endpoint 0x81 address for IN */
const static int endpoint_Int_out=0x02; /* endpoint 1 address for OUT */
const static int endpoint_Bulk_in=0x81; /* endpoint 0x81 address for IN */
const static int endpoint_Bulk_out=0x01; /* endpoint 1 address for OUT */
const static int timeout=5000; /* timeout in ms */

void bad(const char *why) {
	fprintf(stderr,"Fatal error> %s\n",why);
	exit(17);
}

usb_dev_handle *find_lvr_winusb();

usb_dev_handle* setup_libusb_access() {
    usb_dev_handle *lvr_winusb;
    usb_set_debug(255);
    usb_init();
    usb_find_busses();
    usb_find_devices();
            
    if(!(lvr_winusb = find_lvr_winusb())) {
		printf("Couldn't find the USB device, Exiting\n");
		return NULL;
	}
	if (usb_set_configuration(lvr_winusb, 1) < 0) {
		printf("Could not set configuration 1 : \n");
		return NULL;
	}
	if (usb_claim_interface(lvr_winusb, INTERFACE) < 0) {
		printf("Could not claim interface: \n");
		return NULL;
	}
	return lvr_winusb;
}

usb_dev_handle *find_lvr_winusb() 
{
    struct usb_bus *bus;
	struct usb_device *dev;

	for (bus = usb_busses; bus; bus = bus->next) {
    	for (dev = bus->devices; dev; dev = dev->next) {
   			if (dev->descriptor.idVendor == VENDOR_ID && 
		  		dev->descriptor.idProduct == PRODUCT_ID ) {
				usb_dev_handle *handle;
		  		printf("lvr_winusb with Vendor Id: %x and Product Id: %x found.\n", VENDOR_ID, PRODUCT_ID);
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

static int test_control_transfer(usb_dev_handle *dev)
{
   	int r,i;
   	char answer[reqCtlLen];
   	char question[reqCtlLen];
   	for (i=0;i<reqCtlLen; i++) question[i]=0x20+i;

	r = usb_control_msg(dev,CTRL_OUT,WINUSB_REQUEST_1,2, 0,question, reqCtlLen,timeout);
	if (r < 0) {
		fprintf(stderr, "Control Out error %d\n", r);
		return r;
	}
	r = usb_control_msg(dev,CTRL_IN,WINUSB_REQUEST_2,2,0, answer,reqCtlLen,timeout);
	if (r < 0) {
		fprintf(stderr, "Control IN error %d\n", r);
		return r;
	}
	for(i = 0;i < reqCtlLen; i++) {
		if(i%8 == 0)
			printf("\n");
		printf("%02x, %02x; ",question[i],answer[i]);
	}
	printf("\n");

	return 0;
}

 void test_interrupt_transfer(usb_dev_handle *dev)
 {
   int r,i;
   char answer[reqIntLen];
   char question[reqIntLen];
   for (i=0;i<reqIntLen; i++) question[i]=0x40+i;
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
//   for (i=0;i<reqIntLen; i++) printf("%i, %i, \n",question[i],answer[i]);
	for(i = 0;i < reqIntLen; i++) {
		if(i%8 == 0)
			printf("\n");
		printf("%02x, %02x; ",question[i],answer[i]);
	}
	printf("\n");

 }

 void test_bulk_transfer(usb_dev_handle *dev)
 {
   int r,i;
   char answer[reqBulkLen];
   char question[reqBulkLen];
   for (i=0;i<reqBulkLen; i++) question[i]=i;
   r = usb_bulk_write(dev, endpoint_Bulk_out, question, reqBulkLen, timeout);
   if( r < 0 )
   {
	  perror("USB bulk write"); bad("USB write failed"); 
   }
   r = usb_bulk_read(dev, endpoint_Bulk_in, answer, reqBulkLen, timeout);
   if( r != reqBulkLen )
   {
	  perror("USB bulk read"); bad("USB read failed"); 
   }
//   for (i=0;i<reqBulkLen;i++) printf("%i, %i, \n",question[i],answer[i]);
	for(i = 0;i < reqBulkLen; i++) {
		if(i%8 == 0)
			printf("\n");
		printf("%02x, %02x; ",question[i],answer[i]);
	}
	printf("\n");
 }

int main( int argc, char **argv)
{
	usb_dev_handle *lvr_winusb;
	if ((lvr_winusb = setup_libusb_access()) == NULL) {
		exit(-1);
	} 

	printf("Testing control transfer: ");
	test_control_transfer(lvr_winusb);
	printf("Testing interrupt transfer: ");
	test_interrupt_transfer(lvr_winusb);
	printf("Testing bulk transfer: ");
	test_bulk_transfer(lvr_winusb);


	usb_release_interface(lvr_winusb, 0);
	usb_reset(lvr_winusb);
	usb_close(lvr_winusb);

	return 0;
}
