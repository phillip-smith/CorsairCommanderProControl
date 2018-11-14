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

int main(int argc, char* argv[])
{
	int res;
	unsigned char outbuf[63];
    unsigned char inbuf[16];
    
#define handle_write() ({ \
        res = hid_write(handle, outbuf, 63); \
        res = hid_read(handle, inbuf, 16); \
        \
        write_out(outbuf); \
        write_result(inbuf); \
        memset(outbuf, 0, 63); \
    })

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

    // Clear the outbuf array
    int i = 0;
    for (; i < 63; ++i)
        outbuf[i] = 0x00;

    // Construct a command
    outbuf[0] = 0x02;   // Get firmware version
    
    // Write 63 bytes, then read 16 bytes.
//     res = hid_write(handle, outbuf, 63);
//     res = hid_read(handle, inbuf, 16);
// 
//     // Print result
//     write_out(outbuf);
//     write_result(inbuf);
//handle_write();
    int p = 0;
    outbuf[ 0] = 0x35;
    outbuf[ 1] = 0x01;
    
    //offset, len
    //0/28 for all LED strips (channel 2/0x1)
    outbuf[ 2] = 0x00;
    outbuf[ 3] = 0x28;
    
    outbuf[ 4] = 0x04; //mode
    outbuf[ 5] = 0x00; //speed
    
    outbuf[ 6] = 0x00;
    outbuf[ 7] = 0x00;
    
    outbuf[ 8] = 0xFF;
    
    outbuf[ 9] = 0xFF;
    outbuf[10] = 0xFF;
    outbuf[11] = 0xFF;
    
    outbuf[12] = 0xFF;
    outbuf[13] = 0x00;
    outbuf[14] = 0xFF;
    
    outbuf[15] = 0xFF;
    outbuf[16] = 0x00;
    outbuf[17] = 0xFF;
    
    handle_write();
    
    
    //outbuf[0] = 0x23;   // Set fan to fixed %
    //outbuf[1] = 0x00;   // Select fan number 1
    //outbuf[2] = 0x64;   // Set speed to 100%
    //handle_write();
//     sleep(5);
// 
//     outbuf[0] = 0x23;   // Set fan to fixed %
//     outbuf[1] = 0x00;   // Select fan number 1
//     outbuf[2] = 0x1E;   // Set speed to 30%
//     res = hid_write(handle, outbuf, 63);
//     res = hid_read(handle, inbuf, 16);
//     write_result(inbuf);
    
    p = 0;
    outbuf[p++] = 0x39;
    outbuf[p++] = 0x01;
    outbuf[p++] = 0x64;
    handle_write();

    p = 0;
    outbuf[p++] = 0x39;
    outbuf[p++] = 0x00;
    outbuf[p++] = 0x00;
    handle_write();

    p = 0;
    outbuf[p++] = 0x33;
    outbuf[p++] = 0xFF;

    handle_write();


//     p = 0;
//     outbuf[p++] = 0x39;
//     outbuf[p++] = 0x00;
//     outbuf[p++] = 0x00;
// 
//     handle_write();
    
    return 0;
}
