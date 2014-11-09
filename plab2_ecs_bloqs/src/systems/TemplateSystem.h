#pragma once

#include <Artemis/Artemis.h>
#include "ofxECS.h"
#include "components/Components.h"

using namespace artemis;

class TemplateSystem : public ECSsystem 
{ 

  public:

    TemplateSystem(string _id) : ECSsystem(_id)
    {
      addComponentType<TemplateComponent>();
    };

    virtual void initialize() 
    {
      template_m.init( *world );
    };

    virtual void added(Entity &e) 
    {
      //template_m.get(e)->data;
    };

    virtual void processEntity(Entity &e) 
    {
      //ofLogNotice("TemplateSystem") << "process entity " << e.getId();
      //template_m.get(e)->data;
    };

    virtual void processEntities( ImmutableBag<Entity*>& bag ) 
    {
      int len = bag.getCount();
      for ( int i = 0; i < len; i++ )
        processEntity( *bag.get(i) );
    };

    virtual void render()
    {};

  private:

    ComponentMapper<TemplateComponent> template_m;

};

