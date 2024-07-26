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

void aloca_dados(struct frame *f, unsigned char tam) {
	f->dados = calloc(tam, sizeof(__uint8_t));
	if (f->dados == NULL) {
		fprintf(stderr, "Erro ao alocar memória para dados\n");
		exit(-1);
	}
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

