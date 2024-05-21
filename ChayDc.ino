const uint8_t relayPins[] = {2, 3, 4, 5, 6, 7};
const int numRelays = sizeof(relayPins) / sizeof(relayPins[0]);
//oke
struct Relay {
    uint8_t pin;
    bool state;
    unsigned long endTime;
};

Relay relays[numRelays];

void setup() {
    Serial.begin(19200);
    for (int i = 0; i < numRelays; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
        relays[i] = {relayPins[i], false, 0};
    }
}

void loop() {
    unsigned long currentTime = millis();
    
    // Check each relay to see if it should be turned off
    for (int i = 0; i < numRelays; i++) {
        if (relays[i].state && currentTime >= relays[i].endTime) {
            digitalWrite(relays[i].pin, LOW);
            relays[i].state = false;
        }
    }

    // Handle new serial input
    if (Serial.available() > 0) {
        // Read the entire input line
        String input = Serial.readStringUntil('\n');
        
        // Split the input into components
        int firstSpace = input.indexOf(' ');
        int secondSpace = input.indexOf(' ', firstSpace + 1);
        
        if (firstSpace != -1 && secondSpace != -1) {
            // Extract pumpIndex, pumpState, and duration from the input string
            int pumpIndex = input.substring(0, firstSpace).toInt();
            char pumpState = input.substring(firstSpace + 1, secondSpace).charAt(0);
            unsigned long duration = input.substring(secondSpace + 1).toInt();
            

            // Validate the pumpIndex
            if (pumpIndex >= 1 && pumpIndex <= numRelays) {
                // Calculate the end time for the relay
                unsigned long endTime = currentTime + duration;

                if (pumpState == '1') {
                    digitalWrite(relayPins[pumpIndex - 1], HIGH);
                    relays[pumpIndex - 1].state = true;
                    relays[pumpIndex - 1].endTime = endTime;
                } else if (pumpState == '0') {
                    digitalWrite(relayPins[pumpIndex - 1], LOW);
                    relays[pumpIndex - 1].state = false;
                }
            }
        }
    }
}
