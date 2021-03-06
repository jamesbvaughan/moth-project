/*
 * gps.c
 *
 *      Authors: Allen Edwards and James Vaughan
 */

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "log.h"
#include "gps.h"

const char* gps_dev = "/dev/ttyO4";
volatile int gps_fd;

// used by verify_nmea
void btoh(unsigned char c, char *str) {
	unsigned char x;
	x = c >> 4;
	str[0] = x + (x > 9? 'A'-10: '0');
	x = c & 0x0f;
	str[1] = x + ( x > 9? 'A' - 10 : '0');
	str[2] = '\0';
}

// used to verify the validity of NMEA sentences
int verify_nmea(char *string){
	if (string[0] != '$')
		return -1;
	int length = strlen(string);
	if (length > 90) {
		printf("ERROR: string  %s too long\n",string);
		return -1;
	}
	if (string[length-1]!= '\n') {
		printf("ERROR: no newline\n");
		return -1;
	}
	if (string[length-2] != '\r') {
		printf("ERROR: no return\n");
		return -1;
	}
	int p = 1; // start after the $, stop before the *
	char temp[3];
	char cs[3];
	cs[2] = '\0';
	cs[0] = string[length-4];
	cs[1] = string[length-3];
	temp[2]='\0';
	temp[1] = '\0';
	char c = '\0';
	while (string[p] != '*') {
		c = c ^ string[p++];
	}
	// compare the checksums
	btoh(c,temp);
	p = strcmp(cs,temp);
	if (p != 0) {
		printf("ERROR: CHECKSUM ERROR in %s, %s != %s\n",string, cs, temp);
		return -1;
	}

	return 0;
}

// initialize the gps
int gps_init() {
	char *dataSetting = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
	char *baudSetting = "$PMTK251,115200*1F\r\n";
	char *freqSetting = "$PMTK220,100*2F\r\n";
	gps_fd = open(gps_dev, O_RDWR);
	if (gps_fd < 0) {
		printf("ERROR: couldn't open gps file descriptor\n");
		return -1;
	}
	struct termios tio;

	// set machine UART to 9600 baud
	tcgetattr(gps_fd, &tio);
	cfsetspeed(&tio, B9600);
	cfmakeraw(&tio);
	tcsetattr(gps_fd, TCSANOW, &tio);
	// set gps to 115200 baud if it wasn't already
	write(gps_fd, baudSetting, strlen(baudSetting));

	// set machine UART to 115200 baud
	tcgetattr(gps_fd, &tio);
	cfsetspeed(&tio, B115200);
	cfmakeraw(&tio);
	tcsetattr(gps_fd, TCSADRAIN, &tio);

	// tell gps to send the RMC only, and at 10hz
	write(gps_fd, dataSetting, strlen(dataSetting));
	write(gps_fd, freqSetting, strlen(freqSetting));
	usleep(500000);
	// do it again; it didn't work a few times.
	write(gps_fd, dataSetting, strlen(dataSetting));
	write(gps_fd, freqSetting, strlen(freqSetting));

	printf("STATUS: initialized gps\n");
	return 0;
}

// read from the gps forever
void* gps_begin() {
	printf("STATUS: GPS read is running\n");
	gps_init();
	char aa;
	char local_buffer[500];
	int idx = 0;
	tcflush(gps_fd, TCIFLUSH);
	int gps_read_more = 0;

	while (1) {
		while (read(gps_fd, &aa, 1) == -1 || (aa != '$' && gps_read_more != 1));   // read 1 character from stream (blocking call)
		gps_read_more = 1;
		if (aa != '\n') {
			local_buffer[idx++] = aa;
			if (idx > 400) {
				// prevent segmentation faults
				idx = 0;
				printf("\nERROR: ***********************************************\nlocal_buffer overflow in read nmea.\n Connect inputs and Cycle power to recover\n*****************************************\n");
				int flag = 1;
				char old = 'x';
				while (flag == 1) {
					read(gps_fd, &aa, 1);
					if (aa == '\n'  && old == '\r') flag = 0;
					old = aa;
				}
			}
		} else {
			// at end of sentence
			gps_read_more = 0;
			local_buffer[idx] = '\n';
			local_buffer[idx + 1] = '\0';
			idx = 0;
			if (verify_nmea(local_buffer) == 0) {
				if (strstr(local_buffer, "RMC")) {
					parse_rmc(local_buffer);
					printf("GPS: %s", local_buffer);
				}
			}
		}
	}
}
