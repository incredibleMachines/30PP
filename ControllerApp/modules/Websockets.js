var colors = require('colors')
var WebSocketServer = require('ws').Server;
var _socket;
var _Database;
var _socket_status;
var folders = require('../modules/FolderStructure');
var _playerApp;


exports.Connect = function(_port,_Db,_pA){
	_Database = _Db; //setup our global _db Connection when we load our socket
	_playerApp = _pA;
	ofSocket = new WebSocketServer({port:_port})
	_socket_status = false;
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
					console.error("WEBSOCKET COMMAND - JSON PARSE Error: "+e)
				}
				//console.log(message);
				parseCommand(message);
			}

			//sendFakeData(socket);

		})

		socket.on('close',function(msg){
			console.log('Socket Disconnected'.cyan);
			_socket_status=false;
		})
	})
}

//a reference object to our single websocket connection
exports.socket = function(cb){ cb(_socket) };


//a status check for the socket
exports.status = function(cb){ cb(_socket_status)}


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
			case 'close':
					_playerApp.closeState(true)
					_playerApp.end()
			break;
			case 'restart':
					_playerApp.end()
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
	_playerApp.activate()
	var OUTPUT_FOLDER = folders.outputDir();
	//console.log('Prepparing Init');
	_Database.formatInit(function(e,_events){
		if(!e) var resp = {
			command: 'init',
			movie_file: OUTPUT_FOLDER+"/output.mov",
			events:_events
		};
		else  var resp = {'command':'error', 'error': 'Database Error: '+JSON.stringify(e)};

		console.log(JSON.stringify(resp));
		_socket.send(JSON.stringify(resp));

	});
}
