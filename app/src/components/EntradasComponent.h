#pragma once

#include "ofMain.h"
#include <Artemis/Artemis.h>
#include "ofxECS.h"

class EntradasComponent : public ECScomponent 
{
  private:
    gpgpu::Process _output; 

  public:

    gpgpu::Process& output()
    { return _output; }; 

    ofParameter<float> threshold_near;
    ofParameter<float> threshold_far;
    ofParameter<float> close_iter;
    ofParameter<bool> render; 
    //ofParameter<bool> render_smoothed;
    //ofParameter<float> bilateral_domain;
    //ofParameter<float> bilateral_range;
    //ofParameter<float> bilateral_kernel;

    EntradasComponent(string _id) : ECScomponent(_id) {};

    virtual void init( string e_id, Json::Value d, ECSparams* p )
    {
      ECScomponent::init(e_id,d,p);

      param(render, "render");
      //param(render_smoothed, "render_smoothed");
      param(threshold_near, "threshold_near");
      param(threshold_far, "threshold_far"); 
      param(close_iter, "close_iter"); 
      //param(bilateral_domain,"bilateral_domain"); 
      //param(bilateral_range,"bilateral_range"); 
      //param(bilateral_kernel,"bilateral_kernel");
    };

};

