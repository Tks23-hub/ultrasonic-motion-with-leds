// arduino motion sensor project 
// wrote by Mohamed Soliman

#define pinRed 2
#define trigPin  12
#define echoPin  11
long duration;
int distance;
bool isRedOn = false;
unsigned long exiting = 0;
const unsigned long exitDelay = 2000;
int cnt = 0;

enum State { Normal, IN, OUT };
State currentState = Normal;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pinRed, OUTPUT);
  Serial.begin(9600);
}

float maxLenInMicroSec = 100 * 2 / 0.034;

void loop() {
  sendTrig();
  duration = pulseIn(echoPin, HIGH, maxLenInMicroSec);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println(cnt);

  switch (currentState) {
    case Normal:
      if (distance >= 10 && distance <= 30 ) {
        exiting = millis();
        redOn();
        currentState = IN;
      }
      else if (distance < 10 || distance > 30) {
        redOff();
      }
      break;

    case IN:
      if (distance < 10 || distance > 30) {
        currentState = OUT;
      }
      break;

    case OUT:
      if (distance >= 10 && distance <= 30) {
        exiting = millis();
        redOn();
        currentState = IN;
      }
      else if (millis() - exiting >= exitDelay) {
        redOff();
        currentState = Normal;
      }
      break;
  }

  
}

void sendTrig() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

void redOn() {
  digitalWrite(pinRed, HIGH);
  if (digitalRead(pinRed) == HIGH) {
    cnt += 1;
  }
  isRedOn = true;
}

void redOff() {
  if (isRedOn && cnt >= 2) {
    digitalWrite(pinRed, LOW);
  }
  isRedOn = false;
}
