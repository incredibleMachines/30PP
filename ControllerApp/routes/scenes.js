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

		_Database.queryCollection('scenes',{type:post.type},function(e,_scenes){
			post.order = (_scenes.length==0)? 1 : _scenes.length
			var regex = new RegExp(post.slug)
			_Database.queryCollection('scenes',{slug:{$regex: regex }},function(e,_slugs){
				if(_slugs.length>0) post.slug+='-'+_slugs.length.toString()
				_Database.add('scenes', post, function(e,_scene){
					if(!e) res.redirect('/events?type='+_scene.type)
					else res.redirect('/events')
				})
			})
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
								if(!e) res.render('scenes/single',{current: req.url, title:_scene.title, page_slug: 'scenes-single',scene: _scene, files: _files, auth: req.session.name, error: null})
								else res.render('scenes/single',{current:req.url, title: 'Get Files Scene Error', page_slug: 'scenes-single error', scene: _scene,files:[], auth: req.session.name,error:_e})
							})
						}else{
							res.render('scenes/single',{current:req.url, title: 'Format Scene Error', page_slug: 'scenes-single error', scene:_scene,files:[], auth: req.session.name, error:_e})

						}
					})
					//res.render('scenes/single',{current: req.url, title:_scene.title, page_slug: 'scenes-single', scene: _scene, files: _files, error: null })

				}else{ //no clips if(_scene.clips.length>0)

					_Database.getAll('files',function(_e, _files){
						if(!_e){

							res.render('scenes/single',{current: req.url, title:_scene.title, page_slug: 'scenes-single', scene: _scene, files: _files, auth: req.session.name, error: null })

						}else{
							res.render('scenes/single',{current:req.url, title: 'Get Files Scene Error', page_slug: 'scenes-single error', scene:_scene,files:[], auth: req.session.name, error:_e})
						}
					})

				}//end if(_scene.clips.length>0)

			}else{ //if(!e)
				res.render('scenes/single', {current: req.url, title: 'Find Scene Error', page_slug: 'scenes-single error', scene: [], files:[], auth: req.session.name, error: e})
			}// end if(!e)
		})


	}
}
exports.update = function(_Database){
	return function(req,res){
		var post = req.body
		var slug = req.params.slug
		//console.log(post)
		var newSlug = utils.makeSlug(post.title)
		//console.log(newSlug)
		_Database.update('scenes',{slug: slug},{$set:{title:post.title,slug:newSlug}},function(e){
			if(!e){
				_Database.getDocumentBySlug('scenes',utils.makeSlug(post.title),function(_e,_scene){
					if(!_e) res.redirect('/events/?type='+_scene.type)
					else res.jsonp(500,{error:_e})
				})

			}else res.jsonp(500,{error:e})
		})
		//res.jsonp({working:"onit"})
	}
}
exports.delete = function(_Database){
	return function(req,res){
		//res.jsonp({working:"onit"})
		var slug = req.params.slug
		var type ='';
		_Database.getDocumentBySlug('scenes',slug,function(e,_scene){
			type = _scene.type
			order = _scene.order
			_Database.update('scenes',{type: _scene.type, order:{$gt:_scene.order}}, {$inc:{order:-1}},function(_e){
				_Database.remove('clips',{scene_id: _scene._id},function(_e){
					_Database.remove('scenes',{_id:_scene._id},function(__e){
						res.redirect('/events/?type='+type)
					})
				})
			})
		})
	}
}

exports.reorder = function(_Database){
	return function(req,res){
		//res.jsonp({working:"onit"})
		var post = req.body;
		post.id = req.params.id
		var desiredIndex = 0;

		_Database.getDocumentByID('scenes',post.id,function(e,_scene){
			//console.log(_scene)
			var currentIndex = _scene.order;
			if(post.type =='up'){
				desiredIndex = parseInt(_scene.order)-1;
			}else{
				desiredIndex = parseInt(_scene.order)+1;
			}
			_Database.update('scenes',{type: _scene.type ,order: desiredIndex},{$set:{order:currentIndex}},function(_e){
				_Database.update('scenes',{_id:_scene._id},{$set:{order:desiredIndex}},function(__e){

					res.redirect('/events/?type='+_scene.type)

				})
			})
		})
	}
}
