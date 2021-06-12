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
