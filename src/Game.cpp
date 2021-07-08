#include "Game.h"

#include <PolledTimeout.h>
#include <SPI.h>
#include <Wire.h>
#include <MPU9250.h>

using namespace std;

#ifdef abs
#undef abs
#endif

#include <random>

uint32_t b(float x) {
    return *reinterpret_cast<uint32_t*>(&x);
}

class MPU9250i : public MPU9250 {
public:
    MPU9250i() : MPU9250(Wire, 0x68) {
        Wire.begin(D2,D1);
        begin();                                      // Sensor wird initialisiert, eine aufwendigere Lösung findet sich in den Beispieldateien der library
        setAccelRange(MPU9250::ACCEL_RANGE_8G);       // Setzt den Beschleunigungssensor auf den max. Bereich
        setGyroRange(MPU9250::GYRO_RANGE_500DPS);     // Setzt den Maximalwert düe den Gyrosensor auf +/- 500°/s
        setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ); // Setzt einen Filter für Störfreqnezen bei 20Hz
        setSrd(19);                                   // Setzt die Datenrate auf 19, was 50Hz entspricht

        readSensor();
    }
};

inline MPU9250i Sensor;

std::default_random_engine eng(b(Sensor.getAccelX_mss()) ^ b(Sensor.getAccelY_mss()) ^ b(Sensor.getAccelZ_mss()));
std::uniform_int_distribution<int8_t> from1to7(0, 6);

Piece random_piece() { return {from1to7(eng), {0, 0}, 0}; }

Game game;

Game::Game() :
    _controller(),
    _piece(moveToStart(random_piece())),
    _preview(random_piece()),
    _board(),
    _points(0),
    _level(0),
    tft()
{
    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
}

void Game::cycleDown() {
    Piece moved_piece = _piece.moved({0, 1});
    if (_board.collision(moved_piece)) {
        levelUp();
    } else {
        clearPiece(_piece);
        _piece = moved_piece;
    }
    drawPiece(_piece);
}

void Game::levelUp() {
    _board.addPiece(_piece);

    int n = _board.deleteLines();
    if (n > 0) drawBoard();
    _points += n * n;
    ++_level;

    _piece = moveToStart(_preview);
    _preview = random_piece();
}

void Game::cycleUser() {
    Piece changed_piece = _piece;

    if (_controller.rotation()) changed_piece.rotate();

    IntVec shift{0, 0};
    shift[0] = _controller.shift();
    changed_piece.move(shift);

    clearPiece(_piece);
    if (!_board.collision(changed_piece)) _piece = changed_piece;
    drawPiece(_piece);
}

using Clock = esp8266::polledTimeout::periodicFastMs;
Clock user_cycle(94);  // 1/32 = 0.03125, 3/32 = 0.09375

void Game::loop() {
    static constexpr int initial_speed = 666;
    Clock down_cycle(initial_speed);
    while (!_board.isFull()) {
        while (!down_cycle) {
            if (user_cycle) {
              _controller.measure();
              if (_controller.down()) {
                  break;
              }
              cycleUser();
            }
            yield();
        }
        down_cycle.reset(initial_speed - _level);
        cycleDown();
    }
}

Game::Input::Input() : last_was_rot(false) {
}

int Game::Input::shift() {
    static constexpr float x_threshold = 2;
    float shift = Sensor.getAccelX_mss();
    if (shift > x_threshold)
        return 1;
    else if (shift < -x_threshold)
        return -1;
    return 0;
}

static constexpr float rot_threshold = -3;
void Game::Input::measure() {
    last_was_rot = Sensor.getGyroX_rads() < rot_threshold;
    Sensor.readSensor();
}

bool Game::Input::rotation() {
    bool is_rot = Sensor.getGyroX_rads() < rot_threshold;
    return last_was_rot && last_was_rot != is_rot;
}

bool Game::Input::down() {
    static constexpr float y_threshold = 7;
    return Sensor.getAccelY_mss() > y_threshold;
}

Piece &Game::moveToStart(Piece &piece) { return piece.move({_start_pos, 0}); }
Piece Game::moveToStart(Piece &&piece) { return piece.move({_start_pos, 0}); }

void Game::drawBlock(IntVec at, uint32_t color) {
    static constexpr size_t left_bound = (Display::width - Board::width * block_size) / 2;
    for (int dx = 0; dx < block_size; ++dx) {
        for (int dy = 0; dy < block_size; ++dy) {
            tft.drawPixel(left_bound + at[0] * block_size + dx, at[1] * block_size + dy, color);
        }
    }
}

void Game::drawPiece(Piece piece) {
    for (int i = 0; i < 4; ++i) {
        drawBlock(piece[i], piece.color());
    }
}

void Game::clearPiece(Piece piece) {
    for (int i = 0; i < 4; ++i) {
        drawBlock(piece[i], TFT_BLACK);
    }
}

void Game::drawBoard() {
    for (int y = 0; y < Board::height; ++y) {
        for (int x = 0; x < Board::width; ++x) {
            drawBlock({x,y}, _board.at({x,y}).color());
        }
    }
}
