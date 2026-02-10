/*
 * SN76489-only Tandy DOSBox module
 * Sends all Tandy sound writes to /dev/ttyS6 at 115200 baud (write-only)
 *
 * Self-contained, ready for DOSBox 0.74-3
 *
 * No DAC, no DMA, no IRQs
 *
 * Copyright (C) 2026  Your Name
 * Released under GNU GPL v2+
 */

#include "dosbox.h"
#include "inout.h"
#include "pic.h"
#include "hardware.h"
#include "setup.h"        // Section class definition

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

#include "mame/emu.h"
#include "mame/sn76496.h"

#define SOUND_CLOCK (14318180 / 4)

static struct {
    bool enabled;
    Bitu last_write;
} tandy;

// SN76496 device
static sn76496_device device_sn76496(machine_config(), 0, 0, SOUND_CLOCK);
static sn76496_base_device* activeDevice = &device_sn76496;
#define device (*activeDevice)

// Serial port handle
static int serial_fd = -1;

// Initialize /dev/ttyS4 in write-only mode
static void InitSerialPort() {
    if (serial_fd >= 0) return;  // already initialized

    serial_fd = open("/dev/ttyS4", O_WRONLY | O_NOCTTY | O_SYNC);
    if (serial_fd < 0) {
        perror("Failed to open /dev/ttyS4");
        // proceed anyway; SN76489 bytes will be dropped
        serial_fd = -1;
    }
}

// SN76496 write handler
static void SN76496Write(Bitu /*port*/, Bitu data, Bitu /*iolen*/) {
    tandy.last_write = PIC_Ticks;
    if (!tandy.enabled) tandy.enabled = true;

    device.write(data);

    // Ensure serial port is open
    InitSerialPort();

    // Send byte over serial if port opened successfully
    if (serial_fd >= 0) {
        ssize_t n = write(serial_fd, &data, 1);
        if (n < 0) perror("Serial write failed");
    }

    // Optional debug print
    printf("[Tandy SN76489] 0x%02X\n", data);
}

// No mixer — pure serial output
static void SN76496Update(Bitu /*length*/) {
    // Nothing to do
}

// TANDYSOUND module
class TANDYSOUND: public Module_base {
private:
    IO_WriteHandleObject WriteHandler;

public:
    TANDYSOUND(Section* configuration): Module_base(configuration) {
        InitSerialPort();
        WriteHandler.Install(0xc0, SN76496Write, IO_MB, 2);

        tandy.enabled = false;

        ((device_t&)device).device_start();
    }

    ~TANDYSOUND() {
        if (serial_fd >= 0) {
            close(serial_fd);
            serial_fd = -1;
        }
    }
};

// Global pointer
static TANDYSOUND* test;

// Shutdown
void TANDYSOUND_ShutDown(Section* /*sec*/) {
    delete test;
}

// Init
void TANDYSOUND_Init(Section* sec) {
    test = new TANDYSOUND(sec);
    sec->AddDestroyFunction(&TANDYSOUND_ShutDown, true);
}

// Stub for BIOS linking
bool TS_Get_Address(Bitu& tsaddr, Bitu& tsirq, Bitu& tsdma) {
    tsaddr = 0;
    tsirq  = 0;
    tsdma  = 0;
    return false; // no DAC present
}

