
function InitMapCanvas(_type, _locs, _cb){
	var type = _type; //single or multi

	var canvas = document.getElementById("map");
	var ctx = canvas.getContext("2d");
	// make ajax call for matrix
	var matrix = null;
  var path = null;

	$.getJSON('/location/matrix',function(data){

		//console.log('got matrix')
		//console.log(data)
		matrix = data
		//TODO: make this dynamic
		canvas.width  = matrix[0].length //512;
		canvas.height = matrix.length //202;

		//console.log("30_PP")
		//console.log("y:123 x:363")
		//console.log(matrix[123][363])

		if(typeof locs !== 'undefined'){ //if we received locations previously stored
			//console.log("numLocs: "+ locs.length);
			ctx.drawImage(bgImage, 0, 0);
			drawLocations(null,null);
			locationChosen = true;
		} else {
			drawInstruction(null,null);
		}
	})

/*  var grid = Class.New("Grid", [matrix]);
	grid.pathfinding(0, 0, 2, 4);

	// returns : [[0, 0], [1, 0], [2, 0], [2, 1], [2, 2], [2, 3], [2, 4]]

	grid.setPropertyByCell(0, 1, 1);
	grid.pathfinding(0, 0, 2, 4);*/

	var bgImage = new Image();
	//bgImage.src = "/imgs/ManhattanStreets_512_202_raster.png";
	bgImage.src = "/imgs/ManhattanStreets_512_202_pretty.png";
	var locs = _locs;

	var locDiameter = 8; // size of circles
	var locColorOn = "#0000FF";

	var selectionId; 	// index for selecting locations
	var clickType = 1;  // 0=delete  ,  1=add location

	var debugMsg; //holds debug mouse info

	//  --- draw locations on load
	function drawLocations(x,y){
		//******** COMMENTED OUT FOR TESTING ******//
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
		drawPFinderTest(path);
	}

	function drawSingleLoc(x,y,color){
		ctx.fillStyle = color;
		ctx.beginPath();
		ctx.arc(x, y, 5, 0, 2*Math.PI);
		ctx.stroke();
		ctx.fill();
	}

	function drawInstruction(x,y){
		ctx.drawImage(bgImage, 0, 0);
		ctx.font = '12pt Calibri';
		ctx.fillStyle = 'white';
		ctx.fillText("click to choose location", 33, 105);
		//ctx.fillText(type+" location", 57, 100);
		if(x&&y) drawSingleLoc(x,y,"#000000")
		drawPFinderTest(path);
	}



	//--- process any click on the canvas
	function processClick(mousePos){

		if(typeof locs === 'undefined') locs = new Array();
		switch (clickType){
			case 0: //removing a location
				var thisLoc = {"x":locs[selectionId].x, "y":locs[selectionId].y};
				_cb(thisLoc, clickType);
				locs.splice(selectionId, 1);
				path = null;
				//console.log("REMOVE LOC");
			break;

			case 1: //adding a location
				if(matrix){
				//console.log(matrix[parseInt(mousePos.y-1)][parseInt(mousePos.x-1)])
					if(matrix[parseInt(mousePos.y-1)][parseInt(mousePos.x-1)] == 0){

						var thisLoc = {"x":mousePos.x-1, "y":mousePos.y-1};

						var testLoc = {"x":mousePos.x-1, "y":mousePos.y-1};

						//console.log("testLoc: ");
						//console.log(testLoc);
						/* FOR TESTING */
						//pass this loc out as a POST
						$.ajax({
							url:'/location/pathTest',
							type: 'POST',
							data: testLoc,
							success: function(result){
									//for(var i=0; i<result.length; i++) console.log(result[i][0]+","+result[i][1]);
									if(result.length<1){
										//console.log("EMPTY ARRAY RETURNED FROM PFINDER");
									}else{
										//console.log("length: ");
										//console.log(result.length);
										path = result;
										drawPFinderTest(path)
									}
							},
							error: function( jqXHR, textStatus, errorThrown ){
									//console.log(jqXHR);
							}
						});
						/**************/

						_cb(thisLoc, clickType);
						locs.push({"x":parseInt(mousePos.x-1),
							"y":parseInt(mousePos.y-1),
							"color":locColorOn});
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

	function drawPFinderTest(locs){
		if(locs){
			for(var i=0; i<locs.length; i++){
				x = locs[i][0];
				y = locs[i][1];
				ctx.fillStyle = "rgb(200,0,0)";
				ctx.fillRect(x,y,3,3);
			}
		}
	}


	//--- process hovering of mouse on canvas
	function checkHover(mousePos){
		var X = null,
			Y = null;
		if(matrix){
			if(mousePos.y-1 > 0
				&&mousePos.y-1 < canvas.height
				&& mousePos.x-1 > 0
				&& mousePos.x-1 < canvas.width){
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
			drawInstruction(X,Y);
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
