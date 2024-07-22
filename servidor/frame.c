//estruturas de dados e funções para manipulação do frame de rede
#include <stdio.h>
#include <stdlib.h>
#include "frame.h"

//LEMBRETE: FAZER O DADOS SEREM ALOCADOS NA PILHA AO INVES DA HEAP

struct frame *cria_frame(unsigned char tam, unsigned char seq, unsigned char tipo) {
	struct frame *f = malloc(sizeof(struct frame));
	if (f == NULL) {
		fprintf(stderr, "Erro ao alocar memória para frame\n");
		exit(-1);
	}
	f->marcadorDeInicio = 126;
	f->tam = tam;
	f->seq = seq;
	f->tipo = tipo;
	f->crc = 0;
	return f;
}

void destroi_frame(struct frame *f) {
	free(f);
}

__uint8_t* frameParaBuffer(struct frame *f) {
	__uint8_t *buffer = (__uint8_t*) f;
	return buffer;
}

void desempacotaBits(__uint8_t *tam, __uint8_t *seq, __uint8_t *tipo, __uint8_t byte1, __uint8_t byte2) {
	*tam = byte1 >> 2;
	*seq = byte1 << 3;
	*seq = *seq | (byte2 >> 5);
	*tipo = byte2 & 31;
}

struct frame *bufferParaFrame(__uint8_t *buffer) {
	__uint8_t tam = 0 , seq = 0, tipo = 0;
	desempacotaBits(&tam, &seq, &tipo, buffer[1], buffer[2]);
	struct frame *f = cria_frame(tam, seq, tipo);
	return f;
}

/*
__uint8_t* frameParaBuffer(struct frame *f) {
	__uint8_t* buffer = malloc(4 + f->tam);
	if (buffer == NULL) {
		fprintf(stderr, "Erro ao alocar memória para buffer\n");
		exit(-1);
	}
	__uint8_t byte1, byte2;
	empacotaBits(f, &byte1, &byte2);
	buffer[0] = f->marcadorDeInicio;
	buffer[1] = byte1;
	buffer[2] = byte2;
	for (int i = 3; i < 3 + f->tam; i++) {
		buffer[i] = f->dados[i - 3];
	}
	buffer[3 + f->tam] = f->crc;
}

// Funcao que empacota os bits de tam, seq e tipo em dois bytes
void empacotaBits(struct frame *f, __uint8_t *byte1, __uint8_t *byte2) {
	*byte1 = 0;
	*byte2 = 0;
	*byte1 = f->tam << 2;
	*byte1 = *byte1 | (f->seq >> 3);
	*byte2 = f->seq << 5;
	*byte2 = *byte2 | f->tipo;
}
*/