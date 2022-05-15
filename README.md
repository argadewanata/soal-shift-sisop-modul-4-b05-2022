# Laporan Resmi Praktikum Modul 4 Kelompok B05 Sistem Operasi B Tahun 2022   
  
Nama Lengkap | NRP
------------- | -------------
Fahmi Muhazir        | 5025201043
Sidrotul Munawaroh   | 5025201047
Rere Arga Dewanata   | 5025201078  

## Soal 1  
Anya adalah adalah seorang programmer wibu yang suka mengoleksi anime. Dia sangat senang membuat program yang dikolaborasikan dengan anime.  

### Kendala Soal 1  
1. Sulit memahami soal dan mekanisme code nya
2. Butuh waktu untuk memahami library

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
Soalnya sulit dipahami

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

```
else if(isIANDir(fpath) && !isIANDir(tpath)){
            printf("[Mendekode %s.]\n", fpath);
            logofSystem(fpath, tpath, 2);
            int itung = dcdofFolderRekursifIAN(fpath, 1000);
            printf("[Total file yang terdekode: %d]\n", itung);
        }
        else if(!isIANDir(fpath) && isIANDir(tpath)){
            printf("[Mengenkode %s.]\n", fpath);
            logofSystem(fpath, tpath, 1);
            int itung = encdofFolderRekursifIAN(fpath, 1000);
            printf("[Total file yang terenkode: %d]\n", itung);
        }
```

### 2D  
**Deskripsi Soal**  
Untuk memudahkan dalam memonitor kegiatan yang dilakukan pada filesystem yang telah dibuat, ia membuat log system pada direktori “/home/[user]/hayolongapain_[kelompok].log”. Dimana log ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.  

```
void logofInfo(char *cmd, char *des) 
{
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    char waktu[30];
    strftime(waktu, 30, "%d%m%Y-%H:%M:%S", lt);
    char logNya[1100];
    sprintf(logNya, "INFO::%s:%s::%s", waktu, cmd, des);
    FILE *out = fopen(hayolog, "a");
    fprintf(out, "%s\n", logNya);
    fclose(out);
    return;
}
```

Kemudian di tautkan dengan fungsi 'sistemofLog'

```
void logofSystem(char *dir1, char *dir2, int tipe) 
{
    char buff[1024], cmd[32];
    if(dir1[0]!='\0') strcpy(cmd, "RENAME"), sprintf(buff, "%s --> %s", dir1, dir2), logofRename(cmd, tipe, buff), logofInfo(cmd,buff);
    else{
        if(tipe == 3){ //mkdir
            strcpy(cmd, "MKDIR"), sprintf(buff, "%s", dir2), logofInfo(cmd, buff);
        }else if(tipe == 4){ //rmdir
            strcpy(cmd, "RMDIR"), sprintf(buff, "%s", dir2), logofWarning(cmd, buff);
        }else if(tipe == 5){ //unlink
            strcpy(cmd, "UNLINK"), sprintf(buff, "%s", dir2), logofWarning(cmd, buff);
        }
    } 
 
}
```
Fungsi ini dipanggil sebelum proses encode dan decode berlangsung.

### 2E  
**Deskripsi Soal**  
Karena Innu adalah seorang perfeksionis, ia membagi isi dari log systemnya menjadi 2 level, yaitu level INFO dan WARNING. Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink. Sisanya, akan dicatat pada level INFO dengan format sebagai berikut :   
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]::[CMD]::[DESC :: DESC]  

```
void logofWarning(char *cmd, char *des) 
{
    time_t t = time(NULL);
    struct tm* lt = localtime(&t); 
    char waktu[30];
    strftime(waktu, 30, "%d%m%Y-%H:%M:%S", lt); 
    char logNya[1100];
    sprintf(logNya, "WARNING::%s:%s::%s", waktu, cmd, des); 
    FILE *out = fopen(hayolog, "a");
    fprintf(out, "%s\n", logNya);
    fclose(out);
}
```

Sama seperti pada no 2D, akan di integrasikan dengan fungsi 'systemofLog'

## Soal 3  
Ishaq adalah seseorang yang terkenal di kalangan anak informatika seluruh indonesia. Ia memiliki teman yang bernama innu dan anya, lalu ishaq bertemu dengan mereka dan melihat program yang mereka berdua kerjakan  sehingga ia tidak mau kalah dengan innu untuk membantu anya dengan menambah fitur yang ada pada programnya dengan ketentuan:

### Kendala Soal 3  
Belum terlalu mengerti FUSE   

### Screenshot Soal 3  

### 3A  
**Deskripsi Soal**  
Jika suatu direktori dibuat dengan awalan “nam_do-saq_”, maka direktori tersebut akan menjadi sebuah direktori spesial  

Dibuat fungsi untuk mengecek nilai true dan false
```
bool isNamdosaqDir(const char *path) 
{
    for(int i=strlen(path)-1;i>=11;i--)
    {
        if (path[i-11] == 'n' 
            && path[i-10] == 'a' 
            && path[i-9] == 'm' 
            && path[i-8] == '_' 
            && path[i-7] == 'd' 
            && path[i-6] == 'o' 
            && path[i-5] == '-' 
            && path[i-4] == 's' 
            && path[i-3] == 'a' 
            && path[i-2] == 'q' 
            && path[i-1] == '_') 
            return 1;
    }
    return 0;
}
```
direktori spesial

