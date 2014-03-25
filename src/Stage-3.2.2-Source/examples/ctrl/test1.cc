#include "stage.hh"
using namespace Stg;
#include "ctrlHeadFile.h"
#include <vector>


typedef struct
{
  std::vector<ModelPosition*> pos;
} robot_t;

int PositionUpdate( Model* mod, robot_t* robot );

// Stage calls this when the model starts up
extern "C" int Init( Model* mod, CtrlArgs* args )
{
  robot_t* robot = new robot_t;
  ModelPosition* pmpos = (ModelPosition*)mod->GetChild("position2d:1");
  robot->pos.push_back(pmpos);

  mod->AddUpdateCallback( (stg_model_callback_t)PositionUpdate, robot );
  mod->Subscribe(); // starts the position updates
  
  return 0; //ok
}


int PositionUpdate( Model* mod, robot_t* robot )
{
  Pose pose = mod->GetPose();

  // assign some values:
  for (unsigned i=0; i<robot->pos.size(); i++){
	  robot->pos.at(i)->SetPose(pose);
  }

  return 0; // run again
}

