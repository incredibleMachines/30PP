
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
var mailer  = require('../modules/MailClient')

// concatenation globals
var __Database;
var OUTPUT_FOLDER;
var ASSET_FOLDER;
var allSceneFileNames = new Array();
var concatOutputDir;
var outputArchivedDir;
var outputNewDir;
var concatListTxtDir;

// execute the concatenation of all scene videos
exports.concat = function(_Database, cb){

	__Database 	 			= _Database;
	OUTPUT_FOLDER 			= folders.outputDir();
	ASSET_FOLDER  			= folders.videosDir();
	concatOutputDir 		= OUTPUT_FOLDER+"/concatOutput.mov";
	outputArchivedDir 	= OUTPUT_FOLDER+"/concatOutput_archived.mov";
	outputNewDir				= OUTPUT_FOLDER+"/concatOutput_NEW.mov";
	concatListTxtDir		= OUTPUT_FOLDER+"/concat_list.txt";

	async.waterfall([

		populateSceneFiles,

		writeList,

		checkForAbortedConcatFile,

		checkForAllSourceFiles,

		executeConcat,

	  renameOldOutputFile,

		renameNewOutputFile

	], function(err, result){
		if(err){
			console.log("concat: CONCAT FAILED, err: "+err);
			var subject = "[30PP] ControllerApp Concatenation has Failed";
			var message = "This is an automated message to inform you that the ControllerApp failed while trying to concatenate final renders. Error: "+err;
			mailer.send(subject,message,function(e,resp){
				if(e){
					console.error(e)
					cb(err); //should also add 'e' to this callback
				} else cb(err)
			})
		}else{
			console.log("concat: CONCAT COMPLETED.");
			var subject = "[30PP] ControllerApp Concatenation has Completed"
			var message = "This is an automated message to inform you that the ControllerApp successfully completed a concatentation of the most recent renders."
			mailer.send(subject,message,function(e,resp){
				if(e){
					console.error(e)
					cb(e);
				} else cb(null);//DONE DONE DONE
			})
		}
	});
}


//** get all scene files from db
function populateSceneFiles (callback){
	//console.log("concat: populating scene files");
	__Database.getAll('timeline',function(e, tEvents){
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
		var clearFile = exec("> "+concatListTxtDir, function(err,stdout,stderr){
			if(err) callback(err);
			else callback(null);
		})
	});
}


//** save file list to concat_list.txt
function writeList (callback){
	console.log("concat: saving concat_list.txt");
	var writeline="";
	async.eachSeries(allSceneFileNames, function(filename, _cb){
		writeline += "file '"+filename+"'\n";
		_cb(null);
	});
	var write = exec("echo \""+writeline+"\" >> "+concatListTxtDir, function(err,stdout,stderr){
		if(err) callback(err);
		else callback(null);
	});
}


//** check to be sure that there isn't an aborted concatOutput_NEW.mov file in the folder
function checkForAbortedConcatFile(callback){ //first execute concat
	fs.exists(outputNewDir, function (exists) {
		if(exists){
			console.log("concat: found aborted file, deleting now");
			fs.unlink(outputNewDir, function(err){
				if(err)callback(err);
				else callback(null);
			})
		} else {
			callback(null); // no aborted file found, that's fine let's keep going
		}
	})
}


//** check that all files exist
function checkForAllSourceFiles(callback){
	var missingFile = false;
	var missingFiles = new Array();
	allSceneFileNames.forEach(function(file,i){
		var fileDir = OUTPUT_FOLDER+"/"+file;
		//console.log("checking for file #"+i+" dir: "+fileDir);
		fs.exists(fileDir, function (exists) {
			if(!exists){
				missingFile = true;
				missingFiles.push(file);
			}
			if (i === allSceneFileNames.length-1){
				if(missingFile === true){
					console.log("concat error: missing files: "+JSON.stringify(missingFiles));
					callback("missing source files: "+JSON.stringify(missingFiles));
				} else {
					callback(null);
				}
			}
		})
	})
}


//** attempt to execute the actual ffmpeg concat
function executeConcat(callback){ //first execute concat
	console.log("concat: execute ffmpeg concat");
	//var concatFromFileScript = "ffmpeg -f concat -i "+OUTPUT_FOLDER+"/concat_list.txt -c copy "+OUTPUT_FOLDER+"/concatOutput_NEW.mov";
	//console.log("allfilenames from execConcat: "+ JSON.stringify(allSceneFileNames));

	var concatenate = spawn('ffmpeg',['-f','concat','-i',concatListTxtDir,'-c','copy',outputNewDir])
	concatenate.stdout.on('data',function(data){
		//console.log(data)
	})
	concatenate.stderr.on('data',function(data){
		//console.error(data)
	})
	concatenate.on('error',function(error){
		console.error(error)
		callback(error)
	})
	concatenate.on('close',function(code){
		console.log('concat: Spawn Complete')
		callback(null)
	})
	//**** EXECUTE CONCATENATE *****//
	// var CONCATENATE = exec(concatFromFileScript, function(err,stdout,stderr){
	//
	// 		if(err) console.error(err);
	// 		else callback();
	// })
}


function renameOldOutputFile(callback){
	var fileName = concatOutputDir; //concatOutput.mov
	var newName	= outputArchivedDir; //concatOutput_archive.mov

	fs.exists(fileName, function (exists) {
		if(exists){
			console.log("concat: found existing output, archiving it now");
			var renameScript = "mv -f "+fileName+" "+newName;
			var renameFile = exec(renameScript, function(err,stdout,stderr){
				if(err) callback(err)
				else callback(null);
			})

			// fs.rename(fileName, newName, function(err){
			// 	if(err){
			// 		console.log(err);
			// 		callback(err);
			// 	}
			// 	else callback(null);
			// })
		} else {
			console.log("concat: rename did not find "+fileName);
			callback(null); //cool, keep going. no old concatOutput to archive.
		}
	})
}

function renameNewOutputFile(callback){
	var fileName = outputNewDir;	//concatOutput_NEW.mov
	var newName	= concatOutputDir; //concatOutput.mov

	fs.exists(fileName, function (exists) {
		if(exists){
			console.log("concat: renaming NEW to concatOutput.mov");
			var renameScript = "mv -f "+fileName+" "+newName;
			var renameFile = exec(renameScript, function(err,stdout,stderr){
				if(err) callback(err)
				else callback(null);
			});
			//console.log("concat: renaming old output file");
			// fs.rename(fileName, newName, function(err){
			// 	if(err){
			// 		console.log(err);
			// 		callback(err);
			// 	}
			// 	else callback(null);
			// })
		} else {
			console.log("concat: rename did not find "+fileName);
			callback("error: rename did not find "+fileName);
		}
	})
}
