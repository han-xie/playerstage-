// file: model_cyzxlaser.c
// Author: Richard Vaughan
// Date: 10 June 2004
//
// CVS info:
//  $Source: /home/tcollett/stagecvs/playerstage-cvs/code/stage/libstage/model_cyzxlaser.cc,v $
//  $Author: rtv $
//  $Revision: 1.7 $
//
///////////////////////////////////////////////////////////////////////////

//#define DEBUG 

#include <sys/time.h>

#include "stage.hh"
#include "worldfile.hh"
#include "option.hh"
using namespace Stg;

// DEFAULT PARAMETERS FOR cyzxLASER MODEL
static const bool DEFAULT_FILLED = true;
static const stg_watts_t DEFAULT_WATTS = 17.5;
static const Size DEFAULT_SIZE(0.15, 0.15, 0.2);
static const stg_meters_t DEFAULT_MAXRANGE = 8.0;
static const stg_radians_t DEFAULT_FOV = M_PI;
static const unsigned int DEFAULT_SAMPLES = 180;
static const unsigned int DEFAULT_RESOLUTION = 1;
static const char* DEFAULT_COLOR = "blue";
//static int cyzxlaserC = 360; // add by ke to control cyzxlaser numbers
static int cyzxlaserC = 359;
//static int cyzxlaserA = 360;
static int cyzxlaserA = 359;
static int cyzxlaserPerA = cyzxlaserC / cyzxlaserA;
static const unsigned int DEFAULT_CYZXcyzxLASER = 22;

Option ModelcyzxLaser::Vis::showArea("cyzxLaser scans", "show_cyzxlaser", "", true, NULL);
Option ModelcyzxLaser::Vis::showStrikes("cyzxLaser strikes", "show_cyzxlaser_strikes", "",
		false, NULL);
Option ModelcyzxLaser::Vis::showFov("cyzxLaser FOV", "show_cyzxlaser_fov", "", false, NULL);
Option ModelcyzxLaser::Vis::showBeams("cyzxLaser beams", "show_cyzxlaser_beams", "", false,
		NULL);

/**
 @ingroup model
 @defgroup model_cyzxlaser cyzxLaser model
 The cyzxlaser model simulates a scanning cyzxlaser rangefinder

 API: Stg::ModelcyzxLaser

 <h2>Worldfile properties</h2>

 @par Summary and default values

 @verbatim
 cyzxlaser
 (
 # cyzxlaser properties
 samples 180
 range_max 8.0
 fov 3.14159
 resolution 1

 # model properties
 size [ 0.15 0.15 0.2 ]
 color "blue"
 )
 @endverbatim

 @par Details
 
 - samples <int>\n
 the number of cyzxlaser samples per scan
 - range_max <float>\n
 the maximum range reported by the scanner, in meters. The scanner will not detect objects beyond this range.
 - fov <float>\n
 the angular field of view of the scanner, in radians.
 - resolution <int>\n
 Only calculate the true range of every nth cyzxlaser sample. The missing samples are filled in with a linear interpolation. Generally it would be better to use fewer samples, but some (poorly implemented!) programs expect a fixed number of samples. Setting this number > 1 allows you to reduce the amount of computation required for your fixed-size cyzxlaser vector.
 */

ModelcyzxLaser::ModelcyzxLaser(World* world, Model* parent, const std::string& type) :
		Model(world, parent, type), vis(world), sample_count(DEFAULT_SAMPLES),
		cyzxlaserCount(cyzxlaserC), cyzxlaserAngle(cyzxlaserA), samples(),
		range_max(DEFAULT_MAXRANGE), fov(DEFAULT_FOV), resolution(
				DEFAULT_RESOLUTION) {
	/*PRINT_DEBUG2( "Constructing ModelcyzxLaser %d (%s)\n",
	 id, typestr );*/
	PRINT_DEBUG2( "Constructing ModelcyzxLaser %d (%s)\n",
			id, type.data() );

	SetGeom(Geom(Pose(0, 0, 0, 0), DEFAULT_SIZE));

	// assert that Update() is reentrant for this derived model
	thread_safe = true;

	SetColor(Color(DEFAULT_COLOR));

	// set up our data buffers and raytracing
	SampleConfig();
	//CYZXcyzxLaserConfig();

	// start consuming power
	watts = DEFAULT_WATTS;

	AddVisualizer(&vis, true);

	openHiddenModel();
}

ModelcyzxLaser::~ModelcyzxLaser(void) {
}

