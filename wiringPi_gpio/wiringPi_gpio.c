/* gpio - Pi gpio pins via /sys/etc */
/* see http://elinux.org/RPi_Low-level_peripherals */

/* Copyright Miller Puckette - BSD license */

#include "m_pd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

static t_class *wiringPi_gpio_class;

typedef struct _wiringPi_gpio
{
    t_object x_obj;
    t_outlet *x_out1;
    int x_gpio_pin, x_pin_mode, x_pin_val;
} t_wiringPi_gpio;

static void wiringPi_gpio_bang(t_wiringPi_gpio *x)
{
    int value;
    if (x->x_pin_mode == 0)	pd_error(x, "gpio in OUTPUT MODE");
    else {
    	value = digitalRead (x->x_gpio_pin);
   // 	post("value = %d", value);
    	outlet_float(x->x_out1, value);
    }
}

static void wiringPi_gpio_float(t_wiringPi_gpio *x, t_float f)
{
    int value;
    value = f;
    if (x->x_pin_mode == 1)	pd_error(x, "gpio in INPUT MODE");
    else {  
    	digitalWrite (x->x_gpio_pin, value) ;
    }
}

static void *wiringPi_gpio_new(t_floatarg f1, t_floatarg f2, t_floatarg f3) {
    t_wiringPi_gpio *x = (t_wiringPi_gpio *)pd_new(wiringPi_gpio_class);
    x->x_out1 = outlet_new(&x->x_obj, gensym("float"));
 
    x->x_pin_val = x->x_gpio_pin = x->x_pin_mode = 0;
    x->x_gpio_pin = f1; 
	x->x_pin_mode = f2;
	x->x_pin_val  = f3;
	
	post("pin:%d, pin:%d, pin:%d,", x->x_gpio_pin, x->x_pin_mode, x->x_pin_val); 
	
    if (geteuid () != 0){
    	pd_error(x, "you need to be root...");
    	exit (0) ;
  	}

  	if (wiringPiSetup () == -1)	exit (1) ;

	if (x->x_pin_mode == 0) {
    	pinMode (x->x_gpio_pin, OUTPUT) ;
    	digitalWrite (x->x_gpio_pin, x->x_pin_val) ;
    	post("pin %d output mode", x->x_gpio_pin);
    }
    else {
    	pinMode (x->x_gpio_pin, INPUT) ;
    	post("pin %d input mode", x->x_gpio_pin);
    }

    return (x);
}

void wiringPi_gpio_setup(void)

{
    wiringPi_gpio_class = class_new(gensym("wiringPi_gpio"), 
		(t_newmethod)wiringPi_gpio_new, 
		0, sizeof(t_wiringPi_gpio), 
		0, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
	class_addfloat(wiringPi_gpio_class, wiringPi_gpio_float);
    class_addbang(wiringPi_gpio_class, wiringPi_gpio_bang);
    post("wiringPi_gpio version 0.01");
}
