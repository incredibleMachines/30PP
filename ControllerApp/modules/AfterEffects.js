var util 	= require('util'),
	exec	= require('child_process').exec,
	spawn   = require('child_process').spawn,
	fs		= require('fs'),
	async 	= require('async'),
	folders = require('../modules/FolderStructure'),
	utils 	= require('../modules/Utils');





//NEED TO HAVE AE CURRENTLY RENDERING VARIABLE & Handler
var AFTEREFFECTS,
	bRendering = false,
	bOpen = false,
	currentFile;

/*

	Filepath Globals
	Takes into account the ControllerApp file structure

	needs modifications to ControllerApp Route
*/
var APPLESCRIPT_FOLDER = __dirname+"/../includes/applescripts";
var AESCRIPT_FOLDER = __dirname+"/../includes/aescripts";
var AEPROJECT_FOLDER = __dirname+"/../includes/aeprojects";
var OUTPUT_FOLDER = __dirname+"/../includes/videos";//folders.outputDir;
var ASSET_FOLDER = __dirname+"/../public";


var OM_TEMPLATE = 'ProRes222';
//notes:

// After Effects Object for handling and processing AE Scripts ETC.
// All communication is handled between APPLE SCRIPTS AND AEScripts


/* //SAMPLE OBJECT TO REPLACE TEXT AND IMAGES and RENDER OUT the videos
var sample = {
	file: "/Users/chris/oF/080/apps/30PP/ControllerApp/includes/aeprojects/Seamless_L-R/Seamless_L-R.aep",
	modifications: {	//modifications key specifies the composition names in the AE file to be modified and the updated image to be replaced.
						Source_Text: 	"So much new text is here now!",
						Source_Image1: 	"/Users/chris/Desktop/images/image2.jpg",
						Source_Image2: 	"/Users/chris/Desktop/images/image5.jpg"
	},
    output_options:{		// output_options key specifies the composition names in the AE file to render - as well as the root folder and file name for its render location
                            UV_L: "/Users/chris/Desktop/filename_1",
                            UV_R: "/Users/chris/Desktop/filename_1",
                            UV_LR: "/Users/chris/Desktop/filename_1",
                            //UV_S: "/Users/chris/Desktop/filename_1"
     }
}
*/


/** Deprecated **/
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
	AFTEREFFECTS = exec(script,function(err,stdout,stderr){
						bOpen = true;
						if(err) console.error(err)
						cb(err)
						})

}
//_script = the jsx script to load;
//_call = the jsx function to call as a string containing the JSON Object of the result
//example -- see sample json reference above

//script = 'updateAndRenderBasic.jsx'

//_call = 'updateAndRenderBasic('+JSON.stringify(sample)+')'

//_cb = callback(err, stdout)

exports.runScriptFunction = function(_script,_call,_cb){

	runScriptFunction(_script,_call,_cb);

}

function runScriptFunction(_script,_call,_cb){


	var script = "osascript "+APPLESCRIPT_FOLDER+"/AERunFunction.scpt '"+AESCRIPT_FOLDER+"/"+_script+"' '"+_call+"'";
	//console.log(script)

	AFTEREFFECTS = exec(script,function(err,stdout,stderr){
						if(err) console.error(err)
						_cb(err,stdout)
						})
}
//file = (string) file to open in AE
//callback = cb(err,stdout)
exports.open = function(file,cb){
/* var script = "osascript -e 'tell application \"Adobe After Effects CC\"' -e 'open \"/Users/chris/IncredibleMachines/Dropbox/30PP/AE_Architecture/Template_test_Folder_3/Template_test.aep\"' -e 'end tell'"; */

	var script = "osascript "+APPLESCRIPT_FOLDER+"/AEOpenFile.scpt '"+file+"' "
	currentFile = file;
	AFTEREFFECTS = exec(script,function(err,stdout,stderr){
							bOpen = true;
							if(err) console.error(err,null,stderr);
							cb(err,stdout,stderr);
							})
}
function exit(cb){
	exports.exit(cb)
}
//close after effects
// callback  = cb(err,stdout)
exports.exit = function(cb){

	var script = "osascript "+APPLESCRIPT_FOLDER+"/AERunFunction.scpt '"+AESCRIPT_FOLDER+"/quitWithoutSaving.jsx' 'quitWithoutSaving()'";
	AFTEREFFECTS = exec(script,function(err,stdout,stderr){
						bOpen = false;
						if(err)console.error(err)
						cb(err,stdout)
						})
}

exports.isOpen = function(){
	return bOpen;
}

exports.getCurrentFile = function(){
	return currentFile;
}

//function to delete old file and render new one
//using async queue processes
var concurrency = 5; //number of tasks running in the queue at once

var renderqueue = async.queue(renderWorker,concurrency)

