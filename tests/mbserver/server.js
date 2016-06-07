var net = require('net');
var modbus  = require("modbus-tcp");
var assert  = require("assert");
var helpers = require("./helpers");
//var trials  = helpers.trials();
var coils   = helpers.randomBits();

var mserver = net.createServer(function (socket) {
    var server = new modbus.Server();
	server.writer().pipe(socket);
	socket.pipe(server.reader());
    
    // FC1
    server.on("read-coils", function (from, to, reply) {
        return reply(null, coils.slice(from, to + 1));
    });

/*
    describe("READ_COILS", function () {
        for (var i = 0; i < trials; i++) {
            (function () {
                var from = Math.max(0, Math.floor(Math.random() * coils.length / 2));
                var to   = Math.min(coils.length, from + Math.floor(Math.random() * coils.length / 2));

                it("should pass (" + from + "-" + to + ")", function () {
                    return run(from, to);
                });
            })();
        }
    });
*/    
    
}); // end of mserver

mserver.listen(502, function() {
	console.log('listening...');
});
