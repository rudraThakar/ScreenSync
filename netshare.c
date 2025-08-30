// // netshare.c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <pthread.h>

// #define BUFFER_SIZE 65507  // Maximum UDP packet size

// typedef struct {
//     int sockfd;
//     int is_server;
//     struct sockaddr_in addr;
// } connection_t;

// // Create a socket and prepare for connection
// connection_t* create_connection(const char* ip, int port, int is_server) {
//     connection_t* conn = (connection_t*)malloc(sizeof(connection_t));
//     if (!conn) return NULL;
    
//     conn->is_server = is_server;
//     conn->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//     if (conn->sockfd < 0) {
//         perror("Socket creation failed");
//         free(conn);
//         return NULL;
//     }
    
//     // Set up address structure
//     memset(&conn->addr, 0, sizeof(conn->addr));
//     conn->addr.sin_family = AF_INET;
//     conn->addr.sin_port = htons(port);
    
//     if (is_server) {
//         conn->addr.sin_addr.s_addr = INADDR_ANY;
//         if (bind(conn->sockfd, (struct sockaddr*)&conn->addr, sizeof(conn->addr)) < 0) {
//             perror("Bind failed");
//             close(conn->sockfd);
//             free(conn);
//             return NULL;
//         }
//     } else {
//         if (inet_pton(AF_INET, ip, &conn->addr.sin_addr) <= 0) {
//             perror("Invalid address");
//             close(conn->sockfd);
//             free(conn);
//             return NULL;
//         }
//     }
    
//     return conn;
// }

// // Send data
// int send_data(connection_t* conn, const unsigned char* data, size_t length) {
//     if (!conn || !data) return -1;
    
//     ssize_t sent = sendto(conn->sockfd, data, length, 0, 
//                          (struct sockaddr*)&conn->addr, sizeof(conn->addr));
//     return (int)sent;
// }

// // Receive data
// int receive_data(connection_t* conn, unsigned char* buffer, size_t buffer_size) {
//     if (!conn || !buffer) return -1;
    
//     socklen_t addr_len = sizeof(conn->addr);
//     ssize_t received = recvfrom(conn->sockfd, buffer, buffer_size, 0, 
//                                (struct sockaddr*)&conn->addr, &addr_len);
//     return (int)received;
// }

// // Close connection
// void close_connection(connection_t* conn) {
//     if (conn) {
//         close(conn->sockfd);
//         free(conn);
//     }


// }






// netshare.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Platform-specific includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <BaseTsd.h>   // for SSIZE_T
    typedef SSIZE_T ssize_t;
    #pragma comment(lib, "ws2_32.lib")
    #define close closesocket
    #define SHUT_RDWR SD_BOTH
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <pthread.h>
#endif


#define BUFFER_SIZE 65507  // Maximum UDP packet size

typedef struct {
    int sockfd;
    int is_server;
    struct sockaddr_in addr;
} connection_t;

// Initialize networking (Windows only)
void init_networking() {
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        exit(1);
    }
#endif
}

// Cleanup networking (Windows only)
void cleanup_networking() {
#ifdef _WIN32
    WSACleanup();
#endif
}

// Create a socket and prepare for connection
connection_t* create_connection(const char* ip, int port, int is_server) {
    init_networking();
    
    connection_t* conn = (connection_t*)malloc(sizeof(connection_t));
    if (!conn) return NULL;
    
    conn->is_server = is_server;
    conn->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (conn->sockfd < 0) {
        perror("Socket creation failed");
        free(conn);
        return NULL;
    }
    
    // Set up address structure
    memset(&conn->addr, 0, sizeof(conn->addr));
    conn->addr.sin_family = AF_INET;
    conn->addr.sin_port = htons(port);
    
    if (is_server) {
        conn->addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(conn->sockfd, (struct sockaddr*)&conn->addr, sizeof(conn->addr)) < 0) {
            perror("Bind failed");
            close(conn->sockfd);
            free(conn);
            return NULL;
        }
    } else {
        if (inet_pton(AF_INET, ip, &conn->addr.sin_addr) <= 0) {
            perror("Invalid address");
            close(conn->sockfd);
            free(conn);
            return NULL;
        }
    }
    
    return conn;
}

