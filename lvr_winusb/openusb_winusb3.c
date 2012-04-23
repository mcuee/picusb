#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <openusb.h>

#define VENDOR_ID 0x0925
#define PRODUCT_ID 0x1456 
#define CLASS_CODE  0
#define SUBCLASS_CODE  0
const static int INT_DATA_LEN=2;
const static int BULK_DATA_LEN=64;
const static int INTERFACE=0;
const static int ENDPOINT_INT_IN=0x81; /* endpoint 0x81 address for IN */
const static int ENDPOINT_INT_OUT=0x01; /* endpoint 1 address for OUT */
const static int ENDPOINT_BULK_IN=0x82; /* endpoint 0x81 address for IN */
const static int ENDPOINT_BULK_OUT=0x02; /* endpoint 1 address for OUT */
const static int TIMEOUT=5000; /* timeout in ms */

/*
 void test_control_transfer(openusb_dev_handle_t hdev)
 {
 }
*/

 int test_interrupt_transfer(openusb_dev_handle_t devh)
 {
	char intrdata[INT_DATA_LEN];
	char intrrd[INT_DATA_LEN];
	int i,ret;
	openusb_intr_request_t intr;

	printf("Test Interrupt transfer:\n");

	memset(&intr, 0, sizeof(intr));
	memset(intrrd, 0, INT_DATA_LEN);

	for(i = 0; i< INT_DATA_LEN; i++) {
		intrdata[i] = i;
	}

	intr.payload = intrdata;
	intr.length = INT_DATA_LEN;
	intr.timeout = TIMEOUT;

	/* Interrupt Write */
	ret = openusb_intr_xfer(devh, INTERFACE, ENDPOINT_INT_OUT, &intr);

	if (ret != 0) {
		printf("intr sync xfer test write fail:%s\n", openusb_strerror(ret));
		return -1;
	}
	printf("intr sync xfer write result.status = %d,xfer_bytes=%d, ret=%d\n",
		intr.result.status, intr.result.transferred_bytes,ret);

	/* Interrupt READ */
	intr.payload = intrrd;
	intr.length = INT_DATA_LEN;
	intr.timeout = TIMEOUT;
	ret = openusb_intr_xfer(devh, INTERFACE, ENDPOINT_INT_IN, &intr);
	if (ret != 0) {
		printf("intr sync xfer test read fail:%s\n", openusb_strerror(ret));
		return -1;
	}
	printf("intr sync xfer read result.status = %d,xfer_bytes=%d, ret=%d\n",
		intr.result.status, intr.result.transferred_bytes,ret);

	printf("\nInterrupt DATA: write,readback\n");
	for(i = 0;i < INT_DATA_LEN; i++) {
		if(i%8 == 0)
			printf("\n");
		printf("%02x,%02x  ",(unsigned char)intrdata[i],(unsigned char)intrrd[i]);
	}

	printf("\n");

	/* this can be enhanced to check data integrity */

	printf("Interrupt SYNC xfer test: PASS\n");
	printf("\n");
	
	return 0;

 }


int test_bulk_transfer(openusb_dev_handle_t devh)
 {
	char bulkdata[BULK_DATA_LEN];
	char bulkrd[BULK_DATA_LEN];
	int i,ret;
	openusb_bulk_request_t bulk;

	printf("Test BULK transfer:\n");

	memset(&bulk, 0, sizeof(bulk));
	memset(bulkrd, 0, BULK_DATA_LEN);

	for(i = 0; i< BULK_DATA_LEN; i++) {
		bulkdata[i] = i;
	}

	bulk.payload = bulkdata;
	bulk.length = BULK_DATA_LEN;
	bulk.timeout = TIMEOUT;

	/* Write BULK */
	ret = openusb_bulk_xfer(devh, INTERFACE, ENDPOINT_BULK_OUT, &bulk);

	if (ret != 0) {
		printf("BULK sync xfer test write fail:%s\n", openusb_strerror(ret));
		return -1;
	}
	printf("bulk sync xfer write result.status = %d,xfer_bytes=%d, ret=%d\n",
		bulk.result.status, bulk.result.transferred_bytes,ret);

	/* READ BULK */
	bulk.payload = bulkrd;
	bulk.length = BULK_DATA_LEN;
	bulk.timeout = TIMEOUT;
	ret = openusb_bulk_xfer(devh, INTERFACE, ENDPOINT_BULK_IN, &bulk);
	if (ret != 0) {
		printf("bulk sync xfer test read fail:%s\n", openusb_strerror(ret));
		return -1;
	}
	printf("bulk sync xfer read result.status = %d,xfer_bytes=%d, ret=%d\n",
		bulk.result.status, bulk.result.transferred_bytes,ret);

	printf("\nBULK DATA: write,readback\n");
	for(i = 0;i < BULK_DATA_LEN; i++) {
		if(i%8 == 0)
			printf("\n");
		printf("%02x,%02x  ",(unsigned char)bulkdata[i],(unsigned char)bulkrd[i]);
	}

	printf("\n");

	/* this can be enhanced to check data integrity */

	printf("BULK SYNC xfer test: PASS\n");
	printf("\n");
	
	return 0;
 }


