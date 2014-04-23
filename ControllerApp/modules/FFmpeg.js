
/**** FFMPEG COCATENATION ****/
/*
*	Gist of ffmpeg concatenation strategies can be found here:
*  		https://gist.github.com/jmsaavedra/11160372
*
*	FFmpeg wiki page on concatenation can be found here:
*			https://trac.ffmpeg.org/wiki/How%20to%20concatenate%20(join,%20merge)%20media%20files
*
*/

var util 	= require('util'),
	exec	= require('child_process').exec,
	spawn   = require('child_process').spawn,
	fs		= require('fs'),
	async 	= require('async'),
	folders = require('../modules/FolderStructure'),
	utils 	= require('../modules/Utils');


//Filepath Globals
//Takes into account the ControllerApp file structure
var OUTPUT_FOLDER = __dirname+"/../includes/videos"; //need to implement
var ASSET_FOLDER = __dirname+"/../public/.assets/videos/";

var CONCATENATE;


// execute the concatenation of all scene videos
exports.concat = function(cb){

	//needs to implement OUTPUT_FOLDER... something like:
	// var concatScript = "ffmpeg -f concat -i <(for f in " + ASSET_FOLDER + "/*.mov; do echo \"file '$f'\"; done) -c copy "+OUTPUT_FOLDER+"/output.mov";
	var concatScript = "ffmpeg -f concat -i <(for f in " + ASSET_FOLDER + "*.mov; do echo \"file '$f'\"; done) -c copy output.mov";
	console.log("concatScript: ");
	console.log(concatScript);

	CONCATENATE = exec(concatScript, function(err,stdout,stderr){
	 	if(err) console.error(err);
	 	cb(err);
	})
}


// Create a URI from the type and slug
// of any scene --
exports.getUri = function(type, scene_slug){

	var uri = ASSET_FOLDER + type + "_" + scene_slug + ".mov";
	return uri;
}
