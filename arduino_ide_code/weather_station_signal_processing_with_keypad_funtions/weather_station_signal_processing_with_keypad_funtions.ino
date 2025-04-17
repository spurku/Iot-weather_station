#include <TimerOne.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Keypad.h>

// LCD
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

// Ethernet & MQTT
byte server[] = { 10, 6, 0, 23 };
unsigned int Port = 1883;
EthernetClient ethClient;
PubSubClient client(server, Port, ethClient);
static uint8_t mymac[6] = { 0x44, 0x76, 0x58, 0x10, 0x00, 0x62 };
char* clientId = "a731fsd4";
#define outTopic "ICT4_out_2020"

// Analog (temperature)
int sensorPin = A0;
float voltage, temperature, tempTemperature = 0;
int analogAvarageCounter = 0, avarageTemperature = 0;
unsigned long currentTime = 0, previousTime = 0, ct = 0, ctp = 0;

// Digital (humidity)
int inPin_digital = 2;
volatile int hum = 0, tempHum = 0, avarageHum = 0;
volatile int signalCount = 0;
volatile int digiAvarageCounter = 0;

// Keypad setup
const byte ROWS = 4;
const byte COLS = 1;
char keys[ROWS][COLS] = {
  {'A'},
  {'B'},
  {'C'},
  {'D'}
};
byte rowPins[ROWS] = {A1, A2, A3, A4};
byte colPins[COLS] = {A5};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// JSON buffers
char mittausKosteusJSON[100];
char mittausLampotilaJSON[100];
int c = 0;

// Min/Max tracking
float minTemp = 999, maxTemp = -999;
int minHum = 999, maxHum = -999;
bool showMin = true;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  fetch_IP();

  pinMode(inPin_digital, INPUT);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(myTimerRoutine);
  attachInterrupt(digitalPinToInterrupt(2), pin_ISR, RISING);
}

void loop() {
  // Read analog signal
  voltage = analogRead(sensorPin) * (5.0 / 1023.0);
  voltage = voltage / 3.0;
  temperature = 30.43 * voltage - 25.215;

  // Track min/max
  if (temperature < minTemp) minTemp = temperature;
  if (temperature > maxTemp) maxTemp = temperature;
  if (hum < minHum) minHum = hum;
  if (hum > maxHum) maxHum = hum;

  currentTime = millis();
  if (currentTime - previousTime >= 1000) {
    tempTemperature += temperature;
    analogAvarageCounter++;
    if (analogAvarageCounter == 10) {
      avarageTemperature = tempTemperature / 10;
      analogAvarageCounter = 0;
      tempTemperature = 0;
    }
    previousTime = currentTime;
  }

  // Prepare data to send
  sprintf(mittausKosteusJSON, "IOTJS={\"S_name\":\"Tiim_Kosteus\",\"S_value\":%d}", avarageHum);
  sprintf(mittausLampotilaJSON, "IOTJS={\"S_name\":\"Tiim_Lampotila\",\"S_value\":%d}", avarageTemperature);

  // MQTT sending
  ct = millis();
  if (ct - ctp >= 1000) {
    c++;
    if (c == 11) {
      send_MQTT_message();
      c = 0;
    }
    ctp = ct;
  }

  // Keypad interaction
  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    switch (key) {
      case 'A': // Show IP and MQTT status
        lcd.setCursor(0, 0);
        lcd.print("IP:");
        lcd.print(Ethernet.localIP());
        lcd.setCursor(0, 1);
        if (client.connected()) {
          lcd.print("MQTT: Connected");
        } else {
          lcd.print("MQTT: Not conn.");
        }
        break;

      case 'B': // Show current values
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperature, 1);
        lcd.print(" C");
        lcd.setCursor(0, 1);
        lcd.print("Hum: ");
        lcd.print(hum);
        lcd.print(" %");
        break;

      case 'C': // Show min/max stats
        lcd.setCursor(0, 0);
        if (showMin) {
          lcd.print("Min T:");
          lcd.print(minTemp, 1);
          lcd.print("C H:");
          lcd.print(minHum);
          lcd.print("%");
        } else {
          lcd.print("Max T:");
          lcd.print(maxTemp, 1);
          lcd.print("C H:");
          lcd.print(maxHum);
          lcd.print("%");
        }
        showMin = !showMin;
        break;

      case 'D':
        minTemp = 999; maxTemp = -999;
        minHum = 999; maxHum = -999;
        lcd.setCursor(0, 0);
        lcd.print("Min/Max Reset");
        lcd.setCursor(0, 1);
        lcd.print("Done");
        break;
    }
  }

  delay(100);
}

void pin_ISR(void) {
  signalCount++;
}

void myTimerRoutine() {
  hum = map(signalCount, 6900, 7900, 100, 40);
  tempHum += hum;
  digiAvarageCounter++;
  if (digiAvarageCounter == 10) {
    avarageHum = tempHum / 10;
    digiAvarageCounter = 0;
    tempHum = 0;
  }
  signalCount = 0;
}

void fetch_IP() {
  bool connectionSuccess = Ethernet.begin(mymac);
  if (!connectionSuccess) {
    Serial.println("Failed to access Ethernet controller");
  } else {
    Serial.print("Connected with IP: ");
    Serial.println(Ethernet.localIP());
  }
}

void send_MQTT_message() {
  if (!client.connected()) {
    connect_MQTT_server();
  }
  if (client.connected()) {
    client.publish(outTopic, mittausKosteusJSON);
    client.publish(outTopic, mittausLampotilaJSON);
    Serial.println("Message sent to MQTT server.");
  } else {
    Serial.println("Failed to send message: not connected to MQTT server.");
  }
}

void connect_MQTT_server() {
  Serial.println("Connecting to MQTT");
  if (client.connect(clientId)) {
    Serial.println("Connection OK");
  } else {
    Serial.println("Connection failed.");
  }
}
