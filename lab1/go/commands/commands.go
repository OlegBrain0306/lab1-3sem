// Package commands — модуль обработки запросов.
//
// Файл данных: одна строка на структуру — "<тип> <имя> <элементы через пробел>".
// Типы: M — массив, F — односвязный список, L — двусвязный список, S — стек, Q — очередь.
// Формат файла тот же, что у версии на C++ (дерево T есть только в C++).
package commands

import (
	"errors"
	"fmt"
	"os"
	"strconv"
	"strings"

	"dbms/structures"
)

func fail(format string, args ...any) error {
	return errors.New(fmt.Sprintf(format, args...))
}

func need(q []string, count int, usage string) error {
	if len(q) != count {
		return fail("usage: %s", usage)
	}
	return nil
}

func toInt(s string) (int, error) {
	v, err := strconv.Atoi(s)
	if err != nil {
		return 0, fail("'%s' is not an integer", s)
	}
	return v, nil
}

func boolText(b bool) string {
	if b {
		return "TRUE"
	}
	return "FALSE"
}

// ---------- массив ----------

func runArray(cmd string, q, items []string) (reply string, changed bool, saved string, err error) {
	var arr structures.Array
	for _, it := range items {
		arr.Push(it)
	}
	switch cmd {
	case "MCREATE":
		if err = need(q, 2, "MCREATE name"); err != nil {
			return
		}
		reply, changed = "OK", true
	case "MPUSH":
		if err = need(q, 3, "MPUSH name value"); err != nil {
			return
		}
		arr.Push(q[2])
		reply, changed = q[2], true
	case "MINSERT":
		if err = need(q, 4, "MINSERT name index value"); err != nil {
			return
		}
		var idx int
		if idx, err = toInt(q[2]); err != nil {
			return
		}
		if !arr.Insert(idx, q[3]) {
			err = fail("index out of range")
			return
		}
		reply, changed = q[3], true
	case "MGET":
		if err = need(q, 3, "MGET name index"); err != nil {
			return
		}
		var idx int
		if idx, err = toInt(q[2]); err != nil {
			return
		}
		var ok bool
		if reply, ok = arr.Get(idx); !ok {
			err = fail("index out of range")
			return
		}
	case "MSET":
		if err = need(q, 4, "MSET name index value"); err != nil {
			return
		}
		var idx int
		if idx, err = toInt(q[2]); err != nil {
			return
		}
		if !arr.Set(idx, q[3]) {
			err = fail("index out of range")
			return
		}
		reply, changed = q[3], true
	case "MDEL":
		if err = need(q, 3, "MDEL name index"); err != nil {
			return
		}
		var idx int
		if idx, err = toInt(q[2]); err != nil {
			return
		}
		var ok bool
		if reply, ok = arr.Remove(idx); !ok {
			err = fail("index out of range")
			return
		}
		changed = true
	case "MLEN":
		if err = need(q, 2, "MLEN name"); err != nil {
			return
		}
		reply = strconv.Itoa(arr.Len())
	case "PRINT":
		reply = "[" + arr.ToString(", ") + "]"
	default:
		err = fail("unknown command %s", cmd)
		return
	}
	saved = arr.ToString(" ")
	return
}

// ---------- списки ----------

// list — общий интерфейс односвязного и двусвязного списков,
// поэтому команды F... и L... обрабатываются одной функцией
type list interface {
	PushHead(value string)
	PushTail(value string)
	PushBefore(target, value string) bool
	PushAfter(target, value string) bool
	DelHead() (string, bool)
	DelTail() (string, bool)
	DelBefore(target string) (string, bool)
	DelAfter(target string) (string, bool)
	DelValue(value string) bool
	Find(value string) bool
	Get(index int) (string, bool)
	Len() int
	ToString(sep string) string
	ToStringReverse(sep string) string
}

