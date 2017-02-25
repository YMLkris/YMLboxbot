#include <SparkFun_TB6612.h>

// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = -1;
const int offsetB = -1;

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 5
#define PWMA 10
#define PWMB 6
#define STBY 9

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Pins
const int TRIG_PIN = 11;
const int ECHO_PIN = 12;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void setup() {

  // Setup all of our pins
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(2, OUTPUT); digitalWrite(2, LOW);
  pinMode(4, OUTPUT); digitalWrite(4, LOW);
  pinMode(5, OUTPUT); digitalWrite(5, LOW);
  pinMode(7, OUTPUT); digitalWrite(7, LOW);
  pinMode(3, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(A4, OUTPUT);
  digitalWrite(A4, HIGH);

}

void loop() {

  // Start moving forward
  forward(motor1, motor2, 200);

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters.
  cm = pulse_width / 58.0;

  // If an obstacle is detected fewer than 20 centimeters away,
  // run the motors backwards and then coin flip to decide which
  // way to turn before continuing on.
  if(cm<20){back(motor1, motor2, 250); delay(1000);
  if(flip()){left(motor1, motor2, 250);}else{right(motor1, motor2, 250);}
  delay(2500);}

  // Wait at least 60ms before next measurement
  delay(60);
}

// Coinflip function that randomly returns a 1 or 0
bool flip(){

  static uint32_t buf = 0;
  static uint8_t idx = 0;
  if (idx)
  {
    buf >>= 1;
    idx--;
  }
  else
  {
    buf = random();  // refill
    idx = 30;
  }
  return buf & 0x01;

}

