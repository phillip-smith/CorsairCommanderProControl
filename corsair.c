#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <hidapi/hidapi.h>

#define MAX_STR 255

void write_generic(unsigned char* inbuf, int len){
    for (int i = 0; i < len; ++i)
        printf("%02hhX ", inbuf[i]);
    printf("\n");

}

void write_result(unsigned char* inbuf){ 
    printf("RECV: ");
    write_generic(inbuf, 16);
}
void write_out(unsigned char* inbuf){ 
    printf("SEND: ");
    write_generic(inbuf, 63);
}


#define handle_write() ({ \
        res = hid_write(handle, outbuf, 63); \
        res = hid_read(handle, inbuf, 16); \
        \
        write_out(outbuf); \
        write_result(inbuf); \
        memset(outbuf, 0, 63); \
    })




int main(int argc, char* argv[])
{
	int res, p;
	unsigned char outbuf[63];
    unsigned char inbuf[16];
    
    memset(outbuf, 0, 63);
    

	wchar_t wstr[MAX_STR];
	hid_device *handle;

	// Open the device using the VID, PID, and optionally the Serial number.
	handle = hid_open(0x1b1c, 0x0c10, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	printf("Serial Number String: %ls", wstr);
	printf("\n");


    // Get firmware version
    //outbuf[0] = 0x02;
    //handle_write();

    
    
    // Set LED strips on channel 2 to white
    outbuf[ 0] = 0x35;
    outbuf[ 1] = 0x01; //channel 2
    
    //offset, len
    //0/28 for all LED strips (channel 2/0x1)
    outbuf[ 2] = 0x00;
    outbuf[ 3] = 0x28;
    
    outbuf[ 4] = 0x04; //mode
    outbuf[ 5] = 0x00; //speed
    
    outbuf[ 6] = 0x00;
    outbuf[ 7] = 0x00;
    
    outbuf[ 8] = 0xFF; //always 255
    
    //Color in RGB
    outbuf[ 9] = 0xFF;
    outbuf[10] = 0xFF;
    outbuf[11] = 0xFF;
    
    
    //We don't actually need to set these, as mode 4 only uses color value 1
    outbuf[12] = 0xFF;
    outbuf[13] = 0x00;
    outbuf[14] = 0xFF;
    
    outbuf[15] = 0xFF;
    outbuf[16] = 0x00;
    outbuf[17] = 0xFF;
    
    handle_write();
    
    
    //Set brightness for LED strips (channel 2) to 100%
    p = 0;
    outbuf[p++] = 0x39;
    outbuf[p++] = 0x01;
    outbuf[p++] = 0x64;
    handle_write();

    
    //Set brightness for fan LEDs (channel 1) to 0%
    p = 0;
    outbuf[p++] = 0x39;
    outbuf[p++] = 0x00;
    outbuf[p++] = 0x00;
    handle_write();
    
    
    //Commit (haven't proved this is actually necessary, but CL4 likes sending these, and when in Rome...)
    p = 0;
    outbuf[p++] = 0x33;
    outbuf[p++] = 0xFF;
    handle_write();
 
    
    return 0;
}