func runList(l list, arrow, cmd string, q, items []string) (reply string, changed bool, saved string, err error) {
	for _, it := range items {
		l.PushTail(it)
	}
	c := cmd[1:] // команда без буквы типа
	ok := true

	switch {
	case c == "PUSH" || c == "PUSHTAIL":
		if err = need(q, 3, cmd+" name value"); err != nil {
			return
		}
		l.PushTail(q[2])
		reply, changed = q[2], true
	case c == "PUSHHEAD":
		if err = need(q, 3, cmd+" name value"); err != nil {
			return
		}
		l.PushHead(q[2])
		reply, changed = q[2], true
	case c == "PUSHBEFORE":
		if err = need(q, 4, cmd+" name target value"); err != nil {
			return
		}
		if !l.PushBefore(q[2], q[3]) {
			err = fail("element '%s' not found", q[2])
			return
		}
		reply, changed = q[3], true
	case c == "PUSHAFTER":
		if err = need(q, 4, cmd+" name target value"); err != nil {
			return
		}
		if !l.PushAfter(q[2], q[3]) {
			err = fail("element '%s' not found", q[2])
			return
		}
		reply, changed = q[3], true
	case c == "DELHEAD":
		if err = need(q, 2, cmd+" name"); err != nil {
			return
		}
		if reply, ok = l.DelHead(); !ok {
			err = fail("list is empty")
			return
		}
		changed = true
	case c == "DELTAIL":
		if err = need(q, 2, cmd+" name"); err != nil {
			return
		}
		if reply, ok = l.DelTail(); !ok {
			err = fail("list is empty")
			return
		}
		changed = true
	case c == "DELBEFORE":
		if err = need(q, 3, cmd+" name target"); err != nil {
			return
		}
		if reply, ok = l.DelBefore(q[2]); !ok {
			err = fail("nothing before '%s'", q[2])
			return
		}
		changed = true
	case c == "DELAFTER":
		if err = need(q, 3, cmd+" name target"); err != nil {
			return
		}
		if reply, ok = l.DelAfter(q[2]); !ok {
			err = fail("nothing after '%s'", q[2])
			return
		}
		changed = true
	case c == "DEL":
		if err = need(q, 3, cmd+" name value"); err != nil {
			return
		}
		if !l.DelValue(q[2]) {
			err = fail("element '%s' not found", q[2])
			return
		}
		reply, changed = q[2], true
	case c == "GET":
		if err = need(q, 3, cmd+" name index"); err != nil {
			return
		}
		var idx int
		if idx, err = toInt(q[2]); err != nil {
			return
		}
		if reply, ok = l.Get(idx); !ok {
			err = fail("index out of range")
			return
		}
	case c == "FIND":
		if err = need(q, 3, cmd+" name value"); err != nil {
			return
		}
		reply = boolText(l.Find(q[2]))
	case c == "LEN":
		if err = need(q, 2, cmd+" name"); err != nil {
			return
		}
		reply = strconv.Itoa(l.Len())
	case c == "PRINTREV":
		if err = need(q, 2, cmd+" name"); err != nil {
			return
		}
		reply = l.ToStringReverse(arrow)
		if l.Len() == 0 {
			reply = "(empty)"
		}
	case cmd == "PRINT":
		reply = l.ToString(arrow)
		if l.Len() == 0 {
			reply = "(empty)"
		}
	default:
		err = fail("unknown command %s", cmd)
		return
	}
	saved = l.ToString(" ")
	return
}

// ---------- стек ----------

func runStack(cmd string, q, items []string) (reply string, changed bool, saved string, err error) {
	var st structures.Stack
	for _, it := range items { // в файле от дна к вершине
		st.Push(it)
	}
	ok := true
	switch cmd {
	case "SPUSH":
		if err = need(q, 3, "SPUSH name value"); err != nil {
			return
		}
		st.Push(q[2])
		reply, changed = q[2], true
	case "SPOP":
		if err = need(q, 2, "SPOP name"); err != nil {
			return
		}
		if reply, ok = st.Pop(); !ok {
			err = fail("stack is empty")
			return
		}
		changed = true
	case "SPEEK":
		if err = need(q, 2, "SPEEK name"); err != nil {
			return
		}
		if reply, ok = st.Peek(); !ok {
			err = fail("stack is empty")
			return
		}
	case "SLEN":
		if err = need(q, 2, "SLEN name"); err != nil {
			return
		}
		reply = strconv.Itoa(st.Len())
	case "PRINT":
		reply = "bottom [" + st.ToString(", ") + "] top"
	default:
		err = fail("unknown command %s", cmd)
		return
	}
	saved = st.ToString(" ")
	return
}

// ---------- очередь ----------

