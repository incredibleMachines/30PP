var utils = require('../modules/Utils');


exports.add = function(_Database){

	return function(req,res){
		
		//check post data
		var post = req.body;
		console.log(req.body);
		
		/*
		**
		**	Sample post: 
		**	{ event_id: '52eae3259e23ff4905ca5d31',
		**	  title: '',
		**	  scene_type: '0',
		**	  zone_type: '0',
		**	  text: '' }
		**
		*/
		 
		 post.slug = utils.makeSlug(post.title);
		 post.assets = [];
		 post.event_id = _Database.makeMongoID(post.event_id)
		 post.created_at = new Date();
		 post.last_edited = new Date();
		 
		 //store our new scene
		 _Database.add('scenes', post, function(err,doc){
			 if(!err){
			 	//update the event object with our new scene
			 	var update_obj = {$push: {scenes: doc._id }};
			 	_Database.updateById('events', post.event_id, update_obj,function(e){
			 	
		 			if(!e){
		 				//query to get the event slug
		 				_Database.getDocumentByID('events', doc.event_id, function(_e, _doc){
							if(_e) res.jsonp(500,{error:_e});
							else res.redirect('/events/'+_doc.slug);	
				 		});
		 			} 
		 			else res.jsonp(500,err);
		 			
			 	});
			 
			 }else{ 
			 	//report error with 500 status
			 	res.jsonp(500,err)
				 
			 }
		 })
	}
}
//reorder the scenes in the DB and for output
exports.reorder = function(_Database){
	return function(req,res){
		var id = req.params.id;
		var post = req.body;
		post.id = id;
		console.log(post)
		_Database.getDocumentByID('scenes',id,function(e,scene){
			if(e) res.jsonp(500,{error: e})
			else{
				_Database.getDocumentByID('events',scene.event_id,function(_e,event){
					if(_e) res.jsonp(500,{error:e})
					else{
						var index; 
						var swap_index;
						var holder = event.scenes;
						var bFoundMatch = false;
						console.log(" EVENT SCENES ".inverse.cyan)
						console.log(event.scenes);
						console.log(" END EVENT SCENES ".inverse.cyan)
						//iterate through the events scenes array
						event.scenes.forEach(function(_scene, i){
							console.log(_scene.toString().green.inverse+ "    ".inverse.green+i.toString().inverse.green)
							if(!bFoundMatch){						
							//find the index of the scene
								if( _scene.toString() === id.toString() ){
									bFoundMatch = true;
									index = i;
									console.log(" We Have A Match ".inverse.red+ id.inverse.red+" : at Index : ".inverse.red+index.toString().inverse.red);
									
									//determine if the scene must go up or down
									if(post.type ==='up'){
										swap_index = index-1
									}else if(post.type ==='down'){
										swap_index = index+1
									}
									
									//switch the indexes
									//var obj = event.scenes[index]
									holder[index] = holder[swap_index] 
									holder[swap_index] = _Database.makeMongoID(id)
									var updateObj = {$set: {scenes: holder}}
									console.log(" UPDATED HOLDER ".inverse.blue)
									console.log(holder);							
									console.log(" END UPDATED HOLDER ".inverse.blue)
									
									//update event scene array
		
									_Database.updateById('events',event._id, updateObj,function(__e){
																	//return to the event slug
																	if(!__e) res.redirect('/events/'+event.slug) 
																	else res.jsonp(500,{error: __e})
														})
									
									
								}//end if( _scene.toString() === id.toString() )
							}//end if(!bFoundMatch)
						
						
						})//end for each
								
					}
				})
				
			}
			
			
		})
		
		//console.log(req.body);
		//res.jsonp({message:'reorder not implimented yet'})
		
	}
	
}

