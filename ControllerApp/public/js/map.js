
function InitMapCanvas(_type, _locs, _cb){ 

	var type = _type; //single or multi
	
	var canvas = document.getElementById("map");
	var ctx = canvas.getContext("2d");
	
	//TODO: make this dynamic
	canvas.width = 273;
	canvas.height = 200;
	
	var bgImage = new Image();
	bgImage.src = "/imgs/map.png";
	
	var locs = _locs;
	
	var locDiameter = 8; // size of circles
	var locColor = "#0000FF";
	
	var selectionId; // index for selecting locations
	var clickType = 1; // 0=delete  ,  1=add location
	
	var debugMsg; //holds debug mouse info
	
	
	//  --- draw locations on load
	function drawLocations(){

		ctx.drawImage(bgImage, 0, 0);
		ctx.fillStyle = "#0000FF"
		
		for(var i=0; i<locs.length; i++){
			ctx.fillStyle = locs[i].color;
			
			var x=parseInt(locs[i].x);
			var y=parseInt(locs[i].y);

			ctx.beginPath();
			ctx.arc(x, y, 5, 0, 2*Math.PI);
			ctx.stroke();
			ctx.fill();
			
			//draw coordinates as well
/* 			ctx.font = '12pt Calibri'; */
/* 			ctx.fillStyle = 'black'; */
/* 			ctx.fillText(x+","+y, x+5, y+5); */
		}
	}
	
	function drawInstruction(){
		ctx.drawImage(bgImage, 0, 0);
		ctx.font = '12pt Calibri';
		ctx.fillStyle = 'black';
		ctx.fillText("click to choose "+type+" location", 30, 100);
	}
	
	
	
	//--- process any click on the canvas
	function processClick(mousePos){
		if(typeof locs === 'undefined') locs = new Array();
		switch (clickType){
			case 0:
				var thisLoc = {"x":locs[selectionId].x, "y":locs[selectionId].y};
				_cb(thisLoc, clickType);
				locs.splice(selectionId, 1);
				//console.log("REMOVE LOC");
			break;
			
			case 1:
				var thisLoc = {"x":mousePos.x, "y":mousePos.y};
				_cb(thisLoc, clickType);
				locs.push({"x":parseInt(mousePos.x), "y":parseInt(mousePos.y), "color":"#0000FF"});
				//console.log("ADD LOC");
			break;	
		}
		
		drawLocations();			
	}
	
	
	//--- process hovering of mouse on canvas
	function checkHover(mousePos){

		if(typeof locs !== 'undefined'){ 
			for(var i=0; i<locs.length; i++){
				if( parseInt(mousePos.x) <= parseInt(locs[i].x)+locDiameter+3 && 
					parseInt(mousePos.x) >= parseInt(locs[i].x)-locDiameter-3 &&
					parseInt(mousePos.y) <= parseInt(locs[i].y)+locDiameter+3 && 
					parseInt(mousePos.y) >= parseInt(locs[i].y)-locDiameter-3 ){
					
					clickType = 0;
					selectionId = i;
					locs[i].color = "#FF0000";
					
					drawLocations();
					
					ctx.font = '12pt Calibri';
					ctx.fillStyle = 'black';
					ctx.fillText("remove", parseInt(locs[i].x)-24, parseInt(locs[i].y)-8);
					//console.log("type REMOVE");
					break;

				} else {
					clickType = 1;
					locs[i].color = "#0000FF";

					drawLocations();
					//console.log("type ADD");
				}
			}
		} 
		
		if(locs.length < 1){
			clickType = 1;
			//console.log("hover no locs found");
			drawInstruction();
		} 
	}

	
	// -- get mousePosition function
	function getMousePos(canvas, evt) {
		var rect = canvas.getBoundingClientRect();
		return {
		  x: Math.floor(evt.clientX - rect.left),
		  y: Math.floor(evt.clientY - rect.top)
		};
	}
	
	
	// -- onload of bg image
	bgImage.onload = function(){
	
		ctx.drawImage(bgImage, 0, 0);
		
		if(typeof locs !== 'undefined'){ //if we received locations previously stored
			console.log("numLocs: "+ locs.length);
			drawLocations();
			locationChosen = true;	
		} else {
			drawInstruction();
		}
		
		// -- mouse click event listener
		canvas.addEventListener('click', function(evt) {
			var mousePos = getMousePos(canvas, evt);
			debugMsg = 'Loc: ' + mousePos.x + ', ' + mousePos.y;
			processClick(mousePos);
			//_cb(mousePos, clickType); //clickType 0=delete, 1=add
			}, false);
		
	   // mousemove event listener
		canvas.addEventListener('mousemove', function(evt) {
			var mousePos = getMousePos(canvas, evt);
			debugMsg = 'Loc: ' + mousePos.x + ', ' + mousePos.y;
			if(typeof locs !== 'undefined') checkHover(mousePos);
			}, false);
			
	} //end bgImage.onLoad
}


