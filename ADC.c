#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int ADC(int ch)
{
  int file;
  int addr = 0x48;
  char filename[15];
  uint8_t buffer[2];
  int value;
  sprintf(filename, "/dev/i2c-%d", 3);
  file = open(filename, O_RDWR);
  if(file < 0)
    {
      perror("Opening i2c device node.\n");
      return(1);
    }
  ioctl(file, I2C_SLAVE, addr);
  uint8_t MSB[4] = {0b01000100, 0b01010100, 0b01100100, 0b01110100};
  
  uint8_t setup[4] = {
    0b10010000,       // chip address
    0b00000001,       // config address
    MSB[ch],          // MSB config bits
    0b10000011};      // LSB config bits
   
  uint8_t setread[3] = {
    0b10010000,       // chip address /w write bit
    0b00000000,       // output register
    0b10010001};      // chip address /w read bit
    
  if (write(file, setup, 4)) != 4) {
    /* ERROR HANDLING: i2c transaction failed */
  }
  if (write(file, setread, 3)) != 3) {
    /* ERROR HANDLING: i2c transaction failed */
  }
  if (read(file, buffer, 2)) != 2) {
    /* ERROR HANDLING: i2c transaction failed */
  
  close(file);
  value = ((buffer[0] << 4) | (buffer[1] >> 4));
  
  return(value);
}

void main()
{

  int ain0;
  int ain1;
  while(1)
  {
      ain0 = ADC(0);
      ain1 = ADC(1);
      printf("\r%d\t%d",ain0,ain1);
      usleep(1000);
  }
}
