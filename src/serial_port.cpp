/**
 * @file serial_port.cpp
 *
 * @brief Serial interface functions
 *
 * Functions for opening, closing, reading and writing via serial ports
 *
 */


// ------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------

#include "serial_port.h"
#include <fcntl.h>   // File control definitions
#include <termios.h> // POSIX terminal control definitions
#include <pthread.h> // This uses POSIX Threads
#include <signal.h>
#include <time.h> 

//#include "time_utils.h"


// ------------------------------------------------------------------------
//   Con/De structors
// ------------------------------------------------------------------------
Serial_Port::Serial_Port(char *&uart_name_ , int baudrate_)
{
    initialize_defaults();
    uart_name = uart_name_;
    baudrate  = baudrate_;

    open_serial();

    fdsR[0].fd = fd;
    fdsR[0].revents = POLLIN;
    fdsW[0].fd = fd;
    fdsW[0].revents = POLLOUT;
}

Serial_Port::Serial_Port()
{
    initialize_defaults();
    open_serial();
    
    fdsR[0].fd = fd;
    fdsR[0].revents = POLLIN;
    fdsW[0].fd = fd;
    fdsW[0].revents = POLLOUT;
}

Serial_Port::~Serial_Port()
{
    // destroy mutex
}

void Serial_Port::initialize_defaults()
{
    // Initialize attributes
    debug  = true;
    fd     = -1;
    status = SERIAL_PORT_CLOSED;

    uart_name = (char*)"/dev/ttyUSB0";
    baudrate  = 57600;

}

//
// readBytes
//
int Serial_Port::readBytes(char* buff, unsigned int NBytes)
{
    int result = 0;
    // Wait until new data is on the device (max timeout ms)
    //int ret = poll(fdsR, 1, timeout);
    //if (ret)
    //{
        result = read(fd, buff, NBytes);
    //}
    return result;
}


int Serial_Port::writeBytes(char* buff, unsigned len)
{
    // Write buffer to serial port, locks port while writing
    int writtenB = _write_port(buff,len);

    // Return the number of bytes effectively written
    return writtenB;
}



// ------------------------------------------------------------------------------
//   Open Serial Port
// ------------------------------------------------------------------------------
/**
 * throws EXIT_FAILURE if could not open the port
 */
void Serial_Port::open_serial()
{

    // --------------------------------------------------------------------------
    //   OPEN PORT
    // --------------------------------------------------------------------------
    printf("OPEN PORT\n");

    fd = _open_port(uart_name);

    // Check success
    if (fd == -1)
    {
        printf("failure, could not open port.\n");
        return;
        //throw EXIT_FAILURE;
    }

    // --------------------------------------------------------------------------
    //   SETUP PORT
    // --------------------------------------------------------------------------
    bool success = _setup_port(baudrate, 8, 1, false, false);

    // --------------------------------------------------------------------------
    //   CHECK STATUS
    // --------------------------------------------------------------------------
    if (!success)
    {
        printf("failure, could not configure port.\n");
        //throw EXIT_FAILURE;
    }
    if (fd <= 0)
    {
        printf("Connection attempt to port %s with %d baud, 8N1 failed, exiting.\n", uart_name, baudrate);
        //throw EXIT_FAILURE;
    }

    // --------------------------------------------------------------------------
    //   CONNECTED!
    // --------------------------------------------------------------------------
    printf("Connected to %s with %d baud, 8 data bits, no parity, 1 stop bit (8N1)\n", uart_name, baudrate);

    status = true;

    printf("\n");

    return;

}


// ------------------------------------------------------------------------------
//   Close Serial Port
// ------------------------------------------------------------------------------
void Serial_Port::close_serial()
{
    printf("CLOSE PORT\n");

    usleep(10000);

    int result = close(fd);

    if ( result )
    {
        fprintf(stderr,"WARNING: Error on port close (%i)\n", result );
    }

    status = false;

    printf("\n");

}


// ------------------------------------------------------------------------------
//   Convenience Functions
// ------------------------------------------------------------------------------

void Serial_Port::stop()
{
    close_serial();
}


// ------------------------------------------------------------------------------
//   Quit Handler
// ------------------------------------------------------------------------------
void
Serial_Port::
handle_quit( int sig )
{
    try {
        stop();
    }
    catch (int error) {
        fprintf(stderr,"Warning, could not stop serial port\n");
    }
}


// ------------------------------------------------------------------------------
//   Helper Function - Open Serial Port File Descriptor
// ------------------------------------------------------------------------------
// Where the actual port opening happens, returns file descriptor 'fd'
int
Serial_Port::
_open_port(const char* port)
{
    // Open serial port
    // O_RDWR - Read and write
    // O_NOCTTY - Ignore special chars like CTRL-C
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

    // Check for Errors
    if (fd == -1)
    {
        /* Could not open the port. */
        return(-1);
    }
    // Finalize
    else
    {
        fcntl(fd, F_SETFL, 0);
    }

    // Done!
    return fd;
}