// Send data
int send_data(connection_t* conn, const unsigned char* data, size_t length) {
    if (!conn || !data) return -1;
    
    ssize_t sent = sendto(conn->sockfd, data, length, 0, 
                         (struct sockaddr*)&conn->addr, sizeof(conn->addr));
    return (int)sent;
}

// Receive data
int receive_data(connection_t* conn, unsigned char* buffer, size_t buffer_size) {
    if (!conn || !buffer) return -1;
    
    socklen_t addr_len = sizeof(conn->addr);
    ssize_t received = recvfrom(conn->sockfd, buffer, buffer_size, 0, 
                               (struct sockaddr*)&conn->addr, &addr_len);
    return (int)received;
}

// Close connection
void close_connection(connection_t* conn) {
    if (conn) {
        close(conn->sockfd);
        free(conn);
    }
    cleanup_networking();
}





// // netshare.c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// // Platform-specific includes
// #ifdef _WIN32
//     #include <winsock2.h>
//     #include <ws2tcpip.h>
//     #pragma comment(lib, "ws2_32.lib")
//     #define close closesocket
//     #define SHUT_RDWR SD_BOTH
//     typedef int ssize_t;
// #else
//     #include <sys/socket.h>
//     #include <netinet/in.h>
//     #include <arpa/inet.h>
//     #include <unistd.h>
//     #include <pthread.h>
// #endif

// #define BUFFER_SIZE 65507  // Maximum UDP packet size

// typedef struct {
//     int sockfd;
//     int is_server;
//     struct sockaddr_in addr;
// } connection_t;

// // Initialize networking (Windows only)
// void init_networking() {
// #ifdef _WIN32
//     WSADATA wsa_data;
//     if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
//         fprintf(stderr, "WSAStartup failed\n");
//         exit(1);
//     }
// #endif
// }

// // Cleanup networking (Windows only)
// void cleanup_networking() {
// #ifdef _WIN32
//     WSACleanup();
// #endif
// }

// // Create a socket and prepare for connection
// connection_t* create_connection(const char* ip, int port, int is_server) {
//     init_networking();
    
//     connection_t* conn = (connection_t*)malloc(sizeof(connection_t));
//     if (!conn) return NULL;
    
//     conn->is_server = is_server;
//     conn->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//     if (conn->sockfd < 0) {
//         perror("Socket creation failed");
//         free(conn);
//         return NULL;
//     }
    
//     // Set up address structure
//     memset(&conn->addr, 0, sizeof(conn->addr));
//     conn->addr.sin_family = AF_INET;
//     conn->addr.sin_port = htons(port);
    
//     if (is_server) {
//         conn->addr.sin_addr.s_addr = INADDR_ANY;
//         if (bind(conn->sockfd, (struct sockaddr*)&conn->addr, sizeof(conn->addr)) < 0) {
//             perror("Bind failed");
//             close(conn->sockfd);
//             free(conn);
//             return NULL;
//         }
//     } else {
//         if (inet_pton(AF_INET, ip, &conn->addr.sin_addr) <= 0) {
//             perror("Invalid address");
//             close(conn->sockfd);
//             free(conn);
//             return NULL;
//         }
//     }
    
//     return conn;
// }

// // Send data
// int send_data(connection_t* conn, const unsigned char* data, size_t length) {
//     if (!conn || !data) return -1;
    
//     int sent = sendto(conn->sockfd, (const char*)data, (int)length, 0, 
//                      (struct sockaddr*)&conn->addr, sizeof(conn->addr));
//     return sent;
// }

// // Receive data
// int receive_data(connection_t* conn, unsigned char* buffer, size_t buffer_size) {
//     if (!conn || !buffer) return -1;
    
// #ifdef _WIN32
//     int addr_len = sizeof(conn->addr);
// #else
//     socklen_t addr_len = sizeof(conn->addr);
// #endif
    
//     int received = recvfrom(conn->sockfd, (char*)buffer, (int)buffer_size, 0, 
//                            (struct sockaddr*)&conn->addr, &addr_len);
//     return received;
// }

// // Close connection
// void close_connection(connection_t* conn) {
//     if (conn) {
//         close(conn->sockfd);
//         free(conn);
//     }
//     cleanup_networking();
// }