var 		 fs = require('fs'),
			PNG = require('pngjs').PNG,
	PathFinding = require('pathfinding')

//pathfinding matrix creation threshold	
var threshold = 250;

var matrix;
var grid;
exports.ready = false;

var finder = new PathFinding.AStarFinder({
	allowDiagonal: true
	//,dontCrossCorners: false
});


exports.setup =function(cb){
	//load raster  grayscale png convert 
	fs.createReadStream(__dirname+'/../public/imgs/ManhattanStreets_512_202_bitmap.png').pipe( new PNG({filterType:4}) ).on('parsed',function(){
	
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
		exports.ready = true;
		cb()
		//matrix is now a 2 dimensional array of a black and white image 
		//0/black is walkable
		//255/white is coverted to 1 and is blocked
		//console.log(JSON.stringify(matrix))
		//cb()
	
	})

}
//console.log(matrix)
var nmatrix = [
    [0, 0, 0, 1, 0],
    [1, 0, 0, 0, 1],
    [0, 0, 1, 0, 0],
];

//var grid = new PathFinding.Grid(5, 3, matrix);
exports.returnMatrix = function(){
	return matrix
}
exports.returnPath = function(endPoint){
	//copy grid
	//make new 
	//console.log(JSON.stringify(matrix))
	var newGrid = grid.clone()
	var compressGrid = grid.clone()
	//need to preset 1,2 to be the location of 30PP
	
	
	//return finder.findPath(351,116 , endPoint.x, endPoint.y, newGrid)
	//cb(path)
	var tempFinder = finder.findPath(363,123,endPoint.x,endPoint.y,newGrid)
	//ensure we have something
	//attempt to smooth path
	if(tempFinder.length>0){
		return PathFinding.Util.compressPath(tempFinder)
		//var compress =  PathFinding.Util.compressPath(tempFinder) 
		//return PathFinding.Util.smoothenPath(compressGrid,tempFinder)

	} 	else return tempFinder
	
}