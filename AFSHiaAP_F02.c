#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
char urutan[100];
static const char *dirpath = "/home/rye/shift4";
char ext[100000] = "\0";
int ind_ext =0;

void enkrip (char *kata)
{
	int i,j;
	if(!strcmp(kata,".") || !strcmp(kata,"..")) return;
	for(i=0;i<(strlen(kata));i++)
	{
		printf("%c",kata[i]);
		if(kata[i]!='/')
		{
			for(j=0;j<(strlen(urutan));j++)
			{
				if(urutan[j]==kata[i])
				{
					int aa = (j + 31) % 94;
					kata[i] = urutan[aa];
					break;
				}
			}
		}	
	}
}

void dekrip (char *kata)
{
	int i,j;
	if(!strcmp(kata,".") || !strcmp(kata,"..")) return;
	for(i=0;i<(strlen(kata));i++)
	{
		printf("%c",kata[i]);
		if(kata[i]!='/')
		{
			for(j=0;j<(strlen(urutan));j++)
			{
				if(urutan[j]==kata[i])
				{
					int aa = (j + 94-31) % 94;
					kata[i] = urutan[aa];
					break;
				}
			}
		}	
	}
}

int check_ext(char* file){
	ind_ext=0;
	while(ind_ext<strlen(file)&&file[ind_ext]!='.') ind_ext++;
	memset(ext,0,sizeof(ext));
	strcpy(ext,file+ind_ext);
	return ind_ext;
}

