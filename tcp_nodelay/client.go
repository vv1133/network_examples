package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Args error.")
		os.Exit(1)
	}
	service := os.Args[1]
	tcpAddr, err := net.ResolveTCPAddr("tcp4", service)
	if err != nil {
		fmt.Println("Fatal error.")
		os.Exit(1)
	}
	conn, err := net.DialTCP("tcp", nil, tcpAddr)
	if err != nil {
		fmt.Println("Fatal error.")
		os.Exit(1)
	}
	buf := make([]byte, 512)
	fmt.Println("Write...")
	_, err = conn.Write(buf)
	if err != nil {
		fmt.Println("Fatal error.")
		os.Exit(1)
	}
	fmt.Println("Write...")
	_, err = conn.Write(buf)
	if err != nil {
		fmt.Println("Fatal error.")
		os.Exit(1)
	}
	_, err = conn.Read(buf)
	if err != nil {
		fmt.Println("Fatal error.")
		os.Exit(1)
	}
	fmt.Println("Close.")
	os.Exit(0)
}

