#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>

// Fungsi untuk menghitung (base^exp) % mod
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Parameter publik (harus sama dengan server)
    long long p = 23;
    long long g = 5;

    printf("Parameter Publik:\n");
    printf("P = %lld\n", p);
    printf("G = %lld\n\n", g);

    // Membuat socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Gagal membuat socket");
        exit(EXIT_FAILURE);
    }

    // Mengatur alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Hubungkan ke localhost

    int port = 8888;
    int max_port = 9000;
    int connect_result;

    // Coba konek dari port 8888 sampai max_port
    do {
        server_addr.sin_port = htons(port);
        connect_result = connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (connect_result < 0) {
            port++;
        }
    } while (connect_result < 0 && port <= max_port);

    if (connect_result < 0) {
        fprintf(stderr, "Tidak bisa terhubung ke server di port %d\n", max_port);
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Terhubung ke server di port %d\n\n", port);

    // 1. Menghasilkan kunci privat acak untuk klien (b)
    srand(time(NULL) + 1); // Tambahkan 1 untuk seed yang berbeda dari server
    long long private_key_client = rand() % (p - 2) + 1; // b
    printf("Kunci Privat Klien (b): %lld\n", private_key_client);

    // 2. Menghitung kunci publik klien (B)
    long long public_key_client = power(g, private_key_client, p); // B = g^b mod p
    printf("Kunci Publik Klien (B = G^b mod P): %lld\n", public_key_client);
    
        // Mengirim kunci publik klien ke server
        send(client_socket, &public_key_client, sizeof(public_key_client), 0);
        printf("Mengirim Kunci Publik Klien (B) ke Server...\n");

    // Menerima kunci publik server
    long long public_key_server;
    recv(client_socket, &public_key_server, sizeof(public_key_server), 0);
    printf("Menerima Kunci Publik Server (A): %lld\n", public_key_server);

    // 3. Menghitung kunci rahasia bersama
    long long shared_secret_client = power(public_key_server, private_key_client, p); // S = A^b mod p
    printf("\nKunci Rahasia Bersama di Klien (S = A^b mod P): %lld\n\n", shared_secret_client);

    close(client_socket);
    return 0;
}