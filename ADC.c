#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int ADC(int ch)
{
  int file;
  int addr = 0x48;
  char filename[15];
  char buffer[2];
  int value;
  sprintf(filename, "/dev/i2c-%d", 3);
  file = open(filename, O_RDWR);
  if(file < 0)
    {
      perror("Opening i2c device node.\n");
      return(1);
    }
  ioctl(file, I2C_SLAVE, addr);
  char MSB[4] = {0b01000100, 0b01010100, 0b01100100, 0b01110100};
  
  char setup[4] = {
    0b10010000,       // chip address
    0b00000001,       // config address
    MSB[ch],          // MSB config bits
    0b10000011};      // LSB config bits
  write(file, &setup, 4);
  
  usleep(10);
  read(file, buffer, 2);
  
  close(file);
  value = (buffer[0] | (buffer[1] << 8));
  
  return(value);
}

