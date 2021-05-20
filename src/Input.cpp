#include "Input.h"

MPU9250 Sensor(Wire, 0x68);

void init_mpu() {
    Sensor.begin(); // Sensor wird initialisiert, eine aufwendigere Lösung
    // findet sich in den Beispieldateien der library
    Sensor.setAccelRange(
            MPU9250::ACCEL_RANGE_8G); // Setzt den Beschleunigungssensor auf den max.
    // Bereich
    Sensor.setGyroRange(
            MPU9250::GYRO_RANGE_500DPS); // Setzt den Maximalwert düe den Gyrosensor
    // auf +/- 500°/s
    Sensor.setDlpfBandwidth(
            MPU9250::DLPF_BANDWIDTH_20HZ); // Setzt einen Filter für Störfreqnezen bei
    // 20Hz
    Sensor.setSrd(19); // Setzt die Datenrate auf 19, was 50Hz entspricht
}

int Input::shift() {
    Sensor.readSensor();
    if (Sensor.getGyroX_rads() < -5) {
        return 0;
    } else if (Sensor.getGyroX_rads() > 5) {
        return 2;
    } else {
        return 1;
    }
}

bool Input::rotate() {
    Sensor.readSensor();
    if (Sensor.getGyroY_rads() > 5) {
        return true;
    } else {
        return false;
    }
}