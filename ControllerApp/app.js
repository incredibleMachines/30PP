/**
 * Module dependencies.
 *
 */

var express = require('express');
var less= require('less-middleware');
var fs = require('fs');
var colors = require('colors');
var http = require('http');
var path = require('path');

/**
 * Site Route Responses
 *
 */
 
var events = require('./routes/events');
var files = require('./routes/files');
var scenes = require('./routes/scenes');
var api = require('./routes/api');

/**
 * Custom Modules
 *
 */

var WebSocket = require('./modules/Websockets');
var Database = require('./modules/DBConnection');
var Folders = require('./modules/FolderStructure');
var AfterEffects = require('./modules/AfterEffects')

//AfterEffects.init(function(e){});
/** 
 *	File Checking 	
 *
 */ 
 
Folders.setup();

/**
 * Setup Database
 *
 */
 
Database.MongoConnect();

/**
 * Basic web socket control to PlayerApp
 *
 */

WebSocket.Connect(8080,Database); 

/**
 * Basic Express Environment setup
 *
 */

var app = express();
 
app.set('port', process.env.PORT || 3000);
app.set('title', '30 Park Place Controller');
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser({uploadDir: Folders.tempDir, keepExtensions: true})); //temporary folder to store images on upload
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
app.post('/events/:slug/delete',events.delete(Database))
 
app.get('/events/:slug/test',events.emitOne(Database,WebSocket._socket));

//asset handling pages
app.get('/files', files.index(Database));
app.post('/files', files.add(Database));
app.get('/files/:slug', files.single(Database));
app.post('/files/:slug', files.update(Database));
app.delete('/files/:slug', files.delete(Database));
app.post('files/:slug/delete',files.delete(Database));

//scene handling

app.post('/scenes',scenes.add(Database));
app.post('/scenes/reorder',scenes.reorder(Database));
app.post('/scenes/:id',scenes.update(Database));
app.delete('/scenes/:id',scenes.delete(Database));
app.post('/scenes/:id/delete',scenes.delete(Database));

//api handling
app.get('/api', api.index(Database));
app.get('/api/control/:ctrl', api.control(WebSocket))
app.get('/api/play/ambient',api.sendEvents('ambient',Database,WebSocket))
app.get('/api/play/sales',api.sendEvents('sales',Database,WebSocket))
app.get('/api/play/:slug', api.sendSingle(Database, WebSocket))


//Imaginary Routes 
//for testing or things that would be great to have.

//ALPHA ALPHA ALPHA Mostly just for faster testing and scaffolding, but could be fun in the future

//this route quits AfterEffects
app.get('/AfterEffects/close',function(req,res){
	
	AfterEffects.exit(function(e,stdout){
		if(!e) res.jsonp({result: "After Effects Closed",stdout:stdout})
		else res.jsonp(500,{error: e })

	});

})
//this route opens AfterEffects
app.get('/AfterEffects/open',function(req,res){
	AfterEffects.init(function(e, stdout){
		if(!e) res.jsonp({result: "After Effects Opened",stdout:stdout})
		else res.jsonp(500,{error: e })
	})
	
})
//open a file in AE 
app.get('/AfterEffects/open/:file',function(req,res){
	AfterEffects.open('/Users/chris/Desktop/Template_Test_Folder_3/Template_Test.aep',function(e,stdout){ 
		if(!e) res.jsonp({result:'After Effects Opened File.',stdout:stdout})
		else res.jsonp(500,{error:e})
	}) 
})
//open run a jsx funtion
app.get('/AfterEffects/script/:file',function(req,res){
	var functionCall = "updateAndRender("+JSON.stringify({stuff:'more', _this: 'this', ish: 'things' })+")";
	AfterEffects.runScriptFunction('updateAndRender.jsx',functionCall,function(e,stdout){
		if(!e) res.jsonp({result:'After Effects Ran Script', stdout: stdout})
		else res.jsonp(500,{error:e})
	}) 
	
})


//implimentation wishlist
app.get('/PlayerApp/close',function(req,res){res.jsonp(404,null)})
app.get('/PlayerApp/open',function(req,res){res.jsonp(404,null)})


/*
//location handling
app.post('/locations',locations.add(Database));
app.post('/locations/:slug',locations.update(Database));
*/

http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port '.grey + app.get('port').toString().cyan);
});
