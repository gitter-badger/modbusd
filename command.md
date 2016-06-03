# Command Definition

## Write request
```javascript
{
	"ip": "192.168.3.2",
	"port": 502,
	"slave": 22,
	"tid": 1,
	"cmd": 5,
	"addr": 250,
	"len": 10,
	"data": [1,2,3,4]

}
```

## Write response
```javascript
{
	"tid": 1,
	"data": [1,2,3,4],
	"status": "ok",
}
```

## Read request
```javascript
{
	"ip": "192.168.3.2",
	"port": 502,
	"slave": 22,
	"tid": 1,
	"cmd": 1,
	"addr": 250,
	"len": 10,

}
```

## Read reponse
```javascript
{
	"tid": 1,
	"data": [1,2,3,4],
	"status": "ok",
}
```