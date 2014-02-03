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
	})

}

//a reference object to our single websocket connection
exports.socket = function(){
	return _socket;
}	

function parseCommand(_json){

	if(_json.hasOwnProperty('command')){
		console.log("Incoming JSON Command: ".grey+_json.command.toString().cyan)
		console.log();

		switch(_json.command){
			case 'init':
				sendFakeData();
			break;
			default:
				_socket.send(JSON.stringify({'command':'error', 'error': 'unknown command'}));
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
		
		//console.log(JSON.stringify(resp));
		_socket.send(JSON.stringify(resp));
		
	});
	
}
function sendFakeData(__socket){
	
	//--- fakeLocations
	var fakeLocation1 = {};
	fakeLocation1.title = "location1";
	fakeLocation1.address = "182 Broadway, NY NY";

	var fakeLocation2 = {};
	fakeLocation2.title = "location2";
	fakeLocation2.address = "66 Park Pl, NY NY";
	
	var fakeLocation3 = {};
	fakeLocation3.title = "location3";
	fakeLocation3.address = "1 Ludlow Rd, NY NY";

	var fakeLocation4 = {};
	fakeLocation4.title = "location4";
	fakeLocation4.address = "810 York Rd, NY NY";

	//--- fakeAssets
	var fakeAsset1 = {};
	fakeAsset1.title = 'asset one';
	fakeAsset1.link = 'URI://localhost/documents/media/file.mpg';
	fakeAsset1.type = 'video';
	fakeAsset1.location = fakeLocation1;
	
	var fakeAsset2 = JSON.parse(JSON.stringify(fakeAsset1));
	fakeAsset2.title = 'asset two';
	fakeAsset2.link = 'URI://localhost/documents/media/file2.mpg';
	fakeAsset2.location = fakeLocation2;

	var fakeAsset3 = JSON.parse(JSON.stringify(fakeAsset1));
	fakeAsset3.title = 'asset three';
	fakeAsset3.link = 'URI://localhost/documents/media/file3.mpg';
	fakeAsset3.location = fakeLocation3;

	var fakeAsset4 = JSON.parse(JSON.stringify(fakeAsset1));
	fakeAsset4.title = 'asset four';
	fakeAsset4.type = 'image';
	fakeAsset4.link = 'URI://localhost/documents/media/file4.png';
	fakeAsset4.location = fakeLocation4;
	
	//--- fakeScenes
	var fakeScene1 = {};
	fakeScene1.title = "scene 1";
	fakeScene1.type = "type 1";
	fakeScene1.assets = [fakeAsset1, fakeAsset2, fakeAsset4];
	fakeScene1.created_at = "01-29-2014";
	fakeScene1.last_edited = "01-30-2014";

	var fakeScene2 = JSON.parse(JSON.stringify(fakeScene1));
	fakeScene2.title = "scene 2";
	fakeScene2.assets = [fakeAsset4, fakeAsset2];
	fakeScene2.last_edited = "03-30-2014";

	//--- fakeEvents
	var fakeEvent1 = {};
	fakeEvent1.title = "restaurants"; 
	fakeEvent1.type = "intro";
	fakeEvent1.created_at = "01-20-2014";
	fakeEvent1.scenes = [fakeScene1, fakeScene2];

	var fakeEvent2 = {};
	fakeEvent2.title = "nightlife";
	fakeEvent2.type = "sales";
	fakeEvent2.created_at = "01-21-2014";
	fakeEvent2.scenes = [fakeScene2, fakeScene1, fakeScene2];

	var fakeEvent3 = {};
	fakeEvent3.title = "event3";
	fakeEvent3.type = "outro";
	fakeEvent3.created_at = "02-18-2014";
	fakeEvent3.scenes = [fakeScene1, fakeScene2, fakeScene1, fakeScene2];

	//--- fakePutItAllTogetherNow
	var fakeData = {};
	fakeData.command = 'init';
	fakeData.events = [fakeEvent1, fakeEvent2, fakeEvent3];
	
	//We Can only send strings NOT JSON DATA!
	_socket.send(JSON.stringify(fakeData));
}

