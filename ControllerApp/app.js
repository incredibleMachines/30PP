/**
 * Module dependencies.
 *
 */

var express = require('express');
var less= require('less-middleware');
var fs = require('fs');
var colors = require('colors');


var events = require('./routes/events');
var assets = require('./routes/assets');
var http = require('http');
var path = require('path');
var WebSocket = require('./modules/Websockets');
var Database = require('./modules/DBConnection');

/** 
 *	File Checking 	
 *	
 */ 

var tempDir= './.tmp'; 
var assetsDir = './../.assets';
//check assetsDirectory
try {
    // Query the entry
    var a = fs.lstatSync(assetsDir);

    // Is it a directory?
    if (a.isDirectory()) {
        // Yes it is
        console.log("Directory Assets Exists".green)
    }else{
	    console.log("No Assets Directory".red)
	    fs.mkdirSync(assetsDir);

    }
}catch (e) {
    // ...
    console.error("Assets Find Error: ".red+e);
    
    //make a folder
    fs.mkdirSync(assetsDir);
}
//check temp file directory
try{
	var t = fs.lstatSync(tempDir);
	if(t.isDirectory()){
		console.log("Directory Temp Exists".green);
	}else{
		console.log("No Temp Directory".red);
		fs.mkdirSync(tempDir);
	}
	
}catch(e){
	console.error("Temp Directory Find Error: ".red+e);
	fs.mkdirSync(tempDir);
}

try{
	
	var v = fs.lstatSync(assetsDir+"/video");
	if(v.isDirectory()){
		console.log("Video Directory Exists".green);
	}else{
		console.log("No Video Directory".red);
		fs.mkdirSync(assetsDir+"/video");
	}
}catch(e){
		console.log("No Video Directory".red);
		fs.mkdirSync(assetsDir+"/video");
}

try{
	
	var i = fs.lstatSync(assetsDir+"/image");
	if(i.isDirectory()){
		console.log("Image Directory Exists".green);
	}else{
		console.log("No Image Directory".red);
		fs.mkdirSync(assetsDir+"/image");
	}
}catch(e){
		console.log("No Image Directory".red);
		fs.mkdirSync(assetsDir+"/image");
}

/**
 * Setup Database
 *
 */
 
Database.MongoConnect();

/**
 * Basic web socket control to PlayerApp
 *
 */

WebSocket.Connect(); 

var app = express();

/**
 * Basic Express Environment setup
 *
 */
 
app.set('port', process.env.PORT || 3000);
app.set('title', '30 Park Place Controller');
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser({uploadDir: tempDir, keepExtensions: true})); //temporary folder to store images
app.use(express.methodOverride());
app.use(express.cookieParser('30_PP_ControllerApp'));
app.use( less( {src: __dirname+ '/public', force: true } ) );
app.use(express.session());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

/** 
 * HTTP Routes Handled by Express
 *
 */


app.get('/', events.index(Database));

app.get('/events', events.index(Database));
app.post('/events', events.add(Database));

app.get('/events/:slug',events.single(Database));
app.post('/events/:slug', events.update(Database));
app.delete('/events/:slug', events.delete(Database));
 
app.get('/events/:slug/test',events.emitOne(Database,WebSocket._socket));

//asset handling pages
app.post('/assets', assets.add(Database));
app.get('/assets/:slug', assets.single(Database));
app.post('/assets/:slug', assets.update(Database));
app.delete('/assets/:slug', assets.delete(Database));


http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port '.grey + app.get('port').toString().cyan);
});
