## Aydan Database

aydan_db - база данных с возможностью вводить операции на языке запросов SQL

Прежде чем запустить программу, пожалуйста, ознакомьтесь с ее функционалом. Так вы упростите себе процесс оценивания, а я получу справедливую оценку, которая будет соответствовать тому количеству часов, что я вложил в этот проект

![](https://sun9-74.userapi.com/impg/TJuguPINVFPnxtadKT7QvZuigFEu14sVA4iKBA/UIhqnuVReLg.jpg?size=1120x280&quality=96&sign=31a7acff8f3f63fed2d63fb30395aaa7&type=album)

--- 
В данный момент база данных поддерживает 2 типа данных - `string` и `number`
Столбцы имеют тип, название, а также 2 флага - primary key и auto_increment (второе с "_" потому что так в MySQL)

Названия таблиц/столбцов писать можно как \`в таких кавычках\`, так и без них.

Имеется 7 возможных команд:

### Create
Применяется для создания таблиц

Пример запроса:
```sql
create table `users`(`id` number primary key auto_increment, `name` string)
```

![](https://sun9-1.userapi.com/impg/lq-N2qDSpd2O80s5m_suUPBWZuJDfiD8Lit7CQ/HcmGFUrGOco.jpg?size=1376x202&quality=96&sign=6aa512f15cb68c46d781cc8845a13fe1&type=album)

### Insert
Применяется для добавления строк в базу данных

Примечание: Нельзя указать столбцы, к которым будут применяться значения. Осуществляется только ввод полной строки (кроме auto_increment поля)

Пример запроса:
```sql
insert into `users` values ("aydan")
```

![](https://sun9-62.userapi.com/impg/3yNiaeqKEImWymdtIABt72hzwvWcXHzJzzKKBA/28Gb17Y8gdM.jpg?size=736x260&quality=96&sign=4fe9e317c2748412ca3d4d1111abc572&type=album)

### Select
Применяется для получения строк по какому-либо условию


Примечание: поддерживается только выполнение 1 условия.

### ТАК ДЕЛАТЬ НЕ НАДО
```sql
select * from `users` where `id` = 3 and `name` = "valeriy"
```
Выше указанный запрос выполнить не получится.

Примечание 2: Вместо знака `*` указать столбцы нельзя. Если вы попытаетесь там ввести столбцы, вам выдаст ошибку. Поэтому получить можно только целиком полную строку
![](https://sun9-8.userapi.com/impg/IbYEKAjbSFjtPyoeBmpbZ59D2fCp9NsFVRSmdg/DJZhj2hsT_I.jpg?size=976x210&quality=96&sign=5559a47c8fde44a19721cec0cadaaa41&type=album)

Условие записывается как (в любом другом порядке не сработает)
``` 
поле операция значение
```

Пример запроса:
```sql
select * from `users` where `id` < 2
```

![](https://sun9-33.userapi.com/impg/jw47RGAtbt2wftvpoeAzkoFAO7QQEN4-wuN-0g/wJ9w4hDaQnQ.jpg?size=770x250&quality=96&sign=2dedd12832d9d087f2ae68559a2675d2&type=album)

Операции бывают следующие: `=`, `==` (эквивалентен `=`), `>`, `<`, `>=`, `<=`, `!=`

number принимает все вышеперечисленные операции
string принимает только операции `=`, `==` и `!=` 

Значение вводить следует строго в том типе, который принимает столбец (иначе вам ошибку выдаст о неверном типе).

### Describe
Применяется для получения информации о таблице

Пример запроса:
```sql
describe `users`
```

![](https://sun9-18.userapi.com/impg/S60INUyRoPGlE0h0wCmeRHXiigvPL4r1mRSM_g/O4ejfLlxbS8.jpg?size=828x262&quality=96&sign=78ad2b1259401871ac4e3bb84e2f0336&type=album)

### Drop
Применяется для удаления таблицы

Пример запроса:
```sql
drop table `users`
```

![](https://sun9-27.userapi.com/impg/vxR1rc7NXnRq9SYp_8NBZNj34PU1aD3NbxzSNA/6DgVXkwJq5Y.jpg?size=570x134&quality=96&sign=9fe1474e491a0a8d1554ddc38d8f12db&type=album)

## Установка 

Обратите внимание на то, что должен быть минимум C++17 (из-за __has_include в директивах #if), а еще лучше будет если у вас C++20

В данной работе я использовал:

`cmake` (для поддержки зависимостей)

`git` (и это неоднократно спасало!!!)

`Google Test` (для проверки валидности кода). Очень пригодилось в [binary::Stream](https://github.com/NolikTop/aydan_db/blob/main/src/binary/Stream.cpp), 
потому что руками там невозможно выяснить в каком байте что-то пошло не так. Также пригодилось в [parser::Parser](https://github.com/NolikTop/aydan_db/blob/main/src/parser/Parser.cpp) для проверки функций парсинга

`gulrak/filesystem` (в качестве полифилла для std::filesystem). На Mac OS по непонятной причине в C++20 нет `<filesystem>` и даже `<experimental/filesystem>`. Поэтому пришлось использовать это решение

1) `git clone --recursive git@github.com:NolikTop/aydan_db.git`
2) либо же `git clone --recursive https://github.com/NolikTop/aydan_db.git` если 1 пункт не сработает
3) Войдите в папку `aydan_db`
4) Можете открывать проект в своем любимом редакторе/IDE
5) Если он не поддерживает вдруг cmake - поищите тот что поддерживает, потому что в консоли с cmake сложновато 😁
6) Ваш редактор убедится в том что у вас нужная версия cmake (если он сказал что версия не поддерживается проектом - обновите версию в файле `CMakeLists.txt`. Есть также вариант обновить у себя на компьютере cmake)
7) Создайте папку `db` в папке проекта (`aydan_db`). Вообще можно назвать как угодно и разместить тоже где угодно, однако так будет удобнее. Запомните адрес до этой папки со слешом в конце
8) Откройте файл `src/db/FS.h` и замените переменную `dbPath`. В ней хранится путь к папке базы данных (где будут создаваться таблицы). 
    Если вы этого не сделаете, в лучшем случае (на Unix-based системе) 
    база данных скажет что папки базы данных нет, а в худшем случае (на Windows) база данных скажет, что на Windows не поддерживаются слеши в пути к файлу.
    !!! Обратите внимание, что в конце пути должен быть слеш (=обратный слеш если вы на Windows).
9) Если все предыдущие пункты выполнены верно, значит будет работать. Запускайте конфигурацию `aydan_db_run`
10) Если при запуске программы выводится какой-то бред — попробуйте в `src/utils/Colors.h` изменить макрос `SHOW_COLORS` на `0`. Если не помогло, то проверьте кодировку.

![](https://sun9-60.userapi.com/impg/jlvZhB4YwXRjs2eGX-Y1FiWd_ItyCG0k2fZefQ/szvagyu21eM.jpg?size=458x168&quality=96&sign=cb73b166c27cdc040d45676fdc5286cb&type=album)
![](https://sun9-27.userapi.com/impg/1gYAkZ4KOIisQC-00BOs-BdGRzD9vukqhLbQwQ/6QNw5yACUuU.jpg?size=456x158&quality=96&sign=0b5d3dac1abd2842ed061f2f4c66abe9&type=album)

Ура можно пользоваться

![](https://sun9-26.userapi.com/impg/eegUOzNEV1_DlGLiSl-nQLfyYwlG0IlpYmPWxw/a-YQHB4Ij60.jpg?size=1122x260&quality=96&sign=4ee7f16c8757b06c5e969e5fa9970424&type=album)