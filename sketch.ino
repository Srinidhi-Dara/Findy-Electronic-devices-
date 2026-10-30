#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// ========== LCD CONFIGURATION ==========
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ========== KEYPAD CONFIGURATION ==========
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Keypad pins: R1, R2, R3, R4
byte rowPins[ROWS] = {13, 12, 14, 27};

// Keypad pins: C1, C2, C3, C4
byte colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// ========== LED PINS ==========
const int keysLED = 18;
const int remoteLED = 19;
const int walletLED = 23;

// ========== BUZZER PINS ==========
const int keysBuzzer = 4;
const int remoteBuzzer = 5;
const int walletBuzzer = 15;

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);

  // Initialize I2C LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  // Configure LEDs
  pinMode(keysLED, OUTPUT);
  pinMode(remoteLED, OUTPUT);
  pinMode(walletLED, OUTPUT);

  // Configure buzzers
  pinMode(keysBuzzer, OUTPUT);
  pinMode(remoteBuzzer, OUTPUT);
  pinMode(walletBuzzer, OUTPUT);

  // Turn everything OFF
  turnEverythingOff();

  // Welcome screen
  lcd.setCursor(0, 0);
  lcd.print(" SMART HOME");
  lcd.setCursor(0, 1);
  lcd.print(" LOST FINDER");

  delay(2000);

  showMenu();
}

// ========== MAIN LOOP ==========
void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key pressed: ");
    Serial.println(key);

    switch (key) {
      case '1':
        findObject("KEYS", keysLED, keysBuzzer);
        break;

      case '2':
        findObject("REMOTE", remoteLED, remoteBuzzer);
        break;

      case '3':
        findObject("WALLET", walletLED, walletBuzzer);
        break;

      case '*':
        turnEverythingOff();
        showMenu();
        break;

      default:
        // Ignore other keys
        break;
    }
  }
}

// ========== SEARCH FOR OBJECT ==========
void findObject(const char* objectName, int ledPin, int buzzerPin) {
  turnEverythingOff();

  // Display searching message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SEARCHING...");

  lcd.setCursor(0, 1);
  lcd.print(objectName);

  Serial.print("Searching for: ");
  Serial.println(objectName);

  delay(2000);

  // Activate selected object's LED
  digitalWrite(ledPin, HIGH);

  // Display found message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OBJECT FOUND!");

  lcd.setCursor(0, 1);
  lcd.print(objectName);

  // Activate selected buzzer
  digitalWrite(buzzerPin, HIGH);

  // Keep alert active for 5 seconds
  delay(5000);

  // Stop alert
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  showMenu();
}

// ========== TURN OFF ALL OUTPUTS ==========
void turnEverythingOff() {
  digitalWrite(keysLED, LOW);
  digitalWrite(remoteLED, LOW);
  digitalWrite(walletLED, LOW);

  digitalWrite(keysBuzzer, LOW);
  digitalWrite(remoteBuzzer, LOW);
  digitalWrite(walletBuzzer, LOW);
}

// ========== DISPLAY MENU ==========
void showMenu() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("1:Keys 2:Remote");

  lcd.setCursor(0, 1);
  lcd.print("3:Wallet *:Stop");
}
