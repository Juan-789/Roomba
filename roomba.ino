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
  driveBothWheels(60,60);
  delay(10000); // 10 second delay
  powerOff();


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

void loop() {

}