func runQueue(cmd string, q, items []string) (reply string, changed bool, saved string, err error) {
	var qu structures.Queue
	for _, it := range items { // в файле от головы к хвосту
		qu.Push(it)
	}
	ok := true
	switch cmd {
	case "QPUSH":
		if err = need(q, 3, "QPUSH name value"); err != nil {
			return
		}
		qu.Push(q[2])
		reply, changed = q[2], true
	case "QPOP":
		if err = need(q, 2, "QPOP name"); err != nil {
			return
		}
		if reply, ok = qu.Pop(); !ok {
			err = fail("queue is empty")
			return
		}
		changed = true
	case "QPEEK":
		if err = need(q, 2, "QPEEK name"); err != nil {
			return
		}
		if reply, ok = qu.Peek(); !ok {
			err = fail("queue is empty")
			return
		}
	case "QLEN":
		if err = need(q, 2, "QLEN name"); err != nil {
			return
		}
		reply = strconv.Itoa(qu.Len())
	case "PRINT":
		reply = "head [" + qu.ToString(", ") + "] tail"
	default:
		err = fail("unknown command %s", cmd)
		return
	}
	saved = qu.ToString(" ")
	return
}

// ---------- файл ----------

func readLines(fileName string) []string {
	content, err := os.ReadFile(fileName)
	if err != nil {
		return nil // файла нет — база пустая
	}
	var lines []string
	for _, line := range strings.Split(string(content), "\n") {
		line = strings.TrimRight(line, "\r") // файл, сохранённый в Windows
		if strings.TrimSpace(line) != "" {
			lines = append(lines, line)
		}
	}
	return lines
}

func writeLines(fileName string, lines []string) error {
	text := ""
	for _, line := range lines {
		text += line + "\n"
	}
	if err := os.WriteFile(fileName, []byte(text), 0644); err != nil {
		return fail("cannot write file %s", fileName)
	}
	return nil
}

func createsStructure(cmd string) bool {
	return strings.Contains(cmd, "PUSH") || cmd == "MCREATE" || cmd == "MINSERT"
}

// ExecuteQuery выполняет запрос над файлом данных и возвращает ответ.
// При ошибке файл не меняется.
func ExecuteQuery(fileName, query string) (string, error) {
	q := strings.Fields(query)
	if len(q) < 2 {
		return "", fail("query must be: COMMAND name [args]")
	}
	cmd := strings.ToUpper(q[0])
	name := q[1]

	// Ищем структуру в файле
	lines := readLines(fileName)
	found := -1
	var fileType byte
	var items []string
	for i, line := range lines {
		w := strings.Fields(line)
		if len(w) >= 2 && w[1] == name {
			found, fileType, items = i, w[0][0], w[2:]
			break
		}
	}

	// Тип структуры по команде
	var typ byte
	switch {
	case cmd == "PRINT":
		if found < 0 {
			return "", fail("structure '%s' not found", name)
		}
		typ = fileType
	case cmd == "ISMEMBER":
		typ = 'T'
	default:
		typ = cmd[0]
	}
	if typ == 'T' {
		return "", fail("trees are implemented only in the C++ version")
	}
	if !strings.ContainsRune("MFLSQ", rune(typ)) {
		return "", fail("unknown command %s", cmd)
	}
	if found >= 0 && fileType != typ {
		return "", fail("'%s' is a structure of type %c, not %c", name, fileType, typ)
	}
	if found < 0 && !createsStructure(cmd) {
		return "", fail("structure '%s' not found", name)
	}
	if found >= 0 && cmd == "MCREATE" {
		return "", fail("structure '%s' already exists", name)
	}

	var reply, saved string
	var changed bool
	var err error
	switch typ {
	case 'M':
		reply, changed, saved, err = runArray(cmd, q, items)
	case 'F':
		reply, changed, saved, err = runList(&structures.FList{}, " -> ", cmd, q, items)
	case 'L':
		reply, changed, saved, err = runList(&structures.DList{}, " <-> ", cmd, q, items)
	case 'S':
		reply, changed, saved, err = runStack(cmd, q, items)
	case 'Q':
		reply, changed, saved, err = runQueue(cmd, q, items)
	}
	if err != nil {
		return "", err
	}

	// Файл перезаписываем только если состав элементов изменился
	if changed {
		line := string(typ) + " " + name
		if saved != "" {
			line += " " + saved
		}
		if found >= 0 {
			lines[found] = line
		} else {
			lines = append(lines, line)
		}
		if err := writeLines(fileName, lines); err != nil {
			return "", err
		}
	}
	return reply, nil
}