void ModelcyzxLaser::Load(void) {
	/*sample_count = wf->ReadInt(wf_entity, "samples", sample_count);
	fov = wf->ReadAngle(wf_entity, "fov", fov);*/

	 //I control the cyzxlaser number
	 sample_count = 0;
	 fov =  0;

	range_max = wf->ReadLength(wf_entity, "range_max", range_max);
	resolution = wf->ReadInt(wf_entity, "resolution", resolution);
	lcd = wf->ReadInt(wf_entity,"lcd",1);
	sound = wf->ReadInt(wf_entity,"sound",1);


	if (resolution < 1) {
		PRINT_WARN( "cyzxlaser resolution set < 1. Forcing to 1");
		resolution = 1;
	}

	Model::Load();

	SampleConfig();

	LoadFixModel();
}

ModelcyzxLaser::Config ModelcyzxLaser::GetConfig() {
	Config cfg;
	cfg.sample_count = sample_count;
	cfg.range_bounds.max = range_max;
	cfg.fov = fov;
	cfg.resolution = resolution;
	cfg.interval = interval;
	return cfg;
}

void ModelcyzxLaser::SetConfig(Config& cfg) {
	range_max = cfg.range_bounds.max;
	fov = cfg.fov;
	resolution = cfg.resolution;
	interval = cfg.interval;
	sample_count = cfg.sample_count;

	SampleConfig();
}

static bool cyzxlaser_raytrace_match(Model* hit, Model* finder, const void* dummy) {
	(void) dummy; // avoid warning about unused var

	// Ignore the model that's looking and things that are invisible to
	// cyzxlasers
	return ((hit != finder) && (hit->vis.cyzxlaser_return > 0));
}

void ModelcyzxLaser::SampleConfig() {
	samples.resize(sample_count);
}

void ModelcyzxLaser::openHiddenModel() {
	/*for(int i=0;i<DEFAULT_CYZXcyzxLASER;i++){
	 if(strcmp(cyzxcyzxlaserc[i].type.data(),"hallSen")==0)
	 hallupdate = true;
	 if(strcmp(cyzxcyzxlaserc[i].type.data(),"graySen")==0)
	 grayupdate = true;
	 }*/
}
void ModelcyzxLaser::LoadFixModel() {
	//load temperature model
	tempModel.x = wf->ReadTupleFloat(wf_entity, "temperaturesource", 0, 0);
	tempModel.y = wf->ReadTupleFloat(wf_entity, "temperaturesource", 1, 0);
	tempModel.z = wf->ReadTupleFloat(wf_entity, "temperaturesource", 2, 0);
}



void ModelcyzxLaser::Update(void) {
	// removed UnMapFromRoot() optimization - though neat, it breaks
	// thread-safety by messing with the Cell contents - rtv.

	assert(samples.size() == sample_count);

	double bearing(-fov / 2.0);
	// make the first and last rays exactly at the extremes of the FOV
	double sample_incr(fov / std::max(sample_count - 1, (unsigned int) 1));

	// find the global origin of our first emmitted ray
	Pose rayorg(geom.pose);
	rayorg.z += geom.size.z / 2.0;
	rayorg.a = bearing;
	rayorg = LocalToGlobal(rayorg);

	// set up a ray to trace
	Ray ray(this, rayorg, range_max, cyzxlaser_raytrace_match, NULL, true);

	// trace the ray, incrementing its heading for each sample
	for (unsigned int t(0); t < sample_count; t += resolution) {
		stg_raytrace_result_t r(world->Raytrace(ray));
		samples[t].range = r.range;

		// if we hit a model and it reflects brightly, we set
		// reflectance high, else low
		if (r.mod && (r.mod->vis.cyzxlaser_return >= cyzxLaserBright))
			samples[t].reflectance = 1;
		else
			samples[t].reflectance = 0;

		// point the ray to the next angle
		ray.origin.a += sample_incr;
	}

	// we may need to interpolate the samples we skipped
	if (resolution > 1) {
		for (unsigned int t(resolution); t < sample_count; t += resolution)
			for (unsigned int g(1); g < resolution; g++) {
				if (t >= sample_count)
					break;

				// copy the rightmost sample data into this point
				samples[t - g] = samples[t - resolution];

				double left(samples[t].range);
				double right(samples[t - resolution].range);

				// linear range interpolation between the left and right samples
				samples[t - g].range = (left - g * (left - right) / resolution);
			}
	}

	// removed MapFromRoot() optimization - though neat, it breaks
	// thread-safety by messing with the Cell contents - rtv

	Model::Update();
}

void ModelcyzxLaser::Startup(void) {
	Model::Startup();
	PRINT_DEBUG( "cyzxlaser startup" );
}

void ModelcyzxLaser::Shutdown(void) {
	PRINT_DEBUG( "cyzxlaser shutdown" );
	Model::Shutdown();
}

