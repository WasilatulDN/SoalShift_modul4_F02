# SoalShift_modul4_F02
Jawaban dan Penjelasan Praktikum Modul 4 Sistem Operasi 2019

## Soal Shift Modul 4
Suatu ketika King of YouTube ASEAN, Atta Halilintar, mengunjungi laboratorium AJK karena urusan yang benar-benar genting. Prestasi dan ketenarannya ternyata menarik perhatian hacker yang ingin mencuri video-video berkualitas milik Atta, bahkan ingin mencuri password akun YouTube miliknya. Menurut intel Atta, pemimpin hacker yang berusaha menyerangnya adalah:

<img src="https://github.com/WasilatulDN/SoalShift_modul4_F02/blob/master/atta.png" width="400" height="300">

Hacker tersebut menyusup ke dalam file system yang ada di dalam PC Atta dengan mudah karena ternyata file system tersebut sama sekali tidak memiliki sistem pengamanan. Maka dari itu maksud Atta ke laboratorium AJK karena dia ingin kalian untuk mengembangkan file system yang aman.
Atta menginginkan nama file system yang akan kalian buat adalah “AFSHiaAP”, yang nama kepanjangannya adalah AHHA’s File System: Hierarchical and Advanced Protection. Berikut ini kebutuhan yang harus kalian penuhi:

### Nomor 1
#### Soal
Semua nama file dan folder harus terenkripsi
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:

__qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0__

Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
__“INI_FOLDER/halo”__ saat belum di-mount maka akan bernama __“n,nsbZ]wio/QBE#”__, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi __“INI_FOLDER/halo” .__
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam * NIX, maka dari itu dapat diabaikan.
#### Jawaban
```
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
```
* Melakukan looping untuk menghitung string yang diinputkan.
* i < panjang kata, maka print karakter ke-i.
* Jika karakter ke-i bukan merupakan "/", maka untuk setiap huruf dalam array urutan dicari huruf yang sama dengan karakter ke-i. 
* Setelah itu, karakter ke-i akan diubah sesuai dengan aturan angka yang ditentukan. Misal angka yang ditentukan adalah 3, maka karakter ke-i akan menjadi 3 karakter di sebelah kanan dari karakter ke-i tersebut pada array urutan.
* Apabila lebih dari batas array urutan, maka akan dimulai lagi dari 0.
```
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
```
* Melakukan looping untuk menghitung string yang diinputkan.
* i < panjang kata, maka print karakter ke-i.
* Jika karakter ke-i bukan merupakan "/", maka untuk setiap huruf dalam array urutan dicari huruf yang sama dengan karakter ke-i. 
* Setelah itu, karakter ke-i akan diubah sesuai dengan aturan angka yang ditentukan. Misal angka yang ditentukan adalah 3, maka karakter ke-i akan menjadi 3 karakter di sebelah kiri dari karakter ke-i tersebut pada array urutan.
* Apabila kurang dari batas array urutan, maka akan dimulai lagi dari 93.

### Nomor 2
#### Soal
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”
Urutan operasi dari kebutuhan ini adalah:

a. Tepat saat sebelum file system di-mount
1. Secara otomatis folder “Videos” terbuat di root directory file system
2. Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”,       “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file             video “computer.mkv”
Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa                 file. File pecahan tersebut dijamin terletak di root folder fuse
3. Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video,        file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan             menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang               dikhususkan untuk menggabungkan file video tersebut
4. Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”
5. Jangan tampilkan file pecahan di direktori manapun

b. Tepat saat file system akan di-unmount
1. Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang           terdapat di root directory file system
2. Hapus folder “Videos” 
#### Jawaban
* Proses penggabunggan file video terdapat pada fungsi pre_init.
```
char folder[100000] = "/Videos";
enkrip(folder);
char fpath[1000];
sprintf(fpath,"%s%s", dirpath, folder);
mkdir(fpath,0755);
memset(fpath,0,sizeof(fpath));
```
* Digunakan untuk membuat folder Videos yang nantinya akan digunakan untuk menyimpan video yang telah di join. Permission dari folder Videos adalah 0755.
```
pid_t child1;
		child1=fork();
		if(child1==0){
			DIR *dp;
			struct dirent *de;
			dp = opendir(dirpath);
```
* Child proses akan melakukan pengecekan pada root file system.
```
while((de = readdir(dp))){
				if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
					char ext[1000] = ".mp4";
					enkrip(ext);
					if(strlen(de->d_name)>7 && strncmp(de->d_name+strlen(de->d_name)-8,ext,4)==0){
```
 * Tiap file pada root file system akan dicek apakah dia memiliki ekstensi .mp4 dan merupakan partisi video atau tidak.
