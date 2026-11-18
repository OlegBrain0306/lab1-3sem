# Лабораторная работа №1 — Абстрактные структуры данных (вариант 4)

C++: массив, односвязный и двусвязный списки, стек, очередь, **Full Binary Tree**.
Go: те же структуры без дерева (по заданию деревья только на C++).
Все структуры написаны вручную, без `vector`/`list`/`stack`/`append` и т.п.

```
lab1/
├── .vscode/tasks.json     сборка из VS Code (Ctrl+Shift+B)
├── file.data              пример файла данных
├── cpp/
│   ├── include/           заголовки (интерфейс структур)
│   ├── src/               реализация + commands.cpp (запросы) + main.cpp
│   ├── CMakeLists.txt
│   └── Makefile
└── go/
    ├── structures/        структуры данных
    ├── commands/          обработка запросов
    ├── main.go
    └── go.mod
```

## Сборка

| | Linux | Windows (MinGW g++ в PATH) |
|---|---|---|
| C++ | `cd cpp && make` | `cd cpp && mingw32-make` |
| C++ (CMake) | `cmake -S cpp -B cpp/build && cmake --build cpp/build` | то же самое (подойдёт и MSVC) |
| Go | `cd go && go build -o dbms .` | `cd go && go build -o dbms.exe .` |

В VS Code: **Ctrl+Shift+B** собирает C++; Go — `Ctrl+Shift+P → Tasks: Run Task → Go: build`.

## Запуск

```bash
# Linux / PowerShell
./dbms --file ../file.data --query 'SPUSH mystack item'
-> item
```
```bat
:: Windows cmd.exe — двойные кавычки (одинарные тоже работают, программа их уберёт)
dbms.exe --file ..\file.data --query "SPUSH mystack item"
```

Ответ — строка `-> ...`, ошибка — `-> ERROR: ...`. Регистр команды не важен.
Файл перезаписывается только когда меняется состав элементов; если файла нет, он создаётся.

## Формат файла

Одна строка на структуру: `<тип> <имя> <элементы через пробел>`

```
M myarr asd qwe zxc        массив, по порядку
F mylist asd qwe zxc       список, от головы к хвосту
L mydlist asd qwe zxc
S mystack asd qwe zxc      стек, от дна к вершине (zxc — вершина)
Q myqueue asd qwe zxc      очередь, от головы к хвосту
T mytree 1 2 # # 3 4 5     дерево по уровням, # — нет сына
```

Дерево: первое число — корень, дальше по два значения (левый, правый сын) на каждый
существующий узел в порядке обхода в ширину. Хвостовые `#` не пишутся.
`T t 1 2 3 # 5` — у узла 2 нет левого сына, правый равен 5.

## Команды

**Массив (M):** `MCREATE name`, `MPUSH name value`, `MINSERT name index value`, `MGET name index`,
`MSET name index value`, `MDEL name index`, `MLEN name`

**Односвязный (F) и двусвязный (L) список** — одинаковые команды, буква F или L:

| команда | действие |
|---|---|
| `FPUSH name value` / `FPUSHTAIL` | в хвост |
| `FPUSHHEAD name value` | в голову |
| `FPUSHBEFORE name target value` | перед target |
| `FPUSHAFTER name target value` | после target |
| `FDELHEAD name` / `FDELTAIL name` | удалить голову / хвост |
| `FDELBEFORE name target` / `FDELAFTER name target` | удалить перед / после target |
| `FDEL name value` | удалить по значению |
| `FGET name index` | чтение по индексу |
| `FFIND name value` | поиск → TRUE/FALSE |
| `FLEN name` | длина |
| `FPRINTREV name` | чтение от хвоста к голове |

**Стек (S):** `SPUSH name value`, `SPOP name` (чтение + удаление), `SPEEK name` (без удаления), `SLEN name`

**Очередь (Q):** `QPUSH name value`, `QPOP name`, `QPEEK name`, `QLEN name`

**Full Binary Tree (T)** — у каждого узла 0 или 2 сына; элементы — целые числа без повторов:

| команда | действие |
|---|---|
| `TINSERT name number` | в первое свободное место по уровням |
| `TINSERTAT name parent L\|R number` | левым/правым сыном к parent |
| `TPAIR name parent left right` | сразу два сына к листу parent (дерево остаётся Full); на пустом дереве parent становится корнем |
| `TGET name number`, `ISMEMBER name number` | поиск → TRUE/FALSE |
| `TISFULL name` | проверка на Full Binary Tree (вместо удаления) → TRUE/FALSE |
| `TPREORDER name` | прямой обход: корень → левый → правый |
| `TBFS name` | обход в ширину (по уровням, слева направо) |

**Все структуры:** `PRINT name` (для дерева — значения обходом в ширину, без рисунка).

## Пример

```
$ ./dbms --file t.data --query 'TINSERT tr 1'     -> 1
$ ./dbms --file t.data --query 'TINSERT tr 2'     -> 2
$ ./dbms --file t.data --query 'TISFULL tr'       -> FALSE
$ ./dbms --file t.data --query 'TINSERT tr 3'     -> 3
$ ./dbms --file t.data --query 'TISFULL tr'       -> TRUE
$ ./dbms --file t.data --query 'TPAIR tr 2 4 5'   -> 4 5
$ ./dbms --file t.data --query 'PRINT tr'       -> 1 2 3 4 5
```

## Обход дерева на защите

Используй `TBFS mytree`: обход в ширину (сначала корень, затем узлы каждого уровня слева направо).
В исходном коде функция `bfs()` использует очередь `PtrQueue<Node>` и цикл `while`.
Дополнительно оставлена команда `TPREORDER` для прямого рекурсивного обхода.
Удалены необязательные операции симметричного/обратного обхода, высоты и рисунка.
Вставка `TINSERT`, сохранение и чтение файла по-прежнему используют порядок по уровням,
чтобы сохранить формат `file.data`. Поиск `TGET` теперь выполняется прямым обходом.
