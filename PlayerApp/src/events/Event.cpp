//
//  Event.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//
/*
 
 Event class. Upon receiving an 'init' command from the controller app, all Events are generated.  Each Event contains a vector of Scenes, each of which store all Assets, each of which contain at least 1 file type. 
 
 methods:
    - constructor pulls all data about this event out. Creates a JSON obj for scenes. Runs through array and populates vector <eScenes>
    - update() will update the appropriate scene for this event in the proper order, for proper amount of time
    - draw() will draw the appropriate scene...
 */

#include "Event.h"

//--------------------------------------------------------------
Event::Event(Json::Value thisEvent){
    
    //cout<< "EVENT RECEIVED: "<<thisEvent.toStyledString()<<endl; //entire raw event
    
    //--- pull all data out from this event
    eAnimType       = thisEvent.get("anim_type", "no event anim_type found").asString();
    eId             = thisEvent.get("_id", "no event _id found").asString();
    eTitle          = thisEvent.get("title", "no event title found").asString();
    eType           = thisEvent.get("type", "notype found").asString();
    eCreatedAt      = thisEvent.get("created_at", "no created found").asString();
    int numScenes   = thisEvent.get("scenes", "NO SCENES FOUND").size();
    
    //print EVENT keys
    cout << "\t----> EVENT TITLE: " << eTitle << "\t||\tID: "<< eId << "\t||\tnumScenes: "<< numScenes<< "\t||\teType: " << eType << "\t||\teCreatedAt: " << eCreatedAt << endl;
    
    //--- now lemme get some scenes brah
    Json::Value scenes = thisEvent.get("scenes", "scene object not found");
    
    for (int i=0; i < numScenes; i++){
        Scene thisScene = * new Scene(scenes[i]);
        eScenes.push_back(thisScene);
    }
}


//--------------------------------------------------------------
void Event::update(){ //might make sense
    
    //TODO: update scenes 1 at a time, in order, for proper amount of time

}


//--------------------------------------------------------------
void Event::draw(){ //might make sense

    //TODO: draw scenes 1 at time, in order, for proper amount of time
}
