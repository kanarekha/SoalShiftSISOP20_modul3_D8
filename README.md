# SoalShiftSISOP20_modul3_D08

05111840000001 KANA REKHA

05111740000169	MUHAMMAD FARHAN HAYKAL
- [SoalShiftSISOP20_modul2_D08](#soalshiftsisop20modul2d08)
	- [Soal 2](#soal-2)
	- [Soal 3](#soal-3)
	- [Soal 4](#soal-4)
		- [4A](#4a)
		- [4B](#4b)
		- [4C](#4c)
## Soal 2
### Client
* Pertama saya buat fungsi-fungsi untuk gamenya seperti: Display Message, Receive Attack, Play, Waiting Matchmaking, Game Loops
* Lalu saya buat socketnya seperti di template
* Lalu di dalam while saya buat dari awal game seperti login dan registernya
* Sisanya saya mengisi seperti di template
### Server
* Pertama saya buat struktur untuk Account, Database, Connection, dan Room-nya
* Lalu saya buat fungsi Err untuk output error; DatabseClose untuk menutup koneksi ke database; cfileexist untuk mengecek apakah filenya ada; DatabaseWrite untuk menulis ke database
* Lalu pada line 76-105, saya membuat agar mengecek dulu apakah sudah ada databasenya atau belum; Kalau ada maka tulis ke databasenya berisi Account; Tetapi jika tidak ada maka keluar message error
* Lalu saya buat fungsi register yaitu untuk memasukkan account baru ke database
* Lalu saya buat fungsi register yaitu untuk mengecek account di database
* Lalu ada fungsi AccountList untuk me-list account berdasarkan id, username, dan password
* Saya buat fungsi GameLoops untuk melakukan looping game seperti attack dan game selesai
* Setelah itu saya buat fungsi Room agar memasukkan user 1 dan user 2, jika semua sudah masuk maka jalani GameLoops
* Lalu buat socket seperti yang ada di template
* Di dalam while pertama saya masukkan cara kerja untuk register yaitu mengambil Username dan Password dari client dan jalankan fungsi Register
* Lalu saya buat login, jika user memasukkan username dan password dengan benar maka print "Auth Success" dan lanjutkan ke fungsi AfterLogin, Tetapi jika gagal maka print "Auth Failed". Setelah itu tutup kembali databasenya
* Sisanya saya copy dari template socket

## Soal 3 
* Pertama saya buat struct untuk filename dan extensionnya
* Lalu saya mengambil extensionnya di line 20-24
* Pada line 26-44, saya membuat fungsi agar mengecek ekstensionnya jika ada yang sama maka dibiarkan tetapi jika belum ada maka dibuat baru
* Lalu pada main, pertama saya buat yang -f terlebih dahulu. Yaitu mengecek satu atau lebih file setelah itu mengecek apakah ekstensi mereka sudah ada foldernya atau belum. Setelah itu masukkan file yang disebutkan ke dalam folder ekstensinya masing-masing
* Lalu -d, tidak beda jauh dengan -f. Bedanya ia akan membuka folder yang diinginkan dahulu lalu lanjutannya sama(Mengecek ekstensi dan memindahkan sesuai ekstensi)
* Setelah itu *, tidak beda jauh dengan -d. Bedanya ia tidak masuk direktori apapun melainkan memakai direktori letak program ini berada.
* Setelah semua selesai print "All task finished, Program will now close..."

## Soal 4
### 4a
![isimatriks](https://user-images.githubusercontent.com/57948206/79039495-d6e46c00-7c0b-11ea-9278-e92535c7b80b.png)

-proses diatas merupakan proses pengisian matriks , berdasarkan petunjuk soal angka matriks dari 1-20 . pada gambar diatas sudah dideklarasi val=0 , saat pengisian isi matriks selanjutanya akan bertambah 1 hingga batas ukuran matriks

![createtread](https://user-images.githubusercontent.com/57948206/79039492-d3e97b80-7c0b-11ea-979f-4ac8b8b1c0ac.png)

-proses diatas merupakan create thread yang dibuat dan dijalankan untuk melakukan operasi perkalian pada matriks dan join tread
kemudian melaukan loop sebanyak tread.Didalam loop terdapat fungsi pthread_create untuk membuat thread dan mem-passing data sebagai parameter. Serta pthread_join untuk menunggu thread selesai.

![mult](https://user-images.githubusercontent.com/57948206/79039497-d8159900-7c0b-11ea-80be-dac88912ecca.png)

-proses diatas merupakan sebuah fungsi yang bernama mult yang menjalankan porses perkalian elemen elemen matriks

![printmatriks](https://user-images.githubusercontent.com/57948206/79039498-d8ae2f80-7c0b-11ea-9537-5b8716312fbe.png)

-proses daiatas adalah mencetak hasil dari perkalian matriks yang dilakukan oleh fungsi mult.

### 4b

![key](https://user-images.githubusercontent.com/57948206/79040156-a94df180-7c10-11ea-822a-3254d391a931.png)

-pada petunjuk soal menggunakan hasil perkalian matriks dari soal 4a menggunakan share memory.Pada proses diatas mengunakan angka unik yaitu 1400 dan melakukan proses write ke dalam share memory seperti pada program soal4a.c juga mendeklarasi angka unik 1400.
-shmget() adalah System call untuk membuat suatu segmen shared memory  fungsi shmget kemudian menempelkan (meng-attach) kan memory tsb ke program dengan fungsi shmat.shmat() adalah System call untuk mendaftarkan segment shared memory ke dalam data space dari suatu proses.shmdt() adalah System call untuk melepaskan shared memory segment dari data space dari proses.  

![4a](https://user-images.githubusercontent.com/57948206/79040580-aa345280-7c13-11ea-9334-e60f0b223d5b.png)

pada proses diatas adalah mengambil hasil perkalian elemen matriks dari soal 4a kemudian dipindahkan ke array curr kemudian dicetak.

![thread](https://user-images.githubusercontent.com/57948206/79040668-4d856780-7c14-11ea-8c85-bd527f3715ae.png)

-pada proses create thread dan memanggil fungsi sumi untuk melakuan penjumlan elemen matriks dan kemudian dijointread

![sumi](https://user-images.githubusercontent.com/57948206/79040667-4c543a80-7c14-11ea-8e96-cd95c77d4da1.png)

untuk melakukan proses diatas menggunakan rumus penjumlahan aritmatika dan kemudian dicetak
### 4c
-pertama membuat pipe jika hasilnya -1 maka akan gagal membuat pipe.

-membuat anak proses jika hasilnya kurang dari nol maka gagal membuat proses anak

-jika proses anak lebih dari nol maka akan melakukan command list file dan folder

-kemudian melakukan command wc yaitu menghitung jumlah file dan folder 
		
	
