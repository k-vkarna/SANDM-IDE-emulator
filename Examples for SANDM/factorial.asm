;¬ычисление факториала
;ввод числа
read
save	0
;переменна¤ дл¤ хранени¤ текущего результата
save	4
;цикл перемножени¤ с уменьшением множител¤
;уменьшение множител¤ с проверкой завершени¤ вычислени¤ (команда #3)
sub		1
save	0
jgz		7
jmp		11
;домножение с сохранением результата (команда #7)
mul		&	4
save	4
;подготовка следующего множител¤
set		&	0
jmp		3
;отображение результата (команда #11)
set		&	4
write
