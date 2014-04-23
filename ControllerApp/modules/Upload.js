var colors = require('colors');
var fs = require('fs');
var Folders = require('../modules/FolderStructure');


exports.image = function( _image, _cb){
	console.log('Upload Image Request: '.grey)
	var path = Folders.imagesDir+'/'+_image.originalFilename;
	console.log(path.toString().grey);

	console.log(_image.path)
	fs.rename(_image.path, path, function(err){
		_image.path = path.substring(path.indexOf('.',path.indexOf('.')+1));
		console.log(_image.path)
		_image.type = _image.headers['content-type'];
		_image.name = _image.originalFilename;

		delete _image.ws;
		delete _image.headers;
		delete _image.orginalFilename;

		_cb(_image);
	});


}

exports.video = function( _video, _cb){
	console.log('Upload Video Request: '.grey)
	var path = Folders.videosDir+'/'+_video.originalFilename;
	console.log(path.toString().grey);

	fs.rename(_video.path, path, function(err){
    _video.path = path.substring(path.indexOf('.',path.indexOf('.')+1));
		_video.type = _video.headers['content-type'];
		_video.name = _video.originalFilename;

		delete _video.ws;
		delete _video.headers;
		delete _video.originalFilename;

		_cb(_video);

    });

}
