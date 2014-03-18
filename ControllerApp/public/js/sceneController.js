/* A function to manipulate scenes on the front-end. 

  _scenes = function({OBJ}) Takes a JSON Object of _scenes directly from scene render.
  
  ** See views/includes/footer.ejs  **

*/

function SceneController(_clips,_files){
	//console.log(_.isEmpty({}))
	var currentClip = _clips[0];
	//console.log("Clips:")
	//console.log(_clips);
	//console.log( window.location.hash.substr(1))
	//check if we're looking for a scene

	if(window.location.hash){
		//console.log( window.location.hash.substr(1))
		var slug = window.location.hash.substr(1)
		var clip = _.findWhere(_clips, {slug: slug});
		//var clip _.findWhere( _clips, {slug:slug} ) 

		if(typeof clip !== 'undefined') currentClip = clip

	}
	
	formatClip(currentClip)
		
	//if a scene title button is pressed in the sidebar
	//change the scene with javascript
	$('button.scene-title').click(function(e){
		
		//alert($(this).data('scene-id'))
		var id = $(this).data('scene-id');
		
		//associate ID with obj
		_clips.forEach(function(clip,i){
			//we will always get a match here
			if(clip._id == id ){ 
				currentClip = clip;
								
				
				
				formatClip(currentClip)
		
			}
		})
		
	});
	$(document).delegate('select.zone-map-type','change', function(e){
		$this = $(this)
				
		if($this.val() === 'none'){ 
					$('canvas#map').remove()
					$this.closest('.row').find('img').attr("data-src","holder.js/100%x150/industrial/text:No File Associated")
					Holder.run({
						images: $this.closest('.row').find('img')[0]
					})				
		}else{
			//check if element exists using length
			if(!$('canvas#map').length){
				
				var canvas = '<canvas id="map" style="width: 100%; height: 200px;"> </canvas>'
				$this.parent().parent().append(canvas)
				//init the map
			}
			
			$this.closest('.row').find('select.zone-file').val('none')
			var val = $this.val()
			var type = val.charAt(0).toUpperCase()+val.slice(1)
			$this.closest('.row').find('img').attr("data-src","holder.js/100%x150/industrial/text:Location Mode: "+type)
			//$this.closest('.row').find('img').attr('src','holder.js/100%x150/industrial/text:'+$this.val()+' location')
			Holder.run({
						images: $this.closest('.row').find('img')[0]
					})
					
			var locs;
			
			InitMapCanvas(type, locs, mapCallback); //see mapCallback defined below
		}
	})
	
	
	//toggle of text options
	$(document).delegate('select.zone-text-type','change', function(e){
			
		var $this = $(this)
				
		var classes = $this.closest('.row').attr('class')
		//alert(index)
		var index
		//'asset-single-0'
		$(classes.split(' ')).each(function(){
			var c = this.trim();
			//console.log(c)
			if(c.indexOf('zone-single-')>=0){
				//console.log("Match :: "+c)
				/* _this.removeClass(c) */
				index = c.substr(-1)
				//alert(index)
			}
			//console.log(c)
		})
		
		$('.zone-single-'+index).find('.zone-text').remove()
		
		switch($this.val()){
			case 'single':
				
				var text = $('textarea').attr('name', 'zones['+index+'][text]').attr('placeholder','Enter your text now').addClass('zone-text-0 zone-text')
				console.log(text)
				//alert(text)
				var text = '<section class="zone-text input-group"><textarea name="zones['+index+'][text]" placeholder="Enter your text now" class="form-control zone-text"></textarea></section>'
				
			break;
			case 'multiple':
			
				var text  = '<section class="zone-text input-group"><input name="zones['+index+'][text][0]" placeholder="Input Text" class="form-control zone-text"/>'
					text +=	'<div class="btn-group"><button type="button" class="btn add-zone-text">+</button><button type="button" class="btn remove-zone-text btn">-</button></div>'
					text += '</section>'
				//set image to be multitext and file to be none
				$this.closest('.row').find('select.zone-file').val('none')
				$this.closest('.row').find('img').attr("data-src","holder.js/100%x150/industrial/text:Text Mode: Multitext")
					Holder.run({
						images: $this.closest('.row').find('img')[0]
					})		
			break;
		}
		
		$this.parent().append(text)
				
	})
	$(document).delegate('button.add-zone-text','click',function(e){
	
		$this = $(this)
		
		var _new = $this.parent().parent().clone()
		
		
		
		var name = $(_new).find('input').attr("name")
		//get last index num
		var lastOpenBracket = name.lastIndexOf("[");
		var lastClosedBracket = name.lastIndexOf("]")
		var diff = lastClosedBracket - lastOpenBracket
		var n = parseInt(name.substr(lastOpenBracket+1,diff-1))
		//console.log(n)
		
		
		var texts = $this.closest(".form-group").find('input.zone-text')
		//console.log(texts.length)
		
		var name_updated = name.substr(0,lastOpenBracket+1)+(texts.length)+"]"
		console.log(name_updated)
		
		
		$(_new).find('input').attr("name",name_updated).val("")
		
		$this.closest(".form-group").append(_new)
		
			
		
	})
	
	$(document).delegate('button.remove-zone-text','click',function(e){
		
		$this = $(this)
		input =  $this.parent().siblings('input')
		//alert(input.attr('name'))
		name = input.attr('name')
		lastOpenBracket = name.lastIndexOf("[");
		lastClosedBracket = name.lastIndexOf("]")
		diff = lastClosedBracket - lastOpenBracket
		index = parseInt(name.substr(lastOpenBracket+1,diff-1))
		
		//console.log("Current Index: "+index)
		
		//input.parent().remove()
		
		totalTexts = $this.closest(".form-group").find('input.zone-text')
		//console.log(totalTexts)
		
		for(var i = index+1; i< totalTexts.length; i++){ 
		
				//console.log(totalTexts[i])
				//console.log(i-1)
				var name_updated = name.substr(0,lastOpenBracket+1)+(i-1)+"]"
				$(totalTexts[i]).attr("name", name_updated)
		}
		
		input.parent().remove()
	})
	
	//toggle of map options
	
	//scene order button press
	$('button.scene-order').click(function(e){
		var $this = $(this);
		var direction;
	
		if($this.hasClass('scene-order-up')){
			direction = "up"
		}else{
			direction = "down"
		}
		var scene_id = $this.parent().siblings('button').data('scene-id')
		
		var form = $('<form action="/clips/reorder/'+scene_id+'" method="post"><input type="hidden" name="type" value="'+direction+'"></form>')
		$(form).submit();
	})
	
	$('select.clip-layout').change(function(e){
		
		//console.log($(this).val())
		currentClip.layout = $(this).val()
		formatClip(currentClip)
		
		
	})
	$(document).delegate('select.zone-file','change', function(e){
		$this =$(this)
		var file = _.findWhere(_files,{_id:$(this).val()})

		if(typeof file !=='undefined'){ 
			$(this).closest('.row').find('img').attr('src','/'+file.path).height("auto")
			//
			$(this).closest('.row').find('select.zone-map-type').val('none')
			$(this).closest('.row').find('select.zone-map-type').siblings('section.location').remove()
			$('canvas#map').remove()		
			}
		else{
			$(this).closest('.row').find('img').attr("data-src","holder.js/100%x150/industrial/text:No File Associated")
			Holder.run({
						images: $(this).closest('.row').find('img')[0]
					})
		}
		if($this.closest('.row').find('select.zone-text-type').val() === 'multiple'){
			$this.closest('.row').find('.zone-text').remove()
			$this.closest('.row').find('select.zone-text-type').val('none')
		}
	})

	/*
		formatClip(_clip)
		_clip =	The current clip to format 
	*/
	
	function formatClip(_clip){
		$('#clip-data').find('img').attr("src","/imgs/sculpture_"+_clip.layout+".jpg")
		console.log(_clip)
		$('#clip-contents form').attr('action', '/clips/'+_clip._id);
		$('h3.clip-title').html(_clip.title)
		$('input.clip-title').val(_clip.title)				
		$('select.clip-layout').val(_clip.layout);
		
		var clip = $('article.zone-single-0').clone()

		//clear all the old assets from the HTML
		$('article.zone-single').remove();
		//console.log(parseInt(_clip.layout));
		for(var i = 0; i < parseInt(_clip.layout)+1; i++){
			var title = 'title';
			switch(i){
				case 0:
					if(_clip.layout==0) title = "Sculpture and Walls"
					else title = "Sculpture"
				break;
				case 1:
					if(_clip.layout==2) title = "Left Wall"
					else title = "Walls"
				break;
				case 2:
					title = "Right Wall"
				break;
			}
			

			var newClip = $(clip).clone();
			$(newClip).removeClass('zone-single-0').addClass('zone-single-'+i);
					
			$(newClip).find('select.zone-file').attr('name','zones['+i+'][file]')
			$(newClip).find('select.zone-file option:selected').removeAttr('selected')
			
			$(newClip).find('select.zone-map-type').attr('name', 'zones['+i+'][map_type]')
			$(newClip).find('select.zone-map-type option:selected').removeAttr('selected')
			
			$(newClip).find('select.zone-text-type').attr('name','zones['+i+'][text_type]')
			$(newClip).find('select.zone-text-type option:selected').removeAttr('selected')
			
			
			$(newClip).find('input.zone-title').attr('value',title).attr('name','zones['+i+'][title]')
			
			//showing and hiding shit
			
			if(title === "Sculpture and Walls"){
				$(newClip).find("select.zone-map-type").parent().hide()
				$(newClip).find("select.zone-text-type").parent().hide()
				$(newClip).find("textarea.zone-caption").parent().hide()
			}else if(title==="Sculpture"){
				
				$(newClip).find("select.zone-map-type").parent().show()
				$(newClip).find("select.zone-text-type").parent().hide()
				$(newClip).find("textarea.zone-caption").parent().hide()
				
			}else{
				//dealing with an error or walls
				$(newClip).find("select.zone-map-type").parent().hide()
				$(newClip).find("select.zone-text-type").parent().show()
				$(newClip).find("textarea.zone-caption").parent().hide()
				
			}
			
			$('#zone-data').append(newClip);
		}
		
		if(_clip.zones.length>0){
			console.log(_clip.zones)
			_clip.zones.forEach(function(zone,index){
				if(zone.file){
					var file = _.findWhere(_files,{_id:zone.file})
					console.log(file)
					$('.zone-single-'+index).find('img').attr("src","/"+file.path)
					$('.zone-single-'+index).find('select.zone-file').val(zone.file)
				}else{
					$('.zone-single-'+index).find('img').attr("data-src","holder.js/100%x150/industrial/text:No File Associated")
					Holder.run({
								images: $('.zone-single-'+index).find('img')[0]
							})
					
				}
				if(zone.text){
					console.log(typeof zone.text)
					if(typeof zone.text !== 'object'){ 
						console.log('Text at zone: '+index)
						var text = '<section class="zone-text input-group"><textarea name="zones['+index+'][text]" placeholder="Enter your text now" class="form-control zone-text">'+zone.text+'</textarea></section>'
						$('.zone-single-'+index).find('.zone-text-type').parent().append(text)  
						$('.zone-single-'+index).find('.zone-text-type').val('single')
					
					}else{
						var text = '';
						for(var i = 0; i<zone.text.length; i++){
							    text  = '<section class="zone-text input-group"><input name="zones['+index+'][text]["'+i+'"]" value="'+zone.text[i]+'" class="form-control zone-text"/>'
								text +=	'<div class="btn-group"><button type="button" class="btn add-zone-text">+</button><button type="button" class="btn remove-zone-text btn">-</button></div>'
								text += '</section>'
								//console.log(text)
								$('.zone-single-'+index).find('.zone-text-type').parent().append(text)  

						}
						$('.zone-single-'+index).find('img').attr("data-src","holder.js/100%x150/industrial/text:Text Mode: Multitext")
						Holder.run({
							images: $('.zone-single-'+index).find('img')[0]
						})
						$('.zone-single-'+index).find('select.zone-text-type').val('multiple')
					}
				} 
				
				if(zone.locations){
					//console.log("Locations: "+zone.locations.length)
					var type = (zone.locations.length>1)? 'multiple' : 'single'
					//val = (zone.locations.length>1)? 'multiple' : 'single'
					$('.zone-single-'+index).find('select.zone-map-type').val(type)
					var output = type.charAt(0).toUpperCase()+type.slice(1)
					$('.zone-single-'+index).find('img').attr("data-src","holder.js/100%x150/industrial/text:Location Mode: "+output)
					Holder.run({
						images: $('.zone-single-'+index).find('img')[0]
					})
					
					if(!$('canvas#map').length){
				
						var canvas = '<canvas id="map" style="width: 100%; height: 200px;"> </canvas>'
						$('.zone-single-'+index).find('select.zone-map-type').parent().append(canvas)
						//init the map
					}
					
					for(var i = 0; i<zone.locations.length;i++){
						var input  = '<section style="display:none;" class="location '+zone.locations[i].x+' '+zone.locations[i].y+'" >'
							input += '<input type="hidden" value="'+zone.locations[i].x+'" name="zones[0][locations]['+i+'][x]">'
							input += '<input type="hidden" value="'+zone.locations[i].y+'" name="zones[0][locations]['+i+'][y]"/> </section>';
							
							$('.zone-single-'+index).find('select.zone-map-type').parent().append(input)
					}
					
					var locs = zone.locations;
					
					InitMapCanvas(type, locs, mapCallback); //see mapCallback defined below
				}
			})
		}
	}
	
	
	function mapCallback(locPos, eventType){
								//alert(mousePos.x+","+mousePos.y)
		var length = $('section.location').length
		
		switch(eventType){
			case 0:
				//**** hardcoding the index to '0', assuming that sculpture will always be at index 0 ****//
/* 				console.log($('.zone-single-0').find('section.location.'+locPos.x+'.'+locPos.y)); */
				$('.zone-single-0').find('section.location.'+locPos.x+'.'+locPos.y).remove();						
			break;
			
			case 1:
				//alert("adding: "+locPos.x+","+locPos.y+"  length: "+length);
				var input  = '<section style="display:none;" class="location '+locPos.x+' '+locPos.y+'" >'
				input += '<input type="hidden" value="'+locPos.x+'" name="zones[0][locations]['+length+'][x]">'
				input += '<input type="hidden" value="'+locPos.y+'" name="zones[0][locations]['+length+'][y]"/> </section>';
 				//$('.zone-single-'+index).find('select.zone-map-type').parent().append(input)
				$('.zone-single-0').find('select.zone-map-type').parent().append(input)
			break;
		}
	}
	/*
	**
	** orderAssets()
	**
	*
	
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
	
	/* Out to pasture....
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
					$(newAsset).find('select.asset-zone-type').html("");
					if(title == "Sculpture") {
						$(newAsset).find('select.asset-zone-type').html("<option value=\"\">None</option><option value=\"Multi Location\">Multi Location</option><option value=\"Single Location\">Single Location</option>");
						}else if(title=="Single Wall"){
						
						$(newAsset).find('select.asset-zone-type').html("<option value=\"\">None</option><option value=\"Multi Text\">Multi Text</option>")	
							
						}
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
						console.log(currentScene.assets[i][key].file)
						//console.log($('.asset-single-'+key).find('select.asset-file option[value="'+currentScene.assets[i][key].file._id+'"]'))	
						$('.asset-single-'+key).find('select.asset-file option[value="'+currentScene.assets[i][key].file._id+'"]').attr('selected','selected');
						if(currentScene.assets[i][key].file != "") $('.asset-single-'+key).find('.thumbnail img').attr("src","/"+currentScene.assets[i][key].file.path)
						else{ 
							$('.asset-single-'+key).find('.thumbnail img').attr("data-src","holder.js/100%x150/text:No%20File%20Associated")
							var img = $('.asset-single-'+key).find('.thumbnail img');
							console.log(img)
							Holder.run({
								images: img[0]
							})
						}
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
				$(newAsset).find('.thumbnail img').attr("data-src","holder.js/100%x150/text:Select%20File")
				var img = $('.asset-single-'+key).find('.thumbnail img');
				console.log(img)
				Holder.run({
					images: img[0]
				})
				//reset for array values
				$(newAsset).find('.thumbnail').show()
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
				$(newAsset).find('.thumbnail img').attr("src","");
				$(newAsset).find('.thumbnail').hide();
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
	}) */
}
