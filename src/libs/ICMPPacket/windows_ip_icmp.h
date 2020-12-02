#ifndef IP_ICMP_H
#define IP_ICMP_H

#include <QtGlobal>

#if !defined(Q_OS_WIN)
#error This file can only be used by windows builds
#endif

#define NOMINMAX

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdint.h>

#pragma pack(push, 1)

struct ip {
    uint8_t ip_vhl;                 /* version << 4 | header length >> 2 */
    uint8_t ip_tos;                 /* type of service */
    uint16_t ip_len;                 /* total length */
    uint16_t ip_id;                  /* identification */
    uint16_t ip_off;                 /* fragment offset field */
    uint8_t ip_ttl;                 /* time to live */
    uint8_t ip_p;                   /* protocol */
    uint16_t ip_sum;                 /* checksum */
    struct in_addr ip_src, ip_dst;  /* source and dest address */
};

struct icmp_ra_addr {
    uint32_t ira_addr;
    uint32_t ira_preference;
};

struct icmp {
    uint8_t icmp_type;        /* type of message, see below */
    uint8_t icmp_code;        /* type sub code */
    uint16_t icmp_cksum;        /* ones complement cksum of struct */
    union {
        uint8_t ih_pptr;
        struct in_addr ih_gwaddr;
        struct ih_idseq {
            uint16_t icd_id;    /* network format */
            uint16_t icd_seq; /* network format */
        } ih_idseq;
        int ih_void;

        struct ih_pmtu {
            uint16_t ipm_void;
            uint16_t ipm_nextmtu;
        } ih_pmtu;

        struct ih_rtradv {
            uint8_t irt_num_addrs;
            uint8_t irt_wpa;
            uint16_t irt_lifetime;
        } ih_rtradv;
    } icmp_hun;

    union {
        struct id_ts {
            uint32_t its_otime;
            uint32_t its_rtime;
            uint32_t its_ttime;
        } id_ts;
        struct id_ip {
            struct ip idi_ip;
            /* options and then 64 bits of data */
        } id_ip;
        struct icmp_ra_addr id_radv;
        uint32_t id_mask;
        char id_data[1];
    } icmp_dun;
};

#define ICMP_ECHOREPLY          0
#define ICMP_ECHO               8
#define ICMP_TIMXCEED           11

#pragma pack(pop)

#endif
