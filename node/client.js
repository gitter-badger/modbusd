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
sub.on("message", function(resp) {
    console.log(resp.toString());
});

// FC1
var fc1 = function(){
    var cmd = {
        "ip": "192.168.3.2",
        "port": 502,
        "slave": 22,
        "tid": 1,
        "mode": "tcp",
        "cmd": "fc1",
        "addr": 250,
        "len": 10
    };
    pub.send(JSON.stringify(cmd));
    console.log("FC1");
}
// FC2

// FC3

// FC4

// FC5

// FC6

// FC15

// FC16




// main
fc1();