exports.newUpdate = function(_Database){
	

}
exports.update = function(_Database){

	return function(req,res){
		var post = req.body;
		//console.log(post);
		
		/* *************************************
		** Sample Post:
		**	{ 	title: 'New Sample Scene',
		**		scene_type: '0',
		**		zone_type: '2',	
		**		text: 'dsfdsfdsfsdfsd',
		**		assets: 
		**				[ 	{ file: '52f0319eb45e28f90d5fd2c3', zone: 'zone_1', caption: '' },
		**					{ file: '52f0319eb45e28f90d5fd2c3', zone: 'zone_2', caption: '' },
		**					{ file: '52f0319eb45e28f90d5fd2c3', zone: 'zone_3', caption: '' } ] }
		**
		*/
		
		//scene DB ID
		var scene_id = req.params.id;
		//console.log(scene_id)
		
		//process all information - update scenes and assets
		//need a process to check assets in scenes and update or make new asset
		_Database.getDocumentByID('scenes',scene_id,function(e,_scene){
			
			if(!e){ 
				console.log(_scene.assets)
				console.log(post.assets)
				if(_scene.assets.length == post.assets.length){
					// the zone type has not changed
					// update each asset and then the scene[asset]
					console.log(' Asset Totals Match '.inverse)
					var cbCounter =0;
					var bNeedsRender = true;
					_scene.assets.forEach(function(asset,index){
						//iterate through each of the scenes from the db
						
						Object.keys(asset).forEach(function(key){
							//this convoluted function serves to replace for(key in asset)
							//since we do not know the appropriate key names, all the time
							//we must pull them via this native js operation.
							//scene.assets = [{'0': ObjectId},{'1':ObjectId}] - where the key indicates the zone	
						
							//var value = asset[key];
							//console.log(key+" : "+asset[key])
							
							_Database.getDocumentByID('assets',asset[key],function(err,_asset){
								if(!err){
									//console.log(key+":"+JSON.stringify(_asset))
									//now we check if they items are the same or different.
									if(_asset.file.toString() == post.assets[parseInt(key)].file.toString() && 
									   _asset.caption == post.assets[parseInt(key)].caption && 
									   _asset.zone_type == post.assets[parseInt(key)].zone_type &&
									   _asset.title == post.assets[parseInt(key)].title ){
										cbCounter++;
										//bNeedsRender = false;
										if(cbCounter==_scene.assets.length){
										var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text}}
																
											_Database.updateById('scenes',scene_id,updateObj,function(e){
												if(!e){
													redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
													//res.jsonp({success:'success'})	
												} 
												else res.jsonp(500,{error:_err})
											})	
										} 
										//res.jsonp({success:'success'})
										console.log('Everything is the same');
									}else{
										console.log('Something is different')
										bNeedsRender = true;
										var updateObj = {$set: {file: _Database.makeMongoID(post.assets[parseInt(key)].file), 
																caption: post.assets[parseInt(key)].caption, 
																zone: post.assets[parseInt(key)].zone
																}
														}
										_Database.updateById('assets',_asset._id, updateObj, function(e){
											cbCounter++;
											if(cbCounter==_scene.assets.length){
												var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text}}
													_Database.updateById('scenes',scene_id,updateObj,function(e){
													if(!e) redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
													// res.jsonp({success:'success'})
													else res.jsonp(500,{error:_err})
												})	
											} 
											//res.jsonp({success:'success'})
										})
									}
								}//END if(!err)

							})//END _Database.getDocumentByID
						})//END Object.keys(asset).forEach
					})
					//res.jsonp({hello:'world'})
					
				}else if(_scene.assets.length == 0 ){
					console.log('+++++++ No Scene Assets Yet ++++'.green.inverse);
					//for each asset submitted create new asset, associate order in scene[asset]
					var cb_counter = 0;
					var assetholder = [];
					bNeedsRender = true;

					post.assets.forEach(function(asset,index){
						 //console.log(asset)
						 //console.log(index)
						 if(asset.file != ''){ //if the file string is populated
							_Database.getDocumentByID('files',asset.file,function(err,_file){
								//console.log(_file)
								if(!err){
									asset.type = _file.type;
									asset.file = _file._id;
									//addAsset(asset, _Database, post, assetholder, index, cb_counter, res);
									
									_Database.add('assets',asset,function(_err,_asset){
										//var dbSelector = 'assets.'+index;
										//var updateObj = {$set: {dbSelector: _asset._id }}
										cb_counter++;
										var temp= {};
										temp[index]=_asset._id;
										assetholder.push(temp)
										if(cb_counter == post.assets.length){
										var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text,
																assets:assetholder}}
											//var updateObj = {$set: {title: post.title, last_edited: new Date(), render: bNeedsRender, slug: utils.makeSlug(post.title), zone_type: post.zone_type, text: post.text, assets:assetholder}}
											_Database.updateById('scenes',scene_id,updateObj,function(e){
												if(!e) redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
												// res.jsonp({success:'success'})
												else res.jsonp(500,{error:_err})
											})
										}
									})

								}else{
									res.jsonp(500,{error:err});
								}
							})
						}else{ //if there is no file ie caption only
							asset.type = 0;
							//addAsset(asset, _Database, post, assetholder, index, cb_counter, res)
							bNeedsRender = true;
							_Database.add('assets',asset,function(_err,_asset){
									cb_counter++;
									var temp= {};
									temp[index]=_asset._id;
									assetholder.push(temp)
									if(cb_counter == post.assets.length){
									var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text,
																assets:assetholder}}
										//var updateObj = {$set: {title: post.title, last_edited: new Date(), render: bNeedsRender, slug: utils.makeSlug(post.title), zone_type: post.zone_type, text: post.text, assets:assetholder}}
										_Database.updateById('scenes',scene_id,updateObj,function(e){
											if(!e){
												redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
												// res.jsonp({success:'success'})
											}
											else res.jsonp(500,{error:_err})
										})
									}
							})	
						}
						
					})
					
				}else{
					// we have two different lengths of assets we need to determine what needs to get deleted and or made, as well as which order they should go into
					// we should remove or update our old assets
					
					console.log('One of these things is not like the other, one of these things just doesn\'t belong')
					
					//decide which we have extra of stored assets or incoming assets
					//this means a zone type has changed.
					bNeedsRender = true;
					if(_scene.assets.length > post.assets.length){
						//we need to remove an asset(s) so figure out which one(s)
						console.log('Remove Assets');
						var diff = _scene.assets.length-post.assets.length;
						//console.log('Diff: '+diff)
						var diffCount = 0;
						for(var i = _scene.assets.length-1; i>=_scene.assets.length-diff; i--){							
							//console.log(_scene.assets[i])
							//remove asset from database
							Object.keys(_scene.assets[i]).forEach(function(key){
								_Database.remove('assets',_scene.assets[i][key],function(e){
									diffCount++;
									var position = _scene.assets.indexOf(i);

									_scene.assets.splice(position, 1);
									
									if(diffCount==diff){
									console.log(_scene.assets)
									var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text,
																assets:_scene.assets}}
									//var updateObj = {$set: {title: post.title, last_edited: new Date(), render: bNeedsRender, slug: utils.makeSlug(post.title), zone_type: post.zone_type, text: post.text, assets:_scene.assets}}
									_Database.updateById('scenes', scene_id, updateObj,function(e){
										if(!e) redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
										// res.jsonp({success:'success'})
										else res.jsonp(500,{error:_err})
									})
									
									
									
										//update our scene object and 
									}
								})
							})
						}
						
					}else if(_scene.assets.length<post.assets.length){
						//we need to add an asset(s) to our scene that didn't exist before
						console.log('Add Assets'.inverse.red)
						var diff = post.assets.length-_scene.assets.length;
						console.log(diff)
						var cb_counter = 0;
						bNeedsRender = true;
						post.assets.forEach(function(asset,index){
								if(index>=post.assets.length-diff){
									//check if the asset we are iterating on is a new asset or an old one
									console.log('NEW ASSET'.inverse.green)
									//console.log(index);
									//console.log(asset);
									
									if(asset.file != ''){ //if the file string is populated
										_Database.getDocumentByID('files',asset.file,function(err,_file){
											//console.log(_file)
											if(!err){
												asset.type = _file.type;
												asset.file = _file._id;
												//addAsset(asset, _Database, post, assetholder, index, cb_counter, res);
												
												_Database.add('assets',asset,function(_err,_asset){
													//var dbSelector = 'assets.'+index;
													//var updateObj = {$set: {dbSelector: _asset._id }}
													cb_counter++;
													var temp= {};
													temp[index]=_asset._id;
													_scene.assets.push(temp)
													if(cb_counter == post.assets.length){
														var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text,
																assets:_scene.assets}}
															
															_Database.updateById('scenes',scene_id,updateObj,function(e){
															if(!e) redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
															// res.jsonp({success:'success'})
															else res.jsonp(500,{error:_err})
														})
													}
												})
			
											}else{
												res.jsonp(500,{error:err});
											}
										})	
									}else{
									
										asset.type = 0;
										_Database.add('assets',asset,function(_err,_asset){
													cb_counter++;
													var temp= {};
													temp[index]=_asset._id;
													_scene.assets.push(temp)
													if(cb_counter == post.assets.length){
														var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text,
																assets:_scene.assets}}
															_Database.updateById('scenes',scene_id,updateObj,function(e){
															if(!e) redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
															// res.jsonp({success:'success'})
															else res.jsonp(500,{error:_err})
														})
													}
											})
										
									}//end if(asset.file!='')
								}else{
									//this asset is already set and so we should check if we need to make an update to the asset
									Object.keys(_scene.assets[index]).forEach(function(key){
										console.log(' OLD ASSET '.inverse)
										//console.log(index)
										//console.log(asset)
										//console.log(_scene.assets[index])
										//console.log(_scene.assets[index][key])
										_Database.getDocumentByID('assets',_scene.assets[index][key],function(err,_asset){
											if(!err){
												//console.log(key+":"+JSON.stringify(_asset))
												//now we check if they items are the same or different.
												if(_asset.file.toString() == post.assets[parseInt(key)].file.toString() && 
												   _asset.caption == post.assets[parseInt(key)].caption && 
												   _asset.zone_type == post.assets[parseInt(key)].zone_type &&
												   _asset.title == post.assets[parseInt(key)].title ){
												   
												   console.log(' +++ THIS ASSET HAS NO CHANGES +++ '.inverse.green);

													cb_counter++;
													if(cb_counter == post.assets.length){
														var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text,
																assets:_scene.assets}}
														_Database.updateById('scenes',scene_id,updateObj,function(e){
															if(!e) redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
															// res.jsonp({success:'success'})
															else res.jsonp(500,{error:_err})
														})
													}
												}else{
													console.log(' +++ THIS ASSET HAS CHANGES +++ '.inverse.red)
													bNeedsRender = true;
													var updateObj = {$set: {file: _Database.makeMongoID(post.assets[parseInt(key)].file), 
																			caption: post.assets[parseInt(key)].caption, 
																			zone: post.assets[parseInt(key)].zone }
																	}
													_Database.updateById('assets',_asset._id, updateObj, function(e){
														cb_counter++;
														if(cb_counter == post.assets.length){
															var updateObj = {$set: {title: post.title, 
																last_edited: new Date(),
																text_type: post.text_type, 
																scene_type: post.scene_type, 
																render: bNeedsRender, 
																slug: utils.makeSlug(post.title), 
																text: post.text,
																assets:_scene.assets}}
															_Database.updateById('scenes',scene_id,updateObj,function(e){
																if(!e) redirectByEventIdToScene(res,_Database,_scene.event_id,_scene.slug);
																// res.jsonp({success:'success'})
																else res.jsonp(500,{error:_err})
															})
														}
														//if(cbCounter==_scene.assets.length) res.jsonp({success:'success'})
													})
												}
											}else{
												console.log('we had an error getting asset by id');
											}//END if(!err)
									
										})//END _Database.getDocumentByID
									});//Object.keys()
									
								}								
						})//end post.assets
						

					}//End if(_scene.assets.length > post.assets.length)
					
				}
			}else{
				res.jsonp(500, {error: e})
			}
		})
		//res.jsonp({message:'update'});
		//res.render('events/index', { current: req.url, title: 'add event' });
	}
}


