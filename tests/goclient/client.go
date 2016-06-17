package main

import (
	"encoding/json"
	"fmt"
	"time"

	zmq "github.com/taka-wang/zmq3"
)

type MbReadReq struct {
	IP    string `json:"ip"`
	Port  string `json:"port"`
	Slave int    `json:"slave"`
	Tid   int64  `json:"tid"`
	Cmd   string `json:"cmd"`
	Addr  int    `json:"addr"`
	Len   int    `json:"len"`
}

func main() {
	go subscriber()
	publisher()
	for {

	}
}

func publisher() {
	command := MbReadReq{
		"172.17.0.2",
		"502",
		1,
		12,
		"fc1",
		10,
		10,
	}

	cmd, err := json.Marshal(command) // marshal to json string
	if err != nil {
		fmt.Println("json err:", err)
	}

	sender, _ := zmq.NewSocket(zmq.PUB)
	defer sender.Close()
	sender.Connect("ipc:///tmp/to.modbus")

	for {
		sender.Send("tcp", zmq.SNDMORE) // frame 1
		sender.Send(string(cmd), 0)     // convert to string; frame 2
		time.Sleep(time.Duration(3000) * time.Millisecond)
	}
}

func subscriber() {
	receiver, _ := zmq.NewSocket(zmq.SUB)
	defer receiver.Close()
	receiver.Bind("ipc:///tmp/from.modbus")

	filter := ""
	receiver.SetSubscribe(filter) // filter frame 1
	msg, _ := receiver.RecvMessage(0)
	fmt.Println(msg[0]) // frame 1: method
	fmt.Println(msg[1]) // frame 2: command
}
