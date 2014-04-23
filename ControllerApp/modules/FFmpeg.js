
/**** FFMPEG COCATENATION ****/
/*
*	Gist of ffmpeg concatenation strategies can be found here:
*  		https://gist.github.com/jmsaavedra/11160372
*
*	FFmpeg wiki page on concatenation can be found here:
*			https://trac.ffmpeg.org/wiki/How%20to%20concatenate%20(join,%20merge)%20media%20files
*
*/

var util 	 = require('util')
var exec		= require('child_process').exec
var spawn   = require('child_process').spawn
var fs			= require('fs')
var async 	= require('async')
var folders = require('../modules/FolderStructure')
var utils 	= require('../modules/Utils')
var _ 			= require('underscore')


//Filepath Globals
//Takes into account the ControllerApp file structure
var OUTPUT_FOLDER = __dirname+"/../includes/videos"; //need to implement
var ASSET_FOLDER = __dirname+"/../public/.assets/videos/";

var CONCATENATE;


// execute the concatenation of all scene videos
exports.concat = function(_Database, cb){


	var allSceneFileNames = new Array();

	async.waterfall([
		function populateSceneFiles (callback){
			console.log("hit populateSceneFiles");
			_Database.getAll('timeline',function(e, tEvents){
				async.each(tEvents, function(evt, cb){
					if(evt.scenes.length>0){
						async.each(evt.scenes, function(sce, _cb){
							var thisFilename = sce.concat_slug+".mov";
							allSceneFileNames.push(thisFilename);
							_cb(null);
						});
					}
					cb(null);
				});
				callback();
			});
		},
		function writeList (callback){
			var writeline="";
			async.each(allSceneFileNames, function(filename, _cb){
				writeline += "file '"+filename+"'\n";
				_cb(null);
			});
			var write = exec("echo \""+writeline+"\" >> ~/Desktop/testList.txt", function(err,stdout,stderr){
				if(err) console.error(err);
				else callback();
			})
		},
		function executeConcat(callback){

			var concatFromFileScript = "ffmpeg -f concat -i "+ASSET_FOLDER+"list.txt -c copy "+ASSET_FOLDER+"outputFromFile.mov";

			console.log("allfilenames from execConcat: ");
			console.log(JSON.stringify(allSceneFileNames));

			//**** UNCOMMENT ME *****//
			// CONCATENATE = exec(concatFromFileScript, function(err,stdout,stderr){
			//  	if(err) console.error(err);
			//  	cb(err);
			// })
			//************************//

			/*************************************************/
			//****** single line exec concat strategy ********/
			// TODO: save output.mov to OUTPUT_FOLDER
			// var concatOneLineScript = "ffmpeg -f concat -i <\(for f in " + ASSET_FOLDER + "*.mov; do echo \"file '$f'\"; done\) -c copy outputFromOneLine.mov";
			// console.log("concat one-line Script: ");
			// console.log(concatOneLineScript);
			/*************************************************/
			callback();
		}
	], function(err, result){

		console.log("reached end of waterfall!");
		cb(null); //DONE DONE DONE
	});

}


// Create a URI from the type and slug
// of any scene --
exports.getUri = function(type, scene_slug){

	var uri = ASSET_FOLDER + type + "_" + scene_slug + ".mov";
	return uri;
}
