#include "rawSocket.h"

int criaRawSocket(char* nome_interface_rede) {
    // Cria arquivo para o socket sem qualquer protocolo
    int soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (soquete == -1) {
        fprintf(stderr, "Erro ao criar socket: Verifique se você é root!\n");
        exit(-1);
    }

    int ifindex = if_nametoindex(nome_interface_rede);
 
    struct sockaddr_ll endereco = {0};
    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(soquete, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        fprintf(stderr, "Erro ao fazer bind no socket\n");
        exit(-1);
    }
    
    struct packet_mreq mr = {0};
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        fprintf(stderr, "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente.\n");
        exit(-1);
    }

    return soquete;
}

int setaTimeout(int soquete, int timeoutMillis) {
	struct timeval timeout = { .tv_sec = timeoutMillis / 1000, .tv_usec = (timeoutMillis % 1000) * 1000 }; // 300ms
	return setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout));
}

void enviaPacote(int soquete, struct frame *f) {
    __uint8_t *buffer = frameParaBuffer(f);
    int tamanho = send(soquete, buffer, 4 + f->tam, 0);
    if (tamanho == -1) {
        fprintf(stderr, "Erro ao enviar pacote\n");
        exit(-1);
    }
    free(buffer);
}

struct frame *recebePacote(int soquete) {
    __uint8_t *buffer = malloc(64);
    if (buffer == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(-1);
    }
    int tamanho = recv(soquete, buffer, 31, 0);
    if (tamanho == -1) {
        fprintf(stderr, "Erro ao receber pacote\n");
        exit(-1);
    }
    struct frame *f = bufferParaFrame(buffer);
    free(buffer);
    return f;
}

//implementar o CRC-8

uint8_t calculaCrc8(uint8_t *data, uint8_t tam) {
    uint8_t crc = 0;
    for (int i = 0; i < tam; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void enviaAck(int soquete, uint8_t seq) {
    struct frame *f = cria_frame(0, seq, ACK);
    enviaPacote(soquete, f);
    destroi_frame(f);
}

void enviaNack(int soquete, uint8_t seq) {
    struct frame *f = cria_frame(0, seq, NACK);
    enviaPacote(soquete, f);
    destroi_frame(f);
}

void enviaLista(int soquete, uint8_t seq) {
    struct frame *f = cria_frame(0, seq, LISTA);
    enviaPacote(soquete, f);
    destroi_frame(f);
}

void enviaBaixar(int soquete, char *nome_arquivo, uint8_t seq) {

}

void enviaErro(int soquete, uint8_t seq) {

}
