#include <Braccio.h>
#include <Servo.h>

// Define Servo objects for each joint
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

// Define Braccio servo positions
int basePosition = 0;         // Base servo position (centered)
int shoulderPosition = 0;     // Shoulder position for grabbing
int elbowPosition = 180;        // Elbow position for grabbing
int wristPosition = 0;        // Wrist position
int wristRotation = 90;
int gripperOpen = 0;          // Gripper open position
int gripperClosed = 73;        // Gripper closed position
/*
   Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
unsigned long moveDelay = 1000;           // Delay for movements in milliseconds
 
// Timing variables
//unsigned long previousMillis = 0;
bool isMoving = false;
String currentCommand = "";
// IR sensor pin for obstacle detection
#define IR_SENSOR_PIN 7

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  Braccio.begin();     // Initialize the Braccio library
 pinMode(IR_SENSOR_PIN, INPUT);
  //Braccio.begin(SOFT_START_DISABLED); // Initialize the Braccio library with Soft-start disabled

  // Move to initial position
  moveToInitialPosition();
  Serial.println("Braccio is ready. Waiting for commands...");
}

void loop() {

  // Monitor serial input for commands
  if (Serial.available() > 0) {
    currentCommand = Serial.readStringUntil('\n');
    currentCommand.trim();
    executeCommand(currentCommand);
      //isMoving = true;
   // previousMillis = millis();
  }

  /*if (isMoving) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= moveDelay) {
      previousMillis = currentMillis;
      
      isMoving = false;
    }
  }*/
}

// Move the robotic arm to the initial position
void moveToInitialPosition() {
  Braccio.ServoMovement(20,  basePosition, shoulderPosition, elbowPosition, wristPosition, wristRotation, gripperClosed );
}
// Function to execute the current command
void executeCommand(String command) {
  if (command == "1") { /*grabbing*/
    grabObject();
  } else if (command == "2") { /*point*/
    placeObjectRight();
  } else {
    Serial.println("Unknown command. Use 1 for'grab', 2 for 'place/point'");
    moveToInitialPosition();
  }
}

// Function to grab an object
void grabObject() {
  if (isObstacleDetected()) {
    Serial.println("Obstacle detected! Cannot proceed.");
    return;
  }

  Serial.println("Grabbing object...");
  Braccio.ServoMovement(20, 0, 0, 100, 30, 90, gripperOpen);
  delay(moveDelay);

  Braccio.ServoMovement(20, 0, 0, 100, 30, 90, gripperClosed);
  delay(moveDelay);
  moveToInitialPosition();

  Serial.println("Object grabbed.");
}
 
// Function to place an object
void placeObject() {
 if (isObstacleDetected()) {
    Serial.println("Obstacle detected! Cannot proceed.");
    return;
  }

  Serial.println("Placing object...");

  Braccio.ServoMovement(20, basePosition + 95, shoulderPosition, elbowPosition, wristPosition, 90, gripperClosed);
  delay(moveDelay);

  Braccio.ServoMovement(20, basePosition + 45, shoulderPosition, 100, wristPosition, 90, gripperOpen);
  delay(moveDelay);

  // Return to the initial position
  moveToInitialPosition();
  Serial.println("Object placed.");
}

void placeObjectRight() {
  if (isObstacleDetected()) {
    Serial.println("Obstacle detected! Cannot proceed.");
    return;
  }

  Serial.println("Placing object to the right...");
  int rightPosition = basePosition + 100; // Adjust this value as needed
  Braccio.ServoMovement(20, basePosition, shoulderPosition,elbowPosition , wristPosition, 90, gripperClosed);
  delay(moveDelay);

  Braccio.ServoMovement(20, rightPosition, shoulderPosition, 110, wristPosition, 90, gripperClosed);
  delay(moveDelay);

  // Open the gripper to release the object
  Braccio.ServoMovement(20, rightPosition, shoulderPosition, 110, wristPosition, 90, gripperOpen);
  delay(moveDelay);

  // Return to the initial position
  moveToInitialPosition();
  Serial.println("Object placed to the right.");
}

// Function to check if an obstacle is detected
bool isObstacleDetected() {
  return digitalRead(IR_SENSOR_PIN) == LOW;
}
