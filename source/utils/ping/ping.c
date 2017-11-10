#include "lwip/opt.h"

#if LWIP_RAW && LWIP_SOCKET

#include "lwip/mem.h"
#include "lwip/raw.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timers.h"
#include "lwip/inet_chksum.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"

#include "ping.h"

/// Echo payload size (this is needed to build a valid IP packet).
LOCAL const uint32_t DATA_SIZE = 32;

/// Used to identify the echo response.
LOCAL const uint16_t ID = 0xA5A5;

/// Timeout in ms.
LOCAL const int32_t timeout = 5000;

/// Counter to detect missing packets.
LOCAL uint16_t icmp_seq = 0;

/// Socket used to send and receive packets.
LOCAL int socket = -1;

/// Delay.
LOCAL uint32_t delay = 0;

/// Assemble an echo request.
LOCAL void prepare_echo(struct icmp_echo_hdr *iecho, uint16_t len) {

  ICMPH_TYPE_SET(iecho, ICMP_ECHO);
  ICMPH_CODE_SET(iecho, 0);
  iecho->chksum = 0;
  iecho->id = ID;
  icmp_seq++;
  iecho->seqno = htons(icmp_seq);

  size_t data_len = len - sizeof(struct icmp_echo_hdr);

  for (size_t i = 0; i < data_len; i++) {
    ((uint8_t*) iecho)[sizeof(struct icmp_echo_hdr) + i] = (uint8_t) i;
  }

  iecho->chksum = inet_chksum(iecho, len);
}

LOCAL err_t init(void) {

  if (socket < 0) {
    if ((socket = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
      return ERR_MEM;
    }

    if (lwip_setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
      lwip_close(socket);
      socket = -1;
      return ERR_VAL;
    }
  }

  return ERR_OK;
}

LOCAL err_t ping_send(ip_addr_t *addr) {
  err_t err;
  struct icmp_echo_hdr *iecho;
  struct sockaddr_in to;
  size_t ping_size = sizeof(struct icmp_echo_hdr) + DATA_SIZE;

  iecho = (struct icmp_echo_hdr *) mem_malloc((mem_size_t )ping_size);

  if (!iecho) {
    return ERR_MEM;
  }

  prepare_echo(iecho, (uint16_t) ping_size);

  to.sin_len = sizeof(to);
  to.sin_family = AF_INET;
  inet_addr_from_ipaddr(&to.sin_addr, addr);

  err = lwip_sendto(socket, iecho, ping_size, 0, (struct sockaddr*) &to, sizeof(to));
  delay = sys_now();

  mem_free(iecho);

  return ((err > 0) ? ERR_OK : ERR_VAL);
}

LOCAL err_t ping_recv() {
  size_t len;
  struct ip_hdr *iphdr;
  struct icmp_echo_hdr *iecho;

  // 64 bytes are more than enough.
  uint8_t buf[64];

  // Code bellow will receive and validate the echo response. A for loop is used to remove outdated
  // ICMP packets that resides in the raw socket buffer.
  for (uint8_t i = 0; i < 10; i++) {

    len = lwip_recv(socket, buf, sizeof(buf), 0);

    // A timeout will make len < 0.
    if (len <= 0)
      return ERR_TIMEOUT;

    if (len >= (int) (sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr))) {
      iphdr = (struct ip_hdr *) buf;
      iecho = (struct icmp_echo_hdr *) (buf + (IPH_HL(iphdr) * 4));

      // Check if payload is valid and expected.
      if ((iecho->id == ID) && (iecho->seqno == htons(icmp_seq))) {
        delay = sys_now() - delay;
        return ERR_OK;
      }
    }
  }

  // This will be reached if the raw socket buffer is full of old requests or other random ICMP
  // packets.
  return ERR_TIMEOUT;
}

int32_t ping(ip_addr_t *addr) {

  if (init() != ERR_OK)
    return PING_ERROR_SOCKET;

  if (ping_send(addr) != ERR_OK)
    return PING_ERROR_SEND;

  if (ping_recv() != ERR_OK)
    return PING_ERROR_RECV;

  return delay;
}

#endif /* LWIP_RAW && LWIP_SOCKET */
