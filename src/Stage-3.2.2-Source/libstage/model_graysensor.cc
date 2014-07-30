///////////////////////////////////////////////////////////////////////////
//
// File: model_graysensor.c
// Author: Richard Vaughan
// Date: 10 June 2004
//
// CVS info:
//  $Source: /home/tcollett/stagecvs/playerstage-cvs/code/stage/libstage/model_graysensor.cc,v $
//  $Author: rtv $
//  $Revision: 1.3 $
//
///////////////////////////////////////////////////////////////////////////

//#define DEBUG 

#include <sys/time.h>

#include "stage.hh"
#include "option.hh"
#include "worldfile.hh"
using namespace Stg;


static const stg_watts_t DEFAULT_GRAYSENSORWATTS = 2.0;
static const stg_meters_t DEFAULT_GRAYSENSORRANGE = 12.0;
static const stg_radians_t DEFAULT_GRAYSENSORFOV = M_PI/3.0;
static const stg_radians_t DEFAULT_GRAYSENSORPAN = 0.0;
static const unsigned int DEFAULT_GRAYSENSORINTERVAL_MS = 100;
static const unsigned int DEFAULT_GRAYSENSORRESOLUTION = 1;
static const unsigned int DEFAULT_GRAYSENSORSCANWIDTH = 80;
static const unsigned int DEFAULT_GRAYSENSORSCANHEIGHT = 60;

/**
  @ingroup model
  @defgroup model_graysensor Graysensor model

  The graysensor model simulates a color-gray-finding vision device,
  like a CMUCAM2, or the ACTS image processing software. It can track
  areas of color in a simulated 2D image, giving the location and size
  of the color 'grays'. Multiple colors can be tracked at once; they are
  separated into channels, so that e.g. all red objects are tracked as
  channel one, blue objects in channel two, etc. The color associated
  with each channel is configurable.

API: Stg::ModelGraysensor

<h2>Worldfile properties</h2>

@par Summary and default values

@verbatim
graysensor
(
  # graysensor properties
  colors_count 0
  colors [ ]
  image[ 80 60 ]
  range 12.0
  fov 3.14159/3.0
  pan 0.0

  # model properties
  size [ 0.0 0.0 0.0 ]
)
@endverbatim

@par Details
 
- colors_count <int>\n
  number of colors being tracked
- colors [ col1:<string> col2:<string> ... ]\n
  A list of quoted strings defining the colors detected in each channel, using color names from the X11-style color database (ex. "black", "red").
  The number of strings should match colors_count.
- image [ width:<int> height:<int> ]\n
  dimensions of the image in pixels. This determines the graysensor's resolution
- range <float>\n
  maximum range of the sensor in meters.

 */

  
  ModelGraysensor::ModelGraysensor( World* world,
												Model* parent,
												const std::string& type ) : 
  Model( world, parent, type ),
						vis( world ),
						grays(),
						colors(),
						fov( DEFAULT_GRAYSENSORFOV ),
						pan( DEFAULT_GRAYSENSORPAN ),
						range( DEFAULT_GRAYSENSORRANGE ),
						scan_height( DEFAULT_GRAYSENSORSCANHEIGHT ),
						scan_width( DEFAULT_GRAYSENSORSCANWIDTH )
{
  /*PRINT_DEBUG2( "Constructing ModelGraysensor %d (%s)\n",
					 id, typestr );*/
	  PRINT_DEBUG2( "Constructing ModelGraysensor %d (%s)\n",
	  					 id, type.data() );
  ClearBlocks();
  
  AddVisualizer( &this->vis, true );
}


ModelGraysensor::~ModelGraysensor( void )
{
}

static bool gray_match( Model* candidate,
								Model* finder,
								const void* dummy )
{ 
  (void)dummy; // avoid warning about unused var
  if(candidate->vis.gray_return==false) return false;
  return( ! finder->IsRelated( candidate ));
}	


static bool ColorMatchIgnoreAlpha( Color a, Color b )
{
  double epsilon = 1e-5; // small
  return( fabs(a.r - b.r) < epsilon &&
		  fabs(a.g - b.g) < epsilon &&
		  fabs(a.b - b.b) < epsilon );
}

