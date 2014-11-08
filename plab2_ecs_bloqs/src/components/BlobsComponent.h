#pragma once

#include "ofMain.h"
#include <Artemis/Artemis.h>
#include "ecs/ECScomponent.h"

class BlobsComponent : public ECScomponent 
{
  public:
    vector<ofPolyline> blobs;

    BlobsComponent(string _id) : ECScomponent(_id) {};

};

