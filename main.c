//EECE 287, Spring 2023 Project 2
//Instructor: Meg Jain
// Group # - 3
//Team Members - Owen Bills and Karli DOig

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

//pin locations
#define RIGHT_SENSOR_PIN 4 //PD4
#define LEFT_SENSOR_PIN 5 //PF5
#define FRONT_SENSOR_PIN 1 //PF1
#define R_MOTOR_DIRECTION_LOCATION 1 //PB1
#define L_MOTOR_DIRECTION_LOCATION 2 //PB2
#define R_MOTOR_LOCATION 5 //PB5
#define L_MOTOR_LOCATION 6 //PB6

#define CLEAR_MASK 0b00000000000000000000000011111111


/*
The following operates the proximity sensors by transmitting pulses on both 
the left and right LEDs at six different brightness levels. 
For each sensor, it generates two numbers: the number of brightness levels for the left LEDs that activated the sensor, 
and the number of brightness levels for the right LEDs that activated the sensor. 
A higher reading corresponds to more IR light getting reflected to the sensor, 
which is influenced by the size, reflectivity, proximity, and location of nearby objects. 
However, the presence of other sources of 38 kHz IR pulses (e.g. from another robot) can also affect the readings.
*/


uint16_t BrightnessLevels[] = {4, 15, 32, 55, 85, 120}; //think - duty cyles

//right sensor
void configure_right_proximity_sensor()
{
	DDRD &= ~(1<<RIGHT_SENSOR_PIN);
	PORTD |= (1<<RIGHT_SENSOR_PIN);
}


//higher return value indicates you are closer to an object
uint8_t read_right_proximity_sensor() 
{
	//turn off the line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	uint8_t right_sensor_detection_factor = 0;
	
	for (int i = 0; i < 6; i++){ //6 is the number of brightness levels in the array

		uint16_t brightness_duty_cycle = BrightnessLevels[i];
		
		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);
		
		if(!(PIND & (1<<RIGHT_SENSOR_PIN)))
			right_sensor_detection_factor++;
		
		stop_IR_emitters();
		_delay_us(550);
		
		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);
		
		if(!(PIND & (1<<RIGHT_SENSOR_PIN)))
			right_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);
		
	}
	return right_sensor_detection_factor;
}


void configure_proximity_sensors()
{
	void configure_right_proximity_sensor();
	void configure_left_proximity_sensor();
	void configure_front_proximity_sensor();
}


//left sensor
void configure_left_proximity_sensor()
{
	DDRF &= ~(1<<LEFT_SENSOR_PIN);
	PORTF |= (1<<LEFT_SENSOR_PIN);
}

uint8_t read_left_proximity_sensor() 
{
	//turn off line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	uint8_t left_sensor_detection_factor = 0;
	
	for (int i = 0; i < 6; i++){ //6 is the number of brightness levels in the array

		uint16_t brightness_duty_cycle = BrightnessLevels[i];
		
		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);
		
		if(!(PINF & (1<<LEFT_SENSOR_PIN)))
			left_sensor_detection_factor++;
		
		stop_IR_emitters();
		_delay_us(550);
		
		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);
		
		if(!(PINF & (1<<LEFT_SENSOR_PIN)))
			left_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);
		
	}
	return left_sensor_detection_factor;
}

//front sensor
void configure_front_proximity_sensor()
{
	DDRF &= ~(1<<FRONT_SENSOR_PIN);
	PORTF |= (1<<FRONT_SENSOR_PIN);
}

uint8_t read_front_proximity_sensor()
{
	//turn off line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	uint8_t front_sensor_detection_factor = 0;
	
	for (int i = 0; i < 6; i++){ //6 is the number of brightness levels in the array

		uint16_t brightness_duty_cycle = BrightnessLevels[i];
		
		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);
		
		if(!(PINF & (1<<FRONT_SENSOR_PIN)))
			front_sensor_detection_factor++;
		
		stop_IR_emitters();
		_delay_us(550);
		
		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);
		
		if(!(PINF & (1<<FRONT_SENSOR_PIN)))
			front_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);
		
	}
	return front_sensor_detection_factor;
}

void configure_R_motor()
{DDRB |= ~(1<< R_MOTOR_LOCATION);
//PORTB |= (1<< R_MOTOR_LOCATION);
}

void configure_R_motor_direction()
{DDRB |= ~(1<< R_MOTOR_DIRECTION_LOCATION);
//PORTB |= (1<< R_MOTOR_DIRECTION_LOCATION);
}

void configure_L_motor()
{DDRB |= ~(1<< L_MOTOR_LOCATION);
//PORTB |= (1<< L_MOTOR_LOCATION);
}

void configure_L_motor_direction()
{DDRB |= ~(1<< L_MOTOR_DIRECTION_LOCATION);
//PORTB |= (1<< L_MOTOR_DIRECTION_LOCATION);
}

void turn_motors_forward()
{PORTB &= ~(1<< L_MOTOR_DIRECTION_LOCATION);
PORTB &= ~(1<< R_MOTOR_DIRECTION_LOCATION);}

void turn_motors_backward()
{PORTB |= (1<< L_MOTOR_DIRECTION_LOCATION);
PORTB |= (1<< R_MOTOR_DIRECTION_LOCATION);}

void turn_on_motors()
{OCR1A = 49;
OCR1B = 50;}

void turn_off_motors()
{OCR1A = 0;
OCR1B = 0;}

void right_turn()
{PORTB &= ~(1<< L_MOTOR_DIRECTION_LOCATION);
PORTB |= (1<< R_MOTOR_DIRECTION_LOCATION);}

void left_turn()
{PORTB |= (1<< L_MOTOR_DIRECTION_LOCATION);
PORTB &= ~(1<< R_MOTOR_DIRECTION_LOCATION);}



