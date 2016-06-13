# Node.js ZMQ Tester
A naive zmq client to simulate proactive server

## Installation
```bash
sudo npm install 
```

## Test
```bash
node client.js
```

# Dockerlize test
```bash
# build docker image 
docker build -t takawang/modbus-zclient .
# mount file system
docker run -v /tmp:/tmp -it takawang/modbus-zclient /bin/bash
# Print app output
docker logs <container id>
# Enter the container
docker exec -it <container id> /bin/bash
```

## Note
For modbusd testing, you don't need the sub.js script. 