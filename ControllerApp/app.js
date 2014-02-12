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

/**
 * Custom Modules
 *
 */

var WebSocket = require('./modules/Websockets');
var Database = require('./modules/DBConnection');
var Folders = require('./modules/FolderStructure');

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
/*
//location handling
app.post('/locations',locations.add(Database));
app.post('/locations/:slug',locations.update(Database));
*/

http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port '.grey + app.get('port').toString().cyan);
});
