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
var renderer = require('./routes/renderer')
var files = require('./routes/files');
var scenes = require('./routes/scenes');
var clips = require('./routes/clips')
var api = require('./routes/api');
var timeline = require('./routes/timeline'); //*** NEW ***//

/**
 * Custom Modules
 *
 */

var WebSocket = require('./modules/Websockets');
var Database = require('./modules/DBConnection');
var Folders = require('./modules/FolderStructure');
var AfterEffects = require('./modules/AfterEffects');
var PathFinder = require('./modules/PathFinder')

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
 *	PathFinder
 *
 */

PathFinder.setup(function(){
	//test a path
	//console.log(PathFinder.returnPath({x:322,y:74}))
})

/**
 * Basic Express Environment setup
 *
 */

var app = express();

//LOCALS GLOBAL OBJECT TO BE ORGANIZED

//allow our local express files to use underscore
app.locals._ = require('underscore');
app.locals.utils = require('./modules/Utils');
app.locals.EVENT_TYPES = ["Default", "Ambient", "Gastronomy", "Fashion"]
app.locals.SCENE_TYPES = ["Full Immersion","Single Wall Sculpture","Double Wall Sculpture"]

app.set('port', process.env.PORT || 3000);
app.set('title', '30 Park Place Controller');
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser({limit:'1000mb', uploadDir: Folders.tempDir, keepExtensions: true})); //temporary folder to store images on upload
app.use(express.methodOverride());
app.use(express.cookieParser('30_PP_ControllerApp'));
app.use( less( {src: __dirname+ '/public', force: true } ) );
app.use(express.session());
app.use(express.static(path.join(__dirname, 'public')));
app.use(app.router);

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.use(function(req, res, next){
  res.render('404', {current: req.url});
});

/**
 * HTTP Routes Handled by Express
 *
 */
app.get('/', events.index(Database));
//app.get('/events/:slug/test',events.emitOne(Database,WebSocket._socket));

//event options
app.get('/events', events.index(Database));

//scene object
app.post('/scenes', scenes.add(Database));
app.get('/scenes/:slug',scenes.single(Database));
app.post('/scenes/:slug', scenes.update(Database));
app.post('/scenes/reorder/:id',scenes.reorder(Database));
app.delete('/scenes/:slug', scenes.delete(Database));
app.post('/scenes/:slug/delete',scenes.delete(Database))

//render handling
app.get('/renderqueue', renderer.index(Database));
app.post('/render',renderer.render(Database,AfterEffects,PathFinder,app.locals.EVENT_TYPES,app.locals.SCENE_TYPES))

//timeline page
app.get('/timeline',timeline.index(Database));

//asset handling pages
app.get('/files', files.index(Database));
app.get('/files/:page',files.index(Database));
app.post('/files', files.add(Database));
app.post('/files/ajax', files.addAJAX(Database));
app.post('/files/:slug', files.update(Database));
app.delete('/files/:slug', files.delete(Database));
app.post('/files/:slug/delete',files.delete(Database));

//clip handling
app.post('/clips',clips.add(Database));
app.post('/clips/reorder/:id',clips.reorder(Database));
app.post('/clips/:id',clips.update(Database));
app.delete('/clips/:id',clips.delete(Database));
app.post('/clips/:id/delete',clips.delete(Database));

app.get('/location/matrix',function(req,res){

	res.jsonp(PathFinder.returnMatrix())

})

app.post('/location/pathTest',function(req,res){

  var post = req.body;
  console.log("pathTest: "+post);
  res.jsonp(PathFinder.returnPath(post));
})

//api handling
app.get('/api', api.index(Database,app.locals.EVENT_TYPES));
app.get('/api/control/:ctrl', api.control(WebSocket))
app.get('/api/play/ambient', api.sendEvents('ambient',Database,WebSocket))
app.get('/api/play/sales', api.sendEvents('sales',Database,WebSocket))
app.get('/api/play/:slug', api.sendSingle(Database, WebSocket))


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

//create fake content for timeline collection
app.get('/timeline/make',function(req,res){
	timeline.make(Database, app.locals.EVENT_TYPES, function (e){
		if(!e){
			var timeline = new Array;
			Database.getAll('timeline',function(__e,_timeline){
				_timeline.forEach(function(event){ timeline.push(event); })
				res.jsonp({"timeline":timeline});
			})
		}
		else res.jsonp(500,{error:e})
	})
})

//app.get('/timeline/make', timeline.make(Database, app.locals.EVENT_TYPES));

//implimentation wishlist
app.get('/PlayerApp/close',function(req,res){res.jsonp(404,null)})
app.get('/PlayerApp/open',function(req,res){res.jsonp(404,null)})

http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port '.grey + app.get('port').toString().cyan);
});
