var WebSocketServer = require('ws').Server;
var Socket;
exports._socket; 

exports.Connect = function(){

	ofSocket = new WebSocketServer({port:8080})
	ofSocket.on('connection',function(socket){
		_socket = socket;
		console.log('Socket Connection on 8080');
		socket.on('message',function(msg){
			console.log('Socket Received: %s', msg);
			sendFakeData(socket);
			
		})
	})

}


function sendFakeData(_socket){
	
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
	fakeData.duration = Math.floor(Math.random()*100)+10;
	
	//We Can only send strings NOT JSON DATA!
	_socket.send(JSON.stringify(fakeData));
}


exports.socket = function(){
	return _socket;
}	
