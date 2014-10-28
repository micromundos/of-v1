#pragma once

#include "ofMain.h"
#include "ofxJSON.h"

#include "ecs/ECS.h"
#include "managers/Managers.h"
#include "systems/Systems.h"

class ofApp : public ofBaseApp
{

  public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxJSONElement config;

    ECS ecs;
    //TODO crear lista de EntityConfigManagers de config.json (escena, particulas, etc)
    EntityConfigManager escena;
    //TODO escuchar eventos BloqEvents en desde un BloqsMakerSystem que contiene un BloqsManager para crear entidades
    BloqsManager bloqs;

    void add_systems();
    void bloq_added( Bloq& bloq );
    void bloq_updated( Bloq& bloq );
    void bloq_removed( string& bloq_id );

};

