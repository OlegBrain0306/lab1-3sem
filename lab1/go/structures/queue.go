package structures

type qNode struct {
	data string
	next *qNode
}

// Queue — очередь (FIFO) на односвязном списке: добавляем в хвост, забираем из головы.
type Queue struct {
	head *qNode
	tail *qNode
	size int
}

func (q *Queue) Push(value string) {
	node := &qNode{value, nil}
	if q.tail == nil {
		q.head = node
	} else {
		q.tail.next = node
	}
	q.tail = node
	q.size++
}

// Pop — чтение и удаление головы
func (q *Queue) Pop() (string, bool) {
	if q.head == nil {
		return "", false
	}
	removed := q.head.data
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	q.size--
	return removed, true
}

// Peek — чтение головы без удаления
func (q *Queue) Peek() (string, bool) {
	if q.head == nil {
		return "", false
	}
	return q.head.data, true
}

func (q *Queue) Len() int {
	return q.size
}

func (q *Queue) ToString(sep string) string {
	result := ""
	for cur := q.head; cur != nil; cur = cur.next {
		if cur != q.head {
			result += sep
		}
		result += cur.data
	}
	return result
}
