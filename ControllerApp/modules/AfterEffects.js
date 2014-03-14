var util 	= require('util'),
	exec	= require('child_process').exec, 
	fs		= require('fs'), 
	async 	= require('async');
	
//NEED TO HAVE AE CURRENTLY RENDERING VARIABLE & Handler	
var AFTEREFFECTS, 
	bRendering = false,
	bOpen = false,
	currentFile;

/*
	
	Filepath Globals
	Takes into account the ControllerApp file structure
*/
var APPLESCRIPT_FOLDER = __dirname+"/../includes/applescripts";
var AESCRIPT_FOLDER = __dirname+"/../includes/aescripts";

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
							if(err) console.error(err);
							cb(err,stdout);
							})
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

exports.getCurrentFile = function(){
	return currentFile;
}

exports.isOpen = function(){
	return bOpen;
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