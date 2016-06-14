# Modbus daemon tester in node.js 

[![Dependency Status](https://www.versioneye.com/user/projects/57600571433d18005179252e/badge.svg?style=flat)](https://www.versioneye.com/user/projects/57600571433d18005179252e)

A naive zmq client to simulate proactive server

## Installation
```bash
sudo npm install 
```

## Test
```bash
node client.js
```

## Docker

### From the scratch
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

### Pull pre-built docker image
```bash
docker pull takawang/modbus-zclient
```

## Note
For modbusd testing, you don't need the sub.js script. 