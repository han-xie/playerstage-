#include <boost/lexical_cast.hpp>
#include <libxml/encoding.h>

#include "websim.hh"
using namespace websim;

#define MY_ENCODING "ISO-8859-1"

xmlChar *
ConvertInput(const char *in, const char *encoding);

bool WebSim::HandleModelRequest(const std::string& model,
		const std::string& prop, const std::string& action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	if (prop == "pvaNodeTick")
		return HandleModelPVANodeTickRequest(model, action, format, kv,
				response);

	if (prop == "pvaNode")
		return HandleModelPVANodeRequest(model, action, format, kv, response);

	if (prop == "pva")
		return HandleModelPVARequest(model, action, format, kv, response);

	if (prop == "data")
		return HandleModelDataRequest(model, action, format, kv, response);

	if (prop == "geometryNode")
		return HandleModelGeometryNodeRequest(model, action, format, kv,
				response);

	if (prop == "geometry")
		return HandleModelGeometryRequest(model, action, format, kv, response);

	if (prop == "tree")
		return HandleModelTreeRequest(model, action, format, kv, response);

	response = "ERROR: Unknown property " + prop
			+ " for model. Candidates are: pva data geometry tree.";
	return false;
}

bool WebSim::HandleModelDataRequest(std::string model, std::string action,
		Format format, struct evkeyvalq* kv, std::string& response) {
	if (action == "get") {

		if (!GetModelData(model, response, format, NULL)) {
			response = "ERROR: failed to get the Data of model " + model;
			return false;
		}
	}

	return true;

}

bool WebSim::HandleModelTreeRequest(std::string model, std::string action,
		Format format, struct evkeyvalq* kv, std::string& response) {

	if (action == "get") {
		if (!GetModelTree(model, format, response, false)) {
			response = "ERROR: failed to get the tree of model " + model;
			return false;
		}
	}

	return true;
}

bool WebSim::HandleModelGeometryNodeRequest(std::string model,
		std::string action, Format format, struct evkeyvalq* kv,
		std::string& response) {

	double x, y, z;
	Pose center;
	if (GetModelGeometry(model, x, y, z, center, response)) {

		GetGeometryNodeData(model, x, y, z, center, format, response, NULL);
		return true;

	} else {

		response = "ERROR: failed to get the Geometry of model " + model;
		return false;

	}

	return true;
}

bool WebSim::HandleModelGeometryRequest(std::string model, std::string action,
		Format format, struct evkeyvalq* kv, std::string& response) {

	double x, y, z;
	Pose center;
	if (GetModelGeometry(model, x, y, z, center, response)) {

		GetGeometryData(model, x, y, z, center, format, response, NULL);
		return true;

	} else {

		response = "ERROR: failed to get the Geometry of model " + model;
		return false;

	}

	return true;
}

bool WebSim::HandleModelPVANodeTickRequest(std::string model,
		std::string action, Format format, struct evkeyvalq* kv,
		std::string& response) {
	if (action == "get") {

		myupdateSign = 1;
		Pose p;
		Velocity v;
		Acceleration a;
		Time t;
		std::string tres;
		std::string tres2;

		if(model == ""){
			//{p1:{},p2:{}}
			char tc=0;
			for(int i=0;i<positionModels.size();i++){
				tres="";
				tres2="";
				tc+=i;
				if (GetModelPVA(positionModels[i], t, p, v, a, tres)) {
					GetPVANode(positionModels[i], t, p, v, a, format, tres, NULL);
					if(tres != ""){
						tres2 += "p";
						tres2 += (tc+'0');
						tres2 += ":";
						tres = tres2+tres;
					}
				} else {
					tres = "";
					//return false;
				}

				if(tres!=""){
					if(response!=""){
						response += ",";
						response += tres;
					}else{
						response +=	tres;
					}
				}
			}
			if(response == "") return false;
			response = "{"+response+"}";
			return true;
		}
		else{

		if (GetModelPVA(model, t, p, v, a, response)) {
			GetPVANode(model, t, p, v, a, format, response, NULL);
			return true;
		} else {
			response = "ERROR: failed to get the PVA of model " + model;
			return false;
		}
		}
	}
	else if (action == "set") {
		return false;
	}
	return false;
}

