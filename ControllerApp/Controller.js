//super basic WS implimentation to spit out data to PlayerApp

var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({port: 8080});

wss.on('connection',function(ws){
	
	ws.on('message',function(msg){
		console.log('Socket Received: %s', msg);
		sendFakeData(ws);
	})
	

	
})

function sendFakeData(_ws) {
	
	var fakeAsset = {};
	
	fakeAsset.title = 'restaurant text';
	fakeAsset.link = 'URI://localhost/documents/media/file.mpg';
	fakeAsset.type = 'video';
	fakeAsset.location = 'location_id';
	
	var fakeData = {};
	fakeData.command = 'start';
	fakeData.title = 'restaurants';
	fakeData.assets = [fakeAsset,fakeAsset];
	fakeData.duration = 34;
	
	//We Can only send strings NOT JSON DATA!
	_ws.send(JSON.stringify(fakeData));
	
}