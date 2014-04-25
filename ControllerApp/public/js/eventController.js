function EventController(){

	$('button.scene-order').click(function(e){
		var $this = $(this);
		var direction;

		if($this.hasClass('scene-order-up')){
			direction = "up"
		}else{
			direction = "down"
		}
		var scene_id = $this.closest('.row').data('id')
		//alert(scene_id)
		var form = $('<form action="/scenes/reorder/'+scene_id+'" method="post"><input type="hidden" name="type" value="'+direction+'"></form>')
		//console.log(form)
		$(form).submit();
	})

}
