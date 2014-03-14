var utils = require('../modules/Utils');

exports.add = function(_Database){
	return function(req,res){
		//res.jsonp({working:"onit"})
		//add a new scene to the database
		
		var post = req.body;
		//console.log(post)
		
		/* Sample Post:
			{ title: 'Thing', type: 'default' }
		*/
		
		post.slug = utils.makeSlug(post.title)
		post.clips = []
		post.duration = null
		post.created_at = new Date()
		
		
		_Database.add('scenes', post, function(e,_scene){
			if(!e) res.redirect('/events?type='+_scene.type)
			else res.redirect('/events')
		})
				
	}
}
exports.single = function(_Database){
	return function(req,res){
			
		var slug = req.params.slug;
		_Database.getDocumentBySlug('scenes',slug,function(e,_scene){
			if(!e){ 
				
				if(_scene.clips.length>0){
					
					_Database.formatScenes(_scene._id, _scene.clips, function(e,_clips){
						//console.log(_clips)
						if(!e){
							_scene.clips = _clips
							_Database.getAll('files',function(_e,_files){
								if(!e) res.render('scenes/single',{current: req.url, title:_scene.title, page_slug: 'scenes-single',scene: _scene, files: _files, error: null})
								else res.render('scenes/single',{current:req.url, title: 'Get Files Scene Error', page_slug: 'scenes-single error', scene: _scene,files:[],error:_e})
							})
						}else{
							res.render('scenes/single',{current:req.url, title: 'Format Scene Error', page_slug: 'scenes-single error', scene:_scene,files:[],error:_e})
							
						}
					})
					//res.render('scenes/single',{current: req.url, title:_scene.title, page_slug: 'scenes-single', scene: _scene, files: _files, error: null })
					
				}else{ //no clips if(_scene.clips.length>0)
					
					_Database.getAll('files',function(_e, _files){
						if(!_e){
							
							res.render('scenes/single',{current: req.url, title:_scene.title, page_slug: 'scenes-single', scene: _scene, files: _files, error: null })
							
						}else{
							res.render('scenes/single',{current:req.url, title: 'Get Files Scene Error', page_slug: 'scenes-single error', scene:_scene,files:[],error:_e})
						}
					})
					
				}//end if(_scene.clips.length>0)
			
			}else{ //if(!e)
				res.render('scenes/single', {current: req.url, title: 'Find Scene Error', page_slug: 'scenes-single error', scene: [], files:[], error: e})
			}// end if(!e)
		})
		
		
	}
}
exports.update = function(_Database){
	return function(req,res){
		res.jsonp({working:"onit"})
	}
}
exports.delete = function(_Database){
	return function(req,res){
		res.jsonp({working:"onit"})
	}
}
exports.reorder = function(_Database){
	return function(req,res){
		res.jsonp({working:"onit"})
	}
}