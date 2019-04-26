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
* Tidak dapat dibaca
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.
#### Jawaban
