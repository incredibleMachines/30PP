var colors = require('colors')
var WebSocketServer = require('ws').Server;
var _socket; 
var _Database;
var _socket_status = false;


exports.Connect = function(_port,_Db){
	_Database = _Db; //setup our global _db Connection when we load our socket
	ofSocket = new WebSocketServer({port:_port})
	ofSocket.on('connection',function(socket){
		_socket = socket;
		_socket_status = true;
		console.log('New Socket Connection '.grey+' %s '.inverse,_port);
		socket.on('message',function(msg){
			console.log();
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
		
		socket.on('disconnect',function(msg){
			console.log('Socket Disconnected'.cyan);
			_socket_status=false;
		})
	})
}

//a reference object to our single websocket connection
exports.socket =  _socket;

//a status check for the socket
exports.status = _socket_status;

function parseCommand(_json){

	if(_json.hasOwnProperty('command')){
		console.log("Incoming JSON Command: ".grey+_json.command.toString().cyan)
		console.log();

		switch(_json.command){
			case 'init':
				sendPlayerInit()
			break;
			case 'status':
				processStatus(_json)
			break;
			default:
				_socket.send(JSON.stringify({'command':'error', 'error': 'Request Malformed: Unknown Command Type - '+_json.command}));
			break;
		}
	}else{
		_socket.send(JSON.stringify({command:'error', 'error':'API Malformed: No Command Property'}));
	}
	
}
function sendPlayerInit(){
	
	//console.log('Prepparing Init');
	_Database.formatInit(function(e,_events){
		if(!e) var resp = {command: 'init', events: _events};
		else  var resp = {'command':'error', 'error': 'Database Error: '+JSON.stringify(e)};
		
		console.log(JSON.stringify(resp));
		_socket.send(JSON.stringify(resp));
		
	});
	
}

