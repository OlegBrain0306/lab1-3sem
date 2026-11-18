package structures

type fNode struct {
	data string
	next *fNode
}

// FList
type FList struct {
	head *fNode
	tail *fNode
	size int
}

func (l *FList) PushHead(value string) {
	l.head = &fNode{value, l.head}
	if l.tail == nil {
		l.tail = l.head
	}
	l.size++
}

func (l *FList) PushTail(value string) {
	node := &fNode{value, nil}
	if l.tail == nil {
		l.head = node
	} else {
		l.tail.next = node
	}
	l.tail = node
	l.size++
}

// PushBefore
func (l *FList) PushBefore(target, value string) bool {
	if l.head == nil {
		return false
	}
	if l.head.data == target {
		l.PushHead(value)
		return true
	}
	for cur := l.head; cur.next != nil; cur = cur.next {
		if cur.next.data == target {
			cur.next = &fNode{value, cur.next}
			l.size++
			return true
		}
	}
	return false
}

func (l *FList) PushAfter(target, value string) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.data == target {
			cur.next = &fNode{value, cur.next}
			if cur == l.tail {
				l.tail = cur.next
			}
			l.size++
			return true
		}
	}
	return false
}

func (l *FList) DelHead() (string, bool) {
	if l.head == nil {
		return "", false
	}
	removed := l.head.data
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return removed, true
}

// DelTail
func (l *FList) DelTail() (string, bool) {
	if l.head == nil {
		return "", false
	}
	if l.head == l.tail {
		return l.DelHead()
	}
	cur := l.head
	for cur.next != l.tail {
		cur = cur.next
	}
	removed := l.tail.data
	cur.next = nil
	l.tail = cur
	l.size--
	return removed, true
}

func (l *FList) DelBefore(target string) (string, bool) {
	if l.head == nil || l.head.data == target {
		return "", false
	}
	if l.head.next != nil && l.head.next.data == target {
		return l.DelHead()
	}
	for prev := l.head; prev.next != nil && prev.next.next != nil; prev = prev.next {
		if prev.next.next.data == target {
			removed := prev.next.data
			prev.next = prev.next.next
			l.size--
			return removed, true
		}
	}
	return "", false
}

func (l *FList) DelAfter(target string) (string, bool) {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.data == target {
			victim := cur.next
			if victim == nil {
				return "", false
			}
			cur.next = victim.next
			if victim == l.tail {
				l.tail = cur
			}
			l.size--
			return victim.data, true
		}
	}
	return "", false
}

// DelValue
func (l *FList) DelValue(value string) bool {
	if l.head == nil {
		return false
	}
	if l.head.data == value {
		l.DelHead()
		return true
	}
	for prev := l.head; prev.next != nil; prev = prev.next {
		if prev.next.data == value {
			if prev.next == l.tail {
				l.tail = prev
			}
			prev.next = prev.next.next
			l.size--
			return true
		}
	}
	return false
}

func (l *FList) Find(value string) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.data == value {
			return true
		}
	}
	return false
}

func (l *FList) Get(index int) (string, bool) {
	if index < 0 || index >= l.size {
		return "", false
	}
	cur := l.head
	for i := 0; i < index; i++ {
		cur = cur.next
	}
	return cur.data, true
}

func (l *FList) Len() int {
	return l.size
}

func (l *FList) ToString(sep string) string {
	result := ""
	for cur := l.head; cur != nil; cur = cur.next {
		if cur != l.head {
			result += sep
		}
		result += cur.data
	}
	return result
}

// ToStringReverse
func (l *FList) ToStringReverse(sep string) string {
	return fReverse(l.head, sep)
}

func fReverse(node *fNode, sep string) string {
	if node == nil {
		return ""
	}
	rest := fReverse(node.next, sep)
	if rest == "" {
		return node.data
	}
	return rest + sep + node.data
}
