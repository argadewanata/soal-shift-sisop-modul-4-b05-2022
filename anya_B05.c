#define FUSE_USE_VERSION 28
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fuse.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>

static  const  char *pathdir = "/home/argadewanata/Documents";
static  const  char *wibulog = "/home/argadewanata/Wibu.log";
static  const  char *hayolog = "/home/argadewanata/hayolongapain_B05.log";

// Function to detect Animeku directory
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

// Function to detect Nam_do-saq directory
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

//  Function to encode with Vigenere Cipher
void encdofVIG(char *s) 
{
    char key[] = "INNUGANTENG";
    for (int i=0;s[i];i++)
    {
        if('A' <= s[i] && s[i] <= 'Z') 
            s[i] = ((s[i]-'A'+(key[i%((sizeof(key)-1))]-'A'))%26)+'A';
        else if('a' <= s[i] && s[i] <= 'z') 
            s[i] = ((s[i]-'a'+(key[i%((sizeof(key)-1))]-'A'))%26)+'a';
    }
}

// Function to decode with Vigenere Cipher
void dcdofVIG(char *s) 
{
    char key[] = "INNUGANTENG";
    for(int i=0;s[i];i++)
    {
        if('A' <= s[i] && s[i] <= 'Z') 
            s[i] = ((s[i]-'A'-(key[i%((sizeof(key)-1))]-'A')+26)%26)+'A';
        else if ('a' <= s[i] && s[i] <= 'z') 
            s[i] = ((s[i]-'a'-(key[i%((sizeof(key)-1))]-'A')+26)%26)+'a';
    }        
}

// Function to record all of system log
void logofSystem(char *dir1, char *dir2, int tipe) 
{
    char buff[1024], cmd[32];
    if(dir1[0]!='\0') strcpy(cmd, "RENAME"), sprintf(buff, "%s --> %s", dir1, dir2), logofwibu(cmd, tipe, buff), logofInfo(cmd,buff);
    else
    {
        if(tipe == 3)
        { 
            strcpy(cmd, "MKDIR"), sprintf(buff, "%s", dir2), logofInfo(cmd, buff);
        }
        else if(tipe == 4)
        {
            strcpy(cmd, "RMDIR"), sprintf(buff, "%s", dir2), logofWarning(cmd, buff);
        }
        else if(tipe == 5)
        { 
            strcpy(cmd, "UNLINK"), sprintf(buff, "%s", dir2), logofWarning(cmd, buff);
        }
    } 
}

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

// Function to record all of the info in log 
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
}

// Function to record all of the system rmdir and unlink log
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

// Function to get all detail of files 
void detailofFile(const char *namaFileLengkap, char *nama, char *ekstensi) 
{
    int id = 0, i = 0;
    while(namaFileLengkap[i]) 
    {
        if(namaFileLengkap[i] == '.') 
            break;
        nama[id++] = namaFileLengkap[i++];
    }
    nama[id] = '\0';
    id = 0;
    while(namaFileLengkap[i]) 
    {
        ekstensi[id++] = namaFileLengkap[i++];
    }
    ekstensi[id] = '\0';
}

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