// ------------------------------------------------------------------------------
//   Helper Function - Setup Serial Port
// ------------------------------------------------------------------------------
// Sets configuration, flags, and baud rate
bool
Serial_Port::
_setup_port(int baud, int data_bits, int stop_bits, bool parity, bool hardware_control)
{
    // Check file descriptor
    if(!isatty(fd))
    {
        fprintf(stderr, "\nERROR: file descriptor %d is NOT a serial port\n", fd);
        return false;
    }

    // Read file descritor configuration
    struct termios  config;
    if(tcgetattr(fd, &config) < 0)
    {
        fprintf(stderr, "\nERROR: could not read configuration of fd %d\n", fd);
        return false;
    }

    // Input flags - Turn off input processing
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
            INLCR | PARMRK | INPCK | ISTRIP | IXON);

    // Output flags - Turn off output processing
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
            ONOCR | OFILL | OPOST);

#ifdef OLCUC
    config.c_oflag &= ~OLCUC;
#endif

#ifdef ONOEOT
    config.c_oflag &= ~ONOEOT;
#endif

    // No line processing:
    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

    // Turn off character processing
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= CS8;

    // One input byte is enough to return from read()
    // Inter-character timer off
    config.c_cc[VMIN]  = 0;
    config.c_cc[VTIME] = 0; // was 0

    // Get the current options for the port
    ////struct termios options;
    ////tcgetattr(fd, &options);

    // Apply baudrate
    switch (baud)
    {
        case 1200:
            if (cfsetispeed(&config, B1200) < 0 || cfsetospeed(&config, B1200) < 0)
            {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return false;
            }
            break;
        case 1800:
            cfsetispeed(&config, B1800);
            cfsetospeed(&config, B1800);
            break;
        case 9600:
            cfsetispeed(&config, B9600);
            cfsetospeed(&config, B9600);
            break;
        case 19200:
            cfsetispeed(&config, B19200);
            cfsetospeed(&config, B19200);
            break;
        case 38400:
            if (cfsetispeed(&config, B38400) < 0 || cfsetospeed(&config, B38400) < 0)
            {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return false;
            }
            break;
        case 57600:
            if (cfsetispeed(&config, B57600) < 0 || cfsetospeed(&config, B57600) < 0)
            {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return false;
            }
            break;
        case 115200:
            if (cfsetispeed(&config, B115200) < 0 || cfsetospeed(&config, B115200) < 0)
            {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return false;
            }
            break;

        case 230400:
            if (cfsetispeed(&config, B230400) < 0 || cfsetospeed(&config, B230400) < 0)
            {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return false;
            }
            break;

            // These two non-standard (by the 70'ties ) rates are fully supported on
            // current Debian and Mac OS versions (tested since 2010).
        case 460800:
            if (cfsetispeed(&config, B460800) < 0 || cfsetospeed(&config, B460800) < 0)
            {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return false;
            }
            break;
        case 921600:
            if (cfsetispeed(&config, B921600) < 0 || cfsetospeed(&config, B921600) < 0)
            {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return false;
            }
            break;
        default:
            fprintf(stderr, "ERROR: Desired baud rate %d could not be set, aborting.\n", baud);
            return false;

            break;
    }

    // Finally, apply the configuration
    if(tcsetattr(fd, TCSAFLUSH, &config) < 0)
    {
        fprintf(stderr, "\nERROR: could not set configuration of fd %d\n", fd);
        return false;
    }

    // Done!
    return true;
}



// ------------------------------------------------------------------------------
//   Read Port with Lock
// ------------------------------------------------------------------------------
int Serial_Port::_read_port(uint8_t &cp)
{


    /* non-blocking read. read may return negative values */
    int result = read(fd, &cp, 1);
    if ( result == -1 )
    {
        //printf("%s, %s : _read_port  : Failed to read on the serial\n",__FILE__,__LINE__);
    }
    return result;
}


// ------------------------------------------------------------------------------
//   Write Port with Lock
// ------------------------------------------------------------------------------
int Serial_Port::_write_port(char *buf, unsigned &len)
{

    // Write packet via serial link
    int writtenB = write(fd, buf, len);

    if ( writtenB == -1 )
    {
        //printf("%s, %s : _write_port  : Failed to write on the serial\n",__FILE__,__LINE__);
        return writtenB;
    }
    // Wait until all data has been written
    //tcdrain(fd);


    return writtenB;
}


