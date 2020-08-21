
const int ledPin = 10;
const int speakerPin = 9;
const int inputsoundPin = A0;
const int switchPin = 2;
bool toggle = LOW;
bool last_toggle = LOW;


void setup() {
 pinMode(switchPin, INPUT);
 pinMode(inputsoundPin, INPUT);
 pinMode(ledPin, OUTPUT);
 pinMode(speakerPin, OUTPUT);
 Serial.begin(9600);

}

void loop() {
 int music = analogRead(inputsoundPin);
 music = map(music, 0, 1023, 0, 255); // scale from 1023 to 255
 toggle = digitalRead(switchPin);
 Serial.println(music);
 //Serial.println(toggle);
 if(toggle != last_toggle); {
  last_toggle = toggle;
  //Serial.print("Input: ");
  tone(speakerPin, music);
  digitalWrite(ledPin, LOW);
  delay(music / 4);
  digitalWrite(ledPin, HIGH);
 }
  delay(20); 
}