// Function to encode folder with Vigenere Cipher
int encdofFolderIAN(const char *basePath, const char* folderName) 
{
    char encryptedName[512];
    strcpy(encryptedName, folderName);
    encdofVIG(encryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, encryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

// Function to decode folder with Vigenere Cipher
int dcdofFolderIAN(const char *basePath, const char* folderName) 
{
    char decryptedName[512];
    strcpy(decryptedName, folderName);
    dcdofVIG(decryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, decryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}


int dencdofFile(char *basePath, char *name) 
{
    char fileName[512], ext[64];
    detailofFile(name, fileName, ext);
    encd0fROT(fileName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}


int dencdofFileIAN(char *basePath, char *name) 
{
    char fileName[512], ext[64];
    detailofFile(name, fileName, ext);
    encdofVIG(fileName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int dcdofFile(char *basePath, char *name) 
{
    char fileName[1024], ext[64];
    detailofFile(name, fileName, ext);
    dcdofROT(fileName);
    char f_path[1024], t_path[1100];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}


int dcdofFileIAN(char *basePath, char *name) 
{
    char fileName[1024], ext[64];
    detailofFile(name, fileName, ext);
    dcdofVIG(fileName);
    char f_path[1024], t_path[1100];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encdofFolderRekursif(char *basePath, int depth) 
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
            itung += encdofFolderRekursif(path, depth - 1),
            encdofFolder(basePath, dp->d_name);
        else if(dencdofFile(basePath, dp->d_name) == 0) itung++;
    }
    closedir(dir);
    return itung;
}


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

int dcdofFolderRekursif(char *basePath, int depth) 
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
            itung += dcdofFolderRekursif(path, depth - 1),
            dcdofFolder(basePath, dp->d_name);
        else if(dcdofFile(basePath, dp->d_name) == 0) itung++;
    }
    closedir(dir);
    return itung;
}


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

int binertoDecimal(char *biner){
	int temp = 1, result = 0;
	for(int i=strlen(biner)-1; i>=0; i--){
        if(biner[i] == '1'){
            result += temp; temp *= 2;
        }
    }
	return result;
}

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

int convertDecimal(char *ext){
	int decimal = 0, temp = 1;
	for(int i=strlen(ext)-1; i>=0; i--){
        decimal += (ext[i]-'0')*temp;
        temp *= 10;
    }
	return decimal;
}

void decimaltoBiner(int decimal, char *biner, int len){
	int idx = 0;
	while(decimal){
		if(decimal & 1){
            biner[idx] = '1';
        }
		else {
            biner[idx] = '0';
            idx++;
        }
		decimal /= 2;
	}
	while(idx < len){
		biner[idx] = '0'; idx++;
	}
	biner[idx] = '\0';
	
	for(int i=0; i<idx/2; i++){
		char temp = biner[i];
        biner[i] = biner[idx-1-i];
        biner[idx-1-i] = temp;
	}
}

void getDecimal(char *path, char *biner, char *normalcase){
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
		if(biner[i-idFirst] == '1') normalcase[i-idFirst] = path[i] - 32;
		else normalcase[i-idFirst] = path[i];
	}
	
	for(; i<strlen(path); i++){
		normalcase[i-idFirst] = path[i];
	}
	normalcase[i-idFirst] = '\0';
}

void decryptSpecial(char *filepath){
	chdir(filepath);
	DIR *dir = opendir(".");
	struct dirent *dp;
	struct stat stat_path;
	if(dir == NULL) return;
	
	char pathdir[1000];
	char filePath[1000];
	char newFilePath[1000];
	
    while ((dp = readdir(dir)) != NULL){
		if (stat(dp->d_name, &stat_path) < 0);
		else if (S_ISDIR(stat_path.st_mode)){
			if (strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") == 0) continue;
            sprintf(pathdir,"%s/%s",filepath, dp->d_name);
            decryptSpecial(pathdir);
		}
		else{
			sprintf(filePath,"%s/%s",filepath, dp->d_name);
			char fname[1000], bin[1000], normalcase[1000], clearPath[1000];
			
			strcpy(fname, dp->d_name);
			char *ext = strrchr(fname, '.');
			int dec = convertDecimal(ext+1);
			for(int i=0; i<strlen(fname)-strlen(ext); i++) clearPath[i] = fname[i];
			
			char *ext2 = strrchr(clearPath, '.');
			decimaltoBiner(dec, bin, strlen(clearPath)-strlen(ext2));
            getDecimal(clearPath, bin, normalcase);
            printf(newFilePath,"%s/%s",filepath,normalcase);
            rename(filePath, newFilePath);
		}
	}
    closedir(dir);
}

static int getattrofXMP_ (const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", pathdir, path);
    res = lstat(fpath, stbuf);
    if(res == -1) return -errno;
    return 0;
}

static int readdirofXMP_ (const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0) path = pathdir, sprintf(fpath, "%s", path);
    else sprintf(fpath, "%s%s", pathdir, path);
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    dp = opendir(fpath);
    if (dp == NULL) return -errno;
    while((de = readdir(dp)) != NULL) 
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino; st.st_mode = de->d_type << 12;
        if(filler(buf, de->d_name, &st, 0)) break;
    }
    closedir(dp);
    return 0;
}

static int readofXMP_(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0) path = pathdir, sprintf(fpath, "%s", path);
    else sprintf(fpath, "%s%s", pathdir, path); 
    int fd = open(fpath, O_RDONLY), res;
    (void) fi;
    if (fd == -1) return -errno; 
    res = pread(fd, buf, size, offset); 
    if(res == -1)res = -errno; 
    close(fd);
    return res;
}

