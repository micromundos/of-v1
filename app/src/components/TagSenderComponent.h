#pragma once

#include "ofMain.h"
#include <Artemis/Artemis.h>
#include "ofxECS.h"

class TagSenderComponent : public ECScomponent 
{
  public:

    //float _data; //to get value from config.json or live in code only
    //ofParameter<float> _param; //to get value from a sender via osc
  
    string  host;
    int port;
  
    TagSenderComponent(string _id) : ECScomponent(_id) {};

    virtual void init( string e_id, Json::Value d, ECSparams* p )
    {
      ECScomponent::init(e_id,d,p);
      
      host = d["host"].asString();
      port = d["port"].asInt();
      
      //_data = d.get("_data",1.0f).asFloat();
      //param(_param, "_param");
    };

};

