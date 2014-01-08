/**
 *
 *	Utility functions for general app
 *
 */
 
exports.makeSlug = function(title){
	return title.toLowerCase().replace(/ /g,'-');
}