static void* pre_init(struct fuse_conn_info *conn)
{
        char folder[100000] = "/Videos";
		char folde1r[100000] = "/YOUTUBER";
		enkrip(folder);
		enkrip(folde1r);
		char fpath[1000];
    	sprintf(fpath,"%s%s", dirpath, folder);
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));
		sprintf(fpath,"%s%s", dirpath, folde1r);
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));

		pid_t child1;
		child1=fork();
		if(child1==0){
			DIR *dp;
			struct dirent *de;
			dp = opendir(dirpath);
			while((de = readdir(dp))){
				if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
					char ext[1000] = ".mp4";
					enkrip(ext);
					if(strlen(de->d_name)>7 && strncmp(de->d_name+strlen(de->d_name)-8,ext,4)==0){

							char joined[1000];
							char video[1000] = "/Videos";
							enkrip(video);
							sprintf(joined,"%s%s/",dirpath,video);
							strncat(joined,de->d_name,strlen(de->d_name)-4);
							FILE* mainj;
							mainj = fopen(joined,"a+");
							FILE* need;
							char this[1000];
							sprintf(this,"%s/%s",dirpath,de->d_name);
							need = fopen(this,"r");
							int c;
							while(1) {
   								c = fgetc(need);
   								if( feof(need) ) {
   								   break;
   								}
   								fprintf(mainj,"%c",c);
   							}
							
					}
				}
			}
			exit(EXIT_SUCCESS);
		}

        (void) conn;
        return NULL;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[1000];
	char spath[1000];
	sprintf(spath,"%s",path);
	enkrip(spath);
	sprintf(fpath,"%s%s",dirpath,spath);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	char spath[1000];
	sprintf(spath,"%s",path);
	enkrip(spath);
	sprintf(fpath, "%s%s",dirpath,spath);
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
        char buff[1000];
		sprintf(buff, "%s", de->d_name);
		dekrip(buff);
		res = (filler(buf, buff, &st, 0));
			if(res!=0) break;

		char name[1000];
		sprintf(name, "%s/%s", fpath, de->d_name);
		struct stat sfile;
		stat(name, &sfile); 
		struct passwd *user = getpwuid(sfile.st_uid);
		struct group  *group = getgrgid(sfile.st_gid);
		int usercomp1 = strcmp (user->pw_name, "chipset");
		int usercomp2 = strcmp (user->pw_name, "ic_controller");
		int grupcomp = strcmp (group->gr_name, "rusak"); 

		if ((usercomp1 == 0 || usercomp2 == 0) && grupcomp == 0 && ((sfile.st_mode & S_IRUSR) == 0 || (sfile.st_mode & S_IRGRP) == 0 || (sfile.st_mode & S_IROTH) == 0) ){
			FILE *cek, *file;
			// cek = fopen(name, "r");
			// if(cek == NULL)
			// {
				char miris[1000]="/filemiris.txt";
				char filemiris[1000];
				enkrip(miris);
				sprintf(filemiris, "%s%s", dirpath,miris);
				file = fopen(filemiris, "a+");

                char isiFile[1000];
				char namafile[1000];
				strcpy(namafile,de->d_name);
				dekrip(namafile);
				char buff[20];
				time_t now = time(NULL);
				strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
                sprintf(isiFile, "%s %d:%d %s\n", namafile, user->pw_uid, group->gr_gid, buff);
                fputs(isiFile, file);

				fclose(file);
				remove(name);
			// }
			// else
			// {
				// fclose(cek);
			// }
			
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
	char fpath[1000];
	char spath[1000];
	sprintf(spath,"%s",path);
	enkrip(spath);
	sprintf(fpath,"%s%s",dirpath,spath);

	(void) fi;
	fd = open(fpath, O_RDONLY);
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
    char spath[1000];
	sprintf(spath,"%s",path);
	enkrip(spath);
	
    // printf("%s\n",path);
    int length = strlen(path);
     if (strstr(path,"YOUTUBER") && length!=9)
    {
        mode = 0750;
    }
	// enkrip(path);
	sprintf(fpath,"%s%s",dirpath,spath);
	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char name[1000];
	char temp[1000];
	strcpy(temp,path);
	enkrip(temp);
	sprintf(name, "%s%s", dirpath, temp);
	struct stat sfile;
	stat(name, &sfile);
	// sprintf(fpath,"%s%s",dirpath,path);
    int panjang = strlen(path);
    if(path[panjang-1]=='1' && path[panjang-2]=='z' && path[panjang-3]=='i' && S_ISDIR(sfile.st_mode)==0)
    {
        pid_t child_id;
		int status;
		child_id=fork();
		if(child_id == 0)
		{
			char *argv[5]={"zenity","--error","--text=\"File ekstensi iz1 tidak boleh diubah permissionnya.\n\"","--title=\"Warning!\"",NULL};
			execv("/usr/bin/zenity",argv);
		}
		else
		{
			while((wait(&status)) > 0);
		}
		
    }
    else
    {
		char spath[1000];
		sprintf(spath,"%s",path);
		enkrip(spath);
		// char fpath[1000];
    
		sprintf(fpath,"%s%s",dirpath,spath);
        res = chmod(fpath, mode);
	    if (res == -1)
		    return -errno;
    }
    
	

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    char name[1000];

	sprintf(name,"%s",path);
    enkrip(name);
	sprintf(fpath,"%s%s",dirpath,name);

	char newname[1000];
	char folder[1000] = "/Backup";
	enkrip(folder);
	char folderdir[1000];
	sprintf(folderdir,"%s%s",dirpath,folder);
	mkdir(folderdir,0755);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;
	printf("%s\n",buf);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	struct stat sd;
	if(stat(fpath,&sd)>-1 && strstr(path,".swp")==0){
		char t[1000];
		time_t now = time(NULL);
		char fname[1000];
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));
		dekrip(name);
		check_ext(name);
		char shortn[100000];
		memset(shortn,0,strlen(shortn));
		strncpy(shortn,name,ind_ext);
		sprintf(newname,"/Backup%s%s%s",shortn,t,ext);
		enkrip(newname);
		memset(fname,'\0',sizeof(fname));
		sprintf(fname,"%s%s",dirpath,newname);
		// printf("%s\n",fpath);
		pid_t child1;
		child1=fork();
		if(child1==0){
			execl("/bin/cp","/bin/cp",fpath,fname,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			wait(NULL);
		}

		return res;
	}
	
	return res;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    (void) fi;

    int res;
    char fpath[1000];
	char spath[1000];
	sprintf(spath,"%s",path);
	int jum;
	
	char *found;
	found=strstr(spath,"YOUTUBER");
    if(found)
    {
		int i=found-spath;
		while(spath[i] != '\0')
		{
			if(spath[i]=='/')
			jum++;
			i++;
		}
		if(jum<2)
		{
			mode = 0640;
       		strcat(spath,".iz1");
		}
    }
	enkrip(spath);
    
	sprintf(fpath,"%s%s",dirpath,spath);
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
    char spath[1000];
	sprintf(spath,"%s",path);
	enkrip(spath);
	sprintf(fpath,"%s%s",dirpath,spath);

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
    char name[1000];
	sprintf(name,"%s",path);
	if(strstr(name,".swp")==0 && strstr(name,".gooutpustream")==0 && strstr(name,"/RecycleBin/") == 0){
		char folder[1000] = "/RecycleBin";
		enkrip(folder);
		char fpath[1000];
    	sprintf(fpath,"%s%s", dirpath, folder);
		mkdir(fpath,0755);

		char t[1000];
		time_t now = time(NULL);
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));

		char zip[1000];
		char fzip[1000];
		char fname[1000];
		memset(zip,0,sizeof(zip));
		memset(fzip,0,sizeof(fzip));
		memset(fname,0,sizeof(fname));
		sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
		enkrip(name);
		sprintf(fname,"%s%s",dirpath,name); //yg asli dienkrip
		enkrip(zip);
		sprintf(fzip,"%s%s",dirpath,zip); //tempat zip dienkrip
		pid_t child1;
		char dum[10000];
		dekrip(name);
		sprintf(dum,"%s%s",dirpath,name); //yang asli dekrip, tempat copy
		enkrip(name);

		int tunggu;
		child1=fork();
		if(child1==0){
			execl("/bin/cp","/bin/cp",fname,dum,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			waitpid(child1, &tunggu, 0);
			
		}
		
		child1=fork();
		if(child1==0){
			execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j",fzip,dum,NULL); //hasil cp di zip di fzip
			exit(EXIT_SUCCESS);
		}
		else{
			waitpid(child1, &tunggu, 0);
		}
		remove(fname); //yang asli dihapus
		strcat(fzip,".zip");

		char fback[1000] = "/Backup";
		enkrip(fback);
		char foldbackp[1000];
		sprintf(foldbackp,"%s%s",dirpath,fback);
		DIR *dp;
		struct dirent *de;
        dekrip(name);
        check_ext(name);
        enkrip(name);
        printf("%s\n",foldbackp);
		dp = opendir(foldbackp);
		while((de = readdir(dp))){
				printf("%s\n%s\n%d\n",name,de->d_name,ind_ext-1);

			if(strncmp(name+1,de->d_name,ind_ext-1)==0){
				printf("%s\n%s\n",name,de->d_name);
				memset(fname,0,sizeof(fname));
				dekrip(name);
				sprintf(fname,"%s/%s",foldbackp,de->d_name); //bu yg dienkirp
				enkrip(name);
				char dum[10000];
				dekrip(de->d_name);
				sprintf(dum,"%s/%s",dirpath,de->d_name); //root+bu dekrip, tempat copy
				enkrip(de->d_name);				

				child1=fork();
				if(child1==0){
					execl("/bin/cp","/bin/cp",fname,dum,NULL);
					exit(EXIT_SUCCESS);
				}
				else{
					waitpid(child1, &tunggu, 0);
				}

				child1=fork();
				if(child1==0){
					execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j","-u",fzip,dum,NULL);
					exit(EXIT_SUCCESS);
				}
				else{
					waitpid(child1, &tunggu, 0);					
				}
				remove(fname);
			}
		}
		dekrip(name);
		memset(fzip,0,sizeof(fzip));
		enkrip(name);
		memset(fname,0,sizeof(fname));
		sprintf(fzip,"%s%s",dirpath,zip); //dirpath + recycle bin
		strcpy(fname,fzip);
		strcat(fzip,".zip");
		rename(fzip,fname);
	}
	else{
    	enkrip(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = unlink(fpath);
		if (res == -1)
			return -errno;
	}
	return 0;
}
static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
	char fpath[1000];
    char spath[1000];
	sprintf(spath,"%s",path);
	enkrip(spath);
	sprintf(fpath,"%s%s",dirpath,spath);

	res = lchown(fpath, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}
static void post_destroy(void* private_data)
{
	char fpath[10000];
	char folder[100000] = "/Videos";
	enkrip(folder);
    sprintf(fpath,"%s%s", dirpath, folder);

	DIR *dp;
	struct dirent *de;
	dp = opendir(fpath);
	while((de = readdir(dp))){
		if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
			char file[1000];
			sprintf(file,"%s/%s",fpath,de->d_name);
			remove(file);
		}
	}
	rmdir(fpath);

	return;
}

static int xmp_access(const char *path, int mask)
{
	int res;

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enkrip(name);
	sprintf(fpath, "%s%s",dirpath,name);

	res = access(fpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enkrip(name);
	sprintf(fpath, "%s%s",dirpath,name);
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enkrip(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enkrip(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static struct fuse_operations xmp_oper = {
	.init	= pre_init,
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
    .mkdir      = xmp_mkdir,
    .chmod		= xmp_chmod,
    .write		= xmp_write,
    .create		= xmp_create,
	.utimens	= xmp_utimens,
	.unlink	= xmp_unlink,
	.chown	= xmp_chown,
	.destroy	= post_destroy,
	.mknod	= xmp_mknod,
	.access	= xmp_access,
	.truncate	= xmp_truncate,
	.open	= xmp_open,

};

int main(int argc, char *argv[])
{
	umask(0);
	sprintf(urutan,"qE1~%cYMUR2%c`hNIdPzi%c^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|%c8s;g<{3.u*W-0",' ','"','%','\\');
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
