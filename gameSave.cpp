//gameSave.cpp

#include "WorldManager.h"
#include "gameSave.h"
#include "Object.h"
#include "EventKeyboard.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include "bomb.h"
#include "Fruit.h"
#include "Sword.h"
#include "Grocer.h"
#include "util.h"
#include "Timer.h"
#include "Points.h"


gameSave::gameSave(){
    //register for keyboard events
    registerInterest(df::KEYBOARD_EVENT);

}

int gameSave::eventHandler(const df:: Event *p_e){
    //check df::Keyboard event
    if (p_e->getType() == df::KEYBOARD_EVENT){
    //cast to eventkeyboars
        const df::EventKeyboard *p_k = dynamic_cast <const df::EventKeyboard *> (p_e);
    //check for S and L
        if(p_k->getKey() ==df::Keyboard::S){
            saveGame();
            return 1;
        }
        if(p_k->getKey() ==df::Keyboard::L){
            loadGame();
            return 1;
        }
    }
    return 0;
}

void gameSave::saveGame(){
    //finding all objects
    df::ObjectList all_objects = WM.getAllObjects();
    //storing in iterator
    df::ObjectListIterator list_object(&all_objects);
    //counting all objects
    int count = 0;
    // Create stringstream to hold serialized data.
    std::stringstream ss;
    unsigned int mask = UINT_MAX; // all bits set
    //loop to get total amount of objects for save_count
    for(list_object.first(); !list_object.isDone(); list_object.next()){
        df:: Object *p_o = list_object.currentObject();
        //if it is not a fader or a gameSave Object
        if(p_o != this && p_o -> getType() != "Fader"){
            count++;
        }
    }

    //save count of object to the stream
    ss.write((char*) &count, sizeof(count));
    //open file
    std::ofstream outFile("save.txt", std::ios::binary);
    std:: string type;
    //loop again for the serialize
    for(list_object.first(); !list_object.isDone(); list_object.next()){
        df:: Object *p_o = list_object.currentObject();
        if(p_o != this && p_o -> getType() != "Fader"){
            //get type, save for the object to be seralized
            type = p_o->getType();
            //length of type
            int length = type.length();
            //saving the length first to let it know size of type
            ss.write((char* )&length, sizeof(length));
            //streaming the type string
            ss.write(type.c_str(), length);
            // Serialize the Object.
            p_o -> serialize(&ss, mask);
        }
    }
    //write to the stream
    std::string str = ss.str();            
    outFile.write(str.c_str(), str.size());
    //close file
    outFile.close();
}

void gameSave::loadGame(){
    //finding all objects
    df::ObjectList all_objects = WM.getAllObjects();
    //variable for a coordinate that is far from screen
    df::Vector off_screen_coordinate(-500,-500);
    //storing in iterator
    df::ObjectListIterator list_object(&all_objects);
    for(list_object.first(); !list_object.isDone(); list_object.next()){
        df:: Object *p_o = list_object.currentObject();
        //deleting all objects
        if(p_o != this){
            //set object far away so explosion is gone
            p_o->setPosition(off_screen_coordinate);
            WM.markForDelete(p_o);
        }
    }
    // Open file for reading.
    std::ifstream inFile("save.txt", std::ios::binary);
    std::stringstream ss;
    // Read entire file into stream.
    ss << inFile.rdbuf();

    // Close file.
    inFile.close();

    // Extract number from stream.
    int count; //total number of objects
    ss.read((char*) &count, sizeof(count));

    for(int i = 0; i < count; i++){
        std::string type;
        //reading the length of type
        int type_length = 0;
        ss.read((char* ) &type_length, sizeof(type_length));
        // extract the type for the object
        char buffer[type_length+1];
        ss.read(buffer, type_length);
        buffer[type_length] = '\0';
        type = buffer;
        df::Object *p_o = nullptr;
        //based on type, create the object
        if(type == "bomb"){
            p_o = new Bomb();
        }
        else if(type == SWORD_STRING){
            p_o = new Sword();
        }
        else if(type == GROCER_STRING){
            p_o = new Grocer();
        }
        else if(type == POINTS_STRING){
            p_o = new Points();
        }
        else if(type == TIMER_STRING){
            p_o = new Timer();
        }
        else {
            p_o = new Fruit(type);
        }
        if(p_o){
            unsigned int mask = UINT_MAX; // all bits set
            //deserialize stream on the object
            p_o->deserialize(&ss, &mask);
        }

    }
}