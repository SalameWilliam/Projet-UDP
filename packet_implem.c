#include <stdlib.h>
#include <netinet/in.h>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>
#include "packet_interface.h"


void main(int argc, char** argv){
    pkt_t* oui = pkt_new();
    pkt_set_window(oui,3);
    size_t size1 = sizeof(pkt_get_window(oui));
    pkt_set_seqnum(oui,3);
    size_t size2 = sizeof(pkt_get_type(oui));
    printf("oui");
}

struct __attribute__((__packed__)) pkt {
     ptypes_t type : 3;
     uint8_t window : 5,
            seqnum : 8;
    uint32_t timestamp;
    uint16_t length : 16;
    char *payload;
    uint32_t crc : 32;
};

pkt_t* pkt_new()
{
    return calloc(1, sizeof(struct pkt));
}

void pkt_del(pkt_t *pkt)
{
    free(pkt->payload);
    pkt->payload = NULL;
    free(pkt);
}

pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
    //TODO : Need to encode stuff en shift bits to make them round that's all. Do not forget to put window and type together to make an eight bit (look at git for more info)
    read(0,(void*)data, sizeof(data));
    return PKT_OK;
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
    //TODO : Decode with taking each byte from the buf en shifting them if they are not round
    write(buf,(void*)pkt->type,len);
    return PKT_OK;
}

ptypes_t pkt_get_type  (const pkt_t* pkt)
{
    return  pkt->type;
}

uint8_t  pkt_get_window(const pkt_t* pkt)
{
    return pkt->window;
}

uint8_t  pkt_get_seqnum(const pkt_t* pkt)
{
    return pkt->seqnum;
}

uint16_t pkt_get_length(const pkt_t* pkt)
{
    return ntohs(pkt->length);
}

uint32_t pkt_get_timestamp   (const pkt_t* pkt)
{
    return ntohl(pkt->timestamp);
}

uint32_t pkt_get_crc   (const pkt_t* pkt)
{
    return ntohl(pkt->crc);
}

const char* pkt_get_payload(const pkt_t* pkt)
{
    return pkt->payload;
}


pkt_status_code pkt_set_type(pkt_t *pkt, const ptypes_t type)
{
    pkt->type = type;
    return PKT_OK;
}

pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window)
{
    pkt->window = window;
    return PKT_OK;
}

pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum)
{
    pkt->seqnum = seqnum;
    return PKT_OK;
}

pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length)
{
    pkt->length = htons(length);
    return PKT_OK;
}

pkt_status_code pkt_set_timestamp(pkt_t *pkt, const uint32_t timestamp)
{
    pkt->timestamp = htonl(timestamp);
    return PKT_OK;
}

pkt_status_code pkt_set_crc(pkt_t *pkt, const uint32_t crc)
{
    pkt->crc = htonl(crc);
    return PKT_OK;
}

pkt_status_code pkt_set_payload(pkt_t *pkt,
                                const char *data,
                                const uint16_t length)
{
    memcpy((void*)pkt->payload,(void*)data,length);
    return PKT_OK;
}