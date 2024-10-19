package main

import (
	"fmt"
	"io/fs"
	"os"
)

func main() {
	files, err := os.ReadDir(".")
	if err != nil {
		fmt.Println("Error on reading current directory:", err)
		return
	}

	counts := countDirEntriesByTypes(files)

	for fileType, count := range counts {
		fmt.Printf("%s: %d\n", fileType, count)
	}
}

func countDirEntriesByTypes(dirEntries []fs.DirEntry) map[string]int {
	counts := map[string]int{
		"directories":    0,
		"regular files":  0,
		"symbolic links": 0,
		"named pipes":    0,
		"sockets":        0,
		"other":          0,
	}

	for _, file := range dirEntries {
		info, err := file.Info()
		if err != nil {
			fmt.Println("Error:", err)
			continue
		}

		if info.IsDir() {
			counts["directories"]++
			continue
		}

		switch mode := info.Mode(); {
		case mode.IsRegular():
			counts["regular files"]++
		case mode&os.ModeSymlink != 0:
			counts["symbolic links"]++
		case mode&os.ModeNamedPipe != 0:
			counts["named pipes"]++
		case mode&os.ModeSocket != 0:
			counts["sockets"]++
		default:
			counts["other"]++
		}

	}

	return counts
}
