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
		
		switch(post.asset_type){
			case 'video':
				post.body_text = null;
				//TO DO: verify content type
				upload.video(req.files.content,function(vid){
					console.log(vid);
					
					//add to post and update in database
					res.send(200,'Video')

				})
				break;
			case 'image':
				post.body_text = null;
				//TO DO: verify content type
				upload.image(req.files.content,function(img){
					console.log(img)
					
					//add to post and update in database
					res.send(200,'Image')
				})
				break;
			case 'text':
				
				break;
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