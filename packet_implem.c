#include <stdlib.h>
#include <netinet/in.h>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>
#include <zlib.h>
#include "packet_interface.h"

#define HEADER_BYTES 4
#define SIZE_CRC 4

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
    free(pkt);
}

pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
    //TODO : Need to encode stuff en shift bits to make them round that's all. Do not forget to put window and type together to make an eight bit (look at git for more info)

    //prise de type
    ptypes_t type = (ptypes_t) (data[0] >> 5);

    if(type!=PTYPE_DATA && type!= PTYPE_ACK){
        return E_TYPE;
    }

    //prise de Window
    uint8_t wi1=(uint8_t) (data[0] << 3);
    uint8_t window = (wi1 >> 3);

    uint8_t seqnum =(uint8_t) data[1];

    uint16_t length =(uint16_t) (data[2] << 8|data[3]);

    size_t size_payload = len - HEADER_BYTES - SIZE_CRC;

    char *buffer = (char*)malloc(size_payload);

    memcpy((void *)buffer,(void *)&data[4],size_payload);

    //uint32_t current_crc = (uint32_t) crc32(0, (const void*)data, HEADER_BYTES+length);

    uint32_t old_crc;

    memcpy((void *)&old_crc,(void *)&data[HEADER_BYTES + size_payload],SIZE_CRC);

    //if(old_crc != current_crc)
        //return E_CRC;

    pkt_set_type(pkt,type);
    pkt_set_seqnum(pkt,seqnum);
    pkt_set_window(pkt,window);
    pkt_set_length(pkt,length);
    pkt_set_payload(pkt,buffer,(uint16_t)size_payload);
    pkt_set_crc(pkt,old_crc);

    return PKT_OK;
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
    //TODO : Decode with taking each byte from the buf en shifting them if they are not round

    if (*len < sizeof(pkt)){
        return E_NOMEM;
    }

    ptypes_t type = pkt_get_type(pkt);
    uint8_t window = pkt_get_window(pkt);

    buf[0] = (type<<5)|window;

    uint8_t seqnum = pkt_get_seqnum(pkt);

    buf[1] = seqnum;

    uint16_t length = pkt_get_length(pkt);

    memcpy((void *)&buf[2],(void *)&length, sizeof(length));

    const char* data = pkt_get_payload(pkt);

    memcpy((void *)&buf[4],data,length);

    //uint32_t crc = (uint32_t) crc32(0, (const void*)buf, HEADER_BYTES+length);

    uint32_t crc = pkt_get_crc(pkt);

    memcpy((void *)&buf[HEADER_BYTES+length],(void *) &crc, SIZE_CRC);

    *len = HEADER_BYTES + length + SIZE_CRC;

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
    pkt->payload = (char *)malloc(length);
    memcpy((void*)pkt->payload,(void*)data,length);
    pkt->length = length;

    return PKT_OK;
}