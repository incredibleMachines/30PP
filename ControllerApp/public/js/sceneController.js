/* A function to manipulate scenes on the front-end. 

  _scenes = function({OBJ}) Takes a JSON Object of _scenes directly from scene render.
  
  ** See views/includes/footer.ejs  **

*/

function SceneController(_scenes){
	var currentScene = _scenes[0];
	//console.log(_scenes);
	console.log( window.location.hash.substr(1))
	//check if we're looking for a scene
	if(window.location.hash){
		for(var i =0; i<_scenes.length;i++){ 
			if(_scenes[i].slug == window.location.hash.substr(1)){ 
				currentScene = _scenes[i];
				//alert(id);
				//repopulate the content
				//change form slug
				//console.log(scene)
				//console.log()
				//console.log(currentScene)

				$('#scene-contents	 form').attr('action', '/scenes/'+currentScene._id);
				//console.log('.scene-title.'+currentScene.slug)
				//console.log()
				
				//console.log($('.scene-view .scene-title'));
				$('.scene-view .scene-title').each(function(i){
					$(this).val(currentScene.title);
					$(this).html(currentScene.title)
					
				});
				
				$('.scene-view .scene-text_type').val(currentScene.text_type);
				$('.scene-view .scene-scene_type').val(currentScene.scene_type);
				createAssets(currentScene.scene_type,currentScene.text_type);  
			}
		}
	}else{
		createAssets(currentScene.scene_type,currentScene.text_type)
		
	}	
	//scene type modifications and ui changes when the dropdown changes
	$('select#scene_type').change(function(e){
		var diff =  $(this).val()-currentScene.scene_type;
		var visible = $('select.asset-file').filter(':visible');
		//console.log('diff '+diff)
		//console.log('val '+$(this).val())
		
		
		if(diff > 0){
			//console.log(diff)
			for(var i = visible.length; i < visible.length+diff; i++){
				//console.log('here to add number = '+i);
				var newAsset = $('article.asset-single-0').clone();
				$(newAsset).removeClass(function(index,c){
					//'asset-single-0'
					var _this = $(this)
					$(c.split(' ')).each(function(){
						var c = this.trim();
						//console.log(c)
						if(c.indexOf('asset-single-')>=0){
							//console.log("Match :: "+c)
							_this.removeClass(c)
							
						}
						//console.log(c)
					})
					
				}).addClass('asset-single-'+i);
				
				//reset for array values
				
				$(newAsset).find('input.asset-title').attr('name','assets['+i+'][title]');
				$(newAsset).find('select.asset-zone-type').attr('name','assets['+i+'][zone_type]');
				$(newAsset).find('select.asset-zone-type option:selected').removeAttr('selected');
				$(newAsset).find('select.asset-file').attr('name','assets['+i+'][file]');
				$(newAsset).find('select.asset-file option:selected').removeAttr('selected');
				$(newAsset).find('textarea.asset-caption').attr('name','assets['+i+'][caption]').html('');
				
				
				//console.log(newAsset);
				//$("#asset-data").append(newAsset)//.insertAfter( $('asset-single-'+(i-1) ) );	
				$(newAsset).insertAfter(".asset-single-"+(i-1))
			} 
		}else{
			//console.log(visible.length)
			//$(visible[visible.length-1]).parent().remove()

			//add a negative
					
			for(var i = visible.length-1; i > visible.length+diff-1; i--){
				//console.log('remove')			
				//console.log($(visible[i]).parents('.row'))
				//never delete our last object
				if(i>0) $(visible[i]).closest('.row').remove()
			}
					
		}
		
		//createAssets($(this).val(),currentScene.text_type);
		//sort our types out
		var visible = $('select.asset-file').filter(':visible');
		//iterate through all our visible scene containers
		//console.log(visible[0])
		//console.log(visible.length);
		//set our zone type options
		switch(visible.length){
			case 1:
				$(visible[0]).closest('.asset-contents').find('.asset-title').attr("value","Wall And Sculpture"); 
				$(visible[0]).closest('.asset-contents').find('select.asset-zone-type').parent().hide();
			break;
			case 2:
				$(visible[0]).closest('.asset-contents').find('.asset-title').attr("value","Sculpture");
				$(visible[0]).closest('.asset-contents').find('select.asset-zone-type').html("<option value=\"\">None</option><option value=\"Multi Location\">Multi Location</option><option value=\"Single Location\">Single Location</option>").parent().show();
				$(visible[1]).closest('.asset-contents').find('.asset-title').attr("value","Single Wall")
				$(visible[1]).closest('.asset-contents').find('select.asset-zone-type').html("<option value=\"\">None</option><option value=\"Multi Text\">Multi Text</option>").parent().show();
			break;
			case 3:
				$(visible[0]).closest('.asset-contents').find('.asset-title').attr("value","Sculpture");
				$(visible[0]).closest('.asset-contents').find('select.asset-zone-type').html("<option value=\"\">None</option><option value=\"Multi Location\">Multi Location</option><option value=\"Single Location\">Single Location</option>").parent().show();
				$(visible[1]).closest('.asset-contents').find('.asset-title').attr("value","Left Wall")
				$(visible[2]).closest('.asset-contents').find('.asset-title').attr("value","Right Wall")
			
			default:
				$(visible[0]).closest('.asset-contents').find('.asset-title').attr("value","Sculpture");
				$(visible[0]).closest('.asset-contents').find('select.asset-zone-type').html("<option value=\"\">None</option><option value=\"Multi Location\">Multi Location</option><option value=\"Single Location\">Single Location</option>").parent().show();
				$(visible[1]).closest('.asset-contents').find('.asset-title').attr("value","Left Wall")
				$(visible[2]).closest('.asset-contents').find('.asset-title').attr("value","Right Wall")
				
				for(var i = 3; i<visible.length; i++){
					//console.log('here');
					$(visible[i]).closest('.row').remove()
				} 
			break;
		}
		//set our currentScene Type val to our current val, incase the user wants to make another modification
		currentScene.scene_type = $(this).val()
		orderAssets()
		
		
	}); //#End Zone Type Selection options
	
	$('select#text_type').change(function(e){
		
		var diff =  $(this).val()-currentScene.text_type;
		console.log(diff)
		var visible = $('textarea.asset-caption').filter(':visible');
		//console.log(visible)
		var totalScenes = $('select.asset-file').filter(':visible');
		if(diff > 0){
			//console.log('add')
			for(var i = totalScenes.length+visible.length; i < (totalScenes.length+visible.length)+diff; i++){
				//console.log('here to add number = '+i);
				
				var newAsset = $('article.asset-single-0').clone();
				//console.log('here')
				//callback function to remove 
				$(newAsset).removeClass(function(index,c){
					//'asset-single-0'
					var _this = $(this)
					$(c.split(' ')).each(function(){
						var c = this.trim();
						//console.log(c)
						if(c.indexOf('asset-single-')>=0){
							//console.log("Match :: "+c)
							_this.removeClass(c)
							
						}
						//console.log(c)
					})
					
				}).addClass('asset-single-'+i);
				$(newAsset).find('input.asset-title').attr('name','assets['+i+'][title]');
				$(newAsset).find('select.asset-zone-type').attr('name','assets['+i+'][zone_type]');
				$(newAsset).find('select.asset-zone-type option:selected').removeAttr('selected');
				$(newAsset).find('select.asset-file').attr('name','assets['+i+'][file]');
				$(newAsset).find('select.asset-file option:selected').removeAttr('selected');
				$(newAsset).find('textarea.asset-caption').attr('name','assets['+i+'][caption]').html('');
				$(newAsset).find('textarea.asset-caption').parent().show()
				$(newAsset).find('select.asset-file').parent().hide()
				$(newAsset).find('select.asset-zone-type').parent().hide()
				//console.log(newAsset);
				//$("#asset-data").append(newAsset)//.insertAfter( $('asset-single-'+(i-1) ) );	
				$(newAsset).insertAfter(".asset-single-"+(i-1))
			} 
		}else{
			//console.log('remove')
			for(var i = visible.length-1; i > visible.length+diff-1; i--){
				console.log('remove :: '+i)			
				//console.log($(visible[i]).parents('.row'))
				//never delete our last object
				console.log($(visible[i]))
				$(visible[i]).closest('.row').remove()
			}
		}
		var visible = $('textarea.asset-caption').filter(':visible');
		//switch to change titles
		switch(visible.length){
			
			case 1:
				$(visible[0]).closest('.asset-contents').find('input.asset-title').attr("value","Text"); 
			break;
			case 2:
				$(visible[0]).closest('.asset-contents').find('input.asset-title').attr("value","Text One"); 
				$(visible[1]).closest('.asset-contents').find('input.asset-title').attr("value","Text Two"); 
			break;
		
		}
		
		currentScene.text_type = $(this).val()
		orderAssets()
	})
	//if a scene title button is pressed in the sidebar
	//change the scene with javascript
	$('button.scene-title').click(function(e){
		
		//alert($(this).data('scene-id'))
		var id = $(this).data('scene-id');
		
		//associate ID with obj
		_scenes.forEach(function(scene,i){
			//we will always get a match here
			if(scene._id == id ){ 
				currentScene = scene;

				$('#scene-contents	 form').attr('action', '/scenes/'+currentScene._id);
				
				//console.log($('.scene-view .scene-title'));
				$('.scene-view .scene-title').each(function(i){
					$(this).val(currentScene.title);
					$(this).html(currentScene.title)
					
				});
				
				$('.scene-view .scene-text_type').val(currentScene.text_type);
				$('.scene-view .scene-scene_type').val(currentScene.scene_type);
				
				//setup the correct zones to go with the scene we are viewing
				createAssets(currentScene.scene_type,currentScene.text_type);
				
			
			}
		})
		
	});
	
	/*
	**
	** orderAssets()
	**
	*/
	
	function orderAssets(){
		var assets = $('article.asset-single');
		
		for(var i = 0; i<assets.length; i++){
			console.log(assets[i])
			$(assets[i]).removeClass(function(index,c){
					//'asset-single-0'
					var _this = $(this)
					$(c.split(' ')).each(function(){
						var c = this.trim();
						//console.log(c)
						if(c.indexOf('asset-single-')>=0){
							//console.log("Match :: "+c)
							_this.removeClass(c)
							
						}
						//console.log(c)
					})
					
				}).addClass('asset-single-'+i);
				$(assets[i]).find('input.asset-title').attr('name','assets['+i+'][title]');
				$(assets[i]).find('select.asset-zone-type').attr('name','assets['+i+'][zone_type]');
				$(assets[i]).find('select.asset-file').attr('name','assets['+i+'][file]');
				$(assets[i]).find('textarea.asset-caption').attr('name','assets['+i+'][caption]')
				console.log(assets[i])
			
		}
		
		
	}
	
	/*
	**
	** createAssets(_val) 
	** _val = string, a preset value determined by the system currently 'zone_0', 'zone_1','zone_2','zone_3' (subject to updates)
	** 
	*/
	
	
	function createAssets(_scene_type, _text_type){
			//console.log(_scene_type)
			//TO DO Populate all the assets!
			//console.log(currentScene.assets.length)
			//clone our original asset (store it in the Browsers Memory)
			var asset = $('article.asset-single-0').clone();
			
			//clear all the old assets from the HTML
			$('article.asset-single').remove();
			
			/*
			var assetNum;
			
			//assign a number value to our zones - note this is currently hard coded and will be updated
			switch(_val){
				//This is will have to change once we know what the fucks we need here.
				case '0': assetNum = 1; break;
				case '1': assetNum = 2; break;
				case '2': assetNum = 3; break;
				case '3': assetNum = 4; break; 
			}
			*/
			
		 	var assetNum =0,  contentAsset, textAsset;
			//assign a number value to our zones - note this is currently hard coded and will be updated
			
			switch(_scene_type){
				//This is will have to change once we know what the fucks we need here.
				case "0": contentAsset=1; assetNum+=contentAsset;  break; //Full Immersion
				case "1": contentAsset=2; assetNum+=contentAsset;  break; //single Wall Sculpture
				case "2": contentAsset=3; assetNum+=contentAsset;  break; //double Wall Sculpture
			} 
			switch(_text_type){
				case "0": textAsset=0; assetNum+=textAsset;  break;
				case "1": textAsset=1; assetNum+=textAsset;  break;
				case "2": textAsset=2; assetNum+=textAsset;  break;
			}
							
			
			
			//console.log(currentScene.assets.length +" "+assetNum)
			//clear all the old single assets from the column
			$('article.asset-single').remove();
			
			
			
			for(var i = 0; i< assetNum; i++){
			
				var title = "Title";
				if(i < contentAsset){
					if( i == 0 ){
						if(contentAsset ==1 ) title= "Wall And Sculpture"
						else title = "Sculpture"
					}
					if( contentAsset== 3){
						if(i == 1) title = "Left Wall"
						if(i == 2) title = "Right Wall"
						
					}else{
						if(i == 1) title = "Single Wall"
					}
				}else{
					if(textAsset>1){
						if(i == assetNum - 1) title = "Text Two"
						else title = "Text One"
					}else{
						title = "Text"
					}
				} 
				//copy our model asset for the number of assets we have
				var newAsset = $(asset).clone();
				//console.log(currentScene.assets[i])
				//console.log(i+" "+currentScene.assets.indexOf(i));
				//update the class structure
				$(newAsset).removeClass('asset-single-0').addClass('asset-single-'+i);
				
				$(newAsset).find('select.asset-file').attr('name','assets['+i+'][file]').addClass('')
				$(newAsset).find('select.asset-file option:selected').removeAttr('selected');

				$(newAsset).find('textarea.asset-caption').attr('name','assets['+i+'][caption]').html('');
				$(newAsset).find('input.asset-title').attr('value',title)
				
				if(i < contentAsset) {
					$(newAsset).find('textarea.asset-caption').parent().hide()
					$(newAsset).find('select.asset-file').parent().show()
					$(newAsset).find('select.asset-zone-type').parent().show()
				}
				
				if(i>=contentAsset){
					$(newAsset).find('select.asset-file').parent().hide()
					$(newAsset).find('textarea.asset-caption').parent().show()
					$(newAsset).find('select.asset-zone-type').parent().hide()

					
				}
				if(title == "Sculpture" || title == "Single Wall"){
					$(newAsset).find('select.asset-zone-type').parent().show()
				}else{
					$(newAsset).find('select.asset-zone-type').parent().hide()
				}
				
				//append the data into our holder
				$('#asset-data').append(newAsset);	
			}
			
			for(var i =0; i <assetNum; i++){
				if(typeof currentScene.assets[i] !== 'undefined'){ //check if we have a corresponding asset
					console.log(i+" :: "+JSON.stringify(currentScene.assets[i]));
  					//iterate through the asset
					Object.keys(currentScene.assets[i]).forEach(function(key){
						//make sure that our index's match
						//console.log($('.asset-single-'+key).find('select.asset-file option[value="'+currentScene.assets[i][key].file._id+'"]'))	
						$('.asset-single-'+key).find('select.asset-file option[value="'+currentScene.assets[i][key].file._id+'"]').attr('selected','selected');
						
						
						$('.asset-single-'+key).find('textarea.asset-caption').html(currentScene.assets[i][key].caption);			
						//doesnt' load image from filepath express.js doesn't know about .assets
						//$('.asset-single-'+key).find('.asset-image .thumbnail img').attr('src',currentScene.assets[i][key].file.path);
						if(currentScene.assets[i][key].type !== 0) { 
							$('.asset-single-'+key).find('select.asset-file').parent().show(); 
							$('.asset-single-'+key).find('textarea.asset-caption').parent().hide();
						}
						if(currentScene.assets[i][key].type == 0){ 
							$('.asset-single-'+key).find('select.asset-file').parent().hide();
							$('.asset-single-'+key).find('textarea.asset-caption').parent().show();
						}
						
						if($('.asset-single-'+key+' input.asset-title').val() == "Single Wall"){
							var html;
							console.log("Single Wall")
							if(currentScene.assets[i][key].zone_type === "Multi Text"){
								html = "<option value=\"\">None</option><option selected value=\"Multi Text\">Multi Text</option>"
							}else{
								html = "<option selected value=\"\">None</option><option value=\"Multi Text\">Multi Text</option>"
							}
							
							
							$('.asset-single-'+key).find('select.asset-zone-type').html(html)
							
						}
						if($('.asset-single-'+key+' input.asset-title').val() == "Sculpture"){
							var html;
							if(currentScene.assets[i][key].zone_type === "Multi Location"){
								html = "<option value=\"\">None</option><option selected value=\"Multi Location\">Multi Location</option><option value=\"Single Location\">Single Location</option>"
							}else if(currentScene.assets[i][key].zone_type === "Single Location"){
								html = "<option value=\"\">None</option><option value=\"Multi Location\">Multi Location</option><option selected value=\"Single Location\">Single Location</option>"
							}else{
								html = "<option selected value=\"\">None</option><option value=\"Multi Location\">Multi Location</option><option value=\"Single Location\">Single Location</option>"
							}
							$('.asset-single-'+key).find('select.asset-zone-type').html(html)	
						}
						
					})


				}else{
					console.log('currentScene.assets['+i+'] == undefined ')
				}
			}//end for i<assetNum
			orderAssets();
				
	}//end createAssets(_scene_type, _text_type)


}

