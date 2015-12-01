#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int setupADC()
{
  int file;
  int addr = 0x48;
  char filename[15];
  
  sprintf(filename, "/dev/i2c-%d", 3);
  file = open(filename, O_RDWR);
  if(file < 0)
    {
      perror("Opening i2c device node.\n");
      return(1);
    }
  ioctl(file, I2C_SLAVE, addr);
  
  char setup[4] = {
    0b10010000,       // chip address
    0b00000001,       // config address
    0b00000100,       // MSB config bits
    0b10000011};      // LSB config bits
  write(file, &setup, 4);
  close(file);
  return(0);
}
