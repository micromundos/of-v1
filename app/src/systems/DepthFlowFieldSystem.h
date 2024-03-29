#pragma once

#include <Artemis/Artemis.h>
#include "ofxECS.h"
#include "Components.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGPGPU.h"
#include "ofxTimeMeasurements.h"

using namespace artemis;

class DepthFlowFieldSystem : public ECSsystem 
{ 

  public:

    DepthFlowFieldSystem(string _id) : ECSsystem(_id)
    {
      addComponentType<DepthProcessingComponent>();
      addComponentType<DepthComponent>();
      addComponentType<FlowFieldComponent>();
    };

    virtual void initialize() 
    {
      depth_m.init( *world );
      flowfield_m.init( *world );
      depth_processing_m.init( *world );
      //entradas_m.init(*world);
      entity = NULL;
      scale = 0.3;
    };

    virtual void added(Entity &e) 
    {
      if ( entity != NULL )
      {
        ofLogWarning("DepthFlowFieldSystem")
          << "entity already added";
        return;
      }
      entity = &e;

      DepthComponent* depth_data = depth_m.get(e);

      int w = depth_data->width * scale;
      int h = depth_data->height * scale;

      flowfield(e)
        .init("glsl/flowfield.frag",w,h)
        .on( "update", this, &DepthFlowFieldSystem::update_flowfield ); 
    };

    virtual void processEntity(Entity &e) 
    { 
      DepthComponent* depth_data = depth_m.get(e);
      FlowFieldComponent* ff_data = flowfield_m.get(e); 

      if ( ! depth_data->dirty ) return;

      TS_START("DepthFlowFieldSystem");

      flowfield(e)
        .set( "height_map", surfaces(e).get() )
        //.set( "height_map", height_map(e).get() )
        //.set( "entradas", entradas(e).get() )
        .update();

      flowfield(e).update_debug( ff_data->render );

      TS_STOP("DepthFlowFieldSystem");
    };

    virtual void renderEntity(Entity &e)
    {
      FlowFieldComponent* ff_data = flowfield_m.get(e); 

      if ( !ff_data->render )
        return;

      TS_START("DepthFlowFieldSystem render");

      RenderComponent* render_data = component<RenderComponent>("output");

      //entradas(e).get().draw(0,0,render_data->width, render_data->height );

      flowfield(e).render_debug( 0, 0, render_data->width, render_data->height );
      //flowfield(e).get().draw( 0, 0, render_data->width, render_data->height );
      //surfaces(e).get().draw( 0, 0, render_data->width, render_data->height );

      TS_STOP("DepthFlowFieldSystem render");
    };

    virtual void removed(Entity &e) 
    {
      flowfield(e).off( "update", this, &DepthFlowFieldSystem::update_flowfield ); 
      entity = NULL;
    };

  private:

    float scale;
    Entity* entity;

    ComponentMapper<DepthProcessingComponent> depth_processing_m;
    ComponentMapper<DepthComponent> depth_m;
    ComponentMapper<FlowFieldComponent> flowfield_m;
    //ComponentMapper<EntradasComponent> entradas_m;

    void update_flowfield( ofShader& shader )
    {
      DepthProcessingComponent* depth_proc_data = depth_processing_m.get( *entity );
      FlowFieldComponent* ff_data = flowfield_m.get( *entity ); 

      shader.setUniform1f( "table_height", depth_proc_data->threshold_table_near );
      shader.setUniform1f( "force_weight_min", ff_data->force_weight_min );
      shader.setUniform1f( "force_weight_max", ff_data->force_weight_max * ff_data->force_weight_max_m );
    };

    gpgpu::Process& flowfield(Entity &e)
    {
      return flowfield_m.get(e)->flowfield();
    };

    gpgpu::Process& surfaces(Entity &e)
    {
      return depth_processing_m.get(e)->surfaces();
    };

    gpgpu::Process& height_map(Entity &e)
    {
      return depth_processing_m.get(e)->height_map();
    };

    //gpgpu::Process& entradas(Entity &e)
    //{
      //return entradas_m.get(e)->output();
    //};
};

