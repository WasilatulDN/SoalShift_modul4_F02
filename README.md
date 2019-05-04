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

### Nomor 3
#### Soal
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
* Owner Name 	: ‘chipset’ atau ‘ic_controller’
* Group Name	: ‘rusak’
* Tidak dapat dibaca.

Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.
#### Jawaban

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

### Catatan
* Semua nama file dan folder yang terdapat pada soal harus memenuhi syarat soal no. 1 (terenkripsi saat belum di-mount dan terdekripsi saat di-mount)
* Tidak diperkenankan menggunakan __system()__ dan __exec*()__, kecuali ada pengecualian di butir soal.
* Untuk soal nomor 3 diharapkan secara __manual__ membuat __user__ dan __group__-nya. Pengecekan file dilakukan setiap membuka direktori.
* Untuk soal nomor 4 dan 5 diperbolehkan menggunakan exec*().
* __Pengerjaan hanya dilakukan dalam 1 file program C dengan format nama AFSHiaAP_[Kelompok].c .__ 
* File System __AFSHiaAP__ mengarah ke folder __/home/[user]/shift4.__
