﻿/* Incredible Machines 2014 	cpiuggi 05/22/14  *///this is a super no no//modify the Native Arrayvar project = app.project;var compositions = new AEHelper("Composition")var comp = compositions.byName("Sculpt FILL")var ribs = compositions.byName("ribs")var ribsByRow =[]if(ribs!=null){  $.write("Ribs Comp Found - Num Layers: ")  $.writeln(" "+ribs.numLayers)  for(var i = 1;i <=ribs.numLayers; i++){    var layerMask = ribs.layer(i).property("ADBE Mask Parade")    for(var j = 1; j<=layerMask.numProperties ; j++){      var shape = layerMask.property(j).property("ADBE Mask Shape").value      var obj = {}      obj.vertices = shape.vertices      obj.inTangents= shape.inTangents      obj.outTangents= shape.outTangents      obj.closed = shape.closed      ribsByRow.push(obj)    }  }}$.writeln("Found Ribs: "+ribsByRow.length)for(var i = 0; i< ribsByRow.length;i++){  //$.writeln(i+" "+JSON.stringify(ribsByRow[i]))}//$.writeln(JSON.stringify(ribsByRow))var location = [670,170]//[1440,730]//[1120,260]//[1930,260]//[1530,820] //[670,170]var home    = [1570,450]//make this contain vertices, inTangents, outTangents, closed:false,var pathToDestination = {}    pathToDestination.vertices = []    pathToDestination.inTangents = []    pathToDestination.outTangents = []    pathToDestination.closed = false    // pathToDestination.vertices.push(home)    // pathToDestination.inTangents.push([0,0])    // pathToDestination.outTangents.push([0,0])//left to rightvar layersByRow = [            [1,2,3,4],            [5,6,7,8],            [9,10,11,12],            [13,14,15,16],            [17,18,19],            [20,21,22],            [23,24],            [25,26,27],            [28,29]   ]//array of layer objects containing all the masks in the shapes groupsvar layers =[]if(comp != null){        $.write("Sorting Comps...")        //$.writeln(comp.numLayers)        for(var i = 1; i<= comp.numLayers;i++){          //iterate through the masks          var layerMask = comp.layer(i).property("ADBE Mask Parade")          //$.writeln("Layer Index: "+i+" | NUM Masks: "+layerMask.numProperties)          //set up all our masks for each layer          var counter = 1;          //run through all of the layerMask Properties with a custom counter          while(counter <= layerMask.numProperties){                  var sorter = []                  //create an array to sort the items by value                  //iterate through each and get the max vertices for each X                  for(var j = layerMask.numProperties; j>0 ; j--){                      var shape = layerMask.property(j).property("ADBE Mask Shape").value                      var max = { x: returnMax(shape.vertices,0), y: returnMax(shape.vertices,1)}                      if(counter == layerMask.numProperties) var min = { x: returnMin(shape.vertices,0), y: returnMin(shape.vertices,1)}                      var obj = {}                      obj.index = j                      obj.max = max                      if(counter == layerMask.numProperties){                        obj.min = min                        obj.shape = JSON.parse(JSON.stringify(shape))                        delete obj.shape.featherInterps                        delete obj.shape.featherRadii                        delete obj.shape.featherRelCornerAngles                        delete obj.shape.featherRelSegLocs                        delete obj.shape.featherSegLocs                        delete obj.shape.featherTensions                        delete obj.shape.featherTypes                       }                      sorter.push(obj)                  }                  //sort the array highest to lowest                  sorter.sort(function(a,b){return  b.max.x-a.max.x})                  //reverse them                  sorter.reverse()                  //$.writeln(JSON.stringify(sorter))                  layerMask.property(sorter[counter-1].index).moveTo(counter)                  //set the mask opacity to have a linear expression                  //layerMask.property(counter).property("Mask Opacity").expression = returnOpacityExpression(counter);                  maskOpacity(i,counter,0)                  if(counter == layerMask.numProperties){                    //all indexs should be sorted now by max.x but can later be sorted by anything                    var layer = {}                    layer.index = i                    layer.shapes = sorter                    layers.push(layer)                  }                  counter++              }          }//end for numlayers          $.writeln("  Sort Completed.")          $.write("Creating Layers Obj...");          //sort our layers object we created          //$.writeln(JSON.stringify(layers[0]))          for(var i = 0; i<layers.length; i++){            var xOffset, yOffset;            var xMax,yMax;            var width, height;            var xSortArray = layers[i].shapes.slice(0)            var ySortArray = layers[i].shapes.slice(0)            //var xMaxSortArray = layers[i].shapes.slice(0)            //var yMaxSortArray = layers[i].shapes.slice(0)            xOffset = xSortArray.sort(function(a,b){return a.min.x-b.min.x})[0].min.x            yOffset = ySortArray.sort(function(a,b){return a.min.y-b.min.y})[0].min.y            xMax = xSortArray.sort(function(a,b){return b.max.x-a.max.x})[0].max.x            yMax = xSortArray.sort(function(a,b){return b.max.y-a.max.y})[0].max.y            //xMaxSortArray.sort()            //$.writeln("xOffset: "+xOffset+" yOffset "+yOffset)            //$.writeln( "XMIN min: "+xSortArray[0].min.x+" max: "+ xSortArray[xSortArray.length-1].min.x)            //$.writeln( "YMIN min: "+ySortArray[0].min.y+" max: "+ ySortArray[xSortArray.length-1].min.y)            //make global coordinates            layers[i].row = returnCurrentRow(layers[i].index)            var layerPosition = comp.layer(layers[i].index).property("Position").value            var layerAnchor = comp.layer(layers[i].index).property("Anchor Point").value            layers[i].bounding = {x: layerPosition[0], y: layerPosition[1], width: xMax - xOffset, height: yMax - yOffset}            layers[i].bounding.vertices = [  [layers[i].bounding.x, layers[i].bounding.y],                                             [layers[i].bounding.x, layers[i].bounding.y+layers[i].bounding.height],                                             [layers[i].bounding.x+layers[i].bounding.width, layers[i].bounding.y],                                             [layers[i].bounding.x+layers[i].bounding.width, layers[i].bounding.y+layers[i].bounding.height ]                                             ]            for(var j = 0; j<layers[i].shapes.length; j++){              layers[i].shapes[j].shape.world_vertices = []              for(var k = 0; k<layers[i].shapes[j].shape.vertices.length; k++){                var point = [0,0]                //calculate the world coordinate by normalizing the vertice to the layers anchor point                //then add the position of the layer in the composition                point[0] = layers[i].shapes[j].shape.vertices[k][0] - layerAnchor[0] + layerPosition[0]                point[1] = layers[i].shapes[j].shape.vertices[k][1] - layerAnchor[1] + layerPosition[1]                layers[i].shapes[j].shape.world_vertices.push(point)              }            }          }//end layers.length          $.writeln("Layers Objects Created")          //$.writeln(JSON.stringify(layers[layers.length-1]))          $.writeln("Getting Line Between Points")          var line = returnLine(location[0],location[1],home[0],home[1])          $.writeln("Line Length: "+line.length)          var activeRows = [],              activeLayers = [],              activeRibs = [],              lastActiveRib= 0,              lastActiveRow= 0,              lastActiveLayer = 0          for(var m =0; m<line.length; m++ ){            for(var i = 0; i<layers.length;i++){              //check if line passes through layer              if( isInside( line[m][0],line[m][1], layers[i].bounding.vertices ) )                if( activeLayers.indexOf(layers[i].index) == -1 ) activeLayers.push(layers[i].index)            }          }          $.write("Active Layers: ")          $.writeln(activeLayers.length)          $.writeln(activeLayers)          $.write("Finding Active Ribs: ")          var inactiveRibs = []          //iterate through the active layers for indexs of active ribs          for(var i = 0; i<activeLayers.length; i++){            var currentLayer = returnCurrentLayer(activeLayers[i])            var bActiveRibs = false            //add to active row            if(  activeRows.indexOf(currentLayer.row) == -1) activeRows.push(currentLayer.row)            var activeCurrently= []            for(var j =0; j<currentLayer.shapes.length; j++){              var bInside = false              //get the lowest and highest              //and light the rest up              //active this row              for(var m = 0; m<line.length; m++){                if(isInside( line[m][0],line[m][1], currentLayer.shapes[j].shape.world_vertices)){                  bActiveRibs = true                  //$.writeln(JSON.stringify({layer: currentLayer.index, index: currentLayer.shapes[j].index }))                  activeRibs.push({layer: currentLayer.index, row: currentLayer.row, index: currentLayer.shapes[j].index })                  maskOpacity(currentLayer.index,currentLayer.shapes[j].index ,100)                  bInside = true                  if(  activeCurrently.indexOf(currentLayer.shapes[j].index) == -1){                    activeCurrently.push( currentLayer.shapes[j].index )                  }                  break                }              }            }            if(bActiveRibs == false ) inactiveRibs.push(activeLayers[i])            //$.write("activeCurrently:  ")            //$.writeln(activeCurrently)          }          $.writeln("  Active Ribs Found.")          if(inactiveRibs.length>0){            $.writeln(" InActive Layers Found.")            $.writeln(inactiveRibs)            $.writeln("Removing Inactive Layers")            for(var i = 0; i<inactiveRibs.length; i++){              activeLayers.splice( activeLayers.indexOf(inactiveRibs[i]),1)            }            $.writeln("Updated Active Layers")            $.writeln(activeLayers)            $.writeln()          }          //create path          //check all the active layers and determine the active rows          $.writeln("Active Rows")          $.writeln(activeRows)          $.writeln()          var maxRow = Math.max.apply(Math, activeRows)          //check the direction of the line - up or down, this is done by the max row active.          var minRow = Math.min.apply(Math, activeRows)          $.writeln("Sorting Active Ribs")          //{layer: currentLayer.index, row: currentLayer.row, index: currentLayer.shapes[j].index }          //sort our active ribs by row, layer and index          activeRibs.sort(function(a,b){            var row = a.row - b.row            var layer = null,                _index = null            if(row == 0) layer = a.layer - b.layer            if(row == 0 && (layer == 0 && layer !=null )) _index = a.index - b.index            if(layer == null) layer = 0            if(_index == null) _index  0            return row+layer+_index          })          $.writeln("Completed")          $.writeln(JSON.stringify(activeRibs))          //set our points to rock now          //don't do the last rib we check the next rib after and get it on the last loop          $.writeln("Iterating on Ribs")          for(var rib = 0; rib<activeRibs.length-1;rib++){            //$.writeln(activeRibs[rib].layer+ " "+activeRibs[rib].index)            var _ribLayer = returnCurrentLayer(activeRibs[rib].layer)            //var _nextRibLayer = returnCurrentLayer(activeRibs[rib+1].layer)            var _rib = _ribLayer.shapes[activeRibs[rib].index-1]            $.writeln()            $.writeln(_rib.shape.world_vertices)            $.writeln()            if(rib == 0 ){                //get the first vertex -                //for now get the lowest vertex                pathToDestination.vertices.push(_rib.shape.world_vertices[0])                pathToDestination.inTangents.push(_rib.shape.inTangents[0])                pathToDestination.outTangents.push(_rib.shape.outTangents[0])                //$.writeln(JSON.stringify(_ribLayer))                //var _ribIndex            }            if(activeRibs[rib+1].layer == activeRibs[rib].layer){              //check if theres a break in the width via the indexs              //$.writeln("Same Layer")              //$.writeln("Current Layer: "+activeRibs[rib].layer+" next: "+activeRibs[rib+1].index+" current: "+activeRibs[rib].index+" Length: "+_ribLayer.shapes.length+"")              if( (activeRibs[rib+1].index >= _ribLayer.shapes.length/2 && activeRibs[rib].index <= _ribLayer.shapes.length/2) ||                  (activeRibs[rib+1].index <= _ribLayer.shapes.length/2 && activeRibs[rib].index >= _ribLayer.shapes.length/2)){                    if(activeRibs[rib].row<4){                    }                    //find the middle point.                    var vertIndex=-1                    for(var ly = 0; ly< layersByRow.length; ly++){                      var idx = layersByRow[ly].indexOf(activeRibs[rib].layer)                      if(idx != -1){                        vertIndex = idx                        break                      }                        vertIndex= -1                    }                    $.writeln("Need Middle Point")                    //$.write("Ribs By Row: ")                    //$.writeln(JSON.stringify(ribsByRow[activeRibs[rib].row-1]))                    //$.writeln(ribsByRow[activeRibs[rib].row-1].vertices[vertIndex*2-1])                    //$.writeln("idx "+vertIndex)                    //$.writeln(activeRibs[rib].layer)                    pathToDestination.vertices.push(ribsByRow[activeRibs[rib].row-1].vertices[vertIndex*2])                    pathToDestination.inTangents.push(ribsByRow[activeRibs[rib].row-1].inTangents[vertIndex*2])                    pathToDestination.outTangents.push(ribsByRow[activeRibs[rib].row-1].outTangents[vertIndex*2])                  }            }else{              if(activeRibs[rib+1].row == activeRibs[rib].row){                $.writeln("New Layer Same Row")                    var vertIndex=-1                    for(var ly = 0; ly< layersByRow.length; ly++){                    var idx = layersByRow[ly].indexOf(activeRibs[rib].layer)                    if(idx != -1){                      vertIndex = idx                      break                    }                      vertIndex= -1                    }                    //$.write("Ribs By Row: ")                    //$.writeln(JSON.stringify(ribsByRow[activeRibs[rib].row-1]))                    //$.writeln(ribsByRow[activeRibs[rib].row-1].vertices[vertIndex*2-1])                    $.writeln("idx "+vertIndex+" calculated: ")                    $.writeln(activeRibs[rib].layer)                    pathToDestination.vertices.push(ribsByRow[activeRibs[rib].row-1].vertices[vertIndex*2+1])                    pathToDestination.inTangents.push(ribsByRow[activeRibs[rib].row-1].inTangents[vertIndex*2+1])                    pathToDestination.outTangents.push(ribsByRow[activeRibs[rib].row-1].outTangents[vertIndex*2+1])              }else{                //$.writeln("New Row")                //$.writeln("_ribLayer"+ JSON.stringify(_ribLayer))                //$.writeln("index: "+activeRibs[rib].index)                //var min = Math.min(activeRibs[rib+1].row,activeRibs[rib].row)                pathToDestination.vertices.push(_rib.shape.world_vertices[0])                pathToDestination.inTangents.push(_rib.shape.inTangents[0])                pathToDestination.outTangents.push(_rib.shape.outTangents[0])                var _nextRibLayer = returnCurrentLayer(activeRibs[rib+1].layer)                var _nextRib = _nextRibLayer.shapes[activeRibs[rib+1].index-1]                pathToDestination.vertices.push(_nextRib.shape.world_vertices[0])                pathToDestination.inTangents.push(_nextRib.shape.inTangents[0])                pathToDestination.outTangents.push(_nextRib.shape.outTangents[0])              }            }          }          $.writeln("Complete")          //set next points          /*          if(maxRow < 5 ){            for(var r = 3; r>=minRow; r-- ){              var layersThisRow = []              for(var i = 0; i<activeLayers.length; i++){                var currentLayer = null                for(var j =0; j<layers.length; j++){                  if(layers[j].index == activeLayers[i] ){                    currentLayer =layers[j]                    //$.writeln("Found Current Layer: "+currentLayer.index)                    break                  }                }                //$.writeln(currentLayer.row+"  "+r)                if(currentLayer.row == r) layersThisRow.push(currentLayer)                }                //get the indexs of the layers and then the indexs of the rib points                // for(var l =0; l<layersThisRow.length; l++){                //   var indexs = layersByRow.nestedIndexOf(layersThisRow[l])                //   if( indexs != -1 )                // }                if(layersThisRow.length>1){                  //get the left                  //get the rib points in between the layers                  //tell the direction by the idex of the layersByRow Array Indexs                  for(var la = 1; la < layersThisRow.length; la++){                    var point = Math.max( layersThisRow[la-1].index, layersThisRow[la].index)                      $.writeln("r: "+r+" point: "+point)                      var nestedIndex = layersByRow[r].indexOf(point)                      $.writeln(nestedIndex)                      pathToDestination.vertices.push(ribsByRow[r].vertices[nestedIndex+1])                      pathToDestination.inTangents.push(ribsByRow[r].inTangents[nestedIndex+1])                      pathToDestination.outTangents.push(ribsByRow[r].outTangents[nestedIndex+1])                  }                  var rowMax = Math.max.apply(Math, layersThisRow)                }else{                  //get the left and right most points of the layer                  $.write(" ")                }              $.writeln("Row: "+r+" Layers:  "+layersThisRow.length)            }          }else{            for(var r = minRow; r<=maxRow;r++){              //$.writeln()              //active layers per row              var layersThisRow = []              for(var i = 0; i<activeLayers.length; i++){                var currentLayer = null                for(var j =0; j<layers.length; j++){                  if(layers[j].index == activeLayers[i] ){                    currentLayer =layers[j]                    //$.writeln("Found Current Layer: "+currentLayer.index)                    break                  }                }                //$.writeln(currentLayer.row+"  "+r)                if(currentLayer.row == r) layersThisRow.push(currentLayer)                }              //add next point              $.writeln("Row: "+r+" Layers:  "+layersThisRow.length)            }          }*/          $.writeln(JSON.stringify(pathToDestination))    }// if(comp !=null)    //add last point to destination    var pathShape = new Shape()			  pathShape.vertices = pathToDestination.vertices        pathShape.inTangents = pathToDestination.inTangents        pathShape.outTangents = pathToDestination.outTangents			  pathShape.closed = false    compositions.byName("both").layer(4).property("ADBE Mask Parade").property(1).property("ADBE Mask Shape").setValue(pathShape)/*********** HELPER FUNCTIONS*********/function maskOpacity(layer,index,value){         comp.layer(layer).property("ADBE Mask Parade").property(index).property("Mask Opacity").expression=""         comp.layer(layer).property("ADBE Mask Parade").property(index).property("Mask Opacity").setValue(value)                    }function AEHelper(_type){        //set up our vars        var type =_type        //varialize our methods        this.setup = function(){                for(var i =1; i<=app.project.numItems; i++){                    if(app.project.items[i].typeName === type )                        this.add(app.project.items[i])                }            }        //Array for Project Items        this.all = [];        this.length= function(){ return this.all.length }        // Push Custom Item into Compositions Collection        this.add = function(_CustomItem){            this.all.push(_CustomItem)        }        this.byName=function(name){            //name exists            for(var i =0; i< this.all.length; i++) if(this.all[i].name === name) return this.all[i]            //name doesn't exist            return null;        }        //run setup on creation        this.setup()    }/* MATH */function returnMax(vertices,which){     //which 0 or 1 x or y    var max =vertices[0][which]    for(var k = 1; k < vertices.length; k++){            if(max < vertices[k][which]) max = vertices[k][which]    }    return max    }function returnMin(vertices,which){        var min = vertices[0][which];        for(var k= 1; k<vertices.length;k++){                if(min > vertices[k][which]) min = vertices[k][which]            }        return min    }function returnOpacityExpression(counter){  var exp = "linear(time,thisLayer.inPoint,thisLayer.inPoint+"+counter+",0,100)";  var exp = "start = thisLayer.inPoint +"+(counter/10)+";"  exp+= "end = thisLayer.inPoint +"+(counter/10)+"+ 0.3;"  exp+= " linear(time,start,end,0,100);"  exp+= "if(time>end){linear(time,end+0.5,end+0.5,100,0)};"  return exp}function isInside(x,y,vertices){  var N = vertices.length,      p1,p2,      counter =0,      i,      xinters      p1 = vertices[0]      for( i = 1;i<=N;i++){        p2= vertices[i%N]        if(y> Math.min(p1[1], p2[1])){          if (y <= Math.max(p1[1],p2[1])) {                if (x <= Math.max(p1[0],p2[0])) {                    if (p1[1] != p2[1]) {                        xinters = (y-p1[1])*(p2[0]-p1[0])/(p2[1]-p1[1])+p1[0];                        if (p1[0] == p2[0] || x <= xinters)                            counter++;                    }                }            }        }        p1 = p2      }     if (counter % 2 == 0) return false;	  else return true;}function returnCurrentLayer(index){  //$.writeln("checking")  //$.writeln(layers.length)  for(var j =0; j<layers.length; j++){    if(layers[j].index == index )      return layers[j]  }  //$.writeln("nomatch")  return -1}function returnCurrentRow(layer){  for(var r = 0; r < layersByRow.length; r++)      if(layersByRow[r].indexOf(layer)>-1)        return r    return -1}//http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/function returnLine( x, y, x2,  y2) {    var line =[]    line.push([x,y])    var w = x2 - x ;    var h = y2 - y ;    var dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;    if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;    if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;    if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;    var longest = Math.abs(w) ;    var shortest = Math.abs(h) ;    if (!(longest>shortest)) {        longest = Math.abs(h) ;        shortest = Math.abs(w) ;        if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;        dx2 = 0 ;    }    var numerator = longest >> 1 ;    for (var i=0;i<=longest;i++) {        //putpixel(x,y,color) ;        /*if( i%5==0 )*/ line.push([x,y])        numerator += shortest ;        if (!(numerator<longest)) {            numerator -= longest ;            x += dx1 ;            y += dy1 ;        } else {            x += dx2 ;            y += dy2 ;        }    }    line.push([x2,y2])    return line}Array.prototype.indexOf = function(needle) {    for(var i = 0; i < this.length; i++) {        if(this[i] === needle) {            return i;        }    }    return -1;}Array.prototype.nestedIndexOf = function(needle){  for(var i = 0; i<this.length; i++){    var val = this[i].indexOf(needle)    if( val !=-1 ) return [i,val]  }  return -1}