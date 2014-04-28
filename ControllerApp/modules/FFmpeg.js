
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
		function archiveOldOutput (callback){
			// var date = new Date(); //super verbose, for joe's fun:
			// var year = date.getFullYear();
			// var month = date.getMonth();
			// var day = date.getDay();
			// var hours = date.getHours();
			// var minutes = date.getMinutes();
			// var archiveName = OUTPUT_FOLDER+"/concatOutput_archive_"+year+"-"+month+"-"+day+"-"+hours+"h-"+minutes+"m.mov";
			// console.log("new archived name: "+archiveName);
			var renameScript = "mv "+OUTPUT_FOLDER+"/concatOutput.mov "+OUTPUT_FOLDER+"/concatOutput_archived.mov";
			//console.log("name archiveScript: "+renameScript);

			var renameFile = exec(renameScript, function(err,stdout,stderr){
				if(err) console.log(err);
				else callback();
			})
		},
		function executeConcat(callback){

			var concatFromFileScript = "ffmpeg -f concat -i "+OUTPUT_FOLDER+"/concat_list.txt -c copy "+OUTPUT_FOLDER+"/concatOutput.mov";
			//console.log("allfilenames from execConcat: ");
			//console.log(JSON.stringify(allSceneFileNames));

			//**** EXECUTE CONCATENATE *****//
			var CONCATENATE = exec(concatFromFileScript, function(err,stdout,stderr){

			  	if(err) console.error(err);
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
