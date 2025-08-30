// netshare.h
#ifndef NETSHARE_H
#define NETSHARE_H
#include <stddef.h>   // ✅ for size_t

#ifdef __cplusplus
extern "C" {
#endif

typedef struct connection_t connection_t;

connection_t* create_connection(const char* ip, int port, int is_server);
int send_data(connection_t* conn, const unsigned char* data, size_t length);
int receive_data(connection_t* conn, unsigned char* buffer, size_t buffer_size);
void close_connection(connection_t* conn);

#ifdef __cplusplus
}
#endif

#endif