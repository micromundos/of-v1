#pragma once

#include <Artemis/Artemis.h>
#include "ofxECS.h"
#include "components/Components.h"
#include "ofxKinect.h"

using namespace artemis;

class RGBDSystem : public ECSsystem 
{ 

  public:

    RGBDSystem(string _id) : ECSsystem(_id)
    {
      addComponentType<RGBComponent>();
      addComponentType<DepthComponent>();
    };

    virtual void initialize() 
    {
      rgb_m.init( *world );
      depth_m.init( *world );

      //registration transforms rgb pixels to match depth pixels, not the other way around, right? then we're fine!
      kinect.setRegistration(true);
      kinect.enableDepthNearValueWhite(true);
      // ir, rgb, texture
      kinect.init(false, true, true);
      kinect.open();
    };

    virtual void added(Entity &e) 
    {
      int w = kinect.width;
      int h = kinect.height;
      rgb_m.get(e)->setup( w, h );
      depth_m.get(e)->setup( w, h );
    };

    virtual void processEntities( ImmutableBag<Entity*>& bag ) 
    {
      kinect.update();
      for (int i=0;i<bag.getCount();i++)
        processEntity( *bag.get(i) ); 
    };

    virtual void processEntity(Entity &e) 
    {
      //ofLogNotice("RGBDSystem") << "process entity " << e.getId();

      bool dirty = kinect.isFrameNew();
      depth_m.get(e)->dirty = dirty; 
      rgb_m.get(e)->dirty = dirty; 

      if ( dirty )
      {
        depth_m.get(e)->update( kinect.getRawDepthPixels() );
        depth_m.get(e)->update( kinect.getDepthPixels() );
        rgb_m.get(e)->update( kinect.getPixels() );
      }

    };

    virtual void render_entity(Entity &e)
    {

      RenderComponent* render_data = component<RenderComponent>("output");
      int w = render_data->width;
      int h = render_data->height;

      ofSetColor(255);

      if ( rgb_m.get(e)->render )
        kinect.draw( 0, 0, w, h );

      if ( depth_m.get(e)->render )
        kinect.drawDepth( 0, 0, w, h ); 

    };

  private: 

    ComponentMapper<RGBComponent> rgb_m;
    ComponentMapper<DepthComponent> depth_m;

    ofxKinect kinect; 
};
