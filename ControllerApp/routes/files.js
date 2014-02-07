var utils = require('../modules/Utils');
var upload = require('../modules/Upload');

exports.index = function(_Database){
	return function(req,res){
		res.render('files/index', { current: req.url, title: 'File Library', page_slug:'files-index' });
	}
}

exports.add = function(_Database){

	return function(req,res){
		
		/*********************************************************************************** / 
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
		
		// TO DO: handle a new location 
		delete post.new_location;
		
		
		//check the content type of the file
		if(content.headers['content-type'].indexOf('image')>=0){
			console.log('IMAGE')
			post.type=2;
			
			upload.image(req.files.content,function(img){
				//console.log(img)
				
				post.path = img.path;
				post.created_at = new Date();
				addNewFile(post,_Database,res);
				
			})
		}else if(content.headers['content-type'].indexOf('video')>=0){
			console.log('VIDEO')
			post.type=1;
			
			upload.video(req.files.content,function(vid){
				//console.log(vid);
				
				post.path = vid.path;
				post.created_at = new Date();

				addNewFile(post,_Database,res);

			})
		}else{
			//unaccepted file type remove the file from the temp folder
			utils.deleteFile(content.path ,function(e){
				if(e) res.jsonp(500,{error: 'Unaccepted File Type: '+content.headers['content-type'], status: 'Unresolved' }) //unresolved means that the tmp file is still lurking in our directory
				else  res.jsonp(500,{error: 'Unaccepted File Type: '+content.headers['content-type'], status: 'Resolved'})
			})
			
		}
	}
}

exports.single = function(_Database){

	return function(req,res){
	
		res.render('events/index', { current: req.url, title: 'Single Asset', page_slug: 'files-single'  });
	}
}
exports.update = function(_Database){

	return function(req,res){
		res.jsonp({message:'update'});
		//res.render('events/index', { current: req.url, title: 'add event' });
	}
}
exports.delete = function(_Database){

	return function(req,res){
		res.jsonp({message:'delete'});
		//res.render('events/index', { current: req.url, title: 'add event' });
	}
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