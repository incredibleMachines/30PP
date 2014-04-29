
/**** FFMPEG COCATENATION ****/
/*
*	Gist of ffmpeg concatenation strategies can be found here:
*  		https://gist.github.com/jmsaavedra/11160372
*
*	FFmpeg wiki page on concatenation can be found here:
*			https://trac.ffmpeg.org/wiki/How%20to%20concatenate%20(join,%20merge)%20media%20files
*
*/

var util 	 = require('util');
var exec		= require('child_process').exec
var spawn   = require('child_process').spawn
var fs			= require('fs');
var async 	= require('async');
var folders = require('../modules/FolderStructure');
var utils 	= require('../modules/Utils');
var _ 			= require('underscore');
var fs 		 = require('fs');


// execute the concatenation of all scene videos
exports.concat = function(_Database, cb){

	var OUTPUT_FOLDER = folders.outputDir();
	var ASSET_FOLDER	= folders.videosDir();

	//console.log("OUTPUT_FOLDER:  "+ OUTPUT_FOLDER);
	//console.log("ASSET_FOLDER:  "+ ASSET_FOLDER);

	var allSceneFileNames = new Array();

	async.waterfall([
		function populateSceneFiles (callback){
			//console.log("hit populateSceneFiles");
			_Database.getAll('timeline',function(e, tEvents){
				async.each(tEvents, function(evt, cb){
					if(evt.scenes.length>0){
						async.each(evt.scenes, function(sce, _cb){
							var thisFilename = sce.slug+".mov";
							allSceneFileNames.push(thisFilename);
							_cb(null);
						});
					}
					cb(null);
				});
				//quick, before we move on, clear out the concat_list.txt file if it exists and has stuff in it:
				var clearFile = exec("> "+OUTPUT_FOLDER+"/concat_list.txt", function(err,stdout,stderr){
					if(err) console.error(err);
					else callback();
				})
			});
		},


		function writeList (callback){
			var writeline="";
			async.each(allSceneFileNames, function(filename, _cb){
				writeline += "file '"+filename+"'\n";
				_cb(null);
			});
			var write = exec("echo \""+writeline+"\" >> "+OUTPUT_FOLDER+"/concat_list.txt", function(err,stdout,stderr){
				if(err) console.error(err);
				else callback();
			})
		},


		function executeConcat(callback){ //first execute concat

			var concatFromFileScript = "ffmpeg -f concat -i "+OUTPUT_FOLDER+"/concat_list.txt -c copy "+OUTPUT_FOLDER+"/concatOutput_NEW.mov";
			//console.log("allfilenames from execConcat: "+ JSON.stringify(allSceneFileNames));

			//**** EXECUTE CONCATENATE *****//
			var CONCATENATE = exec(concatFromFileScript, function(err,stdout,stderr){

					if(err) console.error(err);
					else callback();
			})
		},


		function renameOldOutputFile (callback){ //if successful concat, rename old concatOutput.mov

			// var date = new Date(); //super verbose, for joe's fun:
			// var archivedName = OUTPUT_FOLDER + "/concatOutput_archived_"
			//									 + date.getFullYear() + "-" + date.getMonth() + "-"
			//									 + date.getDay() 		 + "-" + date.getHours() + "h-"
			//									 + date.getMinutes()  + "m.mov";
			// console.log("archived name: "+archivedName);

			//**** OLD ****//
			// var renameScript = "mv "+OUTPUT_FOLDER+"/concatOutput.mov "+OUTPUT_FOLDER+"/concatOutput_archived.mov";
			// var renameFile = exec(renameScript, function(err,stdout,stderr){
			// 	if(err) console.log(err);
			// 	else callback();
			// })

			var oldPath = OUTPUT_FOLDER+"/concatOutput.mov";
			var newPath = OUTPUT_FOLDER+"/concatOutput_archived.mov";
			//console.log("HIT renameOldOutputFile");

			fs.rename(oldPath, newPath, function(err){
				if(err) console.log(err);
				else callback();
			})
		},


		function renameNewOutputFile (callback){ //if successful concat, rename NEW concatOutput.mov

			//**** OLD ****//
			// var renameScript = "mv "+OUTPUT_FOLDER+"/concatOutput.mov "+OUTPUT_FOLDER+"/concatOutput_archived.mov";
			// var renameFile = exec(renameScript, function(err,stdout,stderr){
			// 	if(err) console.log(err);
			// 	else callback();
			// })

			var oldPath = OUTPUT_FOLDER+"/concatOutput_NEW.mov";
			var newPath = OUTPUT_FOLDER+"/concatOutput.mov";
			//console.log("HIT renameNewOutputFile");

			fs.rename(oldPath, newPath, function(err){
				if(err) console.log(err);
				else callback();
			})
		}
	], function(err, result){

		//console.log("reached end of waterfall!");
		cb(null); //DONE DONE DONE
	});

}


// Create a URI from the type and slug
// of any scene --
exports.getUri = function(type, scene_slug){

	var uri = ASSET_FOLDER + type + "_" + scene_slug + ".mov";
	return uri;
}
