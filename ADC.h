#ifndef ADC_H_
#define ADC_H_

typedef struct Joystick{
	int value[4];
	int min[4];
	int mid[4];
	int max[4];
}Joystick;

int get_adc_value(int ch);

int get_all_values(Joystick *J);

int initialize_joystick(Joystick *J);

#endif
