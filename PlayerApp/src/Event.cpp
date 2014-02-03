//
//  Event.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#include "Event.h"

//--------------------------------------------------------------
Event::Event(Json::Value thisEvent){
    
    //cout<< "EVENT RECEIVED: "<<thisEvent.toStyledString()<<endl; //entire raw event
    
    //--- this event
    eAnimType       = thisEvent.get("anim_type", "anim_type not found").asString();
    eId             = thisEvent.get("_id", "id not found").asString();
    eTitle          = thisEvent.get("title", "title not found").asString();
    eType           = thisEvent.get("type", "type not found").asString();
    eCreatedAt      = thisEvent.get("created_at", "created not found").asString();
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
//void Event::update(){ //might make sense

//}
