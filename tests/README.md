#Tests
Unit test cases

# Test files
- test.c   (C code for low level function test)
- zclient  (naive zmq client to send request to modbusd)
- mbserver (naive modbus tcp simulation server)

# Install node.js 4.x & zmq binding on ubuntu

```bash
curl -sL https://deb.nodesource.com/setup_4.x | sudo -E bash -
sudo apt-get install -y nodejs
sudo npm install -g zmq              # zmq lib
```

# Build
- for c code 
```bash
mkdir build 
cd build
cmake ..
make 
./test
```

- for zclient and mbserver
```bash
sudo npm install
```