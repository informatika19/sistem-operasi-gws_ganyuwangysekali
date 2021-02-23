# Milestone 1
## Cara Kerja Interrupt
Interrupt adalah mekanisme yang digunakan untuk komunikasi antar hardware
dengan software yang berjalan di CPU. Interrupt yang digunakan sebagai syscall
dalam kernel yang dibuat adalah interrupt 0x21. Interrupt 0x21 didaftarkan
_handler_-nya pada Interrupt Service Routine(ISR), sehingga CPU dapat
memanggil alamat _handler_ yang terdaftar pada ISR. Di masa yang akan datang,
apabila interrupt 0x21 dipanggil maka _handler_ yang terdaftar akan dipanggil
untuk mengeksekusi kode _handling_-nya.
## Cara Kerja kernel
Berkas [kernel.asm](src/kernel.asm) berisi definisi fungsi-fungsi bantuan
yang dapat digunakan dalam kernel.c, yaitu `putInMemory` yang digunakan untuk
memasukkan data ke memori, `interrupt` yang digunakan untuk memanggil
interrupt, dan `makeInterrupt21` yang digunakan untuk secara spesifik memanggil
syscall (interrupt 0x21). Selain itu, kernel.asm juga memiliki data binary
untuk boot logo. Berkas [kernel.c](src/kernel.c) berisi definisi dari fungsi
`handleInterrupt21`, yaitu kode yang bertindak sebagai eksekutor syscall.
Untuk milestone 1, syscall hanya dapat menerima permintaan untuk `readString`
yang menerima input dari keyboard untuk dimasukkan ke buffer, dan
`printString` yang mencetak isi dari buffer ke layar.