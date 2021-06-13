# soal-shift-sisop-modul-4-C06-2021
Praktikum Modul 4 Sistem Operasi Teknik Informatika ITS Tahun 2021

## Penjelasan Singkat Soal:

Pada soal modul 4 kami diminta untuk menggunakan FUSE untuk encode sebuah file jika didalam folder yang memiliki prefix "AtoZ_", Mount Source nya berada di directory  /home/[USER]/Downloads ('/' harus diabaikan dan ekstensi file tidak perlu di code).

## Laporan Soal Nomor 1:

### Permintaan Soal Nomor 1:

a. Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

b. Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

c. Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

d. Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Download/AtoZ_[Nama Direktori].

e. Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

### Penyelesaian Soal Nomor 1:

**Pertama, ada beberapa fungsi yang kami buat yaitu:**

- splitExtension = berfungsi untuk memisah index string untuk extension.
- getExtension = untuk mengambil index extension sebuah file.
- getSlashFile = untuk mengambil index char '/' sebuah file.
- atbashEncrypt = Enkripsi nama file sesuai aturan atbash.
- atbashDecrypt = Dekripsi nama file dari atbash menjadi nama biasa.

```
int splitExtension(char *filePath)
{
    int isFound = 0;
    for (int index = strlen(filePath) - 1; index >= 0; index--)
    {
        if (filePath[index] == '.')
        {
            if (isFound == 1)
                return index;
            else
                isFound = 1;
        }
    }
    return strlen(filePath);
}

int getExtension(char *filePath)
{
    for (int index = strlen(filePath) - 1; index >= 0; index--)
    {
        if (filePath[index] == '.')
            return index;
    }
    return strlen(filePath);
}

int getSlashFile(char *filePath, int end)
{
    for (int index = 0; index < strlen(filePath); index++)
    {
        if (filePath[index] == '/')
            return index + 1;
    }
    return end;
}

void atbashEncrypt(char *filePath)
{
    if (!strcmp(filePath, ".") || !strcmp(filePath, ".."))
        return;

    printf("Encrypt file with atbash: %s\n", filePath);

    int endIndex = splitExtension(filePath);
    if (endIndex == strlen(filePath))
        endIndex = getExtension(filePath);
    int startIndex = getSlashFile(filePath, 0);

    for (int index = startIndex; index < endIndex; index++)
    {
        if (filePath[index] != '/' && isalpha(filePath[index]))
        {
            char tmp = filePath[index];
            if (isupper(filePath[index]))
                tmp -= 'A';
            else
                tmp -= 'a';
            tmp = 25 - tmp; //Atbash cipher
            if (isupper(filePath[index]))
                tmp += 'A';
            else
                tmp += 'a';
            filePath[index] = tmp;
        }
    }
}

void atbashDecrypt(char *path)
{
    if (!strcmp(path, ".") || !strcmp(path, ".."))
        return;

    printf("Decrypt file with atbash: %s\n", path);

    int endIndex = splitExtension(path);
    if (endIndex == strlen(path))
        endIndex = getExtension(path);
    int startIndex = getSlashFile(path, endIndex);

    for (int index = startIndex; index < endIndex; index++)
    {
        if (path[index] != '/' && isalpha(path[index]))
        {
            char tmp = path[index];
            if (isupper(path[index]))
                tmp -= 'A';
            else
                tmp -= 'a';
            tmp = 25 - tmp;
            if (isupper(path[index]))
                tmp += 'A';
            else
                tmp += 'a';
            path[index] = tmp;
        }
    }
}
```

**Untuk soal nomor 1, kami menggunakan 5 fungsi FUSE, yaitu:**

- xmp_getattr
- xmp_readdir
- xmp_read
- xmp_mkdir
- xmp_rmdir
- xmp_rename

### Output Nomor 1:

**State Awal:**

![Output_State_Awal](/screenshot/1_startState.png)

**1a:**

![Output_1a](/screenshot/1AE.png)

**1b:**

![Output_1b](/screenshot/1B.png)

**1c:**

![Output_1c](/screenshot/1C.png)

**1d:**

![Output_1d](/screenshot/1D.png)

**1e:**

![Output_1e](/screenshot/1AE.png)

## Laporan Soal Nomor 4:

### Permintaan Soal Nomor 4:

a. Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.

b. Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.

c. Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.

d. Sisanya, akan dicatat pada level INFO.

e. Format untuk logging yaitu:

- [Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]
- Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan.

Contoh Format :

- INFO::28052021-10:00:00:CREATE::/test.txt
- INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt

### Penyelesaian Soal Nomor 4:

**Pertama, ada dua fungsi yang kami buat untuk proses pencacatan log yaitu:**
- setLog = untuk mencatat log tanpa nama perubahan files.
- setLogWithNameFiles = untuk mencatat log dengan record nama perubahan files.

```
void setLog(char *logCategory, char *fpath)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char tmp[1000];

    FILE *file;
    file = fopen("/home/namdoshaq/SinSeiFS.log", "a");

    if (strcmp(logCategory, "RMDIR") == 0 || strcmp(logCategory, "UNLINK") == 0)
        sprintf(tmp, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, logCategory, fpath);
    else
        sprintf(tmp, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, logCategory, fpath);

    fputs(tmp, file);
    fclose(file);
    return;
}

void setLogWithNameFiles(char *logCategory, const char *old, const char *new)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char tmp[1000];

    FILE *file;
    file = fopen("/home/namdoshaq/SinSeiFS.log", "a");

    if (strcmp(logCategory, "RMDIR") == 0 || strcmp(logCategory, "UNLINK") == 0)
        sprintf(tmp, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, logCategory, old, new);
    else
        sprintf(tmp, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, logCategory, old, new);

    fputs(tmp, file);
    fclose(file);
    return;
}
```

### Output Nomor 4:
