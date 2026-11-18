package structures

// Array
type Array struct {
	data []string
	size int
}

func (a *Array) grow() {
	newCap := 4
	if len(a.data) > 0 {
		newCap = len(a.data) * 2
	}
	newData := make([]string, newCap)
	for i := 0; i < a.size; i++ {
		newData[i] = a.data[i]
	}
	a.data = newData
}

// Push
func (a *Array) Push(value string) {
	if a.size == len(a.data) {
		a.grow()
	}
	a.data[a.size] = value
	a.size++
}

// Insert
func (a *Array) Insert(index int, value string) bool {
	if index < 0 || index > a.size {
		return false
	}
	if a.size == len(a.data) {
		a.grow()
	}
	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = value
	a.size++
	return true
}

// Get
func (a *Array) Get(index int) (string, bool) {
	if index < 0 || index >= a.size {
		return "", false
	}
	return a.data[index], true
}

// Set
func (a *Array) Set(index int, value string) bool {
	if index < 0 || index >= a.size {
		return false
	}
	a.data[index] = value
	return true
}

// Remove
func (a *Array) Remove(index int) (string, bool) {
	if index < 0 || index >= a.size {
		return "", false
	}
	removed := a.data[index]
	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.size--
	a.data[a.size] = ""
	return removed, true
}

// Len
func (a *Array) Len() int {
	return a.size
}

// ToString
func (a *Array) ToString(sep string) string {
	result := ""
	for i := 0; i < a.size; i++ {
		if i > 0 {
			result += sep
		}
		result += a.data[i]
	}
	return result
}
