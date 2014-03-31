﻿/*******          GLOBAL 30_PP Objects and Functions**          ***//*** Objects*/function Compositions(){        this.setup = function(project){                for(var i =1; i<=project.numItems; i++){                    if(project.items[i].typeName === "Composition")                        this.add(new CustomItem(project.items[i],i))                }            }        //Array for CustomItems        this.all = [];        this.length= function(){ return this.all.length }        // Push Custom Item into Compositions Collection        this.add = function(_CustomItem){            this.all.push(_CustomItem)        }        this.byName=function(name){            //name exists            for(var i =0; i< this.all.length; i++) if(this.all[i].item.name === name) return this.all[i]            //name doesn't exist            return null;        }        }//CustomItem is an Object to reference app.project.items(index) with actual index object. //It will provide ability to search for comps by name etc//_item = app.project.items(index)//_index= indexfunction CustomItem(_item,_index){                this.index = _index;        this.item = _item;    }/* ** Functions*///print out complex objects to the Javscript Consolefunction printItemInfo(item,_count){   for( key in item ){           $.write(typeof item[key])           if(typeof item[key] === "function") {               $.writeln(" :: "+key)               //$.write(item[key])           //$.writeln(key+"::"+item[key])           }else if(typeof item[key] === "object"){               $.writeln(" :: "+key)               if(item[key] !== null ){                   var count = (_count !== null) ? _count : 0;                   if(count < 3 ) printItemInfo(item[key], count+1 )               }               //printItemInfo(item[key])           }else{                   $.writeln(" :: "+key+" :: "+item[key])               }              }        $.writeln("");   }function clearRenderQueue(){    for(var i =1; i<=app.project.renderQueue.numItems; i++){            //remove each item in the queue            app.project.renderQueue.item(i).remove()        } }//close the project which is currently open //pass an option to save or notfunction closeCurrentProject(bSave){    if(!bSave){        app.project.close(CloseOptions.DO_NOT_SAVE_CHANGES)        }else{            app.project.close(CloseOptions.SAVE_CHANGES)            }    }//open an after effects project //takes a string or a file objectfunction openProject(file){        //$.writeln(typeof file)                if(typeof file ==="string") file = new File(file)               app.open(file)    }   function Console(){        this.log=function(string){        		if(typeof string !== 'string') string = JSON.stringify(string)                system.callSystem("echo \""+string+"\" >> ~/Desktop/AE.log")            }        }    