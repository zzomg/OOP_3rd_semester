Задача №2. Наследование. Использование исключений.

Требуется реализовать приложение Workflow Executor.
Workflow – вычислительная схема, состоящая из предопределенного набора вычислительных блоков и связей между ними. Программе подается workflow, описанный в файле.

В данной задаче рассматривается единственный тип workflow – линейный, т.е. конвейер.

Список блоков, используемых в схеме:

1. readfile <filename>  – считывание текстового файла в память, целиком. 
Вход – отсутствует, выход – текст.
2. writefile <filename> – запись текста в файл.
Вход – текст, выход – отсутствует. 
3. grep <word> – выбор из входного текста строк, разделенных символами переноса строки, содержащих заданное слово <word>.
Вход – текст, выход – текст. 
4. sort – лексикографическая сортировка входного набора строк.
Вход – текст, выход – текст.
5. replace <word1> <word2> – замена слова word1 словом word2 во входном тексте.
Вход – текст, выход – текст.
6. dump <filename> - сохранить пришедший текст в указанном файле и передать дальше.
  
https://docs.google.com/document/d/1igsUf_MCzhiXTfis-A-3iC7h38_M4J3e3tmEyWm0ZN8/edit
