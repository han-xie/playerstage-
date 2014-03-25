#include "stage.hh"
using namespace Stg;
#include "ctrlHeadFile.h"

/*const double cruisespeed = 0.4;
const double avoidspeed = 0.05; 
const double avoidturn = 0.5;
const double minfrontdistance = 1.0; // 0.6  
const bool verbose = false;
const double stopdist = 0.3;
const int avoidduration = 10;*/

typedef struct
{
  ModelPosition* posTop;
  ModelPosition* posButton;
  //int avoidcount, randcount;
} robot_t;


int PositionUpdate( Model* mod, robot_t* robot );

// Stage calls this when the model starts up
extern "C" int Init( Model* mod, CtrlArgs* args )
{

  robot_t* robot = new robot_t;
 
  robot->posTop = (ModelPosition*)mod->GetChild("position2d:0");
  robot->posButton = (ModelPosition*)mod->GetChild("position2d:1");
  robot->posTop->AddUpdateCallback( (stg_model_callback_t)PositionUpdate, robot );
  robot->posTop->Subscribe();
  return 0; //ok
}


int PositionUpdate( Model* mod, robot_t* robot )
{

  Pose pose = robot->pos->GetPose();

  printf( "Pose: [%.2f %.2f %.2f %.2f]\n",
	  pose.x, pose.y, pose.z, pose.a );





  // get the data
  uint32_t sample_count=0;
	ModelLaser::Sample* scan = robot->laser->GetSamples( &sample_count );
  if( ! scan )
    return 0;
  
  bool obstruction = false;
  bool stop = false;

  // find the closest distance to the left and right and check if
  // there's anything in front
  double minleft = 1e6;
  double minright = 1e6;

  for (uint32_t i = 0; i < sample_count; i++)
    {

		if( verbose ) printf( "%.3f ", scan[i].range );

      if( (i > (sample_count/3)) 
			 && (i < (sample_count - (sample_count/3))) 
			 && scan[i].range < minfrontdistance)
		  {
			 if( verbose ) puts( "  obstruction!" );
			 obstruction = true;
		  }
		
      if( scan[i].range < stopdist )
		  {
			 if( verbose ) puts( "  stopping!" );
			 stop = true;
		  }
      
      if( i > sample_count/2 )
				minleft = std::min( minleft, scan[i].range );
      else      
				minright = std::min( minright, scan[i].range );
    }
  
  if( verbose ) 
	 {
		puts( "" );
		printf( "minleft %.3f \n", minleft );
		printf( "minright %.3f\n ", minright );
	 }

  if( obstruction || stop || (robot->avoidcount>0) )
    {
      if( verbose ) printf( "Avoid %d\n", robot->avoidcount );
	  		
      robot->pos->SetXSpeed( stop ? 0.0 : avoidspeed );      
      
      /* once we start avoiding, select a turn direction and stick
	 with it for a few iterations */
      if( robot->avoidcount < 1 )
        {
			 if( verbose ) puts( "Avoid START" );
          robot->avoidcount = random() % avoidduration + avoidduration;
			 
			 if( minleft < minright  )
				{
				  robot->pos->SetTurnSpeed( -avoidturn );
				  if( verbose ) printf( "turning right %.2f\n", -avoidturn );
				}
			 else
				{
				  robot->pos->SetTurnSpeed( +avoidturn );
				  if( verbose ) printf( "turning left %2f\n", +avoidturn );
				}
        }
		
      robot->avoidcount--;
    }
  else
    {
      if( verbose ) puts( "Cruise" );

      robot->avoidcount = 0;
      robot->pos->SetXSpeed( cruisespeed );	  
		robot->pos->SetTurnSpeed(  0 );
    }

 //  if( robot->pos->Stalled() )
// 	 {
// 		robot->pos->SetSpeed( 0,0,0 );
// 		robot->pos->SetTurnSpeed( 0 );
// 	 }
    


  return 0;


  return 0; // run again
}

