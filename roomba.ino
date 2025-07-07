#define DEVICE_DETECT_PIN 13 // pin to turn rooomba on
#define NUM_PINS 10
#define NUM_SENSOR_PINS 8
#define NUM_MOVEMENT_PINS 2

const int LIGHT_SENSOR_PINS[NUM_SENSOR_PINS] = { 2, 3, 12, 5, 6, 7, 8, 9 };
const int MOVEMENT_PINS[NUM_MOVEMENT_PINS] = { 10, 11 };

void driveLeftWheel(int16_t velocity);
void driveRightWheel(int16_t velocity);
void driveBothWheels(int16_t velocityLeft, int16_t velocityRight);
void powerOff();
void r2d2Sound();


void roomba_setup(){
  // Set up the device detect pin as output
  pinMode(DEVICE_DETECT_PIN, OUTPUT);

  // Initialize Serial1 (or appropriate serial port for your hardware) at 57600 baud
  Serial.begin(57600);

  // Toggle the device detect pin
  digitalWrite(DEVICE_DETECT_PIN, LOW);
  delay(100); // 0.1 second delay
  digitalWrite(DEVICE_DETECT_PIN, HIGH);

  // Wait for 2 seconds
  delay(2000);

  // Perform the toggling 3 times
  for (int i = 0; i < 3; i++) {
    digitalWrite(DEVICE_DETECT_PIN, LOW);
    delay(100); // 0.1 second delay
    digitalWrite(DEVICE_DETECT_PIN, HIGH);
    delay(100); // 0.1 second delay
  }

  // Reinitialize Serial1 at 19200 baud
  Serial.begin(19200);

  // Initialize Roomba Communication
  Serial.write(128);
  delay(1000); 
  Serial.write(130);
  delay(200);
  Serial.write(132);
  delay(200);
}

void setup() {
  delay(1000); // 10 second delay

  roomba_setup();
  // driveBothWheels(60,60);
  // delay(500); // 5 second delay
  BB8Sound1();
  BB8Sound2();
  // delay(5000); // 5 second delay
  // powerOff();


}

void driveLeftWheel(int16_t velocity) {
  // Drive left wheel by setting the right wheel's velocity to 0
  Serial.write(145); // Drive Direct command opcode
  Serial.write((velocity >> 8) & 0xFF); // High byte for left wheel velocity
  Serial.write(velocity & 0xFF);        // Low byte for left wheel velocity
  Serial.write(0x00);                  // High byte for right wheel velocity (0 mm/s)
  Serial.write(0x00);                  // Low byte for right wheel velocity (0 mm/s)
}

void driveRightWheel(int16_t velocity) {
  // Drive right wheel by setting the left wheel's velocity to 0
  Serial.write(145); // Drive Direct command opcode
  Serial.write(0x00);                  // High byte for left wheel velocity (0 mm/s)
  Serial.write(0x00);                  // Low byte for left wheel velocity (0 mm/s)
  Serial.write((velocity >> 8) & 0xFF); // High byte for right wheel velocity
  Serial.write(velocity & 0xFF);        // Low byte for right wheel velocity
}

void driveBothWheels(int16_t velocityLeft, int16_t velocityRight) {
  // Drive both wheels with specified velocities
  Serial.write(145); // Drive Direct command opcode
  Serial.write((velocityLeft >> 8) & 0xFF);  // High byte for left wheel velocity
  Serial.write(velocityLeft & 0xFF);         // Low byte for left wheel velocity
  Serial.write((velocityRight >> 8) & 0xFF); // High byte for right wheel velocity
  Serial.write(velocityRight & 0xFF);        // Low byte for right wheel velocity
}

void powerOff() {
  Serial.write(133); // Drive Direct command opcode
}

void rest(int time) {
  Serial.write(27); //not registered, thus a pause
  Serial.write(time);
}


void BB8Sound1() {
  Serial.write(140);  //opcode
  // #of data types is (2N+2), where N is the # of notes in the song.
  Serial.write(0);  //song number 0 (1)
  Serial.write(16); //max notes (2) 
  Serial.write(75); //D#4 (3) - note (1)
  Serial.write(16); //0.25 (4) - duration
  rest(8); //0.125 (5,6) - note (2), and duration
  Serial.write(80); //G#5 (7) - note(3)
  Serial.write(4); //0.0625s (8) - duration
  Serial.write(58); //A#3 (9) - note(4) //this should be together with the G#5, however roomba cant play both sounds at the same time
  Serial.write(11); //0.167s (10) - duration
  rest(24); // 0.375s (11, 12) (5)
  Serial.write(77); //F4(13) (6)
  Serial.write(8); //0.125s(14)
  Serial.write(75); // D#4 (15) (7)
  Serial.write(11); //0.167s (16)
  rest(16); // 0.25s (17, 18) (8)
  Serial.write(63); // D#3 (19) (9)
  Serial.write(4); //0.0625s (20)
  //segment 2 // much more rapid
  
  Serial.write(75); //D#4 (21) (10)
  Serial.write(4); //0.0625s (22)
  Serial.write(63); //D#3 and D#4 at the same time :(( (22) (11)
  Serial.write(11); //0.167s (23)
  Serial.write(39); //D#1 and D#4 (24) (12)
  Serial.write(31); //0.333s and 0.0625s (25)
  Serial.write(57); //D#1 and A3 (26) (13)
  Serial.write(8); //0.125s (27)
  Serial.write(65); //D#1 and F3 and A3 (28) (14)
  Serial.write(4); //0.333s and 0.125s and 0.0625s (29)
  Serial.write(63); //D#1 and D#3 and A3 (30) (15)
  Serial.write(4); // 0.0125s and 0.167s and 0.167s (31)
  Serial.write(39); //D#1 and D#3, and A3 (32) (16)
  Serial.write(4); //0.0625s (33)  
}

void BB8Sound2() {
  Serial.write(140);  //opcode
  // #of data types is (2N+2), where N is the # of notes in the song.
  Serial.write(1);  //song number 1
  Serial.write(8); //max notes 
  
  Serial.write(65); //D#1 and A3 and A3 (34) (17)
  Serial.write(8); //0.125s and 0.0625s and 0.125s (35)

  Serial.write(39); //D#1 and A3 (36) (18)
  Serial.write(31); //0.333s and 0.167s (37)
  rest(31); //0.333s (38, 39) (19)
}

void playbb8() {
  Serial.write(141);
  Serial.write(0);
  Serial.write(141);
  Serial.write(1);
}

void loop() {
  playbb8();
  delay(2000);
}
