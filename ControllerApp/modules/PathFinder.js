var 		fs = require('fs'),
				PNG = require('pngjs').PNG,
				PathFinding = require('pathfinding'),
				_ = require('underscore')

//pathfinding matrix creation threshold
var threshold = 250;

var matrix;
var grid;
exports.ready = false;


var intersections;

var finder = new PathFinding.AStarFinder({
	allowDiagonal: true,
	dontCrossCorners: false
});


exports.setup =function(cb){
	//load raster  grayscale png convert
	fs.createReadStream(__dirname+'/../public/imgs/ManhattanStreets_512_202_smooth.png').pipe( new PNG({filterType:4}) ).on('parsed',function(){

		//create an array the size of our image
		matrix = new Array(this.height);

		//make it nested
		for(var i = 0; i<this.height; i++){
			var t = new Array(this.width)
			matrix[i]=t
		}

		for(var i = 0; i<this.height; i++){
			for(var j = 0; j<this.width;j++){
				var idx = (this.width*i+j)<<2
				//populate our matrix with our grayscale pixel data
				matrix[i][j] = ( (this.data[idx]+this.data[idx+1]+this.data[idx+2])/3 < threshold )? 0 : 1;

			}
		}
		grid = new PathFinding.Grid(this.width,this.height,matrix)


		fs.createReadStream(__dirname+'/../public/imgs/map_pixel_coordinates_20140410.png').pipe(new PNG({filterType:4})).on('parsed',function(){

			intersections = new Array()

			for(var i =0; i<this.height; i++){
				for(var j =0; j<this.width; j++){
					var idx = (this.width*i+j)<<2
					if(this.data[idx]+this.data[idx+1]+this.data[idx+2]/3<threshold) intersections.push([j,i])
				}
			}

			exports.ready = true;
			cb()

		})


		//matrix is now a 2 dimensional array of a black and white image
		//0/black is walkable
		//255/white is coverted to 1 and is blocked
		//console.log(JSON.stringify(matrix))
		//cb()

	})

}
exports.returnMatrix = function(){
	return matrix
}
exports.returnPath = function(endPoint){
	//copy grid
	//make new
	//console.log(JSON.stringify(matrix))
	var newGrid = grid.clone()
	var smoothGrid = grid.clone()
	//need to preset 1,2 to be the location of 30PP


	//return finder.findPath(351,116 , endPoint.x, endPoint.y, newGrid)
	//cb(path)
	//console.log('PATHMAKING')
	var tempFinder = finder.findPath(363,123,endPoint.x,endPoint.y,newGrid)
	//ensure we have something
	//attempt to smooth path
	if(tempFinder.length>0){
		//sort out the array to see if it exists inside of an intersection
		//var compressedPath =PathFinding.Util.compressPath(tempFinder)
		// var cleanPath = new Array()
		//
		//making a path object off our intersections
		//this is beta
		for(var i = 0; i<tempFinder.length; i++){
			if(i == 0 || i == tempFinder.length-1){
				//push our first and last items into the DB
				cleanPath.push(tempFinder[i])
			}else{
				for(var j = 0; j<intersections.length;j++){
					if(_.isEqual(tempFinder[i],intersections[j])) {

						//if current tempFinder is < 2 away from the next tempFinder
						if(getDistance(tempFinder[i],tempFinder[i+1] < 2){
							break; //then skip this one
						}
						else{
							cleanPath.push(tempFinder[i]);
						}
					}
				}
			}
		}

		//var compressedPath = PathFinding.Util.compressPath(tempFinder)
		//var smoothenPath = PathFinding.Util.smoothenPath(smoothGrid,tempFinder)
		//var compSmoothed = PathFinding.Util.compressPath(smoothenPath);
		//return compSmoothed;
		return cleanPath;


	} 	else return tempFinder

}

function getDistance( p1, p2 ) {
  var xs = 0;
  var ys = 0;

  xs = p2.x - p1.x;
  xs = xs * xs;

  ys = p2.y - p1.y;
  ys = ys * ys;

  return Math.sqrt( xs + ys );
}

exports.returnIntersection = function(){
	return intersection
}
