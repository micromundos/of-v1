#pragma once

#include <Artemis/Artemis.h>

class ECSsystem : public artemis::EntityProcessingSystem
{

  protected:

  //some shortcuts

  template<typename TSystem>
  TSystem* system()
  {
    return ((TSystem*)world->getSystemManager()->getSystem<TSystem>());
  };

  template<typename TComponent>
  TComponent* component(string tag)
  {
    return ((TComponent*)world->getTagManager()->getEntity( tag ).getComponent<TComponent>());
  };

  artemis::EntityManager* entities()
  {
    return world->getEntityManager();
  };

  artemis::SystemManager* systems()
  {
    return world->getSystemManager();
  };

  artemis::TagManager* tags()
  {
    return world->getTagManager();
  };

};
