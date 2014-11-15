#pragma once

#include <Artemis/Artemis.h>
#include "ofxECS.h"
#include "components/Components.h"
#include "ofxCamaraLucida.h"

using namespace artemis;

class CamaraLucidaSystem : public ECSsystem 
{ 

  public:

    CamaraLucidaSystem(string _id) : ECSsystem(_id)
    {
      addComponentType<CamaraLucidaComponent>();
      addComponentType<RenderComponent>();
    };

    virtual void initialize() 
    {
      cml_m.init( *world );
      render_m.init( *world );
    };

    virtual void added(Entity &e) 
    {
      //cml::CamaraLucida* cml = cml_m.get(e)->cml;
      //float w = cml->tex_width();
      //float h = cml->tex_height();
      //render_m.get(e)->update(w,h);
    };

    virtual void processEntity(Entity &e) 
    {
      DepthComponent* depth = component<DepthComponent>("input");

      if ( depth->dirty )
      {
        cml_m.get(e)->cml->update( depth->depth_pix_mm );
      }
    };

    virtual void processEntities( ImmutableBag<Entity*>& bag ) 
    {
      int len = bag.getCount();

      if ( len == 1 )
        processEntity( *bag.get(0) );

      else if ( len > 1 )
        ofLogError("CamaraLucidaSystem") << "expected only 1 entity with CamaraLucidaComponent and got " << len;

      //for ( int i = 0; i < len; i++ )
        //processEntity( *bag.get(i) );
    };

    virtual void render_entity(Entity &e)
    {
      if ( !cml_m.get(e)->render_hue_tex )
        return;

      DepthComponent* depth = component<DepthComponent>("input");

      RenderComponent* render_data = render_m.get(e);
      int w = render_data->width;
      int h = render_data->height;

      ofSetColor(255);
      cml_m.get(e)->cml->depth_camera()->get_hue_tex_ref( depth->depth_pix_mm ).draw( 0, 0, w, h ); 
    };

  private:

    ComponentMapper<CamaraLucidaComponent> cml_m;
    ComponentMapper<RenderComponent> render_m;

};

