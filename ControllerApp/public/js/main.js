//Need to ADD IO Connection to PlayerApp

$(document).ready(function(){

	/*
	**
	** Form controls
	** 
	*/
	
	$('input.duration').change(function(e){
		$(this).siblings('.duration_view').val($(this).val()+" seconds")
	});
	
	/*
	** Modals 
	*/
		
		//submit modal
		
		$("button.modal-submit").click(function(e){
			
			//traverse dom tree to modal body (which is our FORM html Element) to sumbit
			$(this).parent().siblings(".modal-body").submit();
						
		})
		
	/*
	** New Event
	*/
	
	/*
	** New Asset	
	*/
		//new asset
		//add current event ID into the new asset modal form so we have it.
		
		$("button.new-assets").click(function(e){
			var event = $(this).data('event_id');
			
			//the semantic location of our hidden input for the event_id
			$('.new_asset .modal-dialog .modal-content form.modal-body input#event_id').val(event)
			
		})
		
		//control for asset type functionality
		
		$("select#asset_type").change(function(e){
			if($(this).val() == 'text'){
				$(this).parent().siblings(".content-upload").hide();
				$(this).parent().siblings(".content-text").show();
			}else{
				$(this).parent().siblings(".content-upload").show();
				$(this).parent().siblings(".content-text").hide();
			}
		});
	
});

