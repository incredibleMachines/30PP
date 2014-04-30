
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
			//console.log("concat: populating scene files");
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

		//** save file list to concat_list.txt
		function writeList (callback){
			console.log("concat: saving concat_list.txt");
			var writeline="";
			async.eachSeries(allSceneFileNames, function(filename, _cb){
				writeline += "file '"+filename+"'\n";
				_cb(null);
			});
			var write = exec("echo \""+writeline+"\" >> "+OUTPUT_FOLDER+"/concat_list.txt", function(err,stdout,stderr){
				if(err) console.error(err);
				else callback();
			})
		},


		//** check to be sure that there isn't an aborted concatOutput_NEW.mov file in the folder
		function checkForAbortedConcatFile(callback){ //first execute concat
			console.log("concat: checking for aborted file");
			var path = OUTPUT_FOLDER+"/concatOutput_NEW.mov";
			fs.exists(path, function (exists) {
				if(exists){
					//console.log("concat: found aborted file, deleting now");
					fs.unlink(path, function(err){
						if(err) console.log(err);
						else callback();
					})
				} else {
					//console.log("concat: no aborted file found")
					callback();
				}
			})
		},


		//** attempt to execute the actual ffmpeg concat
		function executeConcat(callback){ //first execute concat
			console.log("concat: execute ffmpeg concat");
			var concatFromFileScript = "ffmpeg -f concat -i "+OUTPUT_FOLDER+"/concat_list.txt -c copy "+OUTPUT_FOLDER+"/concatOutput_NEW.mov";
			//console.log("allfilenames from execConcat: "+ JSON.stringify(allSceneFileNames));
			var concatenate = spawn('ffmpeg',['-f','concat','-i',OUTPUT_FOLDER+'/concat_list.txt', '-c', 'copy', OUTPUT_FOLDER+'/concatOutput_NEW.mov'])
			concatenate.stdout.on('data',function(data){
				//console.log(data)
			})
			concatenate.stderr.on('data',function(data){
				//console.error(data)
			})
			concatenate.on('error',function(error){
				console.error(error)
			})
			concatenate.on('close',function(code){
				console.log('Concatenate Spawn Complete')
				callback()
			})
			//**** EXECUTE CONCATENATE *****//
			// var CONCATENATE = exec(concatFromFileScript, function(err,stdout,stderr){
			//
			// 		if(err) console.error(err);
			// 		else callback();
			// })
		},

		//** if concat is successful, rename old concatOutput.mov
		function renameOldOutputFile (callback){
			//**** OLD ****//
			// var date = new Date(); //super verbose, for joe's fun:
			// var archivedName = OUTPUT_FOLDER + "/concatOutput_archived_"
			//									 + date.getFullYear() +"-"+ date.getMonth() +"-"
			//									 + date.getDay() 		 +"-"+ date.getHours() +"h-"
			//									 + date.getMinutes()  +"m.mov";
			// console.log("archived name: "+archivedName);
			// var renameScript = "mv "+OUTPUT_FOLDER+"/concatOutput.mov "+OUTPUT_FOLDER+"/concatOutput_archived.mov";
			// var renameFile = exec(renameScript, function(err,stdout,stderr){
			// 	if(err) console.log(err);
			// 	else callback(); })

			var oldPath = OUTPUT_FOLDER+"/concatOutput.mov";
			var newPath = OUTPUT_FOLDER+"/concatOutput_archived.mov";

			console.log("concat: renaming files");
			fs.exists(oldPath, function (exists) {
			  if(exists){
					//console.log("concat: renaming old output file");
					fs.rename(oldPath, newPath, function(err){
						if(err) console.log(err);
						else callback();
					})
				} else {
					//console.log("concat: no old output file to archive")
					callback();
				}
			})
		},

		//** now, rename the NEW concatOutput.mov
		function renameNewOutputFile (callback){

			//**** OLD ****//
			// var renameScript = "mv "+OUTPUT_FOLDER+"/concatOutput.mov "+OUTPUT_FOLDER+"/concatOutput_archived.mov";
			// var renameFile = exec(renameScript, function(err,stdout,stderr){
			// 	if(err) console.log(err);
			// 	else callback();
			// })

			var oldPath = OUTPUT_FOLDER+"/concatOutput_NEW.mov";
			var newPath = OUTPUT_FOLDER+"/concatOutput.mov";

			//console.log("concat: renaming new output file");
			fs.rename(oldPath, newPath, function(err){
				if(err) console.log(err);
				else callback();
			})
		}
	], function(err, result){

		console.log("concat: CONCAT COMPLETED.");
		cb(null); //DONE DONE DONE
	});
}


// Create a URI from the type and slug
// of any scene --
exports.getUri = function(type, scene_slug){

	var uri = ASSET_FOLDER + type + "_" + scene_slug + ".mov";
	return uri;
}
