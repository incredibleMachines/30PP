var utils = require('../modules/Utils');
var extend = require('util')._extend; //cloning objects

exports.add = function(_Database){
	return function(req,res){
		var post = req.body
		//console.log(post)
		/* Sample Post 
			{ scene_id: '53208b528c49112a77a78df7',
			  title: 'Testing',
			  layout: '0' }
		*/
		post.scene_id = _Database.makeMongoID(post.scene_id)
		post.slug = utils.makeSlug(post.title)
		post.zones = {}
		post.created_at = new Date()
		post.render = false
		
		_Database.add('clips', post, function(e,_clip){
			if(!e){
				//var temp = {}
				//temp[_clip.slug] = _clip._id;
												
				var updateObj = {$push:{clips:  _clip._id } }
				_Database.update('scenes',{_id: _Database.makeMongoID(_clip.scene_id)},updateObj,function(e){
					
					_Database.getDocumentByID('scenes',_clip.scene_id, function(_e,_scene){
					
						if(!_e) res.redirect('/scenes/'+_scene.slug)
						else res.jsonp(500,{error: _e})
						
					})
					
				})
			}else{//if(!e)
				res.jsonp(500,{error:e})
			}//if(!e)
		})
		
		
	}
}
exports.update = function(_Database){
	return function(req,res){
		var post = req.body
		console.log(post)
		var id = req.params.id
		//post.slug = utils.makeSlug(post.title)
		
		//organize our zones
		
		post.zones.forEach(function(zone, i){
			
			delete zone.map_type
			delete zone.text_type
			

			if('text' in zone){
				
			}else{ zone.text=null}
			
			if(zone.file != '') zone.file = _Database.makeMongoID(zone.file)
			else zone.file = null
			
			if('location' in zone){
				//iterate on location
			} else{
				
				zone.location = null	
			} 
			
		})
		
		console.log(post)
		
		var updateObject = { $set: {
										title: post.title,
										slug: utils.makeSlug(post.title),
										layout: post.layout,
										zones: post.zones,
										last_edited: new Date(),
										render: true
									}
								}
		
		_Database.update('clips',{_id: _Database.makeMongoID(id)},updateObject,function(e){
			if(!e){
				makeRedirectBySceneIdToClip(_Database, post.scene_id, utils.makeSlug(post.title), function(_e,url){
					if(!_e) res.redirect(url)
					else res.jsonp(500,{error:_e})
				})
			}else{
				res.jsonp(500,{error: e})
			}
		})
		

	}
}
exports.delete = function(_Database){
	return function(req,res){
		res.jsonp({working:"onit"})
	}
}
exports.reorder = function(_Database){
	return function(req,res){
		//res.jsonp({working:"onit"})
		var id = req.params.id;
		var post = req.body;
		post.id = id;
		console.log(post)
		_Database.getDocumentByID('clips',id,function(e,clip){
			if(e) res.jsonp(500,{error: e})
			else{
				console.log(clip)
				_Database.getDocumentByID('scenes',clip.scene_id,function(_e,scene){
					var holder = extend([], scene.clips);
					scene.clips.forEach(function(scene_clip,i){
						if(scene_clip.toString() === id ){
							if(post.type =='up'){
								var val = holder[i-1]
								holder[i-1] = holder[i]
								holder[i] = val 
								/*
								console.log("Holder")
								console.log(holder)
								console.log("Scene Clips")
								console.log(scene.clips)
								*/
								
							}else if(post.type=='down'){
								var val = holder[i+1]
								holder[i+1] = holder[i]
								holder[i] =val
							}
							
						    _Database.updateByID('scenes',scene._id,{$set:{clips:holder}},function(__e){
							    if(!e) res.redirect('/scenes/'+scene.slug+'#'+clip.slug)
							    else res.jsonp(500,{e:__e})
						    })
						}
					})
					
				})		
			}
		})
	}
}


function makeRedirectBySceneIdToClip(_Database, sceneId, clipSlug, cb){
	/* function to get the event-slug by eventId */
	_Database.getDocumentByID('scenes', sceneId, function(_e, _scene){
		//if(_e) res.jsonp(500,{error:_e});
		//else res.redirect('/events/'+_doc.slug+'#'+clipSlug);	
		if(_e) cb(_e)
		else cb(null, '/scenes/'+_scene.slug+'#'+clipSlug )
	});
}

