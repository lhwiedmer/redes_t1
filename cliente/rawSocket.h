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



//funcoes para cada funcionalidade do programa


//ack -> o cliente ou o servidor recebeu o pacote
//nack -> o cliente ou o servidor não recebeu o pacote
//listar -> o cliente pede a lista de arquivos
//baixar -> o cliente pede para baixar um arquivo
//mostrar na tela -> o servidor mostra a lista de arquivos
//erro -> houve algum erro
//descritor -> o servidor envia o tamanho e a data do arquivo
//dados -> o servidor envia o arquivo
//fim de transmissão -> o servidor terminou a transmissão


/* 
   O cliente pode pedir a funcionalidade "baixar", o servidor vai mandar a
   o tamanho e a data do arquivo com a funcionalidade "descritor". Se o
   cliente aceitar(ack) o servidor envia o arquivo com a função dados
*/

/*
  00000 ack
  00001 nack
  01010 lista
  01011 baixar
  10000 mostra na tela
  10001 descritor arquivo
  10010 dados
  11110 fim tx
  11111 erro
*/

void enviaAck(int soquete, uint8_t seq);

void enviaNack(int soquete, uint8_t seq);

void enviaLista(int soquete, uint8_t seq);

void enviaBaixar(int soquete, char *nome_arquivo, uint8_t seq);

void enviaErro(int soquete, uint8_t seq);

#endif