/************************************/
/*									*/
/*	file:       temperature.h		*/
/*	author: 	Nicola Vianello		*/
/*	last edit:	Venice 21/02/2019 	*/
/*									*/
/************************************/

#define ADC_BIT	8				//ADC bits number
#define ADC_REFERENCE	5.1F	//ADC voltage reference

#define SENSOR_GAIN         0.01F   //sensor V/Celsius
#define SENSOR_OFFSET       0.0F    //sensor output voltage when temperature is zero Celsius degree
#define CONDITIONING_GAIN   10.0F	//gain of conditioning circuit

#define TEMPERATURE_GAIN	(SENSOR_GAIN*CONDITIONING_GAIN)		//overall V/Celsius
#define TEMPERATURE_OFFSET	(SENSOR_OFFSET*CONDITIONING_GAIN)	//conditioning circuit output voltage when temperature is zero Celsius degree

#define ADC_GAP	(ADC_REFERENCE/((float)(1U<<ADC_BIT)))		//voltage of an ADC gap

//return temperature in degree Celsius having a value of ADC conversion
float temperature_get_celsius(unsigned conversion);
