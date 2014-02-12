/* A function to manipulate scenes on the front-end. 

  _scenes = {OBJ} Takes a JSON Object of _scenes directly from scene render.
  
  ** See views/includes/footer.ejs  **

*/

function SceneController(_scenes){
	
	var currentScene = _scenes[0];
	//console.log(_scenes);
	
	//zone_type modifications and ui changes
	
	$('select#zone_type').change(function(e){
		
		createAssets($(this).val());
		
	}); //#End Zone Type Selection options
	
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
				//alert(id);
				//repopulate the content
				//change form slug
				console.log(scene)
				console.log()
				console.log(currentScene)

				$('#scene-contents	 form').attr('action', '/scenes/'+currentScene._id);
				//console.log('.scene-title.'+currentScene.slug)
				//console.log()
				
				//console.log($('.scene-view .scene-title'));
				$('.scene-view .scene-title').each(function(i){
					$(this).val(currentScene.title);
					$(this).html(currentScene.title)
					
				});
				
				$('.scene-view .scene-zone_type').val(currentScene.zone_type);
				$('.scene-view .scene-scene_type').val(currentScene.scene_type);
				$('.scene-view .scene-text').val(currentScene.text);
				
				//setup the correct zones to go with the scene we are viewing
				createAssets(currentScene.zone_type);
				
			
			}
		})
		
	});

	/*
	**
	** createAssets(_val) 
	** _val = string, a preset value determined by the system currently 'zone_0', 'zone_1','zone_2','zone_3' (subject to updates)
	** 
	*/
	
	
	function createAssets(_val){
		
			//TO DO Populate all the assets!
			console.log(currentScene.assets.length)
			//clone our original asset (store it in the Browsers Memory)
			var asset = $('article.asset-single-0').clone();
			
			//clear all the old assets from the HTML
			$('article.asset-single').remove();
			
			var assetNum;
			
			//assign a number value to our zones - note this is currently hard coded and will be updated
			switch(_val){
				//This is will have to change once we know what the fucks we need here.
				case '0': assetNum = 1; break;
				case '1': assetNum = 2; break;
				case '2': assetNum = 3; break;
				case '3': assetNum = 4; break; 
			}
			//console.log(currentScene.assets.length +" "+assetNum)
			//clear all the old single assets from the column
			$('article.asset-single').remove();
			
			//
			
			for(var i = 0; i< assetNum; i++){
				//copy our model asset for the number of assets we have
				var newAsset = $(asset).clone();
				//console.log(currentScene.assets[i])
				//console.log(i+" "+currentScene.assets.indexOf(i));
				//update the class structure
				$(newAsset).removeClass('asset-single-0').addClass('asset-single-'+i);
				
				$(newAsset).find('select.asset-file').attr('name','assets['+i+'][file]')
				$(newAsset).find('select.asset-file option:selected').removeAttr('selected');

				$(newAsset).find('textarea.asset-caption').attr('name','assets['+i+'][caption]').html('');
				$(newAsset).find('select.asset-zone').attr('name','assets['+i+'][zone]')
				//clear out the asset-zone selections
				$(newAsset).find('select.asset-zone').html('')
				
				//plug in our asset data based off the number of total assets
				//if the asset # matches the zone # make that option be default selected
				for(var j = 0; j < assetNum; j++){
					//console.log(j)
					if(j == i) $(newAsset).find('select.asset-zone').append('<option value="'+j+'" selected>Zone '+(j+1)+'</option>');
					else $(newAsset).find('select.asset-zone').append('<option value="'+j+'" >Zone '+(j+1)+'</option>');
				}
				
				if(typeof currentScene.assets[i] !== 'undefined'){ //check if we have a corresponding asset
					console.log(i+" :: "+JSON.stringify(currentScene.assets[i]));
  					//iterate through the asset
					Object.keys(currentScene.assets[i]).forEach(function(key){
						//make sure that our index's match
						if(parseInt(key)==i){
							$(newAsset).find('select.asset-file option[value="'+currentScene.assets[i][key].file+'"]').attr('selected','selected');
							$(newAsset).find('textarea.asset-caption').html(currentScene.assets[i][key].caption);			
	
						}
					})
/*
					Object.keys(currentScene.assets[i]).forEach(function(key){
						$(newAsset).find('select.asset-file option[value="'+currentScene.assets[i][key]+'"]').attr('selected','selected');
					})
*/
					
					//$(newAsset).find('select.asset-file').val()

				}else{
					console.log(i+' undefined ')
				}
				
				//append the data into our holder
				$('#asset-data').append(newAsset);	
			}
		
	}


}

