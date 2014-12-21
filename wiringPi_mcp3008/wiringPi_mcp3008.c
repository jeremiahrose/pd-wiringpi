/* gpio - Pi gpio pins via /sys/etc */
/* see http://elinux.org/RPi_Low-level_peripherals */

/* Copyright Miller Puckette - BSD license */

#include "m_pd.h"
#include <wiringPi.h>
#include <mcp3004.h>
#include <stdio.h>

static t_class *wiringPi_mcp3008_class;

typedef struct _wiringPi_mcp3008 {
    t_object x_obj;
    t_outlet *x_out1;
    int x_adc_pin;
    int x_spi_pin;
} t_wiringPi_mcp3008;

static void wiringPi_mcp3008_bang(t_wiringPi_mcp3008 *x) {
    int value, i, shift;
    t_atom readings[x->x_adc_pin];
    shift = x->x_spi_pin*8;
    for (i=0; i<x->x_adc_pin; i++) {
		value = analogRead(i+100+shift);
		SETFLOAT(&readings[i], value);
    }
    
    outlet_list(x->x_out1, 0, x->x_adc_pin, readings);
//    outlet_float(x->x_out1, value);
}

static void *wiringPi_mcp3008_new(t_floatarg f, t_floatarg g)
{
    t_wiringPi_mcp3008 *x = (t_wiringPi_mcp3008 *)pd_new(wiringPi_mcp3008_class);
    x->x_out1 = outlet_new(&x->x_obj, gensym("list"));
    post("Using SPI port %f and reading %f ADCs", f, g);
    x->x_spi_pin = (int) f;
    x->x_adc_pin = (int) g;
    return (x);
}

void wiringPi_mcp3008_setup_wiringPi(t_wiringPi_mcp3008 *x)
{
    wiringPiSetup();
    int pin_n;
    pin_n = 100 + x->x_spi_pin * 8;
    mcp3004Setup (pin_n, x->x_spi_pin);
    post("mcp_3004/8 is ready to read!");
}


void wiringPi_mcp3008_setup(void)
{
    wiringPi_mcp3008_class = class_new(gensym("wiringPi_mcp3008"), 
		(t_newmethod)wiringPi_mcp3008_new, 
		0, sizeof(t_wiringPi_mcp3008), 
		0, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addbang(wiringPi_mcp3008_class, wiringPi_mcp3008_bang);
    class_addmethod(wiringPi_mcp3008_class, (t_method)wiringPi_mcp3008_setup_wiringPi, gensym("setup_wiringPi"), 0);
    post("wiringPi_mcp3008 version 0.01");
}
