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
#include <pwd.h>
#include <grp.h>
static const char *dirpath = "/home/rye/shift4";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
        //char file[1000];
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
		struct stat file;
		stat(de->d_name, &file); 
		struct passwd *user = getpwuid(file.st_uid);
		struct group  *group = getgrgid(file.st_gid);
		int usercomp1 = strcmp (user->pw_name, "www-data");
		int usercomp2 = strcmp (user->pw_name, "ic_controller");
		int grupcomp = strcmp (group->gr_name, "www-data"); 

		if (usercomp1 == 0 || usercomp2 == 0 ){
			if(grupcomp == 0)
			{
				unlink(de->d_name);
			}
			
		}
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    
	sprintf(fpath,"%s%s",dirpath,path);
    // printf("%s\n",path);
    int length = strlen(path);
     if (strstr(path,"YOUTUBER") && length!=9)
    {
        mode = 0750;
    }
	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    
	sprintf(fpath,"%s%s",dirpath,path);
    int panjang = strlen(fpath);
    if(fpath[panjang-1]=='1' && fpath[panjang-2]=='z' && fpath[panjang-3]=='i')
    {
        fprintf(stderr, "File ekstensi iz1 tidak boleh diubah permissionnya.\n");
    }
    else
    {
        res = chmod(fpath, mode);
	    if (res == -1)
		    return -errno;
    }
    
	

	return 0;
}

// static int xmp_write(const char *path, const char *buf, size_t size,
// 		     off_t offset, struct fuse_file_info *fi)
// {
// 	int fd;
// 	int res;
//     char fpath[1000];
    
// 	sprintf(fpath,"%s%s",dirpath,path);
// 	(void) fi;
// 	fd = open(fpath, O_WRONLY | O_CREAT);
// 	if (fd == -1)
// 		return -errno;

// 	res = pwrite(fd, buf, size, offset);
    
// 	if (res == -1)
// 		res = -errno;
//     if(strstr(fpath,"YOUTUBER"))
//     {
//         char modec[] = "0640";
//     	int i;
//     	i = strtol(modec, 0, 8);
// 	    chmod (path,i);
//     }
// 	close(fd);
// 	return res;
// }

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    (void) fi;

    int res;
    char fpath[1000];
    
	sprintf(fpath,"%s%s",dirpath,path);
    if(strstr(fpath,"YOUTUBER"))
    {
        mode = 0640;
        strcat(fpath,".iz1");
    }
	// mode=0640;
    res = creat(fpath, mode);
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];
	char fpath[1000];
    
	sprintf(fpath,"%s%s",dirpath,path);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_unlink(const char *path)
{
	int res;
	char fpath[1000];
    
	sprintf(fpath,"%s%s",dirpath,path);

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
	char fpath[1000];
    
	sprintf(fpath,"%s%s",dirpath,path);

	res = lchown(fpath, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}
static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
    .mkdir      = xmp_mkdir,
    .chmod		= xmp_chmod,
    // .write		= xmp_write,
    .create		= xmp_create,
	.utimens	= xmp_utimens,
	.unlink	= xmp_unlink,
	.chown	= xmp_chown,

};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
