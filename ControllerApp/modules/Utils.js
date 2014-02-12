/**
 *
 *	Utility functions for general app
 *
 */
 
exports.makeSlug = function(title){
	return title.toLowerCase().replace(/ /g,'-');
}

//custom wrapper to delete single files on the fly
exports.deleteFile = function(file,cb){
	require('fs').unlink(file, function(e){
		if(e) cb(err)
		else cb()
	})
}