```
              char joined[1000];
							char video[1000] = "/Videos";
							enkrip(video);
							sprintf(joined,"%s%s/",dirpath,video);
							strncat(joined,de->d_name,strlen(de->d_name)-4);
							FILE* mainj;
							mainj = fopen(joined,"a+");
```
* Mengecek pada folder Videos apakah terdapat file video dengan nama yang sama seperti file yang di cek atau tidak. Mode fopen adalah a+ sehingga jika terdapat video dengan nama yang dimaksud maka akan dilakukan append, sedangkan jika tidak terdapat file dengan nama yang dimaksud, maka akan dibuat file dengan nama yang sama.
```
              FILE* need;
							char this[1000];
							sprintf(this,"%s/%s",dirpath,de->d_name);
							need = fopen(this,"r");
							int c;
```
* Membuka file yang di cek dengan mode read.
```
                while(1) {
   								c = fgetc(need);
   								if( feof(need) ) {
   								   break;
   								}
   								fprintf(mainj,"%c",c);
   							}
```
* Menyalin isi dari file yang di cek ke file hasil join yang ada pada folder Videos. Proses tersebut dilakukan sampai seluruh isi folder telah di cek seluruhnya.

### Nomor 3
#### Soal
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
* Owner Name 	: ‘chipset’ atau ‘ic_controller’
* Group Name	: ‘rusak’
* Tidak dapat dibaca.

Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.
#### Jawaban
* Proses pengecekan owner dan grup dilakukan pada fungsi readdir.
```
dp = opendir(fpath);
if (dp == NULL)
	return -errno;

while ((de = readdir(dp)) != NULL) {
```
* Proses pengecekan dilakukan pada setiap file yang ada pada folder yang sedang di cek.
```
char name[1000];
sprintf(name, "%s/%s", fpath, de->d_name);
struct stat sfile;
stat(name, &sfile); 
```
* Digunakan untuk mendapatkan informasi dari file yang sedang dicek.
```
struct passwd *user = getpwuid(sfile.st_uid);
struct group  *group = getgrgid(sfile.st_gid);
```
* Digunakan untuk mendapatkan informasi tentang user ID dan grup ID dari file yang sedang dicek.
```
int usercomp1 = strcmp (user->pw_name, "chipset");
int usercomp2 = strcmp (user->pw_name, "ic_controller");
int grupcomp = strcmp (group->gr_name, "rusak"); 
```
* Digunakan untuk mengecek apakah nama user dari file yang dicek adalah chipset atau ic_controller juga mengecek apakah nama grup dari file yang dicek adalah rusak.
```
if ((usercomp1 == 0 || usercomp2 == 0) && grupcomp == 0 && ((sfile.st_mode & S_IRUSR) == 0 || (sfile.st_mode & S_IRGRP) == 0 || (sfile.st_mode & S_IROTH) == 0) ){
```
* Jika nama user dari file yang dicek adalah chipset atau ic_controller dan nama grup dari file yang dicek adalah rusak dan file tidak bisa dibaca, maka :
```
FILE *file;
char miris[1000]="/filemiris.txt";
char filemiris[1000];
enkrip(miris);
sprintf(filemiris, "%s%s", dirpath,miris);
file = fopen(filemiris, "a+");
```
* Digunakan untuk membuka filemiris.txt dengan mode a+. Jika filemiris.txt tidak ada, maka akan dibuat. Jika filemiris.txt ada, maka akan dilakukan penambahan pada file tersebut.
```
char isiFile[1000];
char namafile[1000];
strcpy(namafile,de->d_name);
dekrip(namafile);
```
* Digunakan untuk mendapatkan nama file yang sedang dicek.
```
char buff[20];
time_t now = time(NULL);
strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
```
* Digunakan untuk mendapatkan waktu saat file dicek dan diubah ke string.
```
sprintf(isiFile, "%s %d:%d %s\n", namafile, user->pw_uid, group->gr_gid, buff);
fputs(isiFile, file);
```
* Variabel isiFile diisi dengan nama file, user ID, grup ID, dan waktu saat file di cek.
```
fclose(file);
remove(name);
```
* File filemiris.txt ditutup dan file yang memenuhi kriteria user ID, grup ID dihapus.
### Nomor 4
#### Soal
Pada folder __YOUTUBER__, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah __“.iz1”.__ File berekstensi __“.iz1”__ tidak bisa diubah permissionnya dan memunculkan error bertuliskan *“File ekstensi iz1 tidak boleh diubah permissionnya.”*
#### Jawaban
```
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
```
* Melakukan pengecekan pada substring dan panjang path. Apakah mengandung kata "YOUTUBER" dan memiliki panjang 9 karakter atau tidak.
* Jika memenuhi syarat tersebut, maka mengubah mode menjadi 0750.

