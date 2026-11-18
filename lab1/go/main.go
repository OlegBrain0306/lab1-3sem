package main

import (
	"fmt"
	"os"
	"strings"

	"dbms/commands"
)

func main() {
	var fileName, query string

	args := os.Args[1:]
	for i := 0; i < len(args); i++ {
		switch args[i] {
		case "--file":
			if i+1 < len(args) {
				i++
				fileName = args[i]
			}
		case "--query":
			// собираем все слова до следующего ключа --...
			// (в Windows cmd.exe 'SPUSH s x' приходит тремя аргументами)
			for i+1 < len(args) && !strings.HasPrefix(args[i+1], "--") {
				i++
				if query != "" {
					query += " "
				}
				query += args[i]
			}
		}
	}
	query = strings.Trim(query, "'\"")

	if fileName == "" || query == "" {
		fmt.Println("usage: dbms --file <file> --query \"<COMMAND name [args]>\"")
		os.Exit(1)
	}

	reply, err := commands.ExecuteQuery(fileName, query)
	if err != nil {
		fmt.Println("-> ERROR:", err)
		os.Exit(2)
	}
	fmt.Println("-> " + reply)
}
