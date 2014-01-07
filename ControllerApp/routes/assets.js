var utils = require('../modules/Utils');
var upload = require('../modules/Upload');


exports.add = function(_Database){

	return function(req,res){
		
		//check post data
		var post = req.body;
		console.log(req.body);
		
		/**
		 *
		 *	Sample post: 
		 * 	{ title: 'Title',
		 *	  asset_type: 'video|image|text',
		 *	  body_text: '',
		 *	  duration: '35',
		 *	  duration_view: '35 seconds' 
		 *	 }
		 *
		 */
		 
		delete post.duration_view;
		post.slug = utils.makeSlug(post.title);
		
		var files = req.files;
		//console.log(req.files.content);
		//TO DO: Functionatize whats happening in video and image
		switch(post.asset_type){
			case 'video':
				post.body_text = null;
				//TO DO: Verify Content Type
				upload.video(req.files.content,function(vid){
					//console.log(vid);
					
					post.path = vid.path;
					post.type = post.asset_type;
					delete post.asset_type;
					post.created_at = new Date();

					addNewAsset(post,_Database,res);

				})
			break; // break video
			case 'image':
				post.body_text = null;
				//TO DO: verify content type
				upload.image(req.files.content,function(img){
					//console.log(img)
					
					post.path = img.path;
					post.type = post.asset_type;
					delete post.asset_type;
					post.created_at = new Date();

					addNewAsset(post,_Database,res);
				})
			break;//break image
			case 'text':
				
				post.path = null;
				post.type = post.asset_type;
				delete post.asset_type;
				post.created_at = new Date();
				
				addNewAsset(post,_Database,res);

			break;//break text
		}
		
		//check file data

		 
		//check that if its image or video type 
		//verify that we have files
		
		//res.render('events/index', { title: 'Assets Add' });
	}
}

exports.single = function(_Database){

	return function(req,res){
	
		res.render('events/index', { title: 'Assets' });
	}
}
exports.update = function(_Database){

	return function(req,res){
	
		res.render('events/index', { title: 'add event' });
	}
}
exports.delete = function(_Database){

	return function(req,res){
		
		res.render('events/index', { title: 'add event' });
	}
}

//_post= the json obj
//__Database = _Database

function addNewAsset(_post,__Database,_res){
	
	__Database.add('assets', _post, function(e,_doc){
		console.log('Added New Asset')
		//update event with new asset -
		if(!e){ 
			var updateObj = {$push:{assets:_doc._id.toString()}};
			__Database.updateById('events', _post.event_id,updateObj,function(_e){
				if(!_e){
					//we're all good
					_res.jsonp(200,{success:_doc})
				}else{ //_e
					//we successfully added the asset,but didn't associate it with our event
					_res.jsonp(500,_e);
				}
			})
		}else{ //e 
			_res.jsonp(500,e);
		}
		
	});
	
}