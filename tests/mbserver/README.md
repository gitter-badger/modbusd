# Node.js Modbus slave simulator
A naive modbus TCP server for testing

## Installation
```bash
sudo npm install 
```

## Test
```bash
sudo node server
```

## Dockerized test
```bash
# build docker image 
docker build -t takawang/modbus-server .
# or pull images
docker pull takawang/modbus-server
# run the image (host_port:container_port)
docker run -p 502:502 -d takawang/modbus-server
# Print app output
docker logs <container id>
# Enter the container
docker exec -it <container id> /bin/bash
```

## Credit
According to [node-modbus-tcp package](https://github.com/dresende/node-modbus-tcp), I rewrite some handle functions for testing.