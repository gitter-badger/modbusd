var 
zmq = require('zmq')
, pub = zmq.socket('pub')
, sub = zmq.socket('sub')
, ipc_pub = "ipc:///tmp/to.modbus"
, ipc_sub = "ipc:///tmp/from.modbus"

pub.connect(ipc_pub); // connect to zmq endpoint
sub.connect(ipc_sub); // bind to zmq endpoint
sub.subscribe(""); // filter topic

// start listening response
sub.on("message", function(mode, jstr) {
    //console.log(mode.toString());
    console.log(jstr.toString());
});

// FC1
var fc1 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc1",
        "addr": 10,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC1");
}

// FC2
var fc2 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc2",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC2");
}

// FC3
var fc3 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc3",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC3");
}

// FC4
var fc4 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc4",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC4");
}

// FC5
var fc5 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc5",
        "addr": 10,
        "data": 1
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC5");
}

// FC6
var fc6 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc6",
        "addr": 10,
        "data": 1234
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC6");
}

// FC15
var fc15 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc15",
        "addr": 10,
        "len": 5,
        "data": [1,0,1,0,1]
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC15");
}

// FC16
var fc16 = function(){
    var cmd = {
        "ip": "127.0.0.1",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc16",
        "addr": 10,
        "len": 6,
        "data": [11,22,33,44,55, 33333]
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC16");
}

// main
setInterval(function() {
    fc1();
    fc2();
    fc3();
    fc4();
    fc5();
    fc6();
    fc15();
    fc16();
}, 500); // emit every 0.5 seconds