//the worker function for our renderqueue
function renderWorker(scene,callback){
	//console.log("Running New Process")

	var bError = null;

	utils.deleteFile( scene.output,function(err){


		if(err) console.error("Delete File: %s".grey,scene.output)
		else console.log("Delete File: %s".grey,scene.output)

		//options for render process
		var options = ['-project', scene.template, '-output', scene.output, '-comp', 'UV_OUT', '-OMtemplate', OM_TEMPLATE]
		//spawn a process to the aerender
		var aerender = spawn('/Applications/Adobe\ After\ Effects\ CC/aerender',options)

		console.log()
		console.log(' AERENDER PID: '.inverse+' %s '.cyan.inverse, aerender.pid)
		console.log()

		aerender.stdout.on('data', function (data) {
		  var string = data.toString().replace(/\n$/, "")
		  //var newString = string.replace()
		  //var string = data.toString().replace(/^\s+|\s+$/g, "") //remove last newline
		  var nstring = string.replace(/(\r\n|\n|\r)/gm,'\r\n\t\t')//replace all newline chars with newline tabs
		  console.log('  PID: %s '.inverse+' %s '.grey, aerender.pid, nstring)
		})

		aerender.stderr.on('data', function (data) {
		  console.log('stderr: %s'.orange,  data)
		  bError = true
		})

		aerender.on('error',function(error){
			console.error(error)
			bError = true;
		})
		aerender.on('close', function (code) {
		  console.log('AERENDER PID: %s '.inverse+' exited with code %s '.cyan,aerender.pid,code)
		  if(code !=0) bError = true;
		  //callback once the process has finished
		  callback(bError,scene)
		})



	})

}

exports.processRenderOutput = function(formattedScenes,_Database,cb){
	var renderError = null;
	//console.log(JSON.stringify(formattedScenes))

	renderqueue.drain = function(){
		console.log('Rendering Complete')
		//call a concat function
		cb(renderError)
	}

	async.eachSeries(formattedScenes,setRenderContent,function(e){
		if(e){
			console.error(e)
			console.log("Error Launching AfterEffects".red.inverse);
		}else{
			//close after effects
			exit(function(){
				console.log('After Effects Closed');
				//render all files on the command line
				renderqueue.push(formattedScenes,function(err,scene){

					//a callback emitted from each worker on completion
					//connect to db and update render to true or false
					if(err){
						//if error add scene back into queue
						if(!scene.hasOwnProperty('counter')) scene.counter = 0;
						else scene.counter++;
						console.error("AERENDER: CALLBACK ERROR  Count: %s".red,scene.counter)

						if(scene.counter>=3) {
							//callback to different error
							//wait for queue to drain and pass error back to
							console.error("AERENDER: CALLBACK ERROR END".red)

							scene.error = err;
							renderError = scene;

						}else{
							//callback to the anonymous function itself
							console.error("AERENDER: CALLBACK ERROR RETRY".green)
							renderqueue.push(scene, arguments.callee(err,scene))
						}
					}else{//else if(err)
						var clips_cb = 0;
						//database set object to true
						scene.ids.forEach(function(id,index){

							_Database.updateByID('clips',id,{$set:{render:false}},function(e){})

						})

					}//end if(err)
				})//end renderqueue.push
				//aerender -project PROJECT/default_gastronomy.aep -output OUTPUT/default_gastronomy.mov -comp UV_O
			})//end exit
		}
		//else console.log('done');
	})

}

//data to render as an array[]
//render options
function setRenderContent(scene,cb){
	var timebetween = 1000;
	scene.asset_loc = ASSET_FOLDER+'/'
	scene.output = OUTPUT_FOLDER+'/'+scene.type+'.mov'
	scene.template = AEPROJECT_FOLDER+'/'+scene.template
	if(scene.type === 'default_gastronomy' || scene.type==='default_shopping' || scene.type==='default_activities' || scene.type==='default_fashion' || scene.type==='default_parks' || scene.type==='default_artdesign'){
	setTimeout(function(){

		//console.log(scene)
		var functionCall = scene.type+"("+JSON.stringify(scene)+")";
		console.log(" Function Call to AE ".inverse.cyan)
		console.log(functionCall)

		runScriptFunction(scene.script,functionCall,function(err,stdout,stderr){
							if(err){
								console.error(err)
								cb(err);
							}else{
								cb(null)
							}
							})

	},timebetween)
	}else{
		console.log("Files ".inverse.yellow)
		cb(null)
	}
	//cb(null)
}




//put togeher ae script osascript based off of using the AERunDoScript Object.
function runScriptFile(file){

	AERunDoScript.doscript.file = file;

	return AERunDoScript.begin+AERunDoScript.doscript.begin+AERunDoScript.doscript.file+AERunDoScript.doscript.end+AERunDoScript.end;

}

//old test to create loop of movies
//needs to be deprecated
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
//needs to be deprecated
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
				child = exec(makeScript(),function(err,stdout,stderr){
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
