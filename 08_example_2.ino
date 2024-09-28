#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  analogWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  Serial.begin(57600);
}

void loop() {
  float distance;
  
  if (millis() < (last_sampling_time + INTERVAL))
  return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO);
  
  float P;
  float Q;
  
  if (distance > 300){
    analogWrite(PIN_LED, 255); // LED OFF
  } else if (distance < 100){
    analogWrite(PIN_LED, 255); // LED OFF
  } else if (distance >= 200 && distance <= 300){
      P = distance - 200;
      analogWrite(PIN_LED, int((P/100) * 256));
  } else if (distance >= 100 && distance < 200){
      Q = distance - 100;
      analogWrite(PIN_LED, int((Q/100) * 256));
  }

  // output the distance to the serial port
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.println("");
  
  // do something here
  // delay(50); // Assume that it takes 50ms to do something.
  
  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}  
