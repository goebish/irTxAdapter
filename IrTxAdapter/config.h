
//_______________________________________________________________________________________________

// User settings

// Arduino pin assignments
#define PPM_PIN 3 // must be 2 or 3 
#define IR_PIN 11 // must be 3 or 11

#define CPPM  THROTTLE,ROLL,PITCH,YAW,AUX1,AUX2,AUX3,AUX4 //For JR/Graupner/Spektrum
//#define CPPM  ROLL,PITCH,THROTTLE,YAW,AUX1,AUX2,AUX3,AUX4 //For Robe/Hitec/Futaba
//#define CPPM  ROLL,PITCH,YAW,THROTTLE,AUX1,AUX2,AUX3,AUX4 //For Multiplex
//#define CPPM  PITCH,ROLL,THROTTLE,YAW,AUX1,AUX2,AUX3,AUX4 //For some Hitec/Sanwa/Others

#define PPM_MIN 1150
#define PPM_MAX 1850
#define MIDRC 1500

#define DEBUG false

//_______________________________________________________________________________________________

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
	SH_602X,
};

#if (IR_PIN == 3) && (PPM_PIN == 3)
	#error IR_PIN and PPM_PIN cannot use the same pin
#endif
