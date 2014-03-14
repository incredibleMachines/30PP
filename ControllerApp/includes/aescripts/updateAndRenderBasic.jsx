﻿//include our global functions/ helpers#include '30_PP_Globals.jsx'; function updateAndRenderBasic(args){                //open file a new file        closeCurrentProject();        var projectFile = new File(args.file);        app.open(projectFile);        app.beginSuppressDialogs();  //suppress dialogs from AE        var project = app.project //create a project variable        //process the incoming args as json        //$.writeln(JSON.stringify(args))                   var comps = new Compositions(); //init composition holder                //setup compositions        for(var i =1; i<=project.numItems; i++){            //$.writeln(project.items[i].name+"     :: "+project.items[i].typeName);            if(project.items[i].typeName === "Composition"){                //$.writeln("Found Composition")                //create custom item pass it to our compositions array                comps.add(new CustomItem(project.items[i],i))            }        }                //iterate through the modifications         //var selectedComp = comps.byName("Compositing_BothWalls");                for(var key in args.modifications){                var selectedComp = comps.byName(key);                if(selectedComp){                        if(key.indexOf('Image') >= 0 ){                            //$.writeln("Image!!")                            selectedComp.item.layer(1).source.replace(new File(args.modifications[key]))                        }else if(key.indexOf('Text')>=0){                            //$.writeln("Text!!")                            selectedComp.item.layer(1).property("Source Text").setValue(args.modifications[key])                        }                    }            }//end if(selectedComp)        //clear render queue        clearRenderQueue();        var counter = 0;        for(var key in args.output_options){                var selectedUVComp = comps.byName(key)                if(selectedUVComp){                      //$.writeln(key +" "+args.output_options[key]+" exists")                      //increment our counter                      counter++;                      //add item to renderQueue                      app.project.renderQueue.items.add(selectedUVComp.item);                                            var renderObject = app.project.renderQueue.item(counter);                                           renderObject.outputModule(1).file = new File(args.output_options[key]+"_"+key+".mov")                }//end if(selectedUVComp)            }     app.project.renderQueue.render()}//end updateAndRenderBasic(args)  var sample = {	file: "/Users/chris/oF/080/apps/30PP/ControllerApp/includes/aeprojects/Seamless_L-R/Seamless_L-R.aep",	modifications: {						Source_Text: "Mr. Chows", 						Source_Image1: "/Users/chris/Desktop/images/image2.jpg",						Source_Image2: "/Users/chris/Desktop/images/image5.jpg"	},    output_options:{                            UV_L: "/Users/chris/Desktop/filename_1",                            UV_R: "/Users/chris/Desktop/filename_1",                             UV_LR: "/Users/chris/Desktop/filename_1",                            //UV_S: "/Users/chris/Desktop/filename_1"     }}//updateAndRenderBasic(sample);