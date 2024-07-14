#ifndef RAWSOCKET_H
#define RAWSOCKET_H

int criaRawSocket(char* nome_interface_rede);

int setaTimeout(int soquete, int timeoutMillis);

#endif