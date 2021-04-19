# GWS_GanyuWangySekali

## Milestone 1: Interrupt
[spek](https://s.id/spesifikasi-os19)
[milestone](MILESTONE1.md)

## Milestone 2: Files
[spek](https://s.id/m2-os19)

## Milestone 3: Program dengan syscall
[spek](https://docs.google.com/document/d/1K4EkxrE-Cb1YSwAZhkAek1LczryHC98v90HwxxaCPaI/edit)
Penggunaan program:
- cat: `cat <file>`, dengan file adalah file text yang dapat dibaca
- cd: `cd <directory>`, dengan directory adalah folder yang ada. Direktori khusus mencakup `..` untuk parent folder, `.` untuk folder sekarang, dan `/` untuk root. Setiap folder dipisahkan dengan `/`.
- cp: `cp [-r] <source> <dest>`, dengan source adalah file asal dan dest adalah nama file baru. Flag `-r` digunakan untuk mengkopi sebuah folder secara rekursif.
- ln: `ln [-s] <source> <dest>`, dengan source adalah file asal dan dest adalah nama file baru yang akan menunjuk ke file asal. Flag `-s` digunakan untuk membentuk soft link.
- ls: `ls [directory]`, dengan directory adalah directory yang ingin dilihat isinya. apabila directory tidak diberikan, maka ls akan menampilkan isi dari folder shell saat ini.
- rm: `rm <file>`, dengan file adalah file yang akan dihapus.
- shell: `shell`.