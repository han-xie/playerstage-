#include "stage.hh"
using namespace Stg;
#include "ctrlHeadFile.h"

const double cruisespeed = 0.4; 
const double avoidspeed = 0.05; 
const double avoidturn = 0.5;
const double minfrontdistance = 1.0; // 0.6  
const bool verbose = false;
const double stopdist = 0.3;
const int avoidduration = 10;

typedef struct
{
  ModelPosition* pos;
  ModelLaser* laser;
  int avoidcount, randcount;
} robot_t;

int LaserUpdate( Model* mod, robot_t* robot );
int PositionUpdate( Model* mod, robot_t* robot );

// Stage calls this when the model starts up
extern "C" int Init( Model* mod, CtrlArgs* args )
{
  // local arguments
	/*  printf( "\nWander controller initialised with:\n"
			"\tworldfile string \"%s\"\n" 
			"\tcmdline string \"%s\"",
			args->worldfile.c_str(),
			args->cmdline.c_str() );
	*/

#ifdef CTRLDEBUG
	printf("wander init begin\n");
#endif

  robot_t* robot = new robot_t;
 
  robot->avoidcount = 0;
  robot->randcount = 0;
  
#ifdef CTRLDEBUG
	printf("wander init begin--pos\n");
#endif
  robot->pos = (ModelPosition*)mod;
#ifdef CTRLDEBUG
	printf("wander init begin--laser\n");
#endif
  robot->laser = (ModelLaser*)mod->GetChild( "laser:0" );
#ifdef CTRLDEBUG
	printf("wander init begin--addCallback\n");
#endif
  robot->pos->AddUpdateCallback( (stg_model_callback_t)LaserUpdate, robot );
#ifdef CTRLDEBUG
	printf("wander init begin--subscribe-laser\n");
#endif  
  robot->laser->Subscribe(); // starts the laser updates
#ifdef CTRLDEBUG
	printf("wander init begin--subscribe-pos\n");
#endif
  robot->pos->Subscribe(); // starts the position updates

#ifdef CTRLDEBUG
	printf("wander init end\n");
#endif
    
  return 0; //ok
}


// inspect the laser data and decide what to do
int LaserUpdate( Model* mod, robot_t* robot )
{

#ifdef CTRLDEBUG
	printf("wander laserUpdate begin\n");
#endif
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
    
#ifdef CTRLDEBUG
	printf("wander laserUpdate end\n");
#endif

  return 0;
}

int PositionUpdate( Model* mod, robot_t* robot )
{

#ifdef CTRLDEBUG
	printf("wander positionUpdate begin\n");
#endif
  Pose pose = robot->pos->GetPose();

  printf( "Pose: [%.2f %.2f %.2f %.2f]\n",
	  pose.x, pose.y, pose.z, pose.a );

#ifdef CTRLDEBUG
	printf("wander positionUpdate end\n");
#endif

  return 0; // run again
}

