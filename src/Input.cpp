#include "Input.h"

MPU9250 Sensor(Wire, 0x68);

void init_mpu() {
    Sensor.begin();                                      // Sensor wird initialisiert, eine aufwendigere Lösung
                                                       // findet sich in den Beispieldateien der library
    Sensor.setAccelRange(MPU9250::ACCEL_RANGE_8G);       // Setzt den Beschleunigungssensor auf den max. Bereich
    Sensor.setGyroRange(MPU9250::GYRO_RANGE_500DPS);     // Setzt den Maximalwert düe den Gyrosensor auf +/- 500°/s
    Sensor.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ); // Setzt einen Filter für Störfreqnezen bei 20Hz
    Sensor.setSrd(19);                                   // Setzt die Datenrate auf 19, was 50Hz entspricht
}

Input::Input() : last_was_rot(false) {
}

void Input::measure() {
    static constexpr float threshold = -3;
    last_was_rot = Sensor.getGyroX_rads() < threshold;
    Sensor.readSensor();
}

int Input::shift() {
    static constexpr float threshold = 2;
    float shift = Sensor.getAccelX_mss();
    if (shift > threshold)
        return 1;
    else if (shift < -threshold)
        return -1;
    return 0;
}

bool Input::rotate() {
    static constexpr float threshold = -3;
    bool is_rot = Sensor.getGyroX_rads() < threshold;
    return last_was_rot && last_was_rot != is_rot;
}
