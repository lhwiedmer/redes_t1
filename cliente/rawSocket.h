#ifndef RAWSOCKET_H
#define RAWSOCKET_H

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <sys/time.h>
#include <net/if.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "frame.h"

int criaRawSocket(char* nome_interface_rede);

int setaTimeout(int soquete, int timeoutMillis);

void enviaPacote(int soquete, struct frame *f);

struct frame *recebePacote(int soquete);

uint8_t calculaCrc8(uint8_t *data, uint8_t tam);

#endif