int main(){
	
	configure_zumo();
	configure_proximity_sensors();
	configure_R_motor();
	configure_R_motor_direction();
	configure_L_motor();
	configure_L_motor_direction();
	
	turn_motors_forward();
	turn_off_motors();
	
	oled_clear();
	oled_set_cursor(0,0);

	

	//proximity
	uint8_t right_proximity_value = 0;
	uint8_t left_proximity_value = 0;
	uint8_t front_proximity_value = 0;
	uint8_t ir_command = 0;
	uint8_t turn_command = 0;
	uint8_t objects_found = 0;
	uint8_t objects_remaining = 0;	
	int object = 0;
	uint32_t start = 0;
	uint32_t stop = 0;
	int update = 0;
	int turn = 0;
	uint32_t start_turn = 0;
	uint32_t right_quadrature = 0;
	uint32_t victory_start = 0;
	uint32_t victory_stop = 0;

	
	
	// to input number of objects to avoid using ir remote
	while(ir_command != 0x20)
	{
		ir_command = wait_for_remote_command();
				
		ir_command &= CLEAR_MASK;
		
		oled_set_cursor(3,0);
		
		if(ir_command == 0x50)
		{oled_put_hex(0x5);
		objects_remaining=5;}
		else if(ir_command == 0xD1)
		{oled_put_hex(0x6);
		objects_remaining=6;}
		else if(ir_command == 0x31)
		{oled_put_hex(0x7);
		objects_remaining=7;}
		else if(ir_command == 0x30)
		{oled_put_hex(0x8);
		objects_remaining=8;}
	}

	oled_set_cursor(3,6);
	oled_put_hex(objects_remaining);
		
	oled_set_cursor(3,12);
	oled_put_hex(objects_found);
	
	turn_on_motors();

	while(objects_remaining != 0){
				
		//proximity sensors
		oled_set_cursor(0,0);
		left_proximity_value = read_left_proximity_sensor();
		oled_put_hex(left_proximity_value); //use the values you see to threshold detection of an object	
		
		oled_set_cursor(0,6);
		front_proximity_value = read_front_proximity_sensor();
		oled_put_hex(front_proximity_value); //use the values you see to threshold detection of an object
		
		oled_set_cursor(0,12);
		right_proximity_value = read_right_proximity_sensor();
		oled_put_hex(right_proximity_value); //use the values you see to threshold detection of an object

		right_quadrature = get_right_quadrature_counter();
		
		// when an object is detected
		if((((front_proximity_value >= 0xC) && (left_proximity_value >= 0x9)) || ((front_proximity_value >= 0xC) && (right_proximity_value >= 0x9))) && (object == 0) && (turn == 0))
		{
			//stop
			turn_off_motors();
			object = 1;
			start = get_right_quadrature_counter();
			stop = start + 500;

		}
		
		// movement after object is detected
		if(object == 1)
		{
			turn_motors_backward();
			
			oled_set_cursor(1,0);
			oled_put_hex(right_quadrature>>28);
			oled_put_hex(right_quadrature>>24);
			oled_put_hex(right_quadrature>>20);
			oled_put_hex(right_quadrature>>16);
			oled_put_hex(right_quadrature>>12);
			oled_put_hex(right_quadrature>>8);
			oled_put_hex(right_quadrature>>4);
			oled_put_hex(right_quadrature);
			
			if(right_quadrature < stop)
				turn_on_motors();
			else
			{
				turn_off_motors();
				turn_motors_forward();
				object = 0;
				start = 0;
				update = 1;
				turn = 1;
				start_turn = get_right_quadrature_counter();			
			}
		}
		
		if(update == 1)
		{
			objects_found++;
			objects_remaining--;
	
			oled_set_cursor(3,6);
			oled_put_hex(objects_remaining);
		
			oled_set_cursor(3,12);
			oled_put_hex(objects_found);
			
			update = 0;
			
			if(objects_remaining == 0)
				break;
			
			while((turn_command != 0x21) && (turn_command != 0xA1))
					turn_command = wait_for_remote_command();
				
			turn_command &= CLEAR_MASK;
			
		}
		


		if(turn_command == 0xA1)
		{		
				
			if(right_quadrature < start_turn + 610)
			{
				right_turn();
				turn_on_motors();
			}
			else
			{
				turn_off_motors();
				right_turn();
				turn_motors_forward();
				turn = 0;
				turn_command = 0;
				start_turn = 0;
				turn_on_motors();
			}
		}
		
		if(turn_command == 0x21)
		{
				
			if(right_quadrature < start_turn + 610)
			{
				left_turn();
				turn_on_motors();
			}
			else
			{

				turn_off_motors();
				left_turn();
				turn_motors_forward();
				turn = 0;
				turn_command = 0;
				start_turn = 0;
				turn_on_motors();
			}
			
		}

	}
	turn_off_motors();
	
	//victory spins

	victory_start = get_right_quadrature_counter();
	
	victory_stop = victory_start + 5000;
	
	while(1)
	{
		right_quadrature = get_right_quadrature_counter();
		
		oled_set_cursor(1,0);
		oled_put_hex(right_quadrature>>28);
		oled_put_hex(right_quadrature>>24);
		oled_put_hex(right_quadrature>>20);
		oled_put_hex(right_quadrature>>16);
		oled_put_hex(right_quadrature>>12);
		oled_put_hex(right_quadrature>>8);
		oled_put_hex(right_quadrature>>4);
		oled_put_hex(right_quadrature);

		
		if(right_quadrature < victory_stop)
		{
			right_turn();
			turn_on_motors();
		}
		else
		{
			turn_off_motors();
			break;
		}
	}	
	
	return 0;
}
