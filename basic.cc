/////////////////////////////////
// File: basic.cc
// Desc: minimal controller example
// Created: 2011.10.19
// Author: Richard Vaughan <vaughan@sfu.ca>
// License: GPL
/////////////////////////////////

#include "stage.hh"
using namespace Stg;

static ModelPosition* position(NULL);

/** this is added as a callback to a ranger model, and is called
    whenever the model is updated by Stage. 
*/
int RangerUpdateCb( ModelRanger* mod, void* dummy)
{  	
  const std::vector<ModelRanger::Sensor>& sensors = mod->GetSensors();
  
  // ( inspect the ranger data and decide what to do )
  
  // output something to prove it is working  
  printf( "Hello simulated world\n" ); // console
  mod->Say( "Hello" ); // GUI window
  position->SetSpeed( 0.4, 0, 0.1 );  // output a speed command (X, Y, Z)

  Model* res = position->GetWorld()->GetModel( "resource_name:0" );
  if( res )
    printf( "res %p %s\n", res, res->Token() );
  else
    puts( "res not found" );

  static int count=0;

  if( res && (++count % 50 == 0) )
    {
      res->SetParent( NULL );
      res->SetPose( Pose(0,0,0,0) );
    }

  return 0;
}

/** Stage calls this when the model starts up 
 */
extern "C" int Init( Model* mod )
{
  // the controller is attached to a position model, so Init is called
  // with a pointer to this model.
  position = (ModelPosition*)mod;
  position->Subscribe(); // models are only updated if someone is subscribed to them

  // we obtain a pointer to the first unused ranger that is a child of this position model
  ModelRanger* ranger = (ModelRanger*) mod->GetUnusedModelOfType( "ranger" );

  // install a callback function that will be called every time the
  // ranger model is updated by the simulator
  ranger->AddCallback( Model::CB_UPDATE, (model_callback_t)RangerUpdateCb, NULL );
  ranger->Subscribe(); // models are only updated if someone is subscribed to them


  Model* resource = new Model(mod->GetWorld(), NULL, "resource_name");

  printf( "added model %s\n", resource->Token() );

  resource->SetColor( Color("blue") );
  mod->GetWorld()->AddModel(resource);
 
  resource->SetParent(position);
 
  return 0; //ok
}
 

