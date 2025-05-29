#include <iostream>  
#include <fstream>   
#include <string>
#include <iomanip> 
using namespace std;

// struct = bikin tipe data sendiri buat nyimpan info buku
struct Buku {
    string kode, judul, pengarang;
    int tahun;
    Buku* next; // POINTER: nunjuk ke buku berikutnya di linked list
};

// head = penanda awal dari linked list
Buku* head = nullptr; // POINTER utama buat mulai dari buku pertama
string namaFileGlobal; // buat nyimpan nama file yang dipakai

// Fungsi login sederhana
bool login() {
    string username, password;
    cout << "===== LOGIN =====\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    // kalau username dan password cocok, boleh lanjut
    if (username == "admin" && password == "123") {
        cout << "Login berhasil!\n";
        return true;
    } else {
        cout << "Login gagal. Username/password salah.\n";
        return false;
    }
}

// Fungsi buat minta nama file dari user
void tentukanNamaFile() {
    cout << "Masukkan nama file: ";
    cin >> namaFileGlobal; // disimpan di variabel global
}

// Fungsi buat nambah buku ke linked list dan juga simpan ke file
void tambahBuku(int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        cout << "\nData buku ke-" << (i+1) << ":\n";

        Buku* baru = new Buku; // POINTER: bikin node baru dinamis di heap
        cout << "Masukkan Kode Buku     : ";
        cin >> baru->kode;
        cin.ignore(); // buat ngilangin newline di buffer
        cout << "Masukkan Judul Buku    : ";
        getline(cin, baru->judul);
        cout << "Masukkan Pengarang     : ";
        getline(cin, baru->pengarang);
        cout << "Masukkan Tahun Terbit  : ";
        cin >> baru->tahun;

        baru->next = nullptr; // karena ini buku terakhir (sementara)

        if (head == nullptr) {
            head = baru; // kalau list masih kosong, jadiin ini buku pertama
        } else {
            Buku* temp = head; // POINTER bantu buat nyari ujung list
            while (temp->next != nullptr)
                temp = temp->next; // terus maju sampai akhir
            temp->next = baru; // sambungin node terakhir ke buku baru
        }
    }

    // nyimpen semua data yang ada di linked list ke file
    ofstream file(namaFileGlobal, ios::app); // nambah di akhir file
    if (!file) return;

    Buku* temp = head; // mulai dari kepala
    while (temp != nullptr) {
        file << temp->kode << "|" << temp->judul << "|" << temp->pengarang << "|" << temp->tahun << endl;
        temp = temp->next; // lanjut ke node berikutnya
    }
    file.close();

    // Hapus semua node di memori (biar gak numpuk)
    while (head != nullptr) {
        Buku* hapus = head; // nyimpen yang mau dihapus
        head = head->next;  // pindah head ke node berikutnya
        delete hapus;       // hapus dari memori
    }
}

// Fungsi buat baca data dari file ke linked list
void muatDariFile() {
    head = nullptr; // kosongin dulu linked list
    ifstream file(namaFileGlobal); // buka file
    if (!file) return;

    string line, kode, judul, pengarang;
    int tahun;

    while (getline(file, line)) {
        // pisahin data pake posisi '|'
        size_t pos1 = line.find("|");
        size_t pos2 = line.find("|", pos1 + 1);
        size_t pos3 = line.find("|", pos2 + 1);

        kode = line.substr(0, pos1);
        judul = line.substr(pos1 + 1, pos2 - pos1 - 1);
        pengarang = line.substr(pos2 + 1, pos3 - pos2 - 1);
        tahun = stoi(line.substr(pos3 + 1)); // ubah string jadi int

        Buku* baru = new Buku{kode, judul, pengarang, tahun, nullptr}; // POINTER buat node baru

        if (head == nullptr) {
            head = baru; // kalau kosong, jadi kepala
        } else {
            Buku* temp = head; // bantu cari ujung list
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = baru; // sambungkan ke akhir list
        }
    }
    file.close();
}

// Nampilin isi linked list
void tampilkanBuku() {
    if (head == nullptr) return; // kalau kosong, keluar

    cout << "\nDaftar Buku:\n";
    cout << left
         << setw(10) << "Kode"
         << setw(30) << "Judul"
         << setw(25) << "Pengarang"
         << setw(6) << "Tahun" << endl;
    cout << string(71, '=') << endl;
    Buku* temp = head; // POINTER buat mulai dari awal
    while (temp != nullptr) {
        cout << left
             << setw(10) << temp->kode
             << setw(30) << temp->judul
             << setw(25) << temp->pengarang
             << setw(6) << temp->tahun << endl;
        temp = temp->next; // lanjut ke buku berikutnya
    }
}

void cariBuku() {
    if (head == nullptr) { //cek linked list kosong apa engga
        cout << "Data buku kosong.\n";
        return;
    }

    string kodeCari;
    cout << "Masukkan kode buku yang dicari: ";
    cin >> kodeCari; //input kode buku, kalo mau search by yg lain kasi tau yaa :D

    Buku* temp = head; //mulai dri head
    bool ditemukan = false; //nandain ketemu apa engga

    while (temp != nullptr) {
        if (temp->kode == kodeCari) {
            cout << "\nBuku ditemukan:\n";
            cout << "Kode     : " << temp->kode << endl;
            cout << "Judul    : " << temp->judul << endl;
            cout << "Pengarang: " << temp->pengarang << endl;
            cout << "Tahun    : " << temp->tahun << endl;
            ditemukan = true;
            break; //udah ketemu break
        }
        temp = temp->next; //lanjut ke node selanjutnya
    }

    if (!ditemukan) //kalo dari awal sampe akhir gak ketemu
        cout << "Buku dengan kode tersebut tidak ditemukan.\n";
}

void urutBuku() {
    // Cek dulu, kalau datanya kosong atau cuma satu, nggak usah diurutkan
    if (head == nullptr || head->next == nullptr) return;

    bool swapped; // Buat nandain, ada data yang ditukar atau nggak
    do {
        swapped = false; // Awal-awal anggap belum ada yang ditukar
        Buku* curr = head; // Mulai dari buku pertama
        Buku* prev = nullptr; // Ini buat nyimpan buku sebelumnya

        // Selama masih ada buku setelah curr
        while (curr->next != nullptr) {
            // Bandingin judul buku sekarang sama judul buku setelahnya
            if (curr->judul > curr->next->judul) {
                // Kalau judul sekarang lebih "gede", kita tukar
                Buku* nextNode = curr->next; // Simpan dulu buku setelahnya
                curr->next = nextNode->next; // Sambungin curr ke buku setelah nextNode
                nextNode->next = curr; // nextNode sekarang di depan curr

                // Kalau yang ditukar itu buku pertama (head)
                if (prev == nullptr) {
                    head = nextNode; // Ganti head jadi nextNode
                } else {
                    prev->next = nextNode; // Sambungin buku sebelumnya ke nextNode
                }
                swapped = true; // Catat kalau kita udah tukar data
                prev = nextNode; // Pindah prev ke posisi yang baru
            } else {
                prev = curr; // Kalau nggak ditukar, cukup pindah prev ke curr
                curr = curr->next; // Lanjut ke buku berikutnya
            }
        }
    } while (swapped); // Ulangi terus selama masih ada yang ditukar

    cout << "Data buku berhasil diurutkan berdasarkan judul.\n";
}

void hapusBuku() {
    // Cek dulu, kalau list-nya kosong, ya langsung keluar aja
    if (head == nullptr) {
        cout << "Data buku kosong.\n";
        return;
    }

    string kodeHapus;
    // Minta user masukin kode buku yang mau dihapus
    cout << "Masukkan kode buku yang ingin dihapus: ";
    cin >> kodeHapus;

    Buku* temp = head; // Buat jalanin pointer dari kepala list
    Buku* prev = nullptr; // Buat nyimpen node sebelumnya

    // Cari buku yang kodenya sesuai sama yang mau dihapus
    while (temp != nullptr && temp->kode != kodeHapus) {
        prev = temp; // Simpan node sebelumnya
        temp = temp->next; // Lanjut ke node berikutnya
    }

    // Kalau nggak ketemu (udah sampai akhir list)
    if (temp == nullptr) {
        cout << "Buku dengan kode tersebut tidak ditemukan.\n";
        return;
    }

    // Kalau buku yang dihapus itu adalah node pertama (head)
    if (prev == nullptr) {
        head = head->next; // Langsung geser head ke node selanjutnya
    } else {
        // Kalau bukan di awal, sambungkan node sebelumnya ke node setelahnya
        prev->next = temp->next;
    }

    delete temp; // Hapus node dari memori
    cout << "Buku berhasil dihapus.\n";

    // Simpan ulang data yang masih ada ke file
    ofstream file(namaFileGlobal); // Buka file buat ditulis ulang
    Buku* curr = head; // Mulai lagi dari awal
    // Tulis semua data yang masih ada ke dalam file
    while (curr != nullptr) {
        file << curr->kode << "|" << curr->judul << "|" << curr->pengarang << "|" << curr->tahun << endl;
        curr = curr->next; // Lanjut ke buku berikutnya
    }
    file.close(); // Tutup file setelah selesai nulis
}


// Program utama
int main() {
    // Ulang terus sampai berhasil login
    while (!login()) {
        cout << "Silakan coba lagi.\n\n";
    }

    int pilihan;
    do {
        cout << "\n=== MENU PERPUSTAKAAN MINI ===\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Buku\n";
        cout << "3. Cari Buku\n";     
        cout << "4. Urutkan Buku\n";
        cout << "5. Hapus Buku\n";  
        cout << "6. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        string ulang;
        switch (pilihan) {
            case 1:
                tentukanNamaFile(); // minta nama file
                do {
                    int jumlah;
                    cout << "Berapa banyak buku yang ingin ditambahkan? ";
                    cin >> jumlah;
                    tambahBuku(jumlah); // proses tambah buku
                    cout << "Ingin menambahkan lagi? (y/n): ";
                    cin >> ulang;
                } while (ulang == "y");
                break;

            case 2:
                tentukanNamaFile(); // minta nama file
                muatDariFile();     // load data dari file ke memori
                tampilkanBuku(); // tampilkan isi memori
                break;

            case 3:
                tentukanNamaFile(); // minta nama file
                muatDariFile(); // load data dari file ke memori
                cariBuku(); // proses cari buku berdasarkan kode
                break;
                
            case 4:
                tentukanNamaFile(); // minta nama file
                muatDariFile(); // load data dari file ke memori
                urutBuku(); // proses urut buku berdasarkan alfabet
                tampilkanBuku(); // tampilkan isi memori
                break;

            case 5:
                tentukanNamaFile(); // minta nama file
                muatDariFile(); // load data dari file ke memori
                hapusBuku(); // proses hapus buku berdasarkan kode
                tampilkanBuku(); // tampilkan isi memori
                break;

            case 6:
                cout << "Terima kasih!\n"; // keluar program
                break;

            default:
                break;
        }
    } while (pilihan != 6);

    return 0;
}
