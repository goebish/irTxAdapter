
//_______________________________________________________________________________________________

// User settings

// Arduino pin assignments
#define PPM_PIN 3 // must be 2 or 3 
#define IR_PIN 11 // must be 3 or 11

#define STATUS_LED 13
#define PS0 12 // protocol selection pin 0, active low (S107)
#define PS1 10 // protocol selection pin 1, active low (wltoys)
#define DYNAMIC_YAW_TRIM false // (experimental)
#define DEBUG false // enable serial output

// CPPM settings
#define CPPM  THROTTLE,ROLL,PITCH,YAW,AUX1,AUX2,AUX3,AUX4 //For JR/Graupner/Spektrum
//#define CPPM  ROLL,PITCH,THROTTLE,YAW,AUX1,AUX2,AUX3,AUX4 //For Robe/Hitec/Futaba
//#define CPPM  ROLL,PITCH,YAW,THROTTLE,AUX1,AUX2,AUX3,AUX4 //For Multiplex
//#define CPPM  PITCH,ROLL,THROTTLE,YAW,AUX1,AUX2,AUX3,AUX4 //For some Hitec/Sanwa/Others
#define PPM_MIN 1150
#define PPM_MAX 1850
#define MIDRC 1500

// set to true if using pots instead of CPPM
#define USE_ADC false

// pots & button pins
enum e_adc_pins {
	ADC_THROTTLE = 0, // A0 (stick on bottom = 0V)
	ADC_ROLL	 = 1, // A1 (stick on left = 0V)
	ADC_PITCH	 = 2, // A2 (stick on bottom = 0V)
	ADC_YAW		 = 3, // A3 (stick on left = 0V)
	ADC_AUX1	 = 4, // A4 Yaw trim (knob on left = 0V)
	ADC_BUTTON1	 = 4, // D4 calibration button (active low, internal pull up)
};

//_______________________________________________________________________________________________
// end of user settings

// Should not need changing
#define IR_CLOCK_RATE 38000L
#define RC_CHANS 8

enum e_rc { // must be in this order
	ROLL,
	PITCH,
	YAW,
	THROTTLE,
	AUX1,
	AUX2,
	AUX3,
	AUX4
};

enum e_protocol { 
	SH_602X,	  // San Huan 6020, 6023, 6025 series
	SYMA_S107_32, // 32 bits "genuine" S107 protocol
	WLTOYS,
};

#if !defined(__AVR_ATmega328P__) || F_CPU != 16000000
	#error "This code is for ATmega328P @ 16 MHz only"
#endif
