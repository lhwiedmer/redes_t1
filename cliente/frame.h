#ifndef FRAME_H
#define FRAME_H
#include <stdio.h>
#include <stdlib.h>

#define TAM_MAX 63
#define ACK 0
#define NACK 1
#define LISTA 10
#define BAIXAR 11
#define MOSTRAR_NA_TELA 16
#define DESCRITOR_DE_ARQUIVO 17
#define DADOS 18
#define FIM_DE_TRANSMISSAO 30
#define ERRO 31

struct frame {
	__uint8_t marcadorDeInicio;
	__uint8_t tam: 6;
	__uint8_t seq: 5;
	__uint8_t tipo: 5;
	__uint8_t *dados;
	__uint8_t crc;
};

struct frame *cria_frame(unsigned char tam, unsigned char seq, unsigned char tipo);

void aloca_dados(struct frame *f, unsigned char tam);

void destroi_frame(struct frame *f);

__uint8_t* frameParaBuffer(struct frame *f);

struct frame *bufferParaFrame(__uint8_t *buffer);

#endif