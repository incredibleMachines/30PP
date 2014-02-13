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
				sendPlayerInit()
			break;

			case 'go':
				sendFakeSalesEvent();
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
		
		console.log(JSON.stringify(resp));
		_socket.send(JSON.stringify(resp));
		
	});
	
}

function sendFakeSalesEvent(__socket){

	var fakeCmdSales = 

	{ 	"command": "sales",
		"title": "restaurants",
        "type": "intro",
        "created_at": "01-20-2014",
        "scenes": [
            {
                "title": "scene 1",
                "type": 1,
                "assets": [
                    {
                        "title": "asset one",
                        "link": "URI://localhost/documents/media/file.mpg",
                        "type": 1,
                        "caption": "Dr. Chang's",
                        "location": {
                            "title": "location1",
                            "address": "182 Broadway, NY NY"
                        }
                    },
                    {
                        "title": "asset two",
                        "link": "URI://localhost/documents/media/file2.mpg",
                        "type": 1,
                        "caption": "Dr. Chang's",
                        "location": {
                            "title": "location2",
                            "address": "66 Park Pl, NY NY"
                        }
                    },
                    {
                        "title": "asset four",
                        "link": "URI://localhost/documents/media/file4.png",
                        "type": 2,
                        "caption": "Dr. Chang's",
                        "location": {
                            "title": "location4",
                            "address": "810 York Rd, NY NY"
                        }
                    }
                ],
                "created_at": "01-29-2014",
                "last_edited": "01-30-2014"
            },
            {
                "title": "scene 2",
                "type": 2,
                "assets": [
                    {
                        "title": "asset four",
                        "link": "URI://localhost/documents/media/file4.png",
                        "type": 0,
                        "caption": "Dr. Chang's",
                        "location": {
                            "title": "location1",
                            "address": "182 Broadway, NY NY"
                        }
                    }
                ],
                "created_at": "01-29-2014",
                "last_edited": "02-15-2014"
            }
        ]
    }
    _socket.send(JSON.stringify(fakeCmdSales));
}


/* 
sendFakeInit(socket)
• called when playerApp sends Init
• builds 4 events with variations of the same assets w/files
• sends them to playerApp as single json object {'init':[all events, with scenes, assets, file objects]}
*/

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
	fakeAsset1.link = 'fingers.mov';
	fakeAsset1.type = 1;
	fakeAsset1.zone = 0;
	fakeAsset1.caption = 'Dr. Changs';
	fakeAsset1.location = fakeLocation1;
	
	var fakeAsset2 = JSON.parse(JSON.stringify(fakeAsset1));
	fakeAsset2.title = 'asset two';
	fakeAsset2.type = 1;
	fakeAsset2.zone = 0;
	fakeAsset2.caption = 'Dr. Changs';
	fakeAsset2.link = 'fingers.mov';
	fakeAsset2.location = fakeLocation2;

	var fakeAsset3 = JSON.parse(JSON.stringify(fakeAsset1));
	fakeAsset3.title = 'asset three';
	fakeAsset3.type = 1;
	fakeAsset3.zone = 0;
	fakeAsset3.link = 'fingers.mov';
	fakeAsset3.caption = 'Dr. Changs';
	fakeAsset3.location = fakeLocation3;

	var fakeAsset4 = JSON.parse(JSON.stringify(fakeAsset1));
	fakeAsset4.title = 'asset four';
	fakeAsset4.type = 2;
	fakeAsset4.link = 'Calypso_0604.jpg';
	fakeAsset4.zone=1;
	fakeAsset4.caption="balls";
	fakeAsset4.location = fakeLocation4;

	var fakeAsset5 = JSON.parse(JSON.stringify(fakeAsset1));
	fakeAsset5.title = 'asset four';
	fakeAsset5.type = 0;
	fakeAsset5.zone=1;
	fakeAsset5.caption='balls';
	fakeAsset5.location = fakeLocation1;
	
	//--- fakeScenes
	var fakeScene1 = {};
	fakeScene1.title = "scene 1";
	fakeScene1.type = 1;
	fakeScene1.assets = [fakeAsset1, fakeAsset2, fakeAsset4];
	fakeScene1.created_at = "01-29-2014";
	fakeScene1.last_edited = "01-30-2014";

	var fakeScene2 = {};
	fakeScene2.title = "scene 2";
	fakeScene2.type = 2;
	fakeScene2.assets = [fakeAsset5];
	fakeScene2.last_edited = "02-15-2014";

	var fakeScene3 = {};
	fakeScene3.title = "scene 3";
	fakeScene3.type = 0;
	fakeScene3.assets = [fakeAsset4, fakeAsset2];
	fakeScene3.last_edited = "03-30-2014";

	//--- fakeEvents
	var fakeEvent1 = {};
	fakeEvent1.title = "restaurants"; 
	fakeEvent1.type = "sales";
	fakeEvent1.created_at = "01-20-2014";
	fakeEvent1.scenes = [fakeScene1, fakeScene2];

	var fakeEvent2 = {};
	fakeEvent2.title = "nightlife";
	fakeEvent2.type = "sales";
	fakeEvent2.created_at = "01-21-2014";
	fakeEvent2.scenes = [fakeScene3, fakeScene1, fakeScene2];

	var fakeEvent3 = {};
	fakeEvent3.title = "event3";
	fakeEvent3.type = "sales";
	fakeEvent3.created_at = "02-18-2014";
	fakeEvent3.scenes = [fakeScene2, fakeScene3, fakeScene1, fakeScene2];

	var fakeEvent4 = {};
	fakeEvent4.title = "event4";
	fakeEvent4.type = "sales";
	fakeEvent4.created_at = "02-21-2014";
	fakeEvent4.scenes = [fakeScene2, fakeScene3, fakeScene1];

	//--- fakePutItAllTogetherNow
	var fakeData = {};
	fakeData.command = 'init';
	fakeData.events = [fakeEvent1, fakeEvent2, fakeEvent3, fakeEvent4];

function sendFakeInit(__socket){
	

	
	//We Can only send strings NOT JSON DATA!
	_socket.send(JSON.stringify(fakeData));
}

