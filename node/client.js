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
        "ip": "192.168.3.2",
        "port": 502,
        "slave": 22,
        "tid": 1,
        "cmd": "fc1",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("FC1");
}

// FC2
var fc2 = function(){
    var cmd = {
        "ip": "192.168.3.2",
        "port": 502,
        "slave": 22,
        "tid": 2,
        "cmd": "fc2",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("FC2");
}

// FC3
var fc3 = function(){
    var cmd = {
        "ip": "192.168.3.2",
        "port": 502,
        "slave": 22,
        "tid": 3,
        "cmd": "fc3",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("FC3");
}

// FC4
var fc4 = function(){
    var cmd = {
        "ip": "192.168.3.2",
        "port": 502,
        "slave": 22,
        "tid": 4,
        "cmd": "fc4",
        "addr": 250,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("FC4");
}

// FC5

// FC6

// FC15

// FC16


// main
setInterval(function() {
    fc1();
    fc2();
    fc3();
    fc4();
}, 500); // emit every 0.5 seconds