void ModelGraysensor::ModelGraysensor::AddColor( Color col )
{
	colors.push_back( col );
}

/** Stop tracking grays with this color */
void ModelGraysensor::RemoveColor( Color col )
{
	FOR_EACH( it, colors )
		{
			if( (*it) ==  col  )
				it = colors.erase(it);
		}
}

/** Stop tracking all colors. Call this to clear the defaults, then
  add colors individually with AddColor(); */
void ModelGraysensor::RemoveAllColors()
{
	colors.clear();
}

void ModelGraysensor::Load( void )
{  
	Model::Load();

	Worldfile* wf = world->GetWorldFile();

	scan_width = (int)wf->ReadTupleFloat( wf_entity, "image", 0, scan_width );
	scan_height= (int)wf->ReadTupleFloat( wf_entity, "image", 1, scan_height );
	range = wf->ReadFloat( wf_entity, "range", range );
	fov = wf->ReadAngle( wf_entity, "fov", fov );
	pan = wf->ReadAngle( wf_entity, "pan", pan );
	std::string portname=wf->ReadString(wf_entity,"port","a0");
	port=Model::GetCYZXPort(portname.c_str());

	if( wf->PropertyExists( wf_entity, "colors" ) )
	{
		RemoveAllColors(); // empty the color list to start from scratch

		unsigned int count = wf->ReadInt( wf_entity, "colors_count", 0 );

		for( unsigned int c=0; c<count; c++ )
		{
			const char* colorstr = 
				wf->ReadTupleString( wf_entity, "colors", c, NULL );

			if( ! colorstr )
				break;
			else
			  AddColor( Color( colorstr ));
		}
	}    
}


void ModelGraysensor::Update( void )
{
	// generate a scan for post-processing into a gray image
	
	stg_raytrace_result_t* samples = new stg_raytrace_result_t[scan_width];

	Raytrace( pan, range, fov, gray_match, NULL, samples, scan_width, false );

	// now the colors and ranges are filled in - time to do gray detection
	double yRadsPerPixel = fov / scan_height;

	grays.clear();

	// scan through the samples looking for color grays
	for(unsigned int s=0; s < scan_width; s++ )
	  {
		 if( samples[s].mod == NULL  )
			continue; // we saw nothing
		 
		 unsigned int right = s;
		 Color graycol = samples[s].color;
		 
		 //printf( "gray start %d color %X\n", grayleft, graycol );
		 
		 // loop until we hit the end of the gray
		 // there has to be a gap of >1 pixel to end a gray
		 // this avoids getting lots of crappy little grays
		 while( s < scan_width && samples[s].mod && 
				  ColorMatchIgnoreAlpha( samples[s].color, graycol) )
			{
			  //printf( "%u graycol %X block %p %s color %X\n", s, graycol, samples[s].block, samples[s].block->Model()->Token(), samples[s].block->Color() );
			  s++;
			}
		 
		 unsigned int left = s - 1;

		//if we have color filters in place, check to see if we're looking for this color
		if( colors.size() )
		{
			bool found = false;

			for( unsigned int c=0; c<colors.size(); c++ )
				if( ColorMatchIgnoreAlpha( graycol, colors[c]))
				{
					found = true;
					break;
				}
			if( ! found )
				continue; // continue scanning array for next gray
		}

		//printf( "gray end %d %X\n", grayright, graycol );

		double robotHeight = 0.6; // meters

		// find the average range to the gray;
		stg_meters_t range = 0;
		for( unsigned int t=right; t<=left; t++ )
			range += samples[t].range;
		range /= left-right + 1;

		double startyangle = atan2( robotHeight/2.0, range );
		double endyangle = -startyangle;
		int graytop = scan_height/2 - (int)(startyangle/yRadsPerPixel);
		int graybottom = scan_height/2 -(int)(endyangle/yRadsPerPixel);

		graytop = std::max( graytop, 0 );
		graybottom = std::min( graybottom, (int)scan_height );

		// fill in an array entry for this gray
		Gray gray;
		gray.color = graycol;
		gray.left = scan_width - left - 1;
		gray.top = graytop;
		gray.right = scan_width - right - 1;;
		gray.bottom = graybottom;
		gray.range = range;

		//printf( "Robot %p sees %d xpos %d ypos %d\n",
		//  mod, gray.color, gray.xpos, gray.ypos );

		// add the gray to our stash
		//g_array_append_val( grays, gray );
		grays.push_back( gray );
	}

	delete [] samples;

	Model::Update();
}


