#include "dosbox.h"
#include "inout.h"
#include "pic.h"
#include "setup.h"

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdint>
#include <termios.h>
#include <thread>
#include <atomic>
#include <chrono>

#include "mame/emu.h"
#include "mame/sn76496.h"

#define SOUND_CLOCK (14318180 / 4)
#define SERIAL_FIFO_SIZE 4096

static sn76496_device device_sn76496(machine_config(), 0, 0, SOUND_CLOCK);
static sn76496_base_device* activeDevice = &device_sn76496;
#define device (*activeDevice)

static int serial_fd = -1;
static uint8_t serial_fifo[SERIAL_FIFO_SIZE];
static volatile int fifo_w = 0;
static volatile int fifo_r = 0;
static std::atomic<bool> flush_thread_running(false);
static std::thread flush_thread;

// Initialize serial port
static bool InitSerialPort() {
    if (serial_fd >= 0) return true;

    serial_fd = open("/dev/ttyU0", O_WRONLY | O_NOCTTY | O_NONBLOCK);
    if (serial_fd < 0) {
        perror("Failed to open /dev/ttyU0 (bytes will be dropped)");
        return false;
    }

    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0) {
        perror("tcgetattr failed");
        close(serial_fd);
        serial_fd = -1;
        return false;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD | CSTOPB | CRTSCTS);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY | IGNBRK);
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr failed");
        close(serial_fd);
        serial_fd = -1;
        return false;
    }

    return true;
}

// Queue a byte (safe in port handler)
static inline void SerialQueueByte(uint8_t data) {
    serial_fifo[fifo_w & (SERIAL_FIFO_SIZE - 1)] = data;
    fifo_w++;
}

// Flush bytes to serial
static void SerialFlush() {
    if (serial_fd < 0) return;
    while (fifo_r != fifo_w) {
        uint8_t b = serial_fifo[fifo_r & (SERIAL_FIFO_SIZE - 1)];
        ssize_t n = write(serial_fd, &b, 1);
        if (n < 0) break;
        fifo_r++;
    }
}

// Thread function to flush FIFO periodically
static void FlushThreadFunc() {
    while (flush_thread_running) {
        SerialFlush();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// SN76496 write handler
static void SN76496Write(Bitu /*port*/, Bitu data, Bitu /*iolen*/) {
    device.write(data);
    SerialQueueByte((uint8_t)data);
}

// Minimal TANDYSOUND module
class TANDYSOUND: public Module_base {
private:
    IO_WriteHandleObject WriteHandler;

public:
    TANDYSOUND(Section* configuration): Module_base(configuration) {
        InitSerialPort();
        WriteHandler.Install(0xc0, SN76496Write, IO_MB, 2);

        ((device_t&)device).device_start();

        // Start flush thread
        flush_thread_running = true;
        flush_thread = std::thread(FlushThreadFunc);
    }

    ~TANDYSOUND() {
        flush_thread_running = false;
        if (flush_thread.joinable()) flush_thread.join();
        if (serial_fd >= 0) close(serial_fd);
        serial_fd = -1;
    }
};

static TANDYSOUND* test;

void TANDYSOUND_ShutDown(Section* /*sec*/) {
    delete test;
}

void TANDYSOUND_Init(Section* sec) {
    test = new TANDYSOUND(sec);
    sec->AddDestroyFunction(&TANDYSOUND_ShutDown, true);
}

// BIOS stub
bool TS_Get_Address(Bitu& tsaddr, Bitu& tsirq, Bitu& tsdma) {
    tsaddr = tsirq = tsdma = 0;
    return false;
}

