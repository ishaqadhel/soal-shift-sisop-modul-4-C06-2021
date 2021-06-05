#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

static const char *dirpath = "/home/namdoshaq/Downloads";

void atbashCipher(char *nameFile, int sizeOfNameFile, int isEncrypt)
{
    if (!strcmp(nameFile, ".") || !strcmp(nameFile, "..") || (!strchr(nameFile, '/') && !isEncrypt))
        return;
    int startIndex = -1, endOfIndex = -1;
    sscanf(nameFile, "%*[^/]/%n%*[^./]%n", &startIndex, &endOfIndex);

    if (!(nameFile[sizeOfNameFile - 1] == '/'))
    {
        char *extensionFile = strrchr(nameFile, '.');
        if (extensionFile)
            endOfIndex = extensionFile - nameFile;
        else
            endOfIndex = sizeOfNameFile;
    }

    for (int currIndex = startIndex; currIndex < endOfIndex; currIndex++)
    {
        if (!((nameFile[currIndex] >= 0 && nameFile[currIndex] < 65) || (nameFile[currIndex] > 90 && nameFile[currIndex] < 97) || (nameFile[currIndex] > 122 && nameFile[currIndex] <= 127)))
        {
            if (nameFile[currIndex] >= 'A' && nameFile[currIndex] <= 'Z')
            {
                nameFile[currIndex] = 'Z' + 'A' - nameFile[currIndex];
            }

            if (nameFile[currIndex] >= 'a' && nameFile[currIndex] <= 'z')
            {
                nameFile[currIndex] = 'z' + 'a' - nameFile[currIndex];
            }
        }
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char *folderAtoz = strstr(path, "AtoZ_");
    if (folderAtoz)
        atbashCipher(folderAtoz, strlen(folderAtoz), 0);

    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t ofdirpathet, struct fuse_file_info *fi)
{
    char *folderAtoz = strstr(path, "AtoZ_");
    if (folderAtoz)
        atbashCipher(folderAtoz, strlen(folderAtoz), 0);

    char fpath[1000];
    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);
    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void)ofdirpathet;
    (void)fi;
    dp = opendir(fpath);
    if (!dp)
        return -errno;

    while ((de = readdir(dp)))
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        int d_len = strlen(de->d_name);
        if (folderAtoz)
            atbashCipher(de->d_name, d_len, 1);
        res = (filler(buf, de->d_name, &st, 0));
        if (res != 0)
            break;
    }

    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t ofdirpathet, struct fuse_file_info *fi)
{
    char *folderAtoz = strstr(path, "AtoZ_");
    if (folderAtoz)
        atbashCipher(folderAtoz, strlen(folderAtoz), 0);

    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    int fd, res;

    (void)fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, ofdirpathet);
    if (res == -1)
        res = -errno;
    close(fd);
    return res;
}

static struct fuse_operations _func = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,

};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &_func, NULL);
}