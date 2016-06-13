var net = require('net');
var modbus  = require("modbus-tcp");
var assert  = require("assert");
var helpers = require("./helpers");
//var trials  = helpers.trials();
var coils   = helpers.randomBits();
var inputs  = helpers.randomBits();
var registers = helpers.randomBuffers();

var mserver = net.createServer(function (socket) {
    var server = new modbus.Server();
	server.writer().pipe(socket);
	socket.pipe(server.reader());
    
    // FC1
    server.on("read-coils", function (from, to, reply) {
        return reply(null, coils.slice(from, to + 1));
    });
    
    // FC2
    server.on("read-discrete-inputs", function (from, to, reply) {
	    return reply(null, inputs.slice(from, to + 1));
    });
    
    // FC3
    server.on("read-holding-registers", function (from, to, reply) {
        return reply(null, registers.slice(from, to + 1));
    });
    
    // FC4
    server.on("read-input-registers", function (from, to, reply) {
        return reply(null, registers.slice(from, to + 1));
    });
    
    // FC5
    server.on("write-single-coil", function (addr, val, reply) {
        if (addr % 3 === 0) {
            return reply(new Error(helpers.modbus.Exceptions.ILLEGAL_DATA_ADDRESS));
        }
        console.log(val);
        return reply();
    });
    
    // FC6
    server.on("write-single-register", function (addr, val, reply) {
        if (addr % 7 === 0) {
            return reply(new Error(helpers.modbus.Exceptions.ILLEGAL_DATA_ADDRESS));
        }
        console.log(val);
        return reply();
    });
    
    // FC15
    server.on("write-multiple-coils", function (addr, to, val, reply) {
        if (addr % 3 === 0) {
            return reply(new Error(helpers.modbus.Exceptions.ILLEGAL_DATA_ADDRESS));
        }
        console.log(val);
        return reply();
    });
    
    // FC16
    server.on("write-multiple-registers", function (addr, to, val, reply) {
        if (addr % 7 === 0) {
            return reply(new Error(helpers.modbus.Exceptions.ILLEGAL_DATA_ADDRESS));
        }
        console.log(val);
        return reply();
    });

}); // end of mserver

mserver.listen(502, function() {
	console.log('listening...');
});
