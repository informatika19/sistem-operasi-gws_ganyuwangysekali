# GWS_GanyuWangySekali

## Milestone 1: Interrupt
[spek](https://s.id/spesifikasi-os19)
[milestone](MILESTONE1.md)

## Milestone 2: Files
[spek](https://s.id/m2-os19)

## Milestone 3: Program dengan syscall
[spek](https://docs.google.com/document/d/1K4EkxrE-Cb1YSwAZhkAek1LczryHC98v90HwxxaCPaI/edit) </br>
Penggunaan program:
- cat: `cat <file>`, dengan file adalah file text yang dapat dibaca
- cd: `cd <directory>`, dengan directory adalah folder yang ada. Direktori khusus mencakup `..` untuk parent folder, `.` untuk folder sekarang, dan `/` untuk root. Setiap folder dipisahkan dengan `/`.
- cp: `cp <source> <dest>`, dengan source adalah file asal dan dest adalah nama file baru.
- ln: `ln [-s] <source> <dest>`, dengan source adalah file asal dan dest adalah nama file baru yang akan menunjuk ke file asal. Flag `-s` digunakan untuk membentuk soft link.
- ls: `ls [directory]`, dengan directory adalah directory yang ingin dilihat isinya. Apabila directory tidak diberikan, maka ls akan menampilkan isi dari folder shell saat ini.
- mkdir: `mkdir <directory>`, dengan directory adalah nama folder baru yang akan dibuat. 
- mv: `mv <source> <dest>`, dengan source adalah file asal dengan pathnya dan dest adalah nama directory. 
- rm: `rm [-r] <file>`, dengan file adalah file yang akan dihapus. Flag `-r` digunakan untuk menghapus sebuah folder secara rekursif.
- shell: `shell`.
- game: `game`, memainkan sebuah permain sederhana gunting-kertas-batu.