void ModelGraysensor::Startup(  void )
{ 
	Model::Startup();

	PRINT_DEBUG( "graysensor startup" );

	// start consuming power
	SetWatts( DEFAULT_GRAYSENSORWATTS );
}

void ModelGraysensor::Shutdown( void )
{ 

	PRINT_DEBUG( "graysensor shutdown" );

	// stop consuming power
	SetWatts( 0 );

	// clear the data - this will unrender it too
	grays.clear();

	Model::Shutdown();
}

//******************************************************************************
// visualization

//TODO make instance attempt to register an option (as customvisualizations do)
// Option ModelGraysensor::showGrayData( "Show Graysensor", "show_gray", "", true, NULL );

ModelGraysensor::Vis::Vis( World* world )
  : Visualizer( "Graysensor", "graysensor_vis" )
{
  
  //world->RegisterOption( &showArea );
  //world->RegisterOption( &showStrikes );
  //world->RegisterOption( &showFov );
  //world->RegisterOption( &showBeams );		  
}

void ModelGraysensor::Vis::Visualize( Model* mod, Camera* cam )
{
  ModelGraysensor* bf( dynamic_cast<ModelGraysensor*>(mod) );
  
  if( bf->debug )
	{
	  // draw the FOV
	  GLUquadric* quadric = gluNewQuadric();
	  
	  bf->PushColor( 0,0,0,0.2  );
	  
	  gluQuadricDrawStyle( quadric, GLU_SILHOUETTE );
	  gluPartialDisk( quadric,
							0, 
							bf->range,
							20, // slices	
							1, // loops
							rtod( M_PI/2.0 + bf->fov/2.0 - bf->pan), // start angle
							rtod(-bf->fov) ); // sweep angle
	  
	  gluDeleteQuadric( quadric );
	  bf->PopColor();
	}
  
  if( bf->subs < 1 )
	 return;
  
  glPushMatrix();

	// return to global rotation frame
  Pose gpose( bf->GetGlobalPose() );
  glRotatef( rtod(-gpose.a),0,0,1 );
  
  // place the "screen" a little away from the robot
  glTranslatef( -2.5, -1.5, 0.5 );
  
  // rotate to face screen
  float yaw, pitch;
  pitch = - cam->pitch();
  yaw = - cam->yaw();
  float robotAngle = -rtod(bf->pose.a);
  glRotatef( robotAngle - yaw, 0,0,1 );
  glRotatef( -pitch, 1,0,0 );
  
  // convert gray pixels to meters scale - arbitrary
  glScalef( 0.025, 0.025, 1 );
  
  // draw a white screen with a black border
  bf->PushColor( 1,1,1,1 );
  glRectf( 0,0, bf->scan_width, bf->scan_height );
  bf->PopColor();
  
  glTranslatef(0,0,0.01 );
  
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  bf->PushColor( 1,0,0,1 );
  glRectf( 0,0, bf->scan_width, bf->scan_height );
  bf->PopColor();
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  
  // draw the grays on the screen
  for( unsigned int s=0; s<bf->grays.size(); s++ )
	 {
		Gray* b = &bf->grays[s];
		//stg_graysensor_gray_t* b =
		//&g_array_index( grays, stg_graysensor_gray_t, s);
		
		bf->PushColor( b->color );
		glRectf( b->left, b->top, b->right, b->bottom );

		//printf( "%u l %u t%u r %u b %u\n", s, b->left, b->top, b->right, b->bottom );
		bf->PopColor();
	 }
  
  glPopMatrix();
}


