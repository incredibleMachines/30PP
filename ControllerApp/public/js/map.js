function InitMapCanvas(_type,_cb){ 

	var type = _type; //single or multi
	
	var canvas = document.getElementById("map");
	var ctx = canvas.getContext("2d");
	
	var bgImage = new Image();
	bgImage.src = "/imgs/map.png";
	
	var locationChosen = false;
	var locationPos = {"x":0, "y":0};
	
	
	
	//  --- draw function to run on any click 
	//  	if location has not been chosen, set location.
	function drawClick(canvas, message, mousePos) {
		
		ctx.drawImage(bgImage, 0, 0);
		
		if(!locationChosen){
			locationPos.x = mousePos.x;
			locationPos.y = mousePos.y;
			drawDebug(canvas, message);
			locationChosen = true;
			//console.log("LOCATION CHOSEN == FALSE")
		} else if (locationChosen) {
			locationChosen = false;
			//console.log("LOCATION CHOSEN == TRUE")
		}
		drawLocationMarker();
	}
	
	// -- draw location marker
	//	  TODO: make this an image of some sort
	function drawLocationMarker(){
		if(locationChosen){
			ctx.fillStyle="#aa14fa";
			ctx.beginPath();
			ctx.arc(locationPos.x, locationPos.y, 5, 0, 2*Math.PI);
			ctx.stroke();
			ctx.fill();
		} else {
			ctx.fillStyle = "#ffffff"
			ctx.fillRect(30,55,220,30);
			ctx.font = '12pt Calibri';
			ctx.fillStyle = 'black';
			ctx.fillText("click to choose location", 60, 75);
		}
	}
	
	
	// -- draw debug info on the map
	//    TODO: remove.
	function drawDebug(canvas, message) {
		//ctx.clearRect(0, 0, canvas.width, canvas.height);
		ctx.font = '12pt Calibri';
		ctx.fillStyle = 'black';
		ctx.fillText(message, 10, 25);
	}
	
	// -- get mousePosition function
	function getMousePos(canvas, evt) {
		var rect = canvas.getBoundingClientRect();
		return {
		  x: evt.clientX - rect.left,
		  y: evt.clientY - rect.top
		};
	}
	
	// -- onload of bg image
	bgImage.onload = function(){
	
		ctx.drawImage(bgImage, 0, 0);
		
		drawLocationMarker();
		
		// -- mouse click event listener
		canvas.addEventListener('click', function(evt) {
			var mousePos = getMousePos(canvas, evt);
			var message = 'Loc: ' + Math.floor(mousePos.x) + ', ' + Math.floor(mousePos.y);
			drawClick(canvas, message, mousePos);
			_cb(mousePos)
			}, false);
		
	/*  -- mousemove event listener
		canvas.addEventListener('mousemove', function(evt) {
			var mousePos = getMousePos(canvas, evt);
			var message = 'Mouse: ' + Math.floor(mousePos.x) + ', ' + Math.floor(mousePos.y);
			drawDebug(canvas, message);
			}, false);
	*/
	}
}


