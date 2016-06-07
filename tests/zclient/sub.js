var zmq = require('zmq')
, sub = zmq.socket('sub')

sub.bindSync("ipc:///tmp/to.modbus"); // bind to zmq endpoint
sub.subscribe(""); // filter topic

sub.on("message", function(mode, jstr) {
    console.log(mode.toString());
    console.log(jstr.toString());
});