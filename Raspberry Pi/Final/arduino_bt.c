#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include<stdlib.h>

int main() {
	char buff[128];
	int chars;

    char *portname = "/dev/ttyACM0";

	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		printf("Error al open\n");
		return 0;
	}


	chars = read(0, buff, 128);
	if(chars > 0)
     write(fd, &buff, chars);
	
	sleep(2); //required to make flush work, for some reason
  	tcflush(fd,TCIOFLUSH);
	sleep(5);

	chars = read(fd, buff, 128);
	while(chars <= 0) chars = read(fd, buff, 128);
	write(1, &buff, chars);
}

