#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <MenuSystem.h>
#include <Adafruit_Fingerprint.h> // Bibliothèque pour le capteur d'empreintes digitales

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create an OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Button pins
#define BUTTON_NEXT 22
#define BUTTON_INC 3
#define BUTTON_SET 4
#define BUTTON_CANCEL 5

// Fingerprint sensor pins
#define FINGERPRINT_RX 10
#define FINGERPRINT_TX 11

// Electromagnet pin
#define ELECTROMAGNET_PIN 6

// Presence detection pin
#define PRESENCE_SENSOR_PIN 7

// Variables for alarm settings
int selectedDay = 1;   // 1-31
int selectedHour = 0;  // 0-23
int selectedMin = 0;   // 0-59
bool alarmSet = false;

// Menu system
MenuSystem ms;

// Fingerprint sensor object
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);

// Menu callbacks
void onDaySelected(MenuItem* p_menu_item);
void onHourSelected(MenuItem* p_menu_item);
void onMinuteSelected(MenuItem* p_menu_item);
void onSetAlarm(MenuItem* p_menu_item);

// Menu items
MenuItem dayMenuItem("Set Day", &onDaySelected);
MenuItem hourMenuItem("Set Hour", &onHourSelected);
MenuItem minuteMenuItem("Set Minute", &onMinuteSelected);
MenuItem setAlarmMenuItem("Set Alarm", &onSetAlarm);

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    Serial1.begin(57600); // Serial communication for fingerprint sensor

    // Initialize the OLED display
    if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.clearDisplay();
    display.display();

    // Initialize buttons
    pinMode(BUTTON_NEXT, INPUT_PULLUP);
    pinMode(BUTTON_INC, INPUT_PULLUP);
    pinMode(BUTTON_SET, INPUT_PULLUP);
    pinMode(BUTTON_CANCEL, INPUT_PULLUP);

    // Initialize electromagnet and presence sensor
    pinMode(ELECTROMAGNET_PIN, OUTPUT);
    pinMode(PRESENCE_SENSOR_PIN, INPUT);

    // Setup fingerprint sensor
    if (finger.verifyPassword()) {
        Serial.println("Fingerprint sensor detected!");
    } else {
        Serial.println("Fingerprint sensor not detected. Check wiring.");
        while (1);
    }

    // Setup menu system
    ms.get_root_menu().add_item(&dayMenuItem);
    ms.get_root_menu().add_item(&hourMenuItem);
    ms.get_root_menu().add_item(&minuteMenuItem);
    ms.get_root_menu().add_item(&setAlarmMenuItem);

    // Display initial screen
    updateDisplay();
}

void loop() {
    // Handle button presses
    if (digitalRead(BUTTON_NEXT) == LOW) {
        ms.next(); // Navigate to the next menu item
        delay(200); // Debounce delay
        updateDisplay();
    }
    if (digitalRead(BUTTON_INC) == LOW) {
        MenuItem* current = ms.get_current_menu_item();
        if (current == &dayMenuItem) {
            selectedDay = (selectedDay % 31) + 1;
        } else if (current == &hourMenuItem) {
            selectedHour = (selectedHour + 1) % 24;
        } else if (current == &minuteMenuItem) {
            selectedMin = (selectedMin + 1) % 60;
        }
        delay(200);
        updateDisplay();
    }
    if (digitalRead(BUTTON_SET) == LOW) {
        ms.select(); // Trigger the callback for the current menu item
        delay(200);
        updateDisplay();
    }
    if (digitalRead(BUTTON_CANCEL) == LOW) {
        alarmSet = false;
        delay(200);
        updateDisplay();
    }

    // Check for presence detection
    if (digitalRead(PRESENCE_SENSOR_PIN) == HIGH) {
        Serial.println("Presence detected!");
        updateDisplay();
    }

    // Check fingerprint for access
    if (checkFingerprint()) {
        unlockBox();
    }

    // Check if the alarm should trigger
    if (alarmSet) {
        // Replace this with real-time clock (RTC) logic to compare current time
        if (selectedDay == 1 && selectedHour == 0 && selectedMin == 0) {
            triggerAlarm();
        }
    }
}

void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("Menu:");

    // Display the current menu item
    display.setCursor(0, 10);
    display.print(ms.get_current_menu_item()->get_name());

    display.setCursor(0, 20);
    display.print("Day: ");
    display.print(selectedDay);

    display.setCursor(0, 30);
    display.print("Hour: ");
    display.print(selectedHour);

    display.setCursor(0, 40);
    display.print("Min: ");
    display.print(selectedMin);

    display.setCursor(0, 50);
    if (alarmSet) {
        display.print("Alarm: ON");
    } else {
        display.print("Alarm: OFF");
    }

    display.display();
}

void onDaySelected(MenuItem* p_menu_item) {
    Serial.println("Day selected");
}

void onHourSelected(MenuItem* p_menu_item) {
    Serial.println("Hour selected");
}

void onMinuteSelected(MenuItem* p_menu_item) {
    Serial.println("Minute selected");
}

void onSetAlarm(MenuItem* p_menu_item) {
    alarmSet = true;
    Serial.println("Alarm set");
}

void triggerAlarm() {
    // Simulate alarm action (e.g., buzzer or LED)
    Serial.println("Alarm Triggered!");
    alarmSet = false; // Reset alarm
    updateDisplay();
}

bool checkFingerprint() {
    if (finger.getImage() != FINGERPRINT_OK) return false;
    if (finger.image2Tz() != FINGERPRINT_OK) return false;
    if (finger.fingerFastSearch() != FINGERPRINT_OK) return false;

    Serial.println("Fingerprint matched!");
    return true;
}

void unlockBox() {
    digitalWrite(ELECTROMAGNET_PIN, HIGH); // Activate electromagnet to unlock
    delay(5000); // Keep unlocked for 5 seconds
    digitalWrite(ELECTROMAGNET_PIN, LOW); // Lock again
    Serial.println("Box unlocked!");
}