#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <errno.h>

int get_adc_value(int ch)
{
  int file;								
  int addr = 0x48;
  char *filename = "/dev/i2c-3";		// i2c bus 3 for XU4
  char buffer[2] = {0};					// buffer for incoming data
  int value;							// ADC value after decode
  value = 0;
  
  if ((file = open(filename, O_RDWR)) < 0) {
      perror("Opening i2c device node.\n");
      exit(1);
    }
    
  if (ioctl(file, I2C_SLAVE, addr) < 0) {
      perror("Failed to aquire bus access and/or talk to slave. \n");
      exit(1);
  }
  
  // choose MSB according to what channel to read.  Used in setup.
  char MSB[4] = {
    0b01000100,       // ch 0
    0b01010100,       // ch 1
    0b01100100,       // ch 2
    0b01110100};      // ch 3
  
  char setup[3] = {
    0b00000001,       // config address
    MSB[ch],          // MSB config bits
    0b10000011};      // LSB config bits
	
	// bit 15 		single shot conversion start	:no effect
	// bit 14-12	input multiplexer				:see above
	// bit 11-9		full scale configuration		:2.048V
	// bit 8		single shot mode				:off
	// bit 7-5		data rate						:1600 SPS
	// bit 4		comparator mode					:traditional w/ hysteresis
	// bit 3		comparator polarity				:active low
	// bit 2		latching comparator				:off
	// bit 1-0		comparator queue and disable	:disable
	
   
  char setread[1] = {
    0b00000000}       // output register
   
	// write 3 bytes to ADC for set up including ch.
	// First byte, address and low write bit implied by write command.
  if (write(file, setup, 3) != 3) {
    perror("Error writing setup. \n");
    exit(1);
  }
  	// write 1 byte to ADC pointing to conversion register.
	// First byte, address and low write bit implied by write command.
  if (write(file, setread, 1) != 1) {
    perror("Error writing setread. \n");
    exit(1);
  }
    // Read from Conversion register.
	// First byte, address and high read bit implied by read command.
  if (read(file, buffer, 2) != 2) {
    perror("Error reading to buffer");
    exit(1);
  }
  
  close(file);
   // Shift bits in buffer to correct position.
  value = ((buffer[0] << 8) | buffer[1]) >> 4;
  
  return(value);
}



// int main()
// {

  // int ain0;
  // int ain1;
  // while(1)
  // {
      // ain0 = ADC(0);
      // usleep(650);
      // ain1 = ADC(1);
      // printf("\r%d\t%d",ain0,ain1);
      // usleep(100000);
  // }
  // return 0;
// }
