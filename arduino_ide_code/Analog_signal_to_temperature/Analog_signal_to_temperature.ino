int sensorPin = A0;  // Pin, johon signaali tulee
float voltage, temperature;

void setup() {
    Serial.begin(9600);  // Sarjamonitorin aloitus
}

void loop() {
    // Lue jännite (0-1023) ja muunna se 0-5V alueelle
    voltage = analogRead(sensorPin) * (5.0 / 1023.0);

    // Vahvistettu jännite jaetaan kolmella
    voltage = voltage / 3.0;

    // Laske lämpötila lineaarisella kaavalla
    temperature = 30.43 * voltage - 25.215;

    // Tulosta jännite ja lämpötila sarjamonitoriin
      Serial.print("Vahvistettu jännite: ");
    Serial.print(voltage, 2);
    Serial.print(" V | Lämpötila: ");
    Serial.print(temperature, 2);
    Serial.println(" °C");

    delay(500);  // Pieni viive näytön päivittämiseksi
}