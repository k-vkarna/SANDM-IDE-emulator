;Сравнение двух чисел, вывод наибольшего на экран
;ввод А
read
save	0
;ввод Б
read
save	4
;сравнение чисел при помощи вычитания
set		&	0
sub		&	4
jgz		10
;А <= Б
set		&	4
write
jmp		12
;А > Б (команда #10)
set		&	0
write
;перевод строки (команда #12)
set		10
write	c
;вывод OK
set		79
write	c
set		75
write	c