```
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
```
* Melakukan pengecekan karakter terakhir dari path. apakah mengandung karakter "iz1" dan merupakan direktori. 
* Jika syarat tersebut benar, maka akan mengeluarkan pesan error.
```
		else
		{
			while((wait(&status)) > 0);
		}
    }
```
* Menunggu hingga proses sebelumnya selesai.
```
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
```
* Jika syarat sebelumnya tidak sesuai, maka melakukan perubahan permission.
```
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
```
* Melakukan pengecekan substring pada spath. Jika mengandung kata "YOUTUBER", maka dimasukkan ke dalam variabel found.
* Jika state pada variabel found benar, maka melakukan count jumlah "/" setelah kata "YOUTUBER".
* Kemudian, pada "/" pertama setelah kata "YOUTUBER" dilakukan concat ".iz1" .

### Nomor 5
#### Soal
Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama __Backup__ kemudian hasil dari save tersebut akan disimpan pada backup dengan nama __namafile_[timestamp].ekstensi.__ Dan ketika file asli dihapus, maka akan dibuat folder bernama __RecycleBin__, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama __namafile_deleted_[timestamp].zip__ dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format __[timestamp]__ adalah __yyyy-MM-dd_HH:mm:ss__
#### Jawaban
* Pengecekan dilakukan pada fungsi write (saat mengedit) dan unlink (saat menghapus file)
```
char newname[1000];
char folder[1000] = "/Backup";
enkrip(folder);
char folderdir[1000];
sprintf(folderdir,"%s%s",dirpath,folder);
mkdir(folderdir,0755);
```
* Membuat folder Backup dengan permission 0755
```
char t[1000];
time_t now = time(NULL);
har fname[1000];
trftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));
```
* Digunakan untuk mendapatkan waktu saat file diedit dan diubah ke string.
```
int check_ext(char* file){
	ind_ext=0;
	while(ind_ext<strlen(file)&&file[ind_ext]!='.') ind_ext++;
	memset(ext,0,sizeof(ext));
	strcpy(ext,file+ind_ext);
	return ind_ext;
}
```
* Digunakan untuk mengecek ekstensi file yang diedit. ind_ext menyimpan posisi tanda titik.
```
check_ext(name);
char shortn[100000];
memset(shortn,0,strlen(shortn));
strncpy(shortn,name,ind_ext);
sprintf(newname,"/Backup%s%s%s",shortn,t,ext);
```
* Variabel newname berisi path file baru dengan format __namafile_[timestamp].ekstensi.__
```
pid_t child1;
		child1=fork();
		if(child1==0){
			execl("/bin/cp","/bin/cp",fpath,fname,NULL);
			exit(EXIT_SUCCESS);
		}
```
* Digunakan untuk menyalin isi file yang diedit ke file baru yang telah dibuat.
* Sedangkan pada saat file dihapus :
```
char folder[1000] = "/RecycleBin";
enkrip(folder);
char fpath[1000];
sprintf(fpath,"%s%s", dirpath, folder);
mkdir(fpath,0755);
```
* Digunakan untuk membuat folder RecycleBin dengan permission 0755
```
char t[1000];
time_t now = time(NULL);
strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));
```
* Digunakan untuk mendapatkan waktu saat file dihapus dan diubah ke string.
```
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
```
* Digunakan untuk menyalin file yang dihapus ke file baru
```
if(child1==0){
	execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j",fzip,dum,NULL); //hasil cp di zip di fzip
	exit(EXIT_SUCCESS);
}
else{
	waitpid(child1, &tunggu, 0);
}
remove(fname); //yang asli dihapus
```
* File baru di zip dengan format __namafile_deleted_[timestamp].zip__
```
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
```
* File back up disalin ke file baru 
```
				child1=fork();
				if(child1==0){
					execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j","-u",fzip,dum,NULL);
					exit(EXIT_SUCCESS);
				}
```
* File baru ditambahkan ke file zip dengan format __namafile_deleted_[timestamp].zip__
### Catatan
* Semua nama file dan folder yang terdapat pada soal harus memenuhi syarat soal no. 1 (terenkripsi saat belum di-mount dan terdekripsi saat di-mount)
* Tidak diperkenankan menggunakan __system()__ dan __exec*()__, kecuali ada pengecualian di butir soal.
* Untuk soal nomor 3 diharapkan secara __manual__ membuat __user__ dan __group__-nya. Pengecekan file dilakukan setiap membuka direktori.
* Untuk soal nomor 4 dan 5 diperbolehkan menggunakan exec*().
* __Pengerjaan hanya dilakukan dalam 1 file program C dengan format nama AFSHiaAP_[Kelompok].c .__ 
* File System __AFSHiaAP__ mengarah ke folder __/home/[user]/shift4.__