int main( int argc, char **argv)
{
	openusb_handle_t     libhandle;
	openusb_dev_handle_t lvr_winusb;

	openusb_busid_t    *bus = NULL;
	openusb_devid_t    *devids = NULL;
	openusb_dev_data_t *devdata;
	unsigned int devnum = 0;
	unsigned int busnum = 0;
	int i, j;
	int ret;

	/* openusb initialization */
	if (openusb_init(0, &libhandle) != OPENUSB_SUCCESS)
	{
		printf("OpenUSB initalization error.\n");
	}

	/* find all usb buses on system */
	openusb_get_busid_list(libhandle, &bus, &busnum);

	/* search device in all devices on system */
	for (j = 0; j < busnum; j++)
	{
		openusb_get_devids_by_bus(libhandle, bus[j], &devids, &devnum);
		for (i = 0; i < devnum; i++)
		{
			openusb_get_device_data(libhandle, devids[i], 0, &devdata);
			if (devdata->dev_desc.bDeviceSubClass == SUBCLASS_CODE &&
			    devdata->dev_desc.bDeviceClass == CLASS_CODE &&
			    devdata->dev_desc.idProduct == PRODUCT_ID &&
			    devdata->dev_desc.idVendor == VENDOR_ID)
			{
				openusb_free_device_data(devdata);
				break;
			}
			openusb_free_device_data(devdata);
		}
		if (i >= devnum)
		{
			openusb_free_devid_list(devids);
		}
		else
		{
			break;
		}
	}

	if (j >= busnum)
	{
		printf("Cannot find specified device.\n");
		exit(-1);
	}

	ret = openusb_open_device(libhandle, devids[i], USB_INIT_DEFAULT, &lvr_winusb);
	if (ret != OPENUSB_SUCCESS)
	{
		printf("Cannot open specified device.\n");
		printf("Error code %d.\n",ret);
		exit(-1);
	}
	printf("Specified USB device found.\n");

/*
	ret = openusb_set_configuration(lvr_winusb, 1);
	if ( ret != OPENUSB_SUCCESS)
	{
		printf("Cannot set configuration 1.\n");
		printf("Error code %d.\n",ret);
		exit(-1);
	}
	printf("Successfully set usb configuration 1.\n");
*/

	ret = openusb_claim_interface(lvr_winusb, INTERFACE, USB_INIT_DEFAULT);
	if ( ret != OPENUSB_SUCCESS)
	{
		printf("Cannot claim interface.\n");
		printf("Error code %d.\n",ret);
		exit(-1);
	}
	printf("Successfully claim the interface.\n");

	openusb_free_devid_list(devids);
	openusb_free_busid_list(bus);

//	test_control_transfer(lvr_winusb); //Not implemented yet
	test_interrupt_transfer(lvr_winusb); 
	test_bulk_transfer(lvr_winusb);
	/* release interfaces */
	openusb_release_interface(lvr_winusb, INTERFACE);
	openusb_reset(lvr_winusb);
	/* close device */
	openusb_close_device(lvr_winusb);
	/* deinitialize openusb */
	openusb_fini(libhandle);

	return 0;
}
