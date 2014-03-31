/* to display all files to be concatenated, run ffmpeg? */

var utils = require('../modules/Utils');

//use underscore here to help us facilitate actions
var _ = require('underscore');

exports.index = function(_Database){
 	//get all events and list them in the db
 	//retrieve all events

	return function(req,res){
	 	//retrieve all events
	 	
	 	//get request
	 	var type = req.query.type;
	 	
	 	if(typeof type === 'undefined' || type === "") type = 'default';
	 	console.log(utils.toTitleCase(type))
	 	var title = utils.reverseSlug(type)
	 	//query scenes by event type 
	 	
	 	_Database.queryCollectionWithOptions('files', {type: type}, {sort: 'order'}, function(e,_files){
			if(!e){ 
				_Database.getAll('clips',function(__e,_clips){
					if(!__e){
						_Database.getAll('scenes',function(___e,_scenes){
							if(!___e){
								var _queue = _.where(_clips,{render: true})
								console.log("_queue: "+_queue);
								res.render('renderqueue/concat', { current:req.url, title:'Concat', page_slug:'renderqueue-concat', 
								files:_files, clips:_clips, queue:_queue, scenes:_scenes, error:null });
							}else{
								res.render('renderqueue/concat',{current: req.url, title: 'Concat', page_slug:'renderqueue-concat', 
								file:_files,clips:_clips, scenes:null, queue:null, error:'Return Scenes Error' });	
							} 			
						})
					} else {
						res.render('renderqueue/concat', { current:req.url, title:'Concat Error', page_slug:'clips-index error',
							files:null, clips:null, scenes:null, error:'Return Clips Error' });
					}
				})
					
			}else{ 
				res.render('renderqueue/concat', { current:req.url, title:'Concat Error', page_slug:'files-index error',
					files:null, clips:null, error:'Return Files Error' });
			}
		})
	 		
/*
	 	_Database.queryCollectionWithOptions('scenes', {type: type} , {sort: 'order'} ,function(err, _scenes){
	 		//error handling needs to happen
	 		
		 	res.render('renderqueue/concat', { current: req.url, title: utils.toTitleCase(title), page_slug: 'events-index', scenes: _scenes } );

	 		
	 	});
*/
	}
}