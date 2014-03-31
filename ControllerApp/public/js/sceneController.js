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
		/* this is where we could check for multiple hashes in the URL 
			for example #new-clip#file-532b4e82efd9bea254a6ef75
		*/
		var slug = window.location.hash.substr(1)
		var clip = _.findWhere(_clips, {slug: slug});
		//var clip _.findWhere( _clips, {slug:slug} ) 

		if(typeof clip !== 'undefined') currentClip = clip

	}
	
	formatClip(currentClip)
	
	clipNav()
	firstOn(currentClip.slug)
		
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
					$this.closest('.zone-single').find('img').attr("data-src","holder.js/100%x150/industrial/text:No File Associated")
					Holder.run({
						images: $this.closest('.zone-single').find('img')[0]
					})				
		}else{
			//check if element exists using length
			if(!$('canvas#map').length){
				
				var canvas = '<canvas id="map" style="width: 100%; height: 200px;"> </canvas>'
				$this.parent().parent().append(canvas)
				//init the map
			}
			
			$this.closest('.zone-single').find('select.zone-file').val('none')
			var val = $this.val();

			var type = val.charAt(0).toUpperCase()+val.slice(1)
			$this.closest('.zone-single').find('img').attr("data-src","holder.js/100%x150/industrial/text:Location Mode: "+type)
			//$this.closest('.row').find('img').attr('src','holder.js/100%x150/industrial/text:'+$this.val()+' location')
			Holder.run({
						images: $this.closest('.zone-single').find('img')[0]
					})
					
			var locs;
			
			/* changed to raw val, was type */
			InitMapCanvas(val, locs, mapCallback); //see mapCallback defined below
		}
	})
	
	
	//toggle of text options
	$(document).delegate('select.zone-text-type','change', function(e){
			
		var $this = $(this)
				
		var classes = $this.closest('.zone-single').attr('class')
		//alert(classes)
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
				$this.closest('.zone-single').find('select.zone-file').val('none')
				$this.closest('.zone-single').find('img').attr("data-src","holder.js/100%x150/industrial/text:Text Mode: Multitext")
					Holder.run({
						images: $this.closest('.zone-single').find('img')[0]
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
			if(file.type.indexOf('video')>=0){
				$(this).closest('.zone-single').find('img').attr("data-src","holder.js/100%x150/industrial/text:No Video Preview")
				Holder.run({
					images:$(this).closest('.zone-single').find('img')[0]
				})

			}else{
			$(this).closest('.zone-single').find('img').attr('src','/'+file.path).height("auto")
			}
			$(this).closest('.zone-single').find('select.zone-map-type').val('none')
			$(this).closest('.zone-single').find('select.zone-map-type').siblings('section.location').remove()
			
			if($(this).closest('.zone-single').hasClass('zone-single-0')) $('canvas#map').remove()
			//$('canvas#map').remove()		
			}
		else{
			$(this).closest('.zone-single').find('img').attr("data-src","holder.js/100%x150/industrial/text:No File Associated")
			Holder.run({
						images: $(this).closest('.zone-single').find('img')[0]
					})
		}
		if($this.closest('.zone-single').find('select.zone-text-type').val() === 'multiple'){
			$this.closest('.zone-single').find('.zone-text').remove()
			$this.closest('.zone-single').find('select.zone-text-type').val('none')
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
			$(newClip).find('section.location').remove()
					
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
					if(file.type.indexOf('video')>=0){
						$('.zone-single-'+index).find('img').attr("data-src","holder.js/100%x150/industrial/text:No Video Preview")
						Holder.run({
								images: $('.zone-single-'+index).find('img')[0]
							})
					}else{
						$('.zone-single-'+index).find('img').attr("src","/"+file.path).height("auto")
					}
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
					console.log("Locations: "+zone.locations.length)
					var type = (zone.locations.length>1)? 'multiple' : 'single'
					console.log("type: "+type);
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
	
		
		var current = $('select.zone-map-type').val()
		console.log("eventType: "+eventType+" -- locPos.x: "+locPos.x);
		if(current === 'single') $('section.location').remove()
		var length = $('section.location').length;
		switch(eventType){
			case 0: //remove location
				//**** hardcoding the index to '0', assuming that sculpture will always be at index 0 ****//
/* 				console.log($('.zone-single-0').find('section.location.'+locPos.x+'.'+locPos.y)); */
				//console.log("REMOVING LOC FROM FORM");
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
	
	/* By Jenn: For making selected clip name in scene sub-nav turn black when active */
	function clipNav() {
		$('.sub-nav').click(function() {
			$(this).closest('ul.list-group').find('.active').removeClass("active");
			$(this).closest('.list-group-item').addClass( "active");
		});
		
	}
	
	/* By Jenn: For making 1st sub-nav item be active on page load */
	function firstOn(slug) {
		console.log(slug)
		$('button.'+slug).closest('.list-group-item').addClass("active");
	}
	
}
