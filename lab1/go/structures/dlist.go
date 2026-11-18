package structures

type dNode struct {
	data string
	prev *dNode
	next *dNode
}

// DList
type DList struct {
	head *dNode
	tail *dNode
	size int
}

func (l *DList) findNode(value string) *dNode {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.data == value {
			return cur
		}
	}
	return nil
}

// unlink
func (l *DList) unlink(node *dNode) {
	if node.prev != nil {
		node.prev.next = node.next
	} else {
		l.head = node.next
	}
	if node.next != nil {
		node.next.prev = node.prev
	} else {
		l.tail = node.prev
	}
	l.size--
}

func (l *DList) PushHead(value string) {
	node := &dNode{value, nil, l.head}
	if l.head != nil {
		l.head.prev = node
	} else {
		l.tail = node
	}
	l.head = node
	l.size++
}

func (l *DList) PushTail(value string) {
	node := &dNode{value, l.tail, nil}
	if l.tail != nil {
		l.tail.next = node
	} else {
		l.head = node
	}
	l.tail = node
	l.size++
}

func (l *DList) PushBefore(target, value string) bool {
	cur := l.findNode(target)
	if cur == nil {
		return false
	}
	if cur == l.head {
		l.PushHead(value)
		return true
	}
	node := &dNode{value, cur.prev, cur}
	cur.prev.next = node
	cur.prev = node
	l.size++
	return true
}

func (l *DList) PushAfter(target, value string) bool {
	cur := l.findNode(target)
	if cur == nil {
		return false
	}
	if cur == l.tail {
		l.PushTail(value)
		return true
	}
	node := &dNode{value, cur, cur.next}
	cur.next.prev = node
	cur.next = node
	l.size++
	return true
}

func (l *DList) DelHead() (string, bool) {
	if l.head == nil {
		return "", false
	}
	removed := l.head.data
	l.unlink(l.head)
	return removed, true
}

// DelTail
func (l *DList) DelTail() (string, bool) {
	if l.tail == nil {
		return "", false
	}
	removed := l.tail.data
	l.unlink(l.tail)
	return removed, true
}

func (l *DList) DelBefore(target string) (string, bool) {
	cur := l.findNode(target)
	if cur == nil || cur.prev == nil {
		return "", false
	}
	removed := cur.prev.data
	l.unlink(cur.prev)
	return removed, true
}

func (l *DList) DelAfter(target string) (string, bool) {
	cur := l.findNode(target)
	if cur == nil || cur.next == nil {
		return "", false
	}
	removed := cur.next.data
	l.unlink(cur.next)
	return removed, true
}

func (l *DList) DelValue(value string) bool {
	cur := l.findNode(value)
	if cur == nil {
		return false
	}
	l.unlink(cur)
	return true
}

func (l *DList) Find(value string) bool {
	return l.findNode(value) != nil
}

// Get
func (l *DList) Get(index int) (string, bool) {
	if index < 0 || index >= l.size {
		return "", false
	}
	var cur *dNode
	if index < l.size/2 {
		cur = l.head
		for i := 0; i < index; i++ {
			cur = cur.next
		}
	} else {
		cur = l.tail
		for i := l.size - 1; i > index; i-- {
			cur = cur.prev
		}
	}
	return cur.data, true
}

func (l *DList) Len() int {
	return l.size
}

func (l *DList) ToString(sep string) string {
	result := ""
	for cur := l.head; cur != nil; cur = cur.next {
		if cur != l.head {
			result += sep
		}
		result += cur.data
	}
	return result
}

func (l *DList) ToStringReverse(sep string) string {
	result := ""
	for cur := l.tail; cur != nil; cur = cur.prev {
		if cur != l.tail {
			result += sep
		}
		result += cur.data
	}
	return result
}
