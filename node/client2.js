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
    console.log(mode.toString());
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

var mfc1 = function() {
    var cmd = {
        "ip": "192.168.3.2",
        "port": 502,
        "slave": 22,
        "tid": 1,
        "cmd": "fc1",
        "addr": 250,
        "len": 10
    };
    setInterval(function() {
        pub.send("tcp", zmq.ZMQ_SNDMORE);
        pub.send(JSON.stringify(cmd));
    }, 100); // emit every 0.1 seconds
}
// FC2

// FC3

// FC4

// FC5

// FC6

// FC15

// FC16




// main
mfc1();
