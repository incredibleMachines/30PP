var utils = require('../modules/Utils');
var upload = require('../modules/Upload');


exports.index = function(_Database){
	return function(req,res){

		var page = req.params.page;
		if (page === undefined) page = 1;
		//console.log("files/page: "+page);


		//TODO: calculate num_pages
		var resultsPerPage = 5; /*** how many results per page ***/
		var numPages;

		_Database.getAll('files', function(e,_files){
			if(!e) numPages = Math.ceil(_files.length/resultsPerPage);
			else res.json({error:e});
		})

		//TODO: pagination quantity - how many per page!
		_Database.queryCollectionWithOptions('files', {}, {skip: (page-1)*resultsPerPage, limit:resultsPerPage, sort: {"created_at":-1}}, function(e,_files){
			if(!e){
				_Database.getAll('clips',function(__e,_clips){
					if(!__e){
						_Database.getAll('scenes',function(___e,_scenes){
							if(!___e) res.render('files/index', { current:req.url,
																										title:'File Library',
																										page_slug:'files-index',
																										files:_files,
																										clips:_clips,
																										scenes:_scenes,
																										num_pages:numPages,
																										page_num:page,
																										auth: req.session.name,
																										error:null });
							else res.render('files/index',{ current: req.url,
																						  title: 'File Library Error',
																							page_slug:'files-index error',
																							file:_files,clips:_clips,
																							scenes:null,
																							num_pages:numPages,
																							page_num:page,
																							auth: req.session.name,
																							error:'Return Scenes Error' });
						})
					} else {
						res.render('files/index', { current:req.url,
																				title:'File Library Error',
																				page_slug:'files-index error',
																				files:null,
																				clips:null,
																				scenes:null,
																				num_pages:numPages,
																				page_num:page,
																				auth: req.session.name,
																				error:'Return Files Error' });
					}
				})

			}else{
				res.render('files/index', { current:req.url,
																		title:'File Library Error',
																		page_slug:'files-index error',
																		files:null,
																		clips:null,
																		num_pages:numPages,
																		page_num:page,
																		auth: req.session.name,
																		error:'Return Files Error' });
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
		//console.log("post: "+post);
		post.slug = utils.makeSlug(post.title);
		//post.event_id = _Database.makeMongoID(post.event_id);

		var files = req.files; //all files in the object
		//console.log(files.content)

		var content = files.content; //our form name for the file

		handleFile(content,post,_Database,req,res);

	} //end return function(req,res)
}

exports.addAJAX = function(_Database){
	return function(req,res){
		var post = req.body;
		post.slug = utils.makeSlug(post.title);
		//console.log("post: ")
		//console.log(post)

		var files = req.files;
		//console.log("files:")
		//console.log(files)

		handleFileAJAX(files.content,post,_Database,req,res)
		//res.jsonp({stuff:'here'})
	}
}

/* //never being used, we never look at files individually
exports.single = function(_Database){

	return function(req,res){

		res.render('events/index', { current: req.url, title: 'Single Asset', page_slug: 'files-single'  });
	}
}
*/



exports.update = function(_Database){

	return function(req,res){

		var post = req.body;
		var files = req.files;
		//console.log(post)
		//console.log(files.file.size)

		if(files.content.size === 0){
			//console.log("No Image to upload Just Update document")

			post.slug = utils.makeSlug(post.title)
			var updateObj = {$set:{title: post.title, slug: post.slug, last_edited: new Date()}}
			_Database.update('files',{_id:_Database.makeMongoID(post.id)},updateObj,function(e){
				if(!e) res.redirect('/files')
				else jsonp(500,{error: e})
			})
		}else{
			//console.log("Upload File then Update");
			post.slug = utils.makeSlug(post.title)

			handleFile(files.content,post,_Database,req,res,true)

		}

	}
}
exports.delete = function(_Database){

	return function(req,res){
		//console.log('delete')
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

function handleFileAJAX(content,post,_Database,req,res){
			//check the content type of the file
		if(content.headers['content-type'].indexOf('image')>=0){
			//console.log('IMAGE')
			post.type=2;

			upload.image(req.files.content,function(img){
				//console.log("img: "+JSON.stringify(img));
				//console.log("post: "+JSON.stringify(post));

				post.path = img.path;
				post.created_at = new Date();
				post.size = img.size;
				post.type = img.type;

				addNewFileAJAX(post,_Database,res);

			})
		}else if(content.headers['content-type'].indexOf('video')>=0){
			//console.log('VIDEO')
			post.type=1;

			upload.video(req.files.content,function(vid){
				//console.log(vid);

				post.path = vid.path;
				post.created_at = new Date();
				post.size = vid.size;
				post.type = vid.type;
				addNewFileAJAX(post,_Database,res);

			})
		}else{
			//unaccepted file type remove the file from the temp folder
			utils.deleteFile(content.path ,function(e){
				if(e) res.jsonp(500,{error: 'Unaccepted File Type: '+content.headers['content-type'], status: 'Unresolved' }) //unresolved means that the tmp file is still lurking in our directory
				else  res.jsonp(500,{error: 'Unaccepted File Type: '+content.headers['content-type'], status: 'Resolved'})
			}) //end deleteFile

		} //if(content.headers['content-type']
}


//content = form file submission titled content
//post = post data from req
//req = our route request
//res = our server response

function handleFile(content,post,_Database,req,res,bUpdate){
			//check the content type of the file
		if(content.headers['content-type'].indexOf('image')>=0){
			//console.log('IMAGE')
			post.type=2;

			upload.image(req.files.content,function(img){
				//console.log("img: "+JSON.stringify(img));
				//console.log("post: "+JSON.stringify(post));

				post.path = img.path;
				post.created_at = new Date();
				post.size = img.size;
				post.type = img.type;

				if(!bUpdate)addNewFile(post,_Database,res);
				else updateFile(post,_Database,res)
			})
		}else if(content.headers['content-type'].indexOf('video')>=0){
			//console.log('VIDEO')
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
function addNewFileAJAX(_post,__Database,_res){

	var slug = _post.current
	//console.log("current slug: "+slug)
	delete _post.current
	var index = _post.zone_index
	delete _post.zone_index

	__Database.add('files', _post, function(e,_doc){
		console.log(' ... Added New File '.inverse+_doc.title.toString().inverse+' ... ')

		if(!e){
			//_res.redirect(slug);
			//_res.redirect(slug+"#file-"+_doc._id); /* how we can pass the file id in through URL */
			_post.zone_index = index
			_res.jsonp(200,{success:_post})

		}else{ //e
			_res.jsonp(500,{error:e});
		}

	});

}

//_post= the json obj
//__Database = _Database

function addNewFile(_post,__Database,_res){

	var slug = _post.current
	//console.log("current slug: "+slug)
	delete _post.current

	__Database.add('files', _post, function(e,_doc){
		console.log(' ... Added New File '.inverse+_doc.title.toString().inverse+' ... ')

		if(!e){
			_res.redirect(slug);
			//_res.redirect(slug+"#file-"+_doc._id); /* how we can pass the file id in through URL */
			//_res.jsonp(200,{success:_post})

		}else{ //e
			_res.jsonp(500,{error:e});
		}

	});

}
