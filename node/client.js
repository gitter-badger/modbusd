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
        "ip": "172.16.9.170",
        "port": 502,
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc1",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC1");
}

// FC2
var fc2 = function(){
    var cmd = {
        "ip": "172.16.9.170",
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
        "ip": "172.16.9.170",
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
        "ip": "172.16.9.170",
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
        "ip": "172.16.9.170",
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

// FC15

// FC16


// main
setInterval(function() {
    fc1();
    fc2();
    fc3();
    fc4();
    fc5();
}, 500); // emit every 0.5 seconds


