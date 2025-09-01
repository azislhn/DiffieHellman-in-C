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
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Parameter publik (disepakati)
    long long p = 23; // Bilangan prima
    long long g = 5;  // Generator

    printf("Parameter Publik:\n");
    printf("P = %lld\n", p);
    printf("G = %lld\n\n", g);

    // Membuat socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Gagal membuat socket");
        exit(EXIT_FAILURE);
    }

    // Mengatur alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    // Bind socket ke alamat
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind gagal");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Mendengarkan koneksi masuk
    listen(server_socket, 3);
    printf("Menunggu koneksi dari klien...\n");

    // Menerima koneksi klien
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Accept gagal");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Klien terhubung.\n\n");

    // 1. Menghasilkan kunci privat acak untuk server (a)
    srand(time(NULL));
    long long private_key_server = rand() % (p - 1) + 1; // a
    printf("Kunci Privat Server (a): %lld\n", private_key_server);

    // 2. Menghitung kunci publik server (A)
    long long public_key_server = power(g, private_key_server, p); // A = g^a mod p
    printf("Kunci Publik Server (A = G^a mod P): %lld\n", public_key_server);

    // Mengirim kunci publik server ke klien
    send(client_socket, &public_key_server, sizeof(public_key_server), 0);
    printf("Mengirim Kunci Publik Server (A) ke Klien...\n");

    // Menerima kunci publik klien
    long long public_key_client;
    recv(client_socket, &public_key_client, sizeof(public_key_client), 0);
    printf("Menerima Kunci Publik Klien (B): %lld\n", public_key_client);

    // 3. Menghitung kunci rahasia bersama
    long long shared_secret_server = power(public_key_client, private_key_server, p); // S = B^a mod p
    printf("\nKunci Rahasia Bersama di Server (S = B^a mod P): %lld\n\n", shared_secret_server);

    close(client_socket);
    close(server_socket);
    return 0;
}