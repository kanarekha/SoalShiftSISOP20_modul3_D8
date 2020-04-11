# SoalShiftSISOP20_modul3_D08

05111840000001 KANA REKHA

05111740000169	MUHAMMAD FARHAN HAYKAL
- [SoalShiftSISOP20_modul2_D08](#soalshiftsisop20modul2d08)
	- [Soal 1](#soal-1)
	- [Soal 4](#soal-4)
		- [3A](#4a)
		- [3B](#4b)
		- [3C](#4c)

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
### 4c
-pertama membuat pipe jika hasilnya -1 maka akan gagal membuat pipe.
-membuat anak proses jika hasilnya kurang dari nol maka gagal membuat proses anak
-jika proses anak lebih dari nol maka akan melakukan command list file dan folder
-kemudian melakukan command wc yaitu menghitung jumlah file dan folder 
		
	
