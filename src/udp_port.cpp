/** 
 * 
 * \file udp_port.cpp
 *
 */

// ---------------------------------------------------------------------------
// Includes
// ---------------------------------------------------------------------------
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include "udp_port.h"

// ---------------------------------------------------------------------------
// Costructor / Destructor
// ---------------------------------------------------------------------------

Udp_Port::Udp_Port()
{
    memset(&locAddr, 0, sizeof(locAddr));
    memset(&remAddr, 0, sizeof(remAddr));
    
    sock = -1;
}

void Udp_Port::InitializeOutputPort(const char* ip_addr, uint32_t w_port)
{
    printf("Setting UDP Output port : IP_DEST %s | PORT %d\n", ip_addr, w_port);
    read_port = -1;
    write_port = w_port;
    
    // Endpoint for communication file descriptor
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Initialize remote sockaddr_in structure
    memset(&remAddr, 0, sizeof(remAddr));

    remAddr.sin_family = AF_INET;
    remAddr.sin_port = htons(w_port);
    remAddr.sin_addr.s_addr = inet_addr(ip_addr);
    
    // Perfoming a non blocking access
    if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
    {
        fprintf(stderr, "Udp_Port::Udp_Port error: unable to set nonblocking %s\n",
                    strerror(errno));
        close(sock);
        exit(EXIT_FAILURE);
    }
}

void Udp_Port::InitializeInputPort(const char* ip_addr, uint32_t r_port)
{
    //printf("Setting UDP Input port : IP_DEST %s | PORT %d\n", ip_addr, r_port);
    read_port = r_port;
    write_port = -1;
    
    // Endpoint for communication file descriptor
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Initialize local sockaddr_in structure
    memset(&locAddr, 0, sizeof(locAddr));

    locAddr.sin_family = AF_INET;
    locAddr.sin_port = htons(r_port);
    locAddr.sin_addr.s_addr = INADDR_ANY;
    
    // Binding the socket to read
    if (-1 == bind(sock,(struct sockaddr *)&locAddr,sizeof(struct sockaddr)))
    {
        fprintf(stderr,"Udp_Port::Udp_Port error: bind failed! (IP %s | R %d | W %d)\n", 
                ip_addr, read_port, write_port);
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    // Perfoming a non blocking access
    if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
    {
        fprintf(stderr, "Udp_Port::Udp_Port error: unable to set nonblocking %s\n",
                    strerror(errno));
        close(sock);
        exit(EXIT_FAILURE);
    } 
}

Udp_Port::Udp_Port(const char* ip_addr, uint32_t r_port, uint32_t w_port)
{
    read_port = r_port;
    write_port = w_port;
    
    //printf("Setting UDP Input/Output port : \n IP_DEST %s | READ PORT %d | WRITE PORT %d\n", 
    //       ip_addr, r_port, w_port);
    // Endpoint for communication file descriptor
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Initialize local sockaddr_in structure
    memset(&locAddr, 0, sizeof(locAddr));

    locAddr.sin_family = AF_INET;
    locAddr.sin_port = htons(r_port);
    locAddr.sin_addr.s_addr = INADDR_ANY;

    // Initialize remote sockaddr_in structure
    memset(&remAddr, 0, sizeof(remAddr));

    remAddr.sin_family = AF_INET;
    remAddr.sin_port = htons(w_port);
    remAddr.sin_addr.s_addr = inet_addr(ip_addr);

    // Binding the socket to read
    if (-1 == bind(sock,(struct sockaddr *)&locAddr,sizeof(struct sockaddr)))
    {
        fprintf(stderr,"Udp_Port::Udp_Port error: bind failed! (IP %s | R %d | W %d)\n", 
                ip_addr, r_port, w_port);
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    // Perfoming a non blocking access
    if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
    {
    	fprintf(stderr, "Udp_Port::Udp_Port error: unable to set nonblocking %s\n",
    	strerror(errno));
    	close(sock);
    	exit(EXIT_FAILURE);
    } 
}

Udp_Port::~Udp_Port()
{
    close(sock);
}

int Udp_Port::send_mav_mess(mavlink_message_t* message)
{
    int bytes = 0;
    unsigned int len = mavlink_msg_to_send_buffer(buf, message);
    bytes = sendto(sock, buf, len, 0,
            (struct sockaddr*)&remAddr, sizeof(struct sockaddr_in));

    return bytes;
}

int Udp_Port::writeBytes(char * data, unsigned int len)
{
    int bytes = 0;
    bytes = sendto(sock,data,len,0,(struct sockaddr*)&remAddr,sizeof(struct sockaddr_in));
    return bytes;
}

int Udp_Port::readBytes(char* data, unsigned int len)
{
    int i;
    char rec_buf[len];
    memset(rec_buf, 0, len);

    ssize_t recsize = 0;

    recsize = recvfrom(sock, (void* )rec_buf, len, 0, 0, 0);

    if (recsize > 0)
    {
        for(i = 0 ; i < recsize ; i++)
        {
            data[i] = rec_buf[i];
            //printf(" %x ",data[i]);
        }
        //printf("\n");
    }
    
    if (recsize == -1)  
        if (errno != EAGAIN)
            perror("Error reading UDP\n");

    return recsize;

}

int Udp_Port::getWport()
{
    return write_port;
}




