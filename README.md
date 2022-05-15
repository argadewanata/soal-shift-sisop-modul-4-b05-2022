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

**Penyelesaian**  
Melakukan pendeteksian direktori apabila terdapat awalan "Animeku_" dengan fungsi `isAnimekuDir`. Jika sudah terdeteksi, akan dilakukan encode atau decode pada directory tersebut.  

**Kode Program Fungsi isAnimekuDir**  
```
bool isAnimekuDir(const char *path) 
{
    for(int i=0;i<strlen(path)-8+1;i++)
    {
        if(path[i] == 'A' 
            && path[i+1] == 'n' 
            && path[i+2] == 'i' 
            && path[i+3] == 'm' 
            && path[i+4] == 'e'
            && path[i+5] == 'k' 
            && path[i+6] == 'u' 
            && path[i+7] == '_') 
            {
                return 1;
            }
    }
    return 0;
}

``` 

**Kode Program Fungsi Decode dan Encode**  
```
// Function to encode with Atbash Cipher and Rot13
void encd0fROT(char *s)
{
    for(int i=0;i<strlen(s);i++)
    {
        if('A' <= s[i] && s[i] <= 'Z')
            s[i] = 'Z'-s[i]+'A';
        else if('a' <= s[i] && s[i] <= 'z') 
            s[i] = ((s[i]-'a'+13)%26)+'a';
    }
}

// Function to decode with Atbash Cipher and Rot13
void dcdofROT(char *s)
{
    for(int i=0;s[i];i++)
    {
        if('A' <= s[i] && s[i] <= 'Z') 
            s[i] = 'Z'-s[i]+'A';
        else if(s[i]>='a'&&s[i]<110) 
            s[i] = ((s[i]-'a'-13)+26)+'a';
        else if(s[i]>=110&&s[i]<='z') 
            s[i] = ((s[i]-'a'-13)%26)+'a';
    }
}
``` 
### 1B  
**Deskripsi Soal**    
Semua direktori di-rename dengan awalan “Animeku_”, maka direktori tersebut akan menjadi direktori ter-encode dengan ketentuan sama dengan 1a.  

**Penyelesaian**    
Menggunakan pemanggilan salah satu fungsi FUSE, yaitu `xmp_rename`. Apabila folder yang telah di-rename berawalan "Animeku_" maka akan dilakukan encode.  

**Kode Program xmp_rename**  
``` 
    else if (!isAnimekuDir(fpath) && isAnimekuDir(tpath)) 
    {
        printf("[Mengenkode %s.]\n", fpath);
        sistemLog(fpath, tpath, 1);
        int itung = encodeFolderRekursif(fpath, 1000);
        printf("[Total file yang terenkode: %d]\n", itung);
    }
```      

### 1C  
**Deskripsi Soal**  
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.  

**Penyelesaian**  
Menggunakan pemanggilan salah satu fungsi FUSE, yaitu `xmp_rename`. Apabila folder yang telah di-rename tak lagi berawalan "Animeku_" maka akan dilakukan decode.  

**Kode Program**

```
if (isAnimekuDir(fpath) && isNamdosaqDir(tpath)) 
{
    decodeFolderRekursif(fpath, 0);
    sistemLog(fpath, tpath, 2);
    printf("[Mendekode %s dengan kedalaman = 0.]\n", fpath);
}
else if(isAnimekuDir(fpath) && isIANDir(tpath)){
    decodeFolderRekursif(fpath, 0);
    sistemLog(fpath, tpath, 2);
    printf("[Mendekode %s dengan kedalaman = 0.]\n", fpath);
}
```

### 1D  
**Deskripsi Soal**  
Setiap data yang terencode akan masuk dalam file “Wibu.log”   
Contoh isi:  
RENAME terenkripsi /home/[USER]/Downloads/hai --> /home/[USER]/Downloads/Animeku_hebat   
RENAME terdecode /home/[USER]/Downloads/Animeku_ --> /home/[USER]/Downloads/Coba  

**Penyelesaian**  
Memanggil fungsi logofwibu untuk melakukan pencatatan setiap data yang terenkripsi ataupun terdecode

**Kode Program**
```
// Function to record all of the encription and decription log
void logofwibu(char *cmd, int tipe, char *des) 
{
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    char waktu[30];
    strftime(waktu, 30, "%d%m%Y-%H:%M:%S", lt);
    char logNya[1100];
    sprintf(logNya, "%s %s %s", cmd, tipe==1?"terenkripsi":"terdecode", des);
    FILE *out = fopen(wibulog, "a");
    fprintf(out, "%s\n", logNya);
    fclose(out);
}
```  

### 1E  
**Deskripsi Soal**  
Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

**Penyelesaian**  
Melakukan pemanggilan fungsi `encdofFolder` untuk melakukan encode. Melakukan pemanggilan fungsi `dcdofFolder` untuk melakukan decode.   

**Kode Program**  
```
// Function to encode folder with atbash cipher and rot13
int encdofFolder(const char *basePath, const char* folderName) 
{
    char encryptedName[512];
    strcpy(encryptedName, folderName);
    encd0fROT(encryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, encryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) 
        return -errno;
    return 0;
}

// Function to decode folder with atbash cipher and rot13
int dcdofFolder(const char *basePath, const char* folderName) 
{
    char decryptedName[512];
    strcpy(decryptedName, folderName);
    dcdofROT(decryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, decryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) 
        return -errno;
    return 0;
}
``` 

## Soal 2    
Saat Anya sedang sibuk mengerjakan programnya, tiba-tiba Innu datang ke rumah Anya untuk mengembalikan barang yang dipinjamnya. Innu adalah programmer jenius sekaligus teman Anya. Ia melihat program yang dibuat oleh Anya dan membantu Anya untuk menambahkan fitur pada programnya  

### Kendala Soal 2  

### Screenshot Soal 2  

### 2A  
**Deskripsi Soal**  
Jika suatu direktori dibuat dengan awalan “IAN_[nama]”, maka seluruh isi dari direktori tersebut akan terencode dengan algoritma Vigenere Cipher dengan key “INNUGANTENG” (Case-sensitive, Vigenere).  

**Penyelesaian**  
Melakukan pendeteksian direktori apabila terdapat awalan "IAN_" dengan fungsi `isIANDir`. Jika sudah terdeteksi, akan dilakukan encode atau decode pada directory tersebut.

**Kode Program**  
```
// Function to detect IAN directory
bool isIANDir(const char *path) 
{
    for(int i=0;i<strlen(path)-4+1;i++)
    {
        if(path[i] == 'I' 
            && path[i+1] == 'A' 
            && path[i+2] == 'N' 
            && path[i+3] == '_') 
        {
            return 1;
        }
    }
    return 0;
}
```  


### 2B  
**Deskripsi Soal**  
Jika suatu direktori di rename dengan “IAN_[nama]”, maka seluruh isi dari direktori tersebut akan terencode seperti pada no. 2a.  

**Penyelesaian**  
Menggunakan pemanggilan salah satu fungsi FUSE, yaitu `xmp_rename`. Apabila folder yang telah di-rename berawalan "IAN_" maka akan dilakukan encode.  
```
else if (!isIANDir(fpath) && isIANDir(tpath)) 
    {
        printf("[Mengenkode %s.]\n", fpath);
        sistemLog(fpath, tpath, 1);
        int itung = encodeFolderRekursif(fpath, 1000);
        printf("[Total file yang terenkode: %d]\n", itung);
    }
```    

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

### Kendala Soal 3    

### Screenshot Soal 3  

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









