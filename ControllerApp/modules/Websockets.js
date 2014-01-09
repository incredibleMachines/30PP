var colors = require('colors')
var WebSocketServer = require('ws').Server;
var _socket; 
var _Database;


exports.Connect = function(_port,_Db){
	_Database = _Db; //setup our global _db Connection when we load our socket
	ofSocket = new WebSocketServer({port:_port})
	ofSocket.on('connection',function(socket){
		_socket = socket;
		console.log('New Socket Connection '.grey+' %s '.inverse,_port);
		socket.on('message',function(msg){
			console.log('Socket Received: %s'.grey, msg);

			if(typeof msg === 'object'){
				//console.log('JSON OBJECT!');
				//look for control statment
				parseCommand(msg);
			}else{
				var message = null;
				try{
					var message =JSON.parse(msg);
				}catch(e){
					_socket.send(JSON.stringify({'command':'error', 'error': 'json parse error'}));
				}
				//console.log(message);
				parseCommand(message);			
			}
			
			//sendFakeData(socket);
			
		})
	})

}

exports.socket = function(){
	return _socket;
}	

function parseCommand(_json){

	if(_json.hasOwnProperty('command')){
		console.log("Incoming JSON Command: ".grey+_json.command.toString().cyan)
		switch(_json.command){
			case 'init':
				sendPlayerInit();
			break;
			default:
				_socket.send(JSON.stringify({'command':'error', 'error': 'unknown command'}));
			break;
		}
	}
	
}
function sendPlayerInit(){
	
	//console.log('Prepparing Init');
	_Database.formatInit(function(e,_events){
		if(!e) var resp = {command: 'init', events: _events};
		else  var resp = {'command':'error', 'error': 'Database Error: '+e};
		
		console.log(JSON.stringify(resp));
		_socket.send(JSON.stringify(resp));
		
	});
	
}
function sendFakeData(__socket){
	
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
	__socket.send(JSON.stringify(fakeData));
}