void ModelcyzxLaser::Print(char* prefix) {
	Model::Print(prefix);

	printf("\tRanges[ ");
	for (unsigned int i = 0; i < sample_count; i++)
		printf("%.2f ", samples[i].range);
	puts(" ]");

	printf("\tReflectance[ ");
	for (unsigned int i = 0; i < sample_count; i++)
		printf("%.2f ", samples[i].reflectance);
	puts(" ]");
}

ModelcyzxLaser::Sample* ModelcyzxLaser::GetSamples(uint32_t* count) {
	// get a C style array from our vector
	if (count)
		*count = sample_count;
	return &samples[0];
}

ModelcyzxLaser::FixModel ModelcyzxLaser::GetTempModelPos() {
	FixModel temp;
	temp.x = tempModel.x;
	temp.y = tempModel.y;
	temp.z = tempModel.z;
	return temp;
}

const std::vector<ModelcyzxLaser::Sample>& ModelcyzxLaser::GetSamples() {
	return samples;
}


// VIS -------------------------------------------------------------------

ModelcyzxLaser::Vis::Vis(World* world) :
		Visualizer("cyzxLaser", "cyzxlaser_vis") {
	world->RegisterOption(&showArea);
	world->RegisterOption(&showStrikes);
	world->RegisterOption(&showFov);
	world->RegisterOption(&showBeams);
}

void ModelcyzxLaser::Vis::Visualize(Model* mod, Camera* cam) {
	(void) cam; // avoid warning about unused var

	ModelcyzxLaser* cyzxlaser(dynamic_cast<ModelcyzxLaser*>(mod));

	const std::vector<Sample>& samples(cyzxlaser->GetSamples());

	size_t sample_count(samples.size());

	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glTranslatef(0, 0, cyzxlaser->GetGeom().size.z / 2.0); // shoot the cyzxlaser beam out at the right height

	// pack the cyzxlaser hit points into a vertex array for fast rendering
	static std::vector<GLfloat> pts;
	pts.resize(2 * (sample_count + 1));

	pts[0] = 0.0;
	pts[1] = 0.0;

	cyzxlaser->PushColor(1, 0, 0, 0.5);
	glDepthMask (GL_FALSE);
	glPointSize(2);

	for (unsigned int s(0); s < sample_count; s++) {
		double ray_angle = (s * (cyzxlaser->fov / (sample_count - 1)))
				- cyzxlaser->fov / 2.0;
		pts[2 * s + 2] = (float) (samples[s].range * cos(ray_angle));
		pts[2 * s + 3] = (float) (samples[s].range * sin(ray_angle));

		// if the sample is unusually bright, draw a little blob
		if (samples[s].reflectance > 0) {
			glBegin (GL_POINTS);
			glVertex2f(pts[2 * s + 2], pts[2 * s + 3]);
			glEnd();
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, &pts[0]);

	cyzxlaser->PopColor();

	if (showArea) {
		// draw the filled polygon in transparent blue
		cyzxlaser->PushColor(0, 0, 1, 0.1);
		glDrawArrays(GL_POLYGON, 0, sample_count + 1);
		cyzxlaser->PopColor();
	}

	glDepthMask (GL_TRUE);

	if (showStrikes) {
		// draw the beam strike points
		cyzxlaser->PushColor(0, 0, 1, 0.8);
		glDrawArrays(GL_POINTS, 0, sample_count + 1);
		cyzxlaser->PopColor();
	}

	if (showFov) {
		for (unsigned int s(0); s < sample_count; s++) {
			double ray_angle(
					(s * (cyzxlaser->fov / (sample_count - 1))) - cyzxlaser->fov / 2.0);
			pts[2 * s + 2] = (float) (cyzxlaser->range_max * cos(ray_angle));
			pts[2 * s + 3] = (float) (cyzxlaser->range_max * sin(ray_angle));
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cyzxlaser->PushColor(0, 0, 1, 0.5);
		glDrawArrays(GL_POLYGON, 0, sample_count + 1);
		cyzxlaser->PopColor();
	}

	if (showBeams) {
		cyzxlaser->PushColor(0, 0, 1, 0.5);
		glBegin (GL_LINES);

		for (unsigned int s(0); s < sample_count; s++) {

			glVertex2f(0, 0);
			double ray_angle(
					(s * (cyzxlaser->fov / (sample_count - 1))) - cyzxlaser->fov / 2.0);
			glVertex2f(samples[s].range * cos(ray_angle),
					samples[s].range * sin(ray_angle));

		}
		glEnd();
		cyzxlaser->PopColor();
	}

	glPopMatrix();
}

