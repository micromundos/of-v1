#pragma once

#include "ofMain.h"
#include <Artemis/Artemis.h>
#include "ofxECS.h"
#include "ofxCamaraLucida.h"
#include "keys.h"

class CamaraLucidaComponent : public ECScomponent 
{
  public:

    cml::CamaraLucida* cml;

    ofParameter<bool> enabled;
    ofParameter<float> xoff;
    ofParameter<float> render_background;
    ofParameter<bool> render_hue_tex;

    ofParameter<bool> tweak_reset;
    ofParameter<bool> tweak_save;
    ofParameter<bool> tweak_load;

    ofParameter<bool> unity_calib_save;
    string tweak_file;
    string unity_calib_file;

    CamaraLucidaComponent(string _id) : ECScomponent(_id) 
    { 
      cml = NULL; 
    };

    virtual ~CamaraLucidaComponent()
    {
      delete cml;
    };

    virtual void init( string e_id, Json::Value d, ECSparams* p )
    {
      ECScomponent::init(e_id,d,p);

      param( enabled, "enabled" );
      param( xoff, "xoff" );
      param( render_background, "render_background" );
      param( render_hue_tex, "render_hue_tex" );

      param( unity_calib_save, "unity_calib_save" );
      param( tweak_reset, "tweak_reset" );
      param( tweak_save, "tweak_save" );
      param( tweak_load, "tweak_load" );

      tweak_file = d["calibration"]["tweak_file"].asString();
      unity_calib_file = d["calibration"]["unity_calib_file"].asString();

      init_cml( 
        d.get( "tex_width", 1024 ).asInt(), 
        d.get( "tex_height", 768 ).asInt(), 
        d.get( "mesh_res", 2 ).asInt(),
        d["calibration"]["kinect_intrinsics_file"].asString(),
        d["calibration"]["projector_intrinsics_file"].asString(),
        d["calibration"]["projector_kinect_extrinsics_file"].asString() );
    }; 

  private:

    void init_cml( 
        int tex_width, 
        int tex_height, 
        int mesh_res,
        string kinect_intrinsics_file,
        string projector_intrinsics_file,
        string projector_kinect_extrinsics_file )
    {
      if ( cml != NULL )
      {
        ofLogWarning("CamaraLucidaComponent") << "init_cml: cml is already inited";
        return;
      }

      cml::Config cfg;

      cfg.kinect_intrinsics_file = kinect_intrinsics_file;
      cfg.projector_intrinsics_file = projector_intrinsics_file;
      cfg.projector_kinect_extrinsics_file = projector_kinect_extrinsics_file;

      cfg.key_debug = keys::cml_key_debug;
      cfg.key_help = keys::cml_key_help;
      cfg.key_scene_zoom = keys::cml_key_scene_zoom;
      cfg.key_scene_reset = keys::cml_key_scene_reset;
      cfg.key_view_next = keys::cml_key_view_next;

      cfg.tex_width = tex_width;
      cfg.tex_height = tex_height;
      cfg.mesh_res = mesh_res;

      cml = new cml::CamaraLucida( cfg );
    };
};

