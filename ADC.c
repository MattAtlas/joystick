#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <errno.h>

int ADC(int ch)
{
  int file;
  int addr = 0x48;
  char *filename = "/dev/i2c-3";
  char buffer[2] = {0};
  int value;
  value = 0;
  
  if ((file = open(filename, O_RDWR)) < 0) {
      perror("Opening i2c device node.\n");
      exit(1);
    }
    
  if (ioctl(file, I2C_SLAVE, addr) < 0) {
      perror("Failed to aquire bus access and/or talk to slave. \n");
      exit(1);
  }
  
  char MSB[4] = {
    0b01000100,       // ch 0
    0b01010100,       // ch 1
    0b01100100,       // ch 2
    0b01110100};      // ch 3
  
  char setup[3] = {
    0b00000001,       // config address
    MSB[ch],          // MSB config bits
    0b10000011};      // LSB config bits
   
  char setread[1] = {
    0b00000000}       // output register
    
  if (write(file, setup, 3) != 3) {
    perror("Error writing setup. \n");
    exit(1);
  }
  if (write(file, setread, 1) != 1) {
    perror("Error writing setread. \n");
    exit(1);
  }
  if (read(file, buffer, 2) != 2) {
    perror("Error reading to buffer");
    exit(1);
  }
  
  close(file);
  value = ((buffer[0] << 8) | buffer[1]) >> 4;
  
  return(value);
}

int main()
{

  int ain0;
  int ain1;
  while(1)
  {
      ain0 = ADC(0);
      usleep(2000);
      ain1 = ADC(1);
      printf("\r%d\t%d",ain0,ain1);
      usleep(100000);
  }
  return 0;
}
