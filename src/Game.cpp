#include "Game.h"

#ifdef abs
#undef abs
#endif

#include <random>
#include <PolledTimeout.h>

using namespace std;

default_random_engine eng;
uniform_int_distribution<int8_t> from1to7(0,6);

Piece random_piece() {
    return {from1to7(eng), {0, 0}, 0};
}

Game::Game() :
    _controller(),
    _piece(moveToStart(random_piece())),
    _preview(random_piece()),
    _board(),
    _points(0),
    _counter(0)
{
}

void Game::cycleDown() {
    Piece moved_piece = _piece.moved({0,1});
    if (_board.collision(moved_piece)) {
        levelUp();
    }
    else {
        _piece = moved_piece;
    }
}

void Game::levelUp() {
    _board.addPiece(_piece);

    int n = _board.deleteLines();
    _points += n * n;
    ++_counter;

    _piece = moveToStart(_preview);
    _preview = random_piece();
}

void Game::cycleUser() {
    Piece changed_piece = _piece;

    if (_controller.rotation())
        changed_piece.rotate();

    IntVec shift{0,0};
    shift[0] = _controller.shift();
    changed_piece.move(shift);

    if (!_board.collision(changed_piece))
        _piece = changed_piece;
}

Piece &Game::moveToStart(Piece piece) {
    return piece.move({_start_pos, 0});
}

using Clock = esp8266::polledTimeout::periodicFastMs;
Clock user_cycle(31); // 1/32 = 0.03125

void Game::loop() {
    Clock down_cycle(1000);
    while (!_board.isFull()) {
        while (!down_cycle) {
            if (user_cycle) {
                _controller.measure();
                cycleUser();
            }
        }
        down_cycle.reset(1000 - _counter);
        cycleDown();
    }
}

Game::Input::Input() :
    Sensor(Wire, 0x68),
    last_was_rot(false)
{
    Sensor.begin();                                      // Sensor wird initialisiert, eine aufwendigere Lösung
    // findet sich in den Beispieldateien der library
    Sensor.setAccelRange(MPU9250::ACCEL_RANGE_8G);       // Setzt den Beschleunigungssensor auf den max. Bereich
    Sensor.setGyroRange(MPU9250::GYRO_RANGE_500DPS);     // Setzt den Maximalwert düe den Gyrosensor auf +/- 500°/s
    Sensor.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ); // Setzt einen Filter für Störfreqnezen bei 20Hz
    Sensor.setSrd(19);
}

void Game::Input::measure() {
    static constexpr float threshold = -3;
    last_was_rot = Sensor.getGyroX_rads() < threshold;
    Sensor.readSensor();
}

int Game::Input::shift() {
    static constexpr float threshold = 2;
    float shift = Sensor.getAccelX_mss();
    if (shift > threshold)
        return 1;
    else if (shift < -threshold)
        return -1;
    return 0;
}

bool Game::Input::rotation() {
    static constexpr float threshold = -3;
    bool is_rot = Sensor.getGyroX_rads() < threshold;
    return last_was_rot && last_was_rot != is_rot;
}
