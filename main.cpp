#include <Arduino.h>
#include <FastPID.h>

#define IN_PIN A0
#define PWM_PIN 12
#define DIR_PIN 13

float Kp = 0;
float Ki = 0;
float Kd = 0;

float Hz = 1000/60;

uint16_t setpoint = 360;

FastPID shoulderPID(Kp, Ki, Kd, Hz, 8, true);


void setup() {
    init();
    Serial.begin(9600);
    delay(100);

    pinMode(IN_PIN, INPUT);
    pinMode(PWM_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    // Re-configure
    shoulderPID.clear();
    shoulderPID.setCoefficients(Kp, Ki, Kd, Hz);
    shoulderPID.setOutputRange(-50, 50);

    if (shoulderPID.err()) {
        Serial.println("Config Error!");
        while(1);
    }
}

int main() {
    setup();

    uint32_t time = millis();

    while(1) {
        // loop for 60ms
        bool flag = true;
        while (millis() - time < 60) {

            if (flag) {
                int16_t feedback = analogRead(A0);
                Serial.println(feedback);
            
                int16_t output = shoulderPID.step(setpoint, feedback);

                if (output > 0) {
                    analogWrite(PWM_PIN, output);
                    digitalWrite(DIR_PIN, 1);
                    Serial.println(output);
                } else {
                    analogWrite(PWM_PIN, abs(output));
                    digitalWrite(DIR_PIN, 0);
                    Serial.println(output);
                }

                flag = false;
            }           
        }

        time = millis();

        if (Serial.available() > 3) {
            char buf[20];

            int p, p_dec, i, i_dec, d, d_dec;

            Serial.readBytes(buf, 20);

            // Print out settings
            char str[50];
            sprintf(str, "Kp: %d.%d, Ki: %d.%d, Kp: %d.%1d",
                    (int) Kp, (int)(Kp*10)%10,
                    (int) Ki, (int)(Ki*10)%10,
                    (int) Kd, (int)(Kd*10)%10);
            Serial.println(str);

            if (buf[0] == 'S' && buf[1] == 'E' && buf[2] == 'T') {
               sscanf(buf, "SET %d.%d %d.%d %d.%d", &p, &p_dec, &i, &i_dec, &d, &d_dec);
               Kp = (float)p + ((float)p_dec / 10);
               Ki = (float)i + ((float)i_dec / 10);
               Kd = (float)d + ((float)d_dec / 10);
               shoulderPID.clear();
               shoulderPID.setCoefficients(Kp, Ki, Kd, Hz);
               shoulderPID.setOutputRange(-50, 50);
            }

            if (buf[0] == 'G' && buf[1] == 'O') {
                sscanf(buf, "GO %d", &setpoint);

                setpoint = constrain(setpoint, 300, 900);
            }
        }
    }
}
