#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "1234";  // Replace with your WiFi SSID
const char* password = "12345678";  // Replace with your WiFi Password

WiFiServer server(80);

#define MOTOR1_A D0
#define MOTOR1_B D1
#define MOTOR2_A D2
#define MOTOR2_B D3
#define SERVO_PIN D4

Servo myServo; 
int servoAngle = 90; // Default position

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle); // Set servo to default position
}

void loop() {
  WiFiClient client = server.available();
  
  if (!client) return;

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/forward") != -1) {
    moveForward();
  } 
  else if (request.indexOf("/backward") != -1) {
    moveBackward();
  } 
  else if (request.indexOf("/left") != -1) {
    turnLeft();
  } 
  else if (request.indexOf("/right") != -1) {
    turnRight();
  } 
  else if (request.indexOf("/stop") != -1) {
    stopMoving();
  } 
  else if (request.indexOf("/servo/") != -1) {
    int pos = request.substring(request.indexOf("/servo/") + 7).toInt();
    if (pos >= 0 && pos <= 180) {
      servoAngle = pos;
      myServo.write(servoAngle);
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("OK");
  client.stop();
}

// Movement Functions
void moveForward() {
  digitalWrite(MOTOR1_A, HIGH);
  digitalWrite(MOTOR1_B, LOW);
  digitalWrite(MOTOR2_A, HIGH);
  digitalWrite(MOTOR2_B, LOW);
  Serial.println("Moving Forward");
}

void moveBackward() {
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, HIGH);
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, HIGH);
  Serial.println("Moving Backward");
}

void turnLeft() {
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, HIGH);
  digitalWrite(MOTOR2_A, HIGH);
  digitalWrite(MOTOR2_B, LOW);
  Serial.println("Turning Left");
}

void turnRight() {
  digitalWrite(MOTOR1_A, HIGH);
  digitalWrite(MOTOR1_B, LOW);
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, HIGH);
  Serial.println("Turning Right");
}

void stopMoving() {
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, LOW);
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, LOW);
  Serial.println("Stopping");
}

// // Servo Control Function
// else if (request.indexOf("/servo/") != -1) {
//   int pos = request.substring(request.indexOf("/servo/") + 7).toInt();
//   if (pos >= 0 && pos <= 180) {
//     servoAngle = pos;
//     myServo.write(servoAngle);
//     Serial.print("Servo moving to: ");
//     Serial.println(servoAngle);
//   }
// }
