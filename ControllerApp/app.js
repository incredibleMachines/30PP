
/**
 * Module dependencies.
 */

var express = require('express');
var routes = require('./routes');
var http = require('http');
var path = require('path');
var WebSocket = require('./modules/Websockets');

/**
 * Basic web socket control 
 *
 */
 
var WSS = WebSocket.wss;
WSS.on('connection',function(socket){
	socket.on('message',function(msg){
		console.log('Socket Received: %s', msg);
		WebSocket.sendFakeData(socket);
	})
})

var app = express();

/**
 * Basic Express Environment setup
 */
 
app.set('port', process.env.PORT || 3000);
app.set('title', '30 Park Place Controller');
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(express.cookieParser('your secret here'));
app.use(express.session());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

/** 
 * HTTP Routes Handled by Express
 */
 
app.get('/', routes.index);

http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});
