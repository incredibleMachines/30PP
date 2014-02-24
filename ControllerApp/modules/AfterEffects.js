var util = require('util');
var exec = require('child_process').exec;
var fs = require('fs');
var async = require('async');
var AFTEREFFECTS;

/*
	
	Filepath Globals
	Takes into account the ControllerApp file structure
*/
var APPLESCRIPT_FOLDER = __dirname+"/../includes/applescripts";
var AESCRIPT_FOLDER = __dirname+"/../includes/aescripts";

//notes:

//After Effects Object for handling and processing AE Scripts ETC.
//All communication is handled between APPLE SCRIPTS AND AEScripts


/** Depreciated **/
//an object to contain all of our applescript commands.
//may need to be modified 
var AERunDoScript = { begin: "osascript -e 'tell application \"Adobe After Effects CC\"'",
					doscript:{	begin : " -e 'DoScriptFile \"",
								file: "/Users/chris/Documents/Adobe Scripts/TestScript1.jsx",
								end:"\"'" },
					end:" -e 'end tell'"
					}
					

//spin up after effects from commandline applescript
exports.init = function(cb){
	var script = "osascript "+APPLESCRIPT_FOLDER+"/AEInit.scpt";
	AFTEREFFECTS = exec(script,function(err,stout,stderr){
						if(err) console.error(err)
						cb(err)
						})
	
}
//the jsx script to load;
//the jsx function to call as a string
//the callback
exports.runScriptFunction = function(_script,_call,_cb){

	
	var script = "osascript "+APPLESCRIPT_FOLDER+"/AERunFunction.scpt '"+AESCRIPT_FOLDER+"/"+_script+"' '"+_call+"'";
	//console.log(script)
	
	AFTEREFFECTS = exec(script,function(err,stout,stderr){
						if(err) console.error(err)
						_cb(err)
						})
}

exports.open = function(file,cb){
	/* var script = "osascript -e 'tell application \"Adobe After Effects CC\"' -e 'open \"/Users/chris/IncredibleMachines/Dropbox/30PP/AE_Architecture/Template_test_Folder_3/Template_test.aep\"' -e 'end tell'"; */

	var script = "osascript "+APPLESCRIPT_FOLDER+"/AEOpenFile.scpt '"+file+"' "

	AFTEREFFECTS = exec(script,function(err,stout,stderr){
		if(err)console.error(err)
		cb(err)	
	})
}

exports.exit = function(cb){

	var script = "osascript "+APPLESCRIPT_FOLDER+"/AERunFunction.scpt '"+AESCRIPT_FOLDER+"/quitWithoutSaving.jsx' 'quitWithoutSaving()'";
	AFTEREFFECTS = exec(script,function(err,stout,stderr){
						if(err)console.error(err)
						cb(err)
						})
}


//data to render as an array[]
//render options
function renderContent(data,options,cb){
	
}


//put togeher ae script osascript based off of using the AERunDoScript Object.
function runScriptFile(file){
	
	AERunDoScript.doscript.file = file;
	
	return AERunDoScript.begin+AERunDoScript.doscript.begin+AERunDoScript.doscript.file+AERunDoScript.doscript.end+AERunDoScript.end;
	
}

//old test to create loop of movies
//needs to be depricated
exports.createMovie = function(){
	//console.log(makeScript("file"))
	var array =[];

	for(var i = 1; i<6;i++){
		
		var data = {};
		data.file = "~/Downloads/AERenderTest/AeRenderTest.aep";
		data.image="~/Desktop/images/image"+i+".jpg";
		data.text="This is Image Number "+i;
		data.output="~/Desktop/node-test"+i+".mov";
		array.push(data)
	}
	
	//use asyn to run creation of single movies
	async.eachSeries(array,createMovie,function(e){
		if(e) console.error(e);
	})
	
}

//old test to create loop of movies
//needs to be depricated
function createMovie(data,cb){
	console.log(data)
	setTimeout(function(){
		console.log('Writing File');
		//make temp file to open and pass data
		fs.writeFile('../temp.txt',JSON.stringify(data),function(e){
			if(e){
				console.log("error: "+e); 
				cb(e)
			}else{
				//execute script via cli
				child = exec(makeScript(),function(err,stout,stderr){
					if(err){
						console.error(err)
						cb(e);	
					}else{ 
						cb()
					}
				})
				
			}
		})
		
	}, 5000)
	
}