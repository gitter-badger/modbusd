// Tester for docker

var 
zmq = require('zmq')
, should = require('should')
, links = require('docker-links').parseLinks(process.env)
, ipc_pub = "ipc:///tmp/to.modbus"
, ipc_sub = "ipc:///tmp/from.modbus"

, zmq_send = function(socket, data, mode, str){
    socket.send(mode, zmq.ZMQ_SNDMORE);
    socket.send(JSON.stringify(data));
    console.log("send " + data.cmd);
}
, fc1 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc1",
    "addr": 10,
    "len": 10
}
, fc2 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc2",
    "addr": 250,
    "len": 10
}
, fc3 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc3",
    "addr": 80,
    "len": 10
}
, fc4 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc4",
    "addr": 80,
    "len": 10
}
, fc5 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc5",
    "addr": 10,
    "data": 1
}
, fc6 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc6",
    "addr": 10,
    "data": 1234
}
, fc15 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc15",
    "addr": 10,
    "len": 5,
    "data": [1,0,1,0,1]
}
, fc16 = {
    "ip": links.slave.hostname,
    "port": "502",
    "slave": 1,
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "fc16",
    "addr": 10,
    "len": 6,
    "data": [11,22,33,44,55, 33333]
}
, set_timeout = {
    "tid": Math.floor((Math.random() * 10000) + 1),
    "cmd": "timeout",
    "data": 210000
}

//
describe('Test modbusd TCP functions', function() {
    // define test timeout
    this.timeout(50000);
    
    var pub, sub;
    
    beforeEach(function() {
        pub = zmq.socket('pub');
        sub = zmq.socket('sub');
    });

    it("should return 'timeout' response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, set_timeout, "tcp");
        }, 100.0);
    });
    
    it("should return fc1 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc1.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc1, "tcp");
        }, 100.0);
    });
    
    it("should return fc2 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc2.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc2, "tcp");
        }, 100.0);    
    });

    it("should return fc3 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc3.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc3, "tcp");
        }, 100.0);    
    });

    it("should return fc4 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc4.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc4, "tcp");
        }, 100.0);  
    });

    it("should return fc5 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc5, "tcp");
        }, 100.0);        
    });

    it("should return fc6 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc6, "tcp");
        }, 100.0);        
    });
    
    it("should return fc15 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc15, "tcp");
        }, 100.0);        
    });

    it("should return fc16 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc16, "tcp");
        }, 100.0);        
    });

    // repeat

    it("should return 'timeout' response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, set_timeout, "tcp");
        }, 100.0);
    });
    
    it("should return fc1 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc1.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc1, "tcp");
        }, 100.0);
    });
    
    it("should return fc2 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc2.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc2, "tcp");
        }, 100.0);    
    });

    it("should return fc3 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc3.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc3, "tcp");
        }, 100.0);    
    });

    it("should return fc4 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.data.should.be.instanceof(Array).and.have.lengthOf(fc4.len);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc4, "tcp");
        }, 100.0);  
    });

    it("should return fc5 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc5, "tcp");
        }, 100.0);        
    });

    it("should return fc6 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc6, "tcp");
        }, 100.0);        
    });
    
    it("should return fc15 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc15, "tcp");
        }, 100.0);        
    });

    it("should return fc16 response", function (done) {
        pub.connect(ipc_pub);
        sub.connect(ipc_sub);
        sub.subscribe("");
        
        sub.on("message", function(mode, jstr) {
            var obj = JSON.parse(jstr);
            pub.close();
            sub.close();
            console.log(jstr.toString());
            mode.toString().should.equal('tcp');
            jstr.should.be.an.instanceof(Buffer);
            obj.status.should.equal("ok");
            done();
        });
        
        setTimeout(function() {
            zmq_send(pub, fc16, "tcp");
        }, 100.0);        
    });


});
