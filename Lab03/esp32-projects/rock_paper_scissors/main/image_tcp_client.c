#include "lwip/err.h"
#include "lwip/sockets.h"

#include "image_tcp_client.h"

#define HOST_IP_ADDR "192.168.201.204"
#define PORT 8081

void image_tcp_client_send(int8_t category_index, int8_t score, uint8_t * buf, size_t size) {
    char host_ip[] = HOST_IP_ADDR;
    int addr_family =  AF_INET;
    int ip_protocol = IPPROTO_IP;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

    int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
    connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));

    send(sock, &category_index, sizeof(int8_t), 0);
    send(sock, &score, sizeof(int8_t), 0);
    
    int bytes_sent = 0;
    int sent;
    uint8_t send_buf[4096];
    size_t send_size;
    while (bytes_sent < size) {
        send_size = (size - bytes_sent < 4096) ? (size - bytes_sent) : 4096;
        memcpy(send_buf, &buf[bytes_sent], send_size);
        sent  = send(sock, send_buf, send_size, 0);
        if (sent < 0) break; 
        bytes_sent += sent;    
    }
    close(sock);
}