bool WebSim::HandleModelPVANodeRequest(std::string model, std::string action,
		Format format, struct evkeyvalq* kv, std::string& response) {
	if (action == "get") {
		Pose p;
		Velocity v;
		Acceleration a;
		Time t;
		if (GetModelPVA(model, t, p, v, a, response)) {
			GetPVANode(model, t, p, v, a, format, response, NULL);
			return true;
		} else {
			response = "ERROR: failed to get the PVA of model " + model;
			return false;
		}
	} else if (action == "set") {
		return false;
	}
	return false;
}

bool WebSim::HandleModelPVARequest(std::string model, std::string action,
		Format format, struct evkeyvalq* kv, std::string& response) {
	if (action == "get") {

		Pose p;
		Velocity v;
		Acceleration a;
		Time t;
		if (GetModelPVA(model, t, p, v, a, response)) {

			GetPVA(model, t, p, v, a, format, response, NULL);
			return true;

		} else {

			response = "ERROR: failed to get the PVA of model " + model;
			return false;
		}

	} else if (action == "set") {

		std::string sx, sy, sz, sroll, spitch, syaw;

		// Get pose first, fill in what the caller provided
		Pose p;
		Velocity v;
		Acceleration a;
		Time t;
		if (!GetModelPVA(model, t, p, v, a, response)) {
			response = "Failed to get pose before setting it";
			return false;
		}
		try {
			if (GetValue(sx, kv, "px"))
				p.x = boost::lexical_cast<float>(sx);
			if (GetValue(sy, kv, "py"))
				p.y = boost::lexical_cast<float>(sy);
			if (GetValue(sz, kv, "pz"))
				p.z = boost::lexical_cast<float>(sz);
			if (GetValue(sroll, kv, "pr"))
				p.r = boost::lexical_cast<float>(sroll);
			if (GetValue(spitch, kv, "pp"))
				p.p = boost::lexical_cast<float>(spitch);
			if (GetValue(syaw, kv, "pa"))
				p.a = boost::lexical_cast<float>(syaw);

			if (GetValue(sx, kv, "vx"))
				v.x = boost::lexical_cast<float>(sx);
			if (GetValue(sy, kv, "vy"))
				v.y = boost::lexical_cast<float>(sy);
			if (GetValue(sz, kv, "vz"))
				v.z = boost::lexical_cast<float>(sz);
			if (GetValue(sroll, kv, "vr"))
				v.r = boost::lexical_cast<float>(sroll);
			if (GetValue(spitch, kv, "vp"))
				v.p = boost::lexical_cast<float>(spitch);
			if (GetValue(syaw, kv, "va"))
				v.a = boost::lexical_cast<float>(syaw);

			if (GetValue(sx, kv, "ax"))
				a.x = boost::lexical_cast<float>(sx);
			if (GetValue(sy, kv, "ay"))
				a.y = boost::lexical_cast<float>(sy);
			if (GetValue(sz, kv, "az"))
				a.z = boost::lexical_cast<float>(sz);
			if (GetValue(sroll, kv, "ar"))
				a.r = boost::lexical_cast<float>(sroll);
			if (GetValue(spitch, kv, "ap"))
				a.p = boost::lexical_cast<float>(spitch);
			if (GetValue(syaw, kv, "aa"))
				a.a = boost::lexical_cast<float>(syaw);
		} catch (boost::bad_lexical_cast e) {
			response = std::string("Failed to parse input value(s): ")
					+ e.what();
			return false;
		}

		return (SetModelPVA(model, p, v, a, response));
	}

	return false;
}
void WebSim::GetPVANode(const std::string& name, Time& t, const Pose& p,
		const Velocity& v, const Acceleration& a, Format format,
		std::string& response, void* xmlnode) {
	if (format == TEXT) {
		char buf[1024];
		/*snprintf(buf, sizeof(buf),
		 "%s's state @%s: \n  pose: (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n"
		 "  vel : (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n"
		 "  acc : (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n",
		 name.c_str(), t.String().c_str(), p.x, p.y, p.z, p.r, p.p, p.a,
		 v.x, v.y, v.z, v.r, v.p, v.a, a.x, a.y, a.z, a.r, a.p, a.a);
		 */
		snprintf(buf, sizeof(buf),
				"{\"posex\":\"%.3f\",\"posey\":\"%.3f\",\"posea\":\"%.3f\",\"time\":\"%s\"}",
				p.x, p.y, p.a, t.String().c_str());
		response = buf;

	} else if (format == XML) {

	}
}
void WebSim::GetPVA(const std::string& name, Time& t, const Pose& p,
		const Velocity& v, const Acceleration& a, Format format,
		std::string& response, void* xmlnode) {
	if (format == TEXT) {
		char buf[1024];
		snprintf(buf, sizeof(buf),
				"%s's state @%s: \n  pose: (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n"
						"  vel : (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n"
						"  acc : (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n",
				name.c_str(), t.String().c_str(), p.x, p.y, p.z, p.r, p.p, p.a,
				v.x, v.y, v.z, v.r, v.p, v.a, a.x, a.y, a.z, a.r, a.p, a.a);
		response = buf;

	} else if (format == XML) {

		xmlNodePtr root_node = NULL;
		xmlDocPtr doc = NULL;
		xmlNodePtr node;
		xmlChar *tmp;
		char str[128];

		if (xmlnode == NULL) {

			doc = xmlNewDoc(BAD_CAST "1.0");
			root_node = xmlNewNode(NULL, BAD_CAST "root");
			xmlDocSetRootElement(doc, root_node);
		node = xmlNewChild(root_node, NULL, BAD_CAST "Model", NULL);
	} else {
		node = (xmlNodePtr) xmlnode;
		xmlNodeSetName(node, xmlCharStrdup("Model"));
		//node = xmlNewChild((xmlNodePtr)parent, NULL, BAD_CAST "PVA", NULL);
		//root_node = (xmlNodePtr)parent;
	}

	sprintf(str, "%s", t.String().c_str());
	tmp = ConvertInput(str, MY_ENCODING);
	xmlNewProp(node, BAD_CAST "Time", BAD_CAST tmp);
	xmlNewProp(node, BAD_CAST "Type", BAD_CAST "pva");

	tmp = ConvertInput(name.c_str(), MY_ENCODING);
	xmlNewProp(node, BAD_CAST "Name", BAD_CAST tmp);

	sprintf(str, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f", p.x, p.y, p.z, p.r, p.p, p.a);
	tmp = ConvertInput(str, MY_ENCODING);
	xmlNewProp(node, BAD_CAST "Pose", BAD_CAST tmp);

	sprintf(str, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f", v.x, v.y, v.z, v.r, v.p, v.a);
	tmp = ConvertInput(str, MY_ENCODING);
	xmlNewProp(node, BAD_CAST "Velocity", BAD_CAST tmp);

	sprintf(str, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f", a.x, a.y, a.z, a.r, a.p, a.a);
	tmp = ConvertInput(str, MY_ENCODING);
	xmlNewProp(node, BAD_CAST "Acceleration", BAD_CAST tmp);

	if (xmlnode == NULL) {
		xmlBufferPtr buf = xmlBufferCreate();
		xmlKeepBlanksDefault(0);
		xmlNodeDump(buf, doc, node, 0, 1);
		response = (const char*) buf->content;
		puts(response.c_str());
		xmlBufferFree(buf);
		xmlFreeDoc(doc);
	}
}

}

void WebSim::GetLaserData(const std::string& name, Time& t,
	uint32_t& resolution, double& fov, Pose& p, std::vector<double>& ranges,
	Format format, std::string& response, void* xmlnode) {
if (format == TEXT) {

	std::string res;
	char temp[128];
	for (unsigned int i = 0; i < ranges.size(); i++) {
		sprintf(temp, "%.3f", ranges.at(i));
		res.append(temp);
		if (i + 1 != ranges.size())
			res.append(",");
	}

	char buf[4096];
	snprintf(buf, sizeof(buf),
			"%s's state @%s: \n  laser: (%s)\n  resolution: (%d)\n  fov(%.2f)\n"
					"  pose (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n", name.c_str(),
			t.String().c_str(), res.c_str(), resolution, fov, p.x, p.y, p.z,
			p.r, p.p, p.a);
	response = buf;

} else if (format == XML) {

	xmlNodePtr root_node = NULL;
	xmlDocPtr doc = NULL;
	xmlNodePtr node;
	xmlChar *tmp;
	char str[128];

	if (xmlnode == NULL) {
		puts("no parent\n");
		doc = xmlNewDoc(BAD_CAST "1.0");
		root_node = xmlNewNode(NULL, BAD_CAST "root");
		xmlDocSetRootElement(doc, root_node);
	node = xmlNewChild(root_node, NULL, BAD_CAST "Model", NULL);
} else {
	node = (xmlNodePtr) xmlnode;
	xmlNodeSetName(node, xmlCharStrdup("Model"));
	//node = xmlNewChild((xmlNodePtr)parent, NULL, BAD_CAST "Laser", NULL);
	//root_node = (xmlNodePtr)parent;
}

std::string res;
char temp[128];
for (unsigned int i = 0; i < ranges.size(); i++) {
	sprintf(temp, "%.3f", ranges.at(i));
	res.append(temp);
	if (i + 1 != ranges.size())
		res.append(",");
}

xmlNewProp(node, BAD_CAST "Time", BAD_CAST t.String().c_str());
xmlNewProp(node, BAD_CAST "Type", BAD_CAST "laser");

tmp = ConvertInput(name.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Name", BAD_CAST tmp);

sprintf(str, "%d", resolution);
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "Resolution", BAD_CAST tmp);

sprintf(str, "%.3f", fov);
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "FOV", BAD_CAST tmp);

sprintf(str, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f", p.x, p.y, p.z, p.r, p.p, p.a);
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "Pose", BAD_CAST tmp);

tmp = ConvertInput(res.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Samples", BAD_CAST tmp);

if (xmlnode == NULL) {
	xmlBufferPtr buf = xmlBufferCreate();
	xmlKeepBlanksDefault(0);
	xmlNodeDump(buf, doc, node, 0, 1);

	response = (const char*) buf->content;
	puts(response.c_str());
	xmlBufferFree(buf);
	xmlFreeDoc(doc);
}

}
}

void WebSim::GetRangerData(const std::string& name, Time& t,
std::vector<Pose>& p, std::vector<double>& ranges, Format format,
std::string& response, void* xmlnode) {
if (format == TEXT) {

std::string res;
char temp[128];
for (unsigned int i = 0; i < ranges.size(); i++) {
	sprintf(temp, "%.3f", ranges.at(i));
	res.append(temp);
	if (i + 1 != ranges.size())
		res.append(",");
}

std::string pstr;
char ptmp[256];
for (unsigned int i = 0; i < p.size(); i++) {
	Pose pos = p.at(i);
	sprintf(ptmp, " (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)", pos.x, pos.y, pos.x,
			pos.r, pos.p, pos.a);
	pstr.append(ptmp);

}

char buf[2048];
snprintf(buf, sizeof(buf),
		"%s's state @%s: \n  ranger\n  ranges: (%s)\n  pose:  %s", name.c_str(),
		t.String().c_str(), res.c_str(), pstr.c_str());
response = buf;

} else if (format == XML) {

xmlNodePtr root_node = NULL;
xmlDocPtr doc = NULL;
xmlNodePtr node;
xmlChar *tmp;
char str[128];

if (xmlnode == NULL) {
	puts("no parent\n");
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);
node = xmlNewChild(root_node, NULL, BAD_CAST "Model", NULL);
} else {
node = (xmlNodePtr) xmlnode;
xmlNodeSetName(node, xmlCharStrdup("Model"));
//node = xmlNewChild((xmlNodePtr)parent, NULL, BAD_CAST "Ranger", NULL);
//root_node = (xmlNodePtr)parent;
}

std::string res;
char temp[1024];
for (unsigned int i = 0; i < ranges.size(); i++) {
sprintf(temp, "%.3f", ranges.at(i));
res.append(temp);
if (i + 1 != ranges.size())
	res.append(",");
}

std::string pstr;
char ptmp[2048];
for (unsigned int i = 0; i < p.size(); i++) {
Pose pos = p.at(i);
sprintf(ptmp, " (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)", pos.x, pos.y, pos.x, pos.r,
		pos.p, pos.a);
pstr.append(ptmp);
}

	//xmlNodePtr node = xmlNewChild(root_node, NULL, BAD_CAST "Ranger", NULL);

sprintf(str, "%s", t.String().c_str());
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "Time", BAD_CAST tmp);
xmlNewProp(node, BAD_CAST "Type", BAD_CAST "ranger");

tmp = ConvertInput(name.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Name", BAD_CAST tmp);

tmp = ConvertInput(res.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Samples", BAD_CAST tmp);

tmp = ConvertInput(pstr.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Positions", BAD_CAST tmp);

if (xmlnode == NULL) {
xmlBufferPtr buf = xmlBufferCreate();
xmlKeepBlanksDefault(0);
xmlNodeDump(buf, doc, node, 0, 1);

response = (const char*) buf->content;
puts(response.c_str());
xmlBufferFree(buf);
xmlFreeDoc(doc);
}

}

}

void WebSim::GetFiducialData(const std::string& name, Time& t,
std::vector<Fiducial>& f, Format format, std::string& response, void* xmlnode) {

if (format == TEXT) {

std::string res;
char temp[512];
for (unsigned int i = 0; i < f.size(); i++) {
sprintf(temp, "((%.3f,%.3f,%.3f),(%.3f,%.3f,%.3f),%d)\n", f.at(i).pos.x,
		f.at(i).pos.y, f.at(i).pos.z, f.at(i).pos.r, f.at(i).pos.p,
		f.at(i).pos.a, f.at(i).id);
res.append(temp);

}

char buf[2048];
snprintf(buf, sizeof(buf), "%s's state @%s: \n  Fiducial:\n", name.c_str(),
	t.String().c_str());

response = buf;
response.append(res);

} else if (format == XML) {

xmlNodePtr root_node = NULL;
xmlDocPtr doc = NULL;
xmlNodePtr node;
xmlChar *tmp;
char str[128];

if (xmlnode == NULL) {
doc = xmlNewDoc(BAD_CAST "1.0");
root_node = xmlNewNode(NULL, BAD_CAST "root");
xmlDocSetRootElement(doc, root_node);
node = xmlNewChild(root_node, NULL, BAD_CAST "Model", NULL);
} else {
node = (xmlNodePtr) xmlnode;
xmlNodeSetName(node, xmlCharStrdup("Model"));
	//node = xmlNewChild((xmlNodePtr)parent, NULL, BAD_CAST "Ranger", NULL);
	//root_node = (xmlNodePtr)parent;
}

std::string res;
char temp[512];
for (unsigned int i = 0; i < f.size(); i++) {
sprintf(temp, "((%.3f,%.3f,%.3f),(%.3f,%.3f,%.3f),%d)", f.at(i).pos.x,
	f.at(i).pos.y, f.at(i).pos.z, f.at(i).pos.r, f.at(i).pos.p, f.at(i).pos.a,
	f.at(i).id);
res.append(temp);

}

sprintf(str, "%s", t.String().c_str());
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "Time", BAD_CAST tmp);
xmlNewProp(node, BAD_CAST "Type", BAD_CAST "fiducial");

tmp = ConvertInput(name.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Name", BAD_CAST tmp);

tmp = ConvertInput(res.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Targets", BAD_CAST tmp);

if (xmlnode == NULL) {
xmlBufferPtr buf = xmlBufferCreate();
xmlKeepBlanksDefault(0);
xmlNodeDump(buf, doc, node, 0, 1);

response = (const char*) buf->content;
puts(response.c_str());
xmlBufferFree(buf);
xmlFreeDoc(doc);
}

}

}
void WebSim::GetGeometryNodeData(const std::string& name, double& x, double& y,
double& z, Pose& center, Format format, std::string& response, void* xmlnode) {
if (format == TEXT) {
char buf[1024];
if (name == "sim")
snprintf(buf, sizeof(buf),
	"World Size: (%.3f,%.3f,%.3f)\n        Origin (%.3f,%.3f,%.3f,%.3f,%.3f,%.3f)",
	x, y, z, center.x, center.y, center.z, center.r, center.p, center.a);
else
/*snprintf(buf, sizeof(buf),
 "%s's Size: (%.3f,%.3f,%.3f)\n        Origin (%.3f,%.3f,%.3f,%.3f,%.3f,%.3f)",
 name.c_str(), x, y, z, center.x, center.y, center.z, center.r, center.p,
 center.a);*/
snprintf(buf, sizeof(buf),
	"{\"sizex\":\"%.3f\",\"sizey\":\"%.3f\",\"sizez\":\"%.3f\",\"centerx\":\"%.3f\",\"centery\":\"%.3f\",\"centerz\":\"%.3f\",\"centerr\":\"%.3f\",\"centerp\":\"%.3f\",\"centera\":\"%.3f\"}",
	x, y, z, center.x, center.y, center.z, center.r, center.p, center.a);

response = buf;
}
}
void WebSim::GetGeometryData(const std::string& name, double& x, double& y,
double& z, Pose& center, Format format, std::string& response, void* xmlnode) {
if (format == TEXT) {
char buf[1024];
if (name == "sim")
snprintf(buf, sizeof(buf),
	"World Size: (%.3f,%.3f,%.3f)\n        Origin (%.3f,%.3f,%.3f,%.3f,%.3f,%.3f)",
	x, y, z, center.x, center.y, center.z, center.r, center.p, center.a);
else
snprintf(buf, sizeof(buf),
	"%s's Size: (%.3f,%.3f,%.3f)\n        Origin (%.3f,%.3f,%.3f,%.3f,%.3f,%.3f)",
	name.c_str(), x, y, z, center.x, center.y, center.z, center.r, center.p,
	center.a);

response = buf;
} else if (format == XML) {

xmlNodePtr root_node = NULL;
xmlDocPtr doc = NULL;
xmlNodePtr node;
xmlChar *tmp;
char str[128];

if (xmlnode == NULL) {
doc = xmlNewDoc(BAD_CAST "1.0");
root_node = xmlNewNode(NULL, BAD_CAST "root");
xmlDocSetRootElement(doc, root_node);
node = xmlNewChild(root_node, NULL, BAD_CAST "ModelGeometry", NULL);
} else {
node = (xmlNodePtr) xmlnode;
xmlNodeSetName(node, xmlCharStrdup("ModelGeometry"));
}

xmlNewProp(node, BAD_CAST "Type", BAD_CAST "geometry");

tmp = ConvertInput(name.c_str(), MY_ENCODING);
xmlNewProp(node, BAD_CAST "Name", BAD_CAST tmp);

sprintf(str, "%.3f", x);
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "X", BAD_CAST tmp);

sprintf(str, "%.3f", y);
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "Y", BAD_CAST tmp);

sprintf(str, "%.3f", z);
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "Z", BAD_CAST tmp);

sprintf(str, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f", center.x, center.y, center.z,
center.r, center.p, center.a);
tmp = ConvertInput(str, MY_ENCODING);
xmlNewProp(node, BAD_CAST "Origin", BAD_CAST tmp);

if (xmlnode == NULL) {
xmlBufferPtr buf = xmlBufferCreate();
xmlKeepBlanksDefault(0);
xmlNodeDump(buf, doc, node, 0, 1);

response = (const char*) buf->content;
puts(response.c_str());
xmlBufferFree(buf);
xmlFreeDoc(doc);
}
}

}

bool WebSim::GetModelTreeNode(const std::string& model, Format format,
std::string& response, bool everything) {
if (format == TEXT) {

std::string str, type;
GetModelType(model, type);
if (type.size() > 0){
	if(type == "position"){
		this->countPosition+=1;
		str += "\"" + type;
		str.append(1,countPosition+'0');
		str +="\"";
		str += ":\"";
		str += model;
		str += "\"";
	}
	else
str += "\"" + type + "\"" + ":\"" + model + "\"";
}
std::vector<std::string> children;
GetModelChildren(model, children);
for (unsigned int i = 0; i < children.size(); i++) {

std::string sub;
GetModelTreeNode(children[i], TEXT, sub, false);
if ((str.size() > 0) && (sub.size() > 0))
str += "," + sub;
else if ((str.size() > 0) && (sub.size() == 0))
str += sub;
else if (str.size() == 0)
str += sub;
}

response = str;
return false;
}

return false;
}

bool WebSim::GetModelTree(const std::string& model, Format format,
std::string& response, bool everything) {

if (format == XML) {
xmlNodePtr root_node = NULL;
xmlDocPtr doc = NULL;
xmlBufferPtr buf;

doc = xmlNewDoc(BAD_CAST "1.0");
buf = xmlBufferCreate();
root_node = xmlNewNode(NULL, BAD_CAST "ModelTree");
xmlDocSetRootElement(doc, root_node);

if (GetModelTreeXML(model, root_node, everything)) {

xmlKeepBlanksDefault(0);
xmlNodeDump(buf, doc, root_node, 0, 1);
response = (const char*) buf->content;
} else {
response = "Error in generating model tree.\n";

}

xmlBufferFree(buf);
xmlFreeDoc(doc);

std::string out = "Model Tree:\n " + response + "\n";
puts(out.c_str());

return true;

} else if (format == TEXT) {

std::string str, type;
GetModelType(model, type);
str += model + "(" + type + ")" + "\n";
std::vector<std::string> children;
GetModelChildren(model, children);
for (unsigned int i = 0; i < children.size(); i++) {

std::string sub;
GetModelTree(children[i], TEXT, sub, false);
str += sub + "\n";
}

response = str;
return false;
}

return false;
}

bool WebSim::GetModelTreeXML(const std::string& model, void* xmlparent,
bool everything) {

std::string res;
xmlNodePtr node;

if (!xmlparent) {
printf("Invalid xmlNode pointer. \n");
return false;
}

if (model != "") {

node = xmlNewChild((xmlNodePtr)xmlparent, NULL, BAD_CAST "Model", NULL);

bool flag = false;
std::string type;

if (everything) {
std::string error;
flag = !GetModelData(model, error, XML, node);

}

if (!everything || flag) {

GetModelType(model, type);
xmlNodeSetName(node, xmlCharStrdup("Model"));
xmlNewProp(node, BAD_CAST "Name", BAD_CAST model.c_str());
xmlNewProp(node, BAD_CAST "Type", BAD_CAST type.c_str());
}

} else
node = (xmlNodePtr) xmlparent;

std::vector<std::string> children;
GetModelChildren(model, children);
for (unsigned int i = 0; i < children.size(); i++)
GetModelTreeXML(children[i], node, everything);

return true;

}
