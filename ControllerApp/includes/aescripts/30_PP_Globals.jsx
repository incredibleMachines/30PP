﻿/*******          GLOBAL 30_PP Objects and Functions*****//*** Settings*/var LOCATION = {                  size_diff: 4}var MULTITEXT = {                  font_size: 50,                  text_space: 150,                  max_width: 30000                  }/*** Objects*/function AEHelper(_type){        //set up our vars        var type =_type        //varialize our methods        this.setup = function(){                for(var i =1; i<=app.project.numItems; i++){                    if(app.project.items[i].typeName === type )                        this.add(app.project.items[i])                }            }        //Array for Project Items        this.all = [];        this.length= function(){ return this.all.length }        // Push Custom Item into Compositions Collection        this.add = function(_CustomItem){            this.all.push(_CustomItem)        }        this.byName=function(name){            //name exists            for(var i =0; i< this.all.length; i++) if(this.all[i].name === name) return this.all[i]            //name doesn't exist            return null;        }        //run setup on creation        this.setup()    }//deprecated Use AEHelperfunction Compositions(){        this.setup = function(project){                for(var i =1; i<=project.numItems; i++){                    if(project.items[i].typeName === "Composition")                        this.add(new CustomItem(project.items[i],i))                }            }        //Array for CustomItems        this.all = [];        this.length= function(){ return this.all.length }        // Push Custom Item into Compositions Collection        this.add = function(_CustomItem){            this.all.push(_CustomItem)        }        this.byName=function(name){            //name exists            for(var i =0; i< this.all.length; i++) if(this.all[i].item.name === name) return this.all[i]            //name doesn't exist            return null;        }    }//deprecated use AEHelper//CustomItem is an Object to reference app.project.items(index) with actual index object.//It will provide ability to search for comps by name etc//_item = app.project.items(index)//_index= indexfunction CustomItem(_item,_index){        this.index = _index;        this.item = _item;    }/*** Functions*///print out complex objects to the Javscript Consolefunction printItemInfo(item,_count){   for( key in item ){           $.write(typeof item[key])           if(typeof item[key] === "function") {               $.writeln(" :: "+key)               //$.write(item[key])           //$.writeln(key+"::"+item[key])           }else if(typeof item[key] === "object"){               $.writeln(" :: "+key)               if(item[key] !== null ){                   var count = (_count !== null) ? _count : 0;                   if(count < 3 ) printItemInfo(item[key], count+1 )               }               //printItemInfo(item[key])           }else{                   $.writeln(" :: "+key+" :: "+item[key])               }       }        $.writeln("");   }function clearRenderQueue(){    for(var i =1; i<=app.project.renderQueue.numItems; i++){            //remove each item in the queue            app.project.renderQueue.item(i).remove()        } }//close the project which is currently open//pass an option to save or notfunction closeCurrentProject(bSave){    if(!bSave){        app.project.close(CloseOptions.DO_NOT_SAVE_CHANGES)        }else{            app.project.close(CloseOptions.SAVE_CHANGES)            }    }//open an after effects project//takes a string or a file objectfunction openProject(file){        //$.writeln(typeof file)        if(typeof file ==="string") file = new File(file)        app.open(file)    }function Console(){        this.log=function(string){        		if(typeof string !== 'string') string = JSON.stringify(string)                system.callSystem("echo \""+string+"\" >> ~/Desktop/AE.log")            }    }/* MATH */function returnMax(vertices,which){     //which 0 or 1 x or y    var max =vertices[0][which]    for(var k = 1; k < vertices.length; k++){            if(max < vertices[k][which]) max = vertices[k][which]    }    return max    }function returnMin(vertices,which){        var min = vertices[0][which];        for(var k= 1; k<vertices.length;k++){                if(min > vertices[k][which]) min = vertices[k][which]            }        return min    }function returnMaxIndex(vertices,which){     //which 0 or 1 x or y    var max =vertices[0][which]    var index = 0    for(var k = 1; k < vertices.length; k++){            if(max < vertices[k][which]){              max = vertices[k][which]              index = k            }    }    return index    }function returnMinIndex(vertices,which){        var min = vertices[0][which];        var index = 0        for(var k= 1; k<vertices.length;k++){                if(min > vertices[k][which] ){                min = vertices[k][which]                index = k              }            }        return index    }function maskOpacity(item,layer,index,value){         item.layer(layer).property("ADBE Mask Parade").property(index).property("Mask Opacity").expression=""         item.layer(layer).property("ADBE Mask Parade").property(index).property("Mask Opacity").setValue(value)                    }function returnOpacityExpression(counter){  var exp = "linear(time,thisLayer.inPoint,thisLayer.inPoint+"+counter+",0,100)";  var exp = "start = thisLayer.inPoint +"+(counter/10)+";"  exp+= "end = thisLayer.inPoint +"+(counter/10)+"+ 0.3;"  exp+= " linear(time,start,end,0,100);"  exp+= "if(time>end){linear(time,end+0.5,end+0.5,100,0)};"  return exp}function isInside(x,y,vertices){  var N = vertices.length,      p1,p2,      counter =0,      i,      xinters      p1 = vertices[0]      for( i = 1;i<=N;i++){        p2= vertices[i%N]        if(y> Math.min(p1[1], p2[1])){          if (y <= Math.max(p1[1],p2[1])) {                if (x <= Math.max(p1[0],p2[0])) {                    if (p1[1] != p2[1]) {                        xinters = (y-p1[1])*(p2[0]-p1[0])/(p2[1]-p1[1])+p1[0];                        if (p1[0] == p2[0] || x <= xinters)                            counter++;                    }                }            }        }        p1 = p2      }     if (counter % 2 == 0) return false;	  else return true;}//http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/function returnLine( x, y, x2,  y2) {    var line =[]    line.push([x,y])    var w = x2 - x ;    var h = y2 - y ;    var dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;    if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;    if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;    if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;    var longest = Math.abs(w) ;    var shortest = Math.abs(h) ;    if (!(longest>shortest)) {        longest = Math.abs(h) ;        shortest = Math.abs(w) ;        if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;        dx2 = 0 ;    }    var numerator = longest >> 1 ;    for (var i=0;i<=longest;i++) {        //putpixel(x,y,color) ;        /*if( i%5==0 )*/ line.push([x,y])        numerator += shortest ;        if (!(numerator<longest)) {            numerator -= longest ;            x += dx1 ;            y += dy1 ;        } else {            x += dx2 ;            y += dy2 ;        }    }    line.push([x2,y2])    return line}//this is a super no no//modify the Native Arrayif(!Array.prototype.indexOf) {  Array.prototype.indexOf = function(needle) {      for(var i = 0; i < this.length; i++) {          if(this[i] === needle) {              return i;          }      }      return -1;  };}if(!Array.prototype.nestedIndexOf){  Array.prototype.nestedIndexOf = function(needle){    for(var i = 0; i<this.length; i++){      var val = this[i].indexOf(needle)      if( val !=-1 ) return [i,val]    }    return -1  }}