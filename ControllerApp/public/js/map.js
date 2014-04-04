
function InitMapCanvas(_type, _locs, _cb){ 
	var type = _type; //single or multi
	
	var canvas = document.getElementById("map");
	var ctx = canvas.getContext("2d");
	//make ajax call for matrix
	var matrix = null;
	
	$.getJSON('/location/matrix',function(data){
		console.log('got matrix')
		//console.log(data)
		matrix = data
		//TODO: make this dynamic
		canvas.width  = matrix[0].length //512;
		canvas.height = matrix.length //202;
		
		console.log("30_PP")
		console.log(matrix[116][351])
	})
	

	

	
	var bgImage = new Image();
	bgImage.src = "/imgs/ManhattanStreets_512_202_raster.png";
	
	var locs = _locs;
	
	var locDiameter = 8; // size of circles
	var locColorOn = "#0000FF";
	
	var selectionId; // index for selecting locations
	var clickType = 1; // 0=delete  ,  1=add location
	
	var debugMsg; //holds debug mouse info
	
	//  --- draw locations on load
	function drawLocations(x,y){

		ctx.drawImage(bgImage, 0, 0);
		ctx.fillStyle = locColorOn;
		
		for(var i=0; i<locs.length; i++){
			//console.log(locs[i])
			drawSingleLoc(locs[i].x,locs[i].y,locs[i].color)
			
			/*//draw coordinates as well
 			ctx.font = '12pt Calibri'; 
 			ctx.fillStyle = 'black'; 
 			ctx.fillText(x+","+y, x+5, y+5); */
		}
		if(x&&y) drawSingleLoc(x,y,"#000000")
	}
	
	function drawSingleLoc(x,y,color){
			ctx.fillStyle = color;
			ctx.beginPath();
			ctx.arc(x, y, 5, 0, 2*Math.PI);
			ctx.stroke();
			ctx.fill();
	}
	
	function drawInstruction(){
		ctx.drawImage(bgImage, 0, 0);
		ctx.font = '12pt Calibri';
		ctx.fillStyle = 'black';
		ctx.fillText("click to choose location", 33, 105);
		//ctx.fillText(type+" location", 57, 100);
	}
	
	
	
	//--- process any click on the canvas
	function processClick(mousePos){
		
				if(typeof locs === 'undefined') locs = new Array();
				switch (clickType){
					case 0: //removing a location
						var thisLoc = {"x":locs[selectionId].x, "y":locs[selectionId].y};
						_cb(thisLoc, clickType);
						locs.splice(selectionId, 1);
		
						//console.log("REMOVE LOC");
					break;
					
					case 1: //adding a location
						if(matrix){
							if(matrix[parseInt(mousePos.y-1)][parseInt(mousePos.x-1)] == 0){ 
								var thisLoc = {"x":mousePos.x, "y":mousePos.y};
								_cb(thisLoc, clickType);
								locs.push({"x":parseInt(mousePos.x), "y":parseInt(mousePos.y), "color":locColorOn});
								if(locs.length>1 && type === 'single'){
									//console.log("map.js: single locs.shift");
									locs.shift()
								} 
							}//if(matrix)
						}else alert("No Matrix")
						//console.log("ADD LOC");
					break;	
				}
			
		drawLocations(null,null);			
	}
	
	
	//--- process hovering of mouse on canvas
	function checkHover(mousePos){
		var X = null,
			Y = null;
		if(matrix){
			if(mousePos.y-1>0&&mousePos.y-1<canvas.height && mousePos.x-1>0 && mousePos.x-1<canvas.width){
				if(matrix[parseInt(mousePos.y-1)][parseInt(mousePos.x-1)]==0) {
					X = parseInt(mousePos.x-1)
					Y = parseInt(mousePos.y-1)
				}
			}
		}
		if(typeof locs !== 'undefined'){ 
			for(var i=0; i<locs.length; i++){
				if( parseInt(mousePos.x) <= parseInt(locs[i].x)+locDiameter+2 && 
					parseInt(mousePos.x) >= parseInt(locs[i].x)-locDiameter-2 &&
					parseInt(mousePos.y) <= parseInt(locs[i].y)+locDiameter+2 && 
					parseInt(mousePos.y) >= parseInt(locs[i].y)-locDiameter-2 ){
					
					clickType = 0;
					selectionId = i;
					locs[i].color = "#FF0000";
					
					drawLocations(X,Y);
					
					ctx.font = '12pt Calibri';
					ctx.fillStyle = 'black';
					ctx.fillText("remove", parseInt(locs[i].x)-24, parseInt(locs[i].y)-8);
					//console.log("type REMOVE");
					break;

				} else {
					clickType = 1;
					locs[i].color = locColorOn;

					drawLocations(X,Y);
					//console.log("type ADD");
				}
			}
		} 
		
		if(locs.length < 1){
			clickType = 1;
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
		//console.log(load)
		if(typeof locs !== 'undefined'){ //if we received locations previously stored
			console.log("numLocs: "+ locs.length);
			drawLocations(null,null);
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


