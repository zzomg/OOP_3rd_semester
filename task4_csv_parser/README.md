Упрощенный парсер CSV

Subtask #1 - print tuple
Используя рекурсивные шаблоны реализовать оператор для печати std:tuple:
…..
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
…..

Subtask #2 - simple CSV parser

CSV format
Табличные данные могут быть представлены как текстовый файл с разделителем ‘\n’ между строками и символом ‘,’ для разделения ячеек внутри строки. Считаем что данные символы не встречаются внутри данных.


CSVParser
Написать класс делающий возможным следующую потоковую работу с CSV

https://docs.google.com/document/d/1FXmDYtpTG6WsjFtm9lysJbni3InftOnkLLeVQ8VwDVY/edit?ts=58423c1a#
