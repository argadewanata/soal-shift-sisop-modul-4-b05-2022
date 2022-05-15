# Laporan Resmi Praktikum Modul 4 Kelompok B05 Sistem Operasi B Tahun 2022   
  
Nama Lengkap | NRP
------------- | -------------
Fahmi Muhazir        | 5025201043
Sidrotul Munawaroh   | 5025201047
Rere Arga Dewanata   | 5025201078  

## Soal 1  
Anya adalah adalah seorang programmer wibu yang suka mengoleksi anime. Dia sangat senang membuat program yang dikolaborasikan dengan anime.  

### Kendala Soal 1  

### Screenshot Soal 1  

### 1A  
**Deskripsi Soal**  
Semua direktori dengan awalan “Animeku_” akan terencode dengan ketentuan semua file yang terdapat huruf besar akan ter encode dengan atbash cipher dan huruf kecil akan terencode dengan rot13  
Contoh : 
“Animeku_/anya_FORGER.txt” → “Animeku_/naln_ULITVI.txt”  

### 1B  
**Deskripsi Soal**    
Semua direktori di-rename dengan awalan “Animeku_”, maka direktori tersebut akan menjadi direktori ter-encode dengan ketentuan sama dengan 1a.  

### 1C  
**Deskripsi Soal**  
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.  

### 1D  
**Deskripsi Soal**  
Setiap data yang terencode akan masuk dalam file “Wibu.log”   
Contoh isi:  
RENAME terenkripsi /home/[USER]/Downloads/hai --> /home/[USER]/Downloads/Animeku_hebat   
RENAME terdecode /home/[USER]/Downloads/Animeku_ --> /home/[USER]/Downloads/Coba  

## Soal 2    
Saat Anya sedang sibuk mengerjakan programnya, tiba-tiba Innu datang ke rumah Anya untuk mengembalikan barang yang dipinjamnya. Innu adalah programmer jenius sekaligus teman Anya. Ia melihat program yang dibuat oleh Anya dan membantu Anya untuk menambahkan fitur pada programnya  

### Kendala Soal 2  

### Screenshot Soal 2  

### 2A  
**Deskripsi Soal**  
Jika suatu direktori dibuat dengan awalan “IAN_[nama]”, maka seluruh isi dari direktori tersebut akan terencode dengan algoritma Vigenere Cipher dengan key “INNUGANTENG” (Case-sensitive, Vigenere).  

### 2B  
**Deskripsi Soal**  
Jika suatu direktori di rename dengan “IAN_[nama]”, maka seluruh isi dari direktori tersebut akan terencode seperti pada no. 2a.  

### 2C  
**Deskripsi Soal**  
Apabila nama direktori dihilangkan “IAN_”, maka isi dari direktori tersebut akan terdecode berdasarkan nama aslinya.  

### 2D  
**Deskripsi Soal**  
Untuk memudahkan dalam memonitor kegiatan yang dilakukan pada filesystem yang telah dibuat, ia membuat log system pada direktori “/home/[user]/hayolongapain_[kelompok].log”. Dimana log ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.  

### 2E  
**Deskripsi Soal**  
Karena Innu adalah seorang perfeksionis, ia membagi isi dari log systemnya menjadi 2 level, yaitu level INFO dan WARNING. Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink. Sisanya, akan dicatat pada level INFO dengan format sebagai berikut :   
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]::[CMD]::[DESC :: DESC]  

## Soal 3  
Ishaq adalah seseorang yang terkenal di kalangan anak informatika seluruh indonesia. Ia memiliki teman yang bernama innu dan anya, lalu ishaq bertemu dengan mereka dan melihat program yang mereka berdua kerjakan  sehingga ia tidak mau kalah dengan innu untuk membantu anya dengan menambah fitur yang ada pada programnya dengan ketentuan:  

### 3A  
**Deskripsi Soal**  
Jika suatu direktori dibuat dengan awalan “nam_do-saq_”, maka direktori tersebut akan menjadi sebuah direktori spesial  

### 3B  
**Deskripsi Soal**  
Jika suatu direktori di-rename dengan memberi awalan “nam_do-saq_”,maka direktori tersebut akan menjadi sebuah direktori spesial.  

### 3C  
**Deskripsi Soal**  
Apabila direktori yang terenkripsi di-rename dengan menghapus “nam_do-saq_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.  

### 3D  
**Deskripsi Soal**  
Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “Animeku_” maupun “IAN_” namun masing masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive “Animeku_” dan “IAN_” tetap berjalan pada subdirektori).  

### 3E  
**Deskripsi Soal**    
Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi uppercase insensitive dan diberi ekstensi baru berupa nilai desimal dari biner perbedaan namanya.  
Contoh : jika pada direktori asli namanya adalah “isHaQ_KEreN.txt” maka pada fuse akan 
menjadi “ISHAQ_KEREN.txt.1670”. 1670 berasal dari biner 11010000110









