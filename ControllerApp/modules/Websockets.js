var WebSocketServer = require('ws').Server;
exports.wss = new WebSocketServer({port:8080});

exports.sendFakeData = function(_socket){
	
	var fakeAsset = {};
	
	fakeAsset.title = 'restaurant one';
	fakeAsset.link = 'URI://localhost/documents/media/file.mpg';
	fakeAsset.type = 'video';
	fakeAsset.location = 'location_id';
	
	var fakeAsset2 = JSON.parse(JSON.stringify(fakeAsset));
	fakeAsset2.title = 'restaurant two';
	fakeAsset2.link = 'URI://localhost/documents/media/file2.mpg';
	
	var fakeData = {};
	fakeData.command = 'start';
	fakeData.title = 'restaurants';
	fakeData.assets = [fakeAsset,fakeAsset2];
	fakeData.duration = 34;
	
	//We Can only send strings NOT JSON DATA!
	_socket.send(JSON.stringify(fakeData));
}


