function FileController(){
	
	$('button.file-replace').click(function(e){
		var $this = $(this)
		
		$this.siblings('input').show()
		$this.hide();
		
	})

}