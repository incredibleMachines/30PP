/**
 *
 *	Utility functions for general app
 *
 */
 
exports.makeSlug = function(title){
	return title.toLowerCase().replace(/ /g,'-');
}
exports.reverseSlug = function(title){
	return title.toLowerCase().replace(/-/g,' ');
}

exports.toTitleCase = function(str){
    return str.replace(/\w\S*/g, function(txt){return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase();});
}

//custom wrapper to delete single files on the fly
exports.deleteFile = function(file,cb){
	require('fs').unlink(file, function(e){
		if(e) cb(e)
		else cb()
	})
}

exports.escapeChars = function(text){
	var searchFor = "'";
	var regex = new RegExp(searchFor, "g");
	return text.replace(regex, "/'");
}