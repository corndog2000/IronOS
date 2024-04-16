#include <Wire.h>

#define INH_PIN 36
#define A_PIN 38
#define B_PIN 40
#define C_PIN 42

int output_select = 0;

uint8_t toSend[6] = { 0x53, 0x1E, 0, 0, 0, 0 };
uint8_t buf = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(INH_PIN, OUTPUT);
  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(C_PIN, OUTPUT);

  digitalWrite(INH_PIN, LOW);
  // By default we want to be on output 7 since there is no hotplate connected to that output.
  digitalWrite(A_PIN, HIGH);
  digitalWrite(B_PIN, HIGH);
  digitalWrite(C_PIN, HIGH);

  Wire.begin(0x42);              // join i2c bus with address #2
  Wire.onRequest(requestEvent);  // register event

  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');

    // Convert the string to an integer
    int number = data.toInt();

    if (number >= 0 && number <= 8) {
      Serial.print("Setting output to: ");
      Serial.println(number);

      // Switch to output 7 for a second before switching to the real output pins.
      digitalWrite(A_PIN, HIGH);
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);

      delay(1500);

      switch (number) {
        case 0:
          digitalWrite(A_PIN, LOW);
          digitalWrite(B_PIN, LOW);
          digitalWrite(C_PIN, LOW);
          break;
        case 1:
          digitalWrite(A_PIN, HIGH);
          digitalWrite(B_PIN, LOW);
          digitalWrite(C_PIN, LOW);
          break;
        case 2:
          digitalWrite(A_PIN, LOW);
          digitalWrite(B_PIN, HIGH);
          digitalWrite(C_PIN, LOW);
          break;
        case 3:
          digitalWrite(A_PIN, HIGH);
          digitalWrite(B_PIN, HIGH);
          digitalWrite(C_PIN, LOW);
          break;
        case 4:
          digitalWrite(A_PIN, LOW);
          digitalWrite(B_PIN, LOW);
          digitalWrite(C_PIN, HIGH);
          break;
        case 5:
          digitalWrite(A_PIN, HIGH);
          digitalWrite(B_PIN, LOW);
          digitalWrite(C_PIN, HIGH);
          break;
        case 6:
          digitalWrite(A_PIN, LOW);
          digitalWrite(B_PIN, HIGH);
          digitalWrite(C_PIN, HIGH);
          break;
        case 7:
          digitalWrite(A_PIN, HIGH);
          digitalWrite(B_PIN, HIGH);
          digitalWrite(C_PIN, HIGH);
          break;
      }
    } else if (number >= 20 && number <= 250) {
      toSend[1] = number;
      Serial.print("New heat value: ");
      Serial.println(toSend[1], DEC);
    } else if (number == 251) {
      toSend[1] = number;
      Serial.println("REBOOTING HOTPALTE");
    } else {
      Serial.println("The number is out of the 8-bit range. Please enter a number between 0 and 255.");
    }
  }

  Serial.println("...");

  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(toSend, 6);  // respond with message of 6 bytes
                          // as expected by master
  Serial.println("Responded");
}
