// Tester for docker

var 
zmq = require('zmq')
, should = require('should')
, links = require('docker-links').parseLinks(process.env)
//, pub = zmq.socket('pub')
//, sub = zmq.socket('sub')
, ipc_pub = "ipc:///tmp/to.modbus"
, ipc_sub = "ipc:///tmp/from.modbus"

// FC1
var fc1 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
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
var fc2 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
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
var fc3 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc3",
        "addr": 80,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC3");
}

// FC4
var fc4 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
        "slave": 1,
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "fc4",
        "addr": 80,
        "len": 10
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Send FC4");
}

// FC5
var fc5 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
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
var fc6 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
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
var fc15 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
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
var fc16 = function(pub){
    var cmd = {
        "ip": links.slave.hostname,
        "port": "502",
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

var set_timeout = function(pub){
    var cmd = {
        "tid": Math.floor((Math.random() * 10000) + 1),
        "cmd": "timeout",
        "data": 210000
    };
    pub.send("tcp", zmq.ZMQ_SNDMORE);
    pub.send(JSON.stringify(cmd));
    console.log("Set Timeout");    
}

describe('modbus.tcp.test.all', function() {
    var pub, sub;
    
    beforeEach(function() {
        pub = zmq.socket('pub');
        sub = zmq.socket('sub');
    });
    
    it("should return fc1 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            done();
        });
        
        setTimeout(function() {
            fc1(pub);
        }, 100.0);
    });
    
    it("should return fc2 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            done();
        });
        
        setTimeout(function() {
            fc2(pub);
        }, 100.0);        
    });
}

/*

var counter = 0;
// main
setInterval(function() {
    counter++;
    set_timeout();
    fc1();
    fc2();
    fc3();
    fc4();
    fc5();
    fc6();
    fc15();
    fc16();    
    if (counter >= 10) {
        process.exit();
    }
}, 300); // emit every 0.5 seconds
*/