exports.delete = function(_Database){



	return function(req,res){
/* 		console.log(req.params); */
//		console.log(req.body);

		_Database.getDocumentByID('scenes', req.params.id, function(e, doc){
			
			if(e) res.jsonp(500,{error:e});

			else {
				_Database.update('events', {_id: doc.event_id}, {$pull: {scenes: doc._id } }, function(_e){

					if(_e) res.jsonp(500,{error:_e});
					
					else {
						_Database.remove('scenes', {_id: doc._id }, function(__e){
				
						if(__e) res.jsonp(500,{error:__e});
						
			 			else{
				 		
				 			_Database.getDocumentByID('events', doc.event_id, function(___e, _doc){
					 		
							if(___e) res.jsonp(500,{error:___e});
							else res.redirect('/events/'+_doc.slug);	
						
							});	
						}			
					});
				
				  }
			});
		}
		});
	}
}

function redirectByEventIdToScene(res, _Database, eventId,sceneSlug){
	/* function to get the event-slug by eventId */
	_Database.getDocumentByID('events', eventId, function(_e, _doc){
		if(_e) res.jsonp(500,{error:_e});
		else res.redirect('/events/'+_doc.slug+'#'+sceneSlug);	
	});
}
function redirectByEventId(res, _Database, eventId){
	/* function to get the event-slug by eventId */
	_Database.getDocumentByID('events', eventId, function(_e, _doc){
		if(_e) res.jsonp(500,{error:_e});
		else res.redirect('/events/'+_doc.slug);	
	});
}

function addAsset(__a,__Database, post, assetholder, index,cb_counter,res){

	__Database.add('assets',__a,function(_err,_asset){
		cb_counter++;
		var temp= {};
		temp[index]=_asset._id;
		assetholder.push(temp)
		if(cb_counter == post.assets.length){
			console.log(assetholder)
			var updateObj = {$set: {assets:assetholder, last_edited: new Date()}}
			__Database.updateById('scenes',scene_id,updateObj,function(e){
				if(!e) res.jsonp({success:'success'})
				else res.jsonp(500,{error:_err})
			})
		}
	
	})
	
}


