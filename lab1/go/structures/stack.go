package structures

type sNode struct {
	data string
	next *sNode // элемент под этим
}

// Stack — стек (LIFO) на односвязном списке, вершина — голова списка.
type Stack struct {
	top  *sNode
	size int
}

func (s *Stack) Push(value string) {
	s.top = &sNode{value, s.top}
	s.size++
}

// Pop — чтение и удаление вершины
func (s *Stack) Pop() (string, bool) {
	if s.top == nil {
		return "", false
	}
	removed := s.top.data
	s.top = s.top.next
	s.size--
	return removed, true
}

// Peek — чтение вершины без удаления
func (s *Stack) Peek() (string, bool) {
	if s.top == nil {
		return "", false
	}
	return s.top.data, true
}

func (s *Stack) Len() int {
	return s.size
}

// ToString — от дна к вершине (список идёт сверху вниз, поэтому рекурсия)
func (s *Stack) ToString(sep string) string {
	return fromBottom(s.top, sep)
}

func fromBottom(node *sNode, sep string) string {
	if node == nil {
		return ""
	}
	below := fromBottom(node.next, sep)
	if below == "" {
		return node.data
	}
	return below + sep + node.data
}