static int renameofXMP_(const char *from, const char *to) 
{
    int res; 
    char fpath[1000], tpath[1000];
    if (strcmp(from, "/") == 0) from = pathdir, sprintf(fpath, "%s", from);
    else sprintf(fpath, "%s%s", pathdir, from);
    if (strcmp(to, "/") == 0) to = pathdir, sprintf(tpath, "%s", to);
    else sprintf(tpath, "%s%s", pathdir, to); 
    struct stat path_stat;
    stat(fpath, &path_stat);
    if (!S_ISREG(path_stat.st_mode)) 
    {
        if (isAnimekuDir(fpath) && isNamdosaqDir(tpath)) 
        {
            dcdofFolderRekursif(fpath, 0);
            logofSystem(fpath, tpath, 2);
            printf("[Mendekode %s dengan kedalaman = 0.]\n", fpath);
        }
        else if(isAnimekuDir(fpath) && isIANDir(tpath)){
            dcdofFolderRekursif(fpath, 0);
            logofSystem(fpath, tpath, 2);
            printf("[Mendekode %s dengan kedalaman = 0.]\n", fpath);
        }
    
        else if (isIANDir(fpath) && isAnimekuDir(tpath)){
            encdofFolderRekursif(fpath,0);
            logofSystem(fpath,tpath,1);
            printf("[Mengenkode %s dengan kedalaman = 0.]\n", tpath);
        }
        else if (isNamdosaqDir(fpath) && isAnimekuDir(tpath)) 
        {
            encdofFolderRekursif(fpath, 0);
            logofSystem(fpath, tpath, 1);
            printf("[Mengenkode %s dengan kedalaman = 0.]\n", tpath);
        }
        else if (isAnimekuDir(fpath) && !isAnimekuDir(tpath)) 
        {
            printf("[Mendekode %s.]\n", fpath);
            logofSystem(fpath, tpath, 2);
            int itung = dcdofFolderRekursif(fpath, 1000);
            printf("[Total file yang terdekode: %d]\n", itung);
        }
        
        else if (!isAnimekuDir(fpath) && isAnimekuDir(tpath)) 
        {
            printf("[Mengenkode %s.]\n", fpath);
            logofSystem(fpath, tpath, 1);
            int itung = encdofFolderRekursif(fpath, 1000);
            printf("[Total file yang terenkode: %d]\n", itung);
      
      
        }
        else if(isAnimekuDir(fpath) && isIANDir(tpath)){
            encdofFolderRekursifIAN(fpath, 0);
            logofSystem(fpath,tpath, 1);
            printf("[Mengenkode %s dengan kedalaman = 0.]\n", tpath);
        }
        else if(isNamdosaqDir(fpath) && isIANDir(tpath)){
            encdofFolderRekursifIAN(fpath, 0);
            logofSystem(fpath,tpath, 1);
            printf("[Mengenkode %s dengan kedalaman = 0.]\n", tpath);
        }
        else if(isIANDir(fpath) && isAnimekuDir(tpath)){
            dcdofFolderRekursifIAN(fpath, 0);
            logofSystem(fpath, tpath, 2);
            printf("[Mendekode %s dengan kedalaman = 0.]\n", fpath);
        }
        else if(isIANDir(fpath) && isNamdosaqDir(tpath)){
            dcdofFolderRekursifIAN(fpath, 0);
            logofSystem(fpath, tpath, 2);
            printf("[Mendekode %s dengan kedalaman = 0.]\n", fpath);
        }
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

static int mknodofXMP_(const char *path, mode_t mode, dev_t rdev)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0) path = pathdir, sprintf(fpath, "%s", path);
    else sprintf(fpath, "%s%s", pathdir, path);
	int res;  
	if (S_ISREG(mode)) 
    {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0) res = close(res);
	} 
    else if (S_ISFIFO(mode)) res = mkfifo(fpath, mode);
	else res = mknod(fpath, mode, rdev);
	if (res == -1) return -errno; 
	return 0;
}

static int mkdirofXMP_(const char *path, mode_t mode) 
{
    int res; char fpath[1000];
    if (strcmp(path, "/") == 0) path = pathdir, sprintf(fpath, "%s", path);
    else sprintf(fpath, "%s%s", pathdir, path);
    res = mkdir(fpath, mode);
    if (res == -1) return -errno;
    if (isAnimekuDir(fpath)) logofSystem("", fpath, 3);
    else if(isIANDir(fpath)) logofSystem("", fpath, 3);
    return 0;
}

static int rmdirofXMP_(const char *path){
    int res; char fpath[1000];
    if (strcmp(path, "/") == 0) path = pathdir, sprintf(fpath, "%s", path);
    else sprintf(fpath, "%s%s", pathdir, path);
    res = rmdir(fpath);
    if (res == -1) return -errno;
    logofSystem("",fpath, 4);
    return 0;
}

static int unlinkofXMP_(const char *path){
    int res; char fpath[1000];
    if (strcmp(path, "/") == 0) path = pathdir, sprintf(fpath, "%s", path);
    else sprintf(fpath, "%s%s", pathdir, path);
    res = unlink(fpath);
    if (res == -1) return -errno;
    logofSystem("",fpath, 5);
    return 0;
}

static const struct fuse_operations xmp_oper = 
{
    .getattr = getattrofXMP_ ,
    .readdir = readdirofXMP_,
    .read = readofXMP_,
    .mkdir = mkdirofXMP_,
    .rename = renameofXMP_,
    .mknod = mknodofXMP_,
    .rmdir = rmdirofXMP_,
    .unlink = unlinkofXMP_,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}