```
void cryptSpecial(char *filepath){
	chdir(filepath);
	DIR *dir = opendir(".");
	struct dirent *dp;
	struct stat path_stat;
	if(dir == NULL) return;
 
	char pathdir[1000];
	char filePath[1000];
	char newFilePath[1000];
 
    while ((dp = readdir(dir)) != NULL){
		if (stat(dp->d_name, &path_stat) < 0);
		else if (S_ISDIR(path_stat.st_mode)){
			if (strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") == 0){
                continue;
                printf(pathdir,"%s/%s",filepath, dp->d_name);
                cryptSpecial(pathdir);
            }
		}
        else{
			sprintf(filePath,"%s/%s",filepath, dp->d_name);
			char biner[1000], lowercase[1000]; 
            binerGet(dp->d_name, biner, lowercase);
			int decimal = binertoDecimal(biner);
			printf(newFilePath,"%s/%s.%d",filepath,lowercase,decimal); 
            rename(filePath, newFilePath);
		}
	}
    closedir(dir);
}
```
### 3B  
**Deskripsi Soal**  
Jika suatu direktori di-rename dengan memberi awalan “nam_do-saq_”,maka direktori tersebut akan menjadi sebuah direktori spesial.  

```
        else if (isNamdosaqDir(fpath) && isAnimekuDir(tpath))
        {
            cryptSpecial(fpath);
        }
 
 
        else if (isNamdosaqDir(fpath) && isIANDir(tpath))
        {
            cryptSpecial(fpath);
        }
 
 
        else if (isAnimekuDir(fpath) && isNamdosaqDir(tpath))
        {
            decryptSpecial(fpath);
            logofSystem(fpath, tpath, 2);
        }
 
 
        else if (isIANDir(fpath) && isNamdosaqDir(tpath))
        {
            decryptSpecial(fpath);
            logofSystem(fpath, tpath, 2);
        }
    }
    res=rename(fpath, tpath);
    if(res == -1) return -errno;
    return 0;
}
```

### 3C  
**Deskripsi Soal**  
Apabila direktori yang terenkripsi di-rename dengan menghapus “nam_do-saq_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.  

### 3D  
**Deskripsi Soal**  
Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “Animeku_” maupun “IAN_” namun masing masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive “Animeku_” dan “IAN_” tetap berjalan pada subdirektori).  

membuat fungsi encode dan decode rekursif

encode
```
int encdofFolderRekursifIAN(char *basePath, int depth) 
{
    char path[1000]; 
    struct dirent *dp; 
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int itung=0;
    while((dp=readdir(dir)) != NULL) 
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath); strcat(path, "/"); strcat(path, dp->d_name);
        struct stat path_stat;
        stat(path, &path_stat);
        if(!S_ISREG(path_stat.st_mode)&&depth>0)
            itung += encdofFolderRekursifIAN(path, depth - 1),
            encdofFolderIAN(basePath, dp->d_name);
        else if(dencdofFileIAN(basePath, dp->d_name) == 0) itung++;
    }
    closedir(dir);
    return itung;
}
```

decode
```
int dcdofFolderRekursifIAN(char *basePath, int depth) 
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if(!dir) return 0;
    int itung = 0;
    while((dp = readdir(dir)) != NULL) 
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath); strcat(path, "/"); strcat(path, dp->d_name);
        struct stat path_stat;
        stat(path, &path_stat);
        if(!S_ISREG(path_stat.st_mode) && depth>0)
            itung += dcdofFolderRekursifIAN(path, depth - 1),
            dcdofFolderIAN(basePath, dp->d_name);
        else if(dcdofFileIAN(basePath, dp->d_name) == 0) itung++;
    }
    closedir(dir);
    return itung;
}
```
### 3E  
**Deskripsi Soal**    
Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi uppercase insensitive dan diberi ekstensi baru berupa nilai desimal dari biner perbedaan namanya.  
Contoh : jika pada direktori asli namanya adalah “isHaQ_KEreN.txt” maka pada fuse akan 
menjadi “ISHAQ_KEREN.txt.1670”. 1670 berasal dari biner 11010000110

Membuat fungsi 'void binerGet'
```
void binerGet(char *path, char *biner, char *lowercase){
	int idFirst = 0;
    for(int i=0; i<strlen(path); i++){
		if (path[i] == '/'){ 
            idFirst = i + 1;
        }
	}
 
    int idEnd = strlen(path);
    for(int i=strlen(path)-1; i>=0; i--){
		if (path[i] == '.'){ 
            idEnd = 1;
        }
	}
 
	int i;
	for(i=idFirst; i<idEnd; i++){
		if(isupper(path[i])){
			biner[i] = '1';lowercase[i] = path[i] + 32;
		}
		else{
			biner[i] = '0';lowercase[i] = path[i];
		}
	}
	biner[idEnd] = '\0';
 
	for(; i<strlen(path); i++){
		lowercase[i] = path[i];
	}
	lowercase[i] = '\0';
}
```







