/*

Serial Transmitter for Spiri Robotics

This script will continously output a double from a text file
over a 433MHz 100 mW antenna connected through USB-A.

Written by Jasper Grant
2024-02-28

*/

// Libraries for serial communications
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

// Libraries to read double from file as stream
#include <iostream>
#include <fstream>

// Macro to define the serial port used
#define PORT "/dev/ttyUSB0"

int main()
{
    // Open serial port to write only
    int serial_port = open(PORT, O_WRONLY);

    // Error check Serial Port
    if (serial_port < 0)
    {
        printf("Could not open serial port %s, error number %i: %s\n", PORT, errno, strerror(errno));
    }

    struct termios tty;

    // Error check current Serial Port settings
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Could not get existing Serial Port settings, error number %i: %s\n", errno, strerror(errno));
    }

    // Configure modes

    // Control modes
    tty.c_cflag &= ~PARENB;  // Set no parity bit
    tty.c_cflag &= ~CSTOPB;  // Set one stop bit
    tty.c_cflag != CS8;      // Set 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    tty.c_cflag != CLOCAL;   // Ignore ctrl lines
    // Local modes
    tty.c_lflag &= ~ICANON; // Turn off canonical mode
    tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT, and SUSP
    // Output modes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes
    tty.c_oflag &= ~ONLCR; // Stop newline from being interpreted as return/line feed

    // Set baud rate
    cfsetspeed(&tty, B576000);

    // Error check set Serial Port settings
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Could not set Serial Port settings, error number %i: %s\n", errno, strerror(errno));
    }

    // Read double from file
    std::ifstream fin("double.txt");
    double file_double;
    fin >> file_double;

    // Continuously transmit double
    for (;;)
    {
        int bytes_written = write(serial_port, &file_double, sizeof(file_double));
        if (bytes_written == 8)
        {
            printf("Successfully broadcast double\n");
        }
        else if (bytes_written == -1)
        {
            printf("An error occured.\n");
        }
        else
        {
            printf("An incorrect number of bytes was written: %d\n", bytes_written);
        }
    }
}