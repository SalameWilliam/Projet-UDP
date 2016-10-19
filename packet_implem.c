#include <stdlib.h>
#include "packet_interface.h"
#include <netinet/in.h>
#include <memory.h>
#include <unistd.h>

struct __attribute__((__packed__)) pkt {
     ptypes_t type : 3;
     uint8_t window : 5,
            seqnum : 8;
    uint32_t timestamp;
    uint16_t length : 16;
    char *payload;
    uint32_t crc;
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
    read(0,(void*)data, sizeof(data));
    return PKT_OK;
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
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