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
		//Submitting a form from the footer of a modal using bootstrap
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
		
	/*
	** Event Actions
	*/
	
	
	$("button.event-view").click(function(e){
		var link = $(this).parents('form').attr('action');
		//console.log(link);
		//find form slug direct page
		window.location.replace(link);
	});
	$("button.event-update").click(function(e){
		e.preventDefault();
		$(this).parents('form').submit();
	});
	$("button.event-delete").click(function(e){
	
		if(confirm("You are about to Delete this Event, Proceed?")){
		
			var path = $(this).parents('form').attr('action')+"/delete";
			$(this).parents('form').attr('action',path);
			$(this).parents('form').submit();
			
		}else{
			e.preventDefault();
		}
	});
});

