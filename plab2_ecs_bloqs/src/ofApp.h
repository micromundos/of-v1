#pragma once

#include "ofMain.h"
#include "ofxKinect.h"

#include "ECS.h"
#include "ECSbloqs.h"
#include "bloqs/BloqsAruco.h"
#include "systems/RenderSystem.h"
#include "systems/ParticleSystem.h"

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


    ECS ecs;
    ECSbloqs ecsbloqs;
    BloqsAruco bloqs;

    ofxKinect kinect;

    void bloq_added( Bloq& bloq );
    void bloq_updated( Bloq& bloq );
    void bloq_removed( int& bloq_id );

};
