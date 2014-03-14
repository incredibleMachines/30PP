//Need to ADD IO Connection to PlayerApp

$(document).ready(function(){

	
	if($('select.new-event-type').length){
	
		$('select.new-event-type').val(GetURLParameter("type"))
	
	}

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
	** Scene Actions
	*/
	
	
	$("button.scene-view").click(function(e){
		var link = $(this).parents('form').attr('action');
		//console.log(link);
		//find form slug direct page
		window.location.replace(link);
	});
	$("button.scene-update").click(function(e){
		e.preventDefault();
		$(this).parents('form').submit();
	});
	$("button.scene-delete").click(function(e){
		
		if(confirm("You are about to Delete this Scene, Proceed?")){
		
			var path = $(this).parents('form').attr('action')+"/delete";
			$(this).parents('form').attr('action',path);
			$(this).parents('form').submit();
			
		}
			e.preventDefault();
		
	});
	
	/*
	** Event Single Actions
	*/
	
	$("button.clip-delete").click(function(e){
	
		var path = $(this).parents('form').attr('action')+"/delete";		
		
		if(confirm("You are about to Delete this Scene, Proceed?")){
	
			$(this).parents('form').attr('action',path);
			$(this).parents('form').submit();

		}
		//console.log(path);
	
	});
	
	/*
	** Render Queue
	*/
	
	
	$("button.render-content").click(function(e){
		//simple form post, could be implimented via AJAX and facilitate page more robust page functionality.
		var form = $('<form action="/render" method="post"></form>')
		
		$(form).submit();
		
	})
	
	/* 
	** File Actions	
	*/
	$("button.file-update").click(function(e){
		e.preventDefault();
		$(this).parents('form').submit();
	});
	$("button.file-delete").click(function(e){
	
		if(confirm("You are about to Delete this File, Proceed?")){
		
			var path = $(this).parents('form').attr('action')+"/delete";
			$(this).parents('form').attr('action',path);
			$(this).parents('form').submit();
			
		}else{
			e.preventDefault();
		}
	});


	
	
});



/*
** GetURLParameter
** http://www.jquerybyexample.net/2012/06/get-url-parameters-using-jquery.html
**
*/
function GetURLParameter(sParam){
    var sPageURL = window.location.search.substring(1);
    var sURLVariables = sPageURL.split('&');
    for (var i = 0; i < sURLVariables.length; i++){
        var sParameterName = sURLVariables[i].split('=');
        if (sParameterName[0] == sParam){
            return sParameterName[1];
        }
    }
}
