var utils = require('../modules/Utils');
var upload = require('../modules/Upload');

exports.index = function(_Database){
	return function(req,res){
		_Database.getAll('files',function(e,_files){
			if(!e){ 
				_Database.getAll('clips',function(__e,_clips){
					if(!__e) res.render('files/index', { current: req.url, title: 'File Library', page_slug:'files-index', files: _files, clips:_clips,error:null });
					else res.render('files/index',{current: req.url, title: 'File Library Error', page_slug: 'files-index error', file:_files,clips:null,error:null })
				})
					
			}else{ 
				res.render('files/index', { current: req.url, title: 'File Library Error', page_slug:'files-index error',files: null,clips:null,error:'Return Files Error' })
			}
		})
	}
}

exports.add = function(_Database){

	return function(req,res){
		
		/**************************************************************************************** / 
		 *
		 *	Sample post: 
		 *
		 * 	{ event_id: '52eafdf946478ab6051163bf',
		 *	  title: 'My Scan 1',
		 *	  location: 'Location',
		 *	  new_location: '' 
		 *	}
		 *
		 *
		 *  Sample file.content:
		 *
		 *	{ originalFilename: 'Scan 1.jpeg',
		 *	  path: '.tmp/3412-1mehh17.jpeg',
		 *	  headers: 
		 *	   { 'content-disposition': 'form-data; name="content"; filename="Scan 1.jpeg"',
		 *	     'content-type': 'image/jpeg' },
		 *	  ws: 
		 *	   { ... },
		 *	  size: 86112,
		 *	  name: 'Scan 1.jpeg' }
         *
		 ***********************************************************************************/
		
		var post = req.body;
		//handle the post
		post.slug = utils.makeSlug(post.title);
		//post.event_id = _Database.makeMongoID(post.event_id);
		
		var files = req.files; //all files in the object
		//console.log(files.content)
		
		var content = files.content; //our form name for the file
		
		handleFile(content,post,_Database,req,res);
		
		// TO DO: handle a new location
/*
		if(post.new_location != ''){
			//check if the location exists already
			post.new_location = post.new_location.toLowerCase();
			//this only checks for the literal match of the word.
			_Database.queryCollection('locations',{address:post.new_location},function(_e,_doc){
				if(!_e){
					//console.log(_doc)
					//check the length of the returned object
					if(_doc.length>0){
						console.log('Location Match');

						delete post.new_location;
						post.location = _doc[0]._id;
						handleFile(content,post,_Database,req,res);
					}else{
						console.log('No Location Match');
						_Database.add('locations',{address:post.new_location},function(__e,__loc){
							delete post.new_location;
							post.location = __loc._id;
							handleFile(content,post,_Database,req,res);
							
						})
					}
				}else{
					res.jsonp(500,{error: 'Location Query Error'})
				}
			})
			
		}else{
			//no new location
			delete post.new_location;
			
			//make our location be a mongoID Object
			post.location = _Database.makeMongoID(post.location);
			handleFile(content,post,_Database,req,res);
		}
*/
		
		//delete post.new_location;
		
	} //end return function(req,res)
}

exports.single = function(_Database){

	return function(req,res){
	
		res.render('events/index', { current: req.url, title: 'Single Asset', page_slug: 'files-single'  });
	}
}
exports.update = function(_Database){

	return function(req,res){
	
		var post = req.body;
		var files = req.files;
		//console.log(post)
		//console.log(files.file.size)
		
		if(files.content.size === 0){
			console.log("No Image to upload Just Update document")
			
			post.slug = utils.makeSlug(post.title)
			var updateObj = {$set:{title: post.title, slug: post.slug, last_edited: new Date()}}
			_Database.update('files',{_id:_Database.makeMongoID(post.id)},updateObj,function(e){
				if(!e) res.redirect('/files')
				else jsonp(500,{error: e})
			})
		}else{
			console.log("Upload File then Update");
			post.slug = utils.makeSlug(post.title)
			
			handleFile(files.content,post,_Database,req,res,true)
			
		}
			
	}
}
exports.delete = function(_Database){

	return function(req,res){
		//res.jsonp({message:'delete'});
		//res.render('events/index', { current: req.url, title: 'add event' });
		var file = req.params.slug;
		_Database.remove('files',{slug:file},function(e){
			
			if(!e) res.redirect('/files')
			else res.json({error: 'Delete file error'})
		})
	
	}
	
	
}
//content = form file submission titled content
//post = post data from req
//req = our route request
//res = our server response

function handleFile(content,post,_Database,req,res,bUpdate){
			//check the content type of the file
		if(content.headers['content-type'].indexOf('image')>=0){
			console.log('IMAGE')
			post.type=2;
			
			upload.image(req.files.content,function(img){
				console.log(img)
				
				post.path = img.path;
				post.created_at = new Date();
				post.size = img.size;
				post.type = img.type;
				if(!bUpdate)addNewFile(post,_Database,res);
				else updateFile(post,_Database,res)
			})
		}else if(content.headers['content-type'].indexOf('video')>=0){
			console.log('VIDEO')
			post.type=1;
			
			upload.video(req.files.content,function(vid){
				//console.log(vid);
				
				post.path = vid.path;
				post.created_at = new Date();
				post.size = vid.size;
				post.type = vid.type;
				if(!bUpdate) addNewFile(post,_Database,res);
				else updateFile(post,_Database,res)
			})
		}else{
			//unaccepted file type remove the file from the temp folder
			utils.deleteFile(content.path ,function(e){
				if(e) res.jsonp(500,{error: 'Unaccepted File Type: '+content.headers['content-type'], status: 'Unresolved' }) //unresolved means that the tmp file is still lurking in our directory
				else  res.jsonp(500,{error: 'Unaccepted File Type: '+content.headers['content-type'], status: 'Resolved'})
			}) //end deleteFile
			
		} //if(content.headers['content-type']
}
function updateFile(_post,__Database,_res){
	
	var updateObj = {$set:{
							title: _post.title,
							slug: _post.slug,
							last_edited: new Date(),
							path:_post.path,
							size: _post.size,
							type: _post.type
							
							}}
	__Database.update('files', {_id: __Database.makeMongoID(_post.id)},updateObj,function(e){
		
		if(!e) _res.redirect('/files')
		else _res.jsonp(500, {error: e})
		
	})
	
	
}

//_post= the json obj
//__Database = _Database

function addNewFile(_post,__Database,_res){
	
	__Database.add('files', _post, function(e,_doc){
		console.log(' ... Added New File '.inverse+_doc.title.toString().inverse+' ... ')
		
		if(!e){ 
			_res.jsonp(200,{success:_doc})
			
			/* Outdated Files No Longer contain links back to objects
			//update event with new asset -	
			var updateObj = {$push:{assets:_doc._id.toString()}};
			__Database.updateById('events', _post.event_id,updateObj,function(_e){
				if(!_e){
					//we're all good
					_res.jsonp(200,{success:_doc})
				}else{ //_e
					//we successfully added the asset,but didn't associate it with our event
					_res.jsonp(500,_e);
				}
			})*/
		}else{ //e 
			_res.jsonp(500,{error:e});
		}
		
	});
	
}