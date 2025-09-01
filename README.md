# Implementasi Sederhana: Protokol Diffie-Hellman dalam C

Proyek ini mendemonstrasikan implementasi dasar dari **protokol pertukaran kunci Diffie-Hellman** menggunakan soket TCP/IP dalam bahasa C. Tujuannya adalah untuk menunjukkan bagaimana dua pihak (klien dan server) dapat secara aman membuat kunci rahasia bersama melalui saluran komunikasi yang tidak aman.

-----

## 📜 Deskripsi

Protokol Diffie-Hellman adalah metode kriptografi yang memungkinkan dua pihak, yang belum pernah berkomunikasi sebelumnya, untuk bersama-sama membuat kunci rahasia bersama. Kunci ini kemudian dapat digunakan untuk mengenkripsi komunikasi selanjutnya menggunakan algoritma enkripsi simetris.

Implementasi ini terdiri dari dua program:

  * `dh_server.c`: Menunggu koneksi, menghasilkan kunci privat acak, bertukar kunci publik dengan klien, dan menghitung kunci rahasia bersama.
  * `dh_client.c`: Terhubung ke server, menghasilkan kunci privat acak, bertukar kunci publik, dan menghitung kunci rahasia bersama.

-----

## ⚙️ Cara Kerja Protokol

1.  **Parameter Publik**: Klien dan server menyetujui dua bilangan bulat publik: bilangan prima `p` dan basis (generator) `g`. Dalam implementasi ini, nilai `p=23` dan `g=5` di-hardcode.
2.  **Kunci Privat**:
      * Server memilih bilangan bulat rahasia `a`.
      * Klien memilih bilangan bulat rahasia `b`.
3.  **Kunci Publik**:
      * Server menghitung kunci publiknya: $A = g^a \bmod p$.
      * Klien menghitung kunci publiknya: $B = g^b \bmod p$.
4.  **Pertukaran Kunci**: Server dan klien saling mengirimkan kunci publik mereka (`A` dan `B`).
5.  **Kunci Rahasia Bersama**:
      * Server menghitung: $S = B^a \bmod p$.
      * Klien menghitung: $S = A^b \bmod p$.

Kedua belah pihak akan menghasilkan nilai `S` yang identik, yang artinya $B^a \bmod p = A^b \bmod p$ dan menjadi kunci rahasia bersama mereka.

-----

## 🛠️ Prasyarat

Untuk mengkompilasi dan menjalankan proyek ini, Anda memerlukan:

  * Kompilator C (misalnya GCC)
  * Sistem operasi berbasis Unix (Linux, macOS) atau WSL (Windows)

Atau bisa dijalankan langsung dengan Github Codespaces.

-----

## 🚀 Cara Menjalankan

1.  **Buka Terminal**
    Navigasikan ke direktori ini.

2.  **Kompilasi Kode**
    Kompilasi kedua program menggunakan `gcc`:

    ```bash
    gcc dh_server.c -o server
    ```
    ```bash
    gcc dh_client.c -o client
    ```

3.  **Jalankan Server**
    Di satu jendela terminal, jalankan server terlebih dahulu. Server akan menunggu koneksi masuk pada port `8888`.

    ```bash
    ./server
    ```

4.  **Jalankan Klien**
    Buka jendela terminal **kedua** dan jalankan klien untuk terhubung ke server.

    ```bash
    ./client
    ```

5. Bisa sekaligus kompilasi dan jalankan
    ```bash
    gcc dh_server.c -o server && ./server
    ```
    ```bash
    gcc dh_client.c -o client && ./client
    ```

-----

## ✅ Contoh Output

Jika semuanya berjalan dengan benar, Anda akan melihat output yang menunjukkan bahwa kedua belah pihak berhasil menghitung kunci rahasia bersama yang **identik**.

**Output Terminal Server:**

```
Parameter Publik:
P = 23
G = 5

Menunggu koneksi dari klien...
Klien terhubung.

Kunci Privat Server (a): 7
Kunci Publik Server (A = G^a mod P): 17
Mengirim Kunci Publik Server (A) ke Klien...
Menerima Kunci Publik Klien (B): 8

Kunci Rahasia Bersama di Server (S = B^a mod P): 12
```

**Output Terminal Klien:**

```
Parameter Publik:
P = 23
G = 5

Terhubung ke server.

Kunci Privat Klien (b): 6
Kunci Publik Klien (B = G^b mod P): 8
Mengirim Kunci Publik Klien (B) ke Server...
Menerima Kunci Publik Server (A): 17

Kunci Rahasia Bersama di Klien (S = A^b mod P): 12
```

**Catatan**: Pada implementasi ini, nilai kunci privat akan berubah setiap kali program dijalankan karena dihasilkan secara acak, yang juga akan mengubah nilai kunci publik. Namun, **kunci rahasia bersama yang dihasilkan harus selalu sama** di kedua sisi.