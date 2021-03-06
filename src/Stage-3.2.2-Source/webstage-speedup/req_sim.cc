#include "websim.hh"
#include <stdio.h>
#include <stdlib.h>

using namespace websim;

const uint32_t TICK_INTERVAL = 100; // 100 msec

// todo: XML format for more sim requests

bool WebSim::HandleSimRequest(const std::string& prop,
		const std::string& action, Format format, struct evkeyvalq* kv,
		std::string& response) {
	printf("[%s] sim %s %s\n", IdentificationString().c_str(), prop.c_str(),
			action.c_str());

	if (prop == "factory")
		return HandleSimFactoryRequest(action, format, kv, response);

	if (prop == "clock")
		return HandleSimClockRequest(action, format, kv, response);

	if (prop == "geometry")
		return HandleSimGeometryRequest(action, format, kv, response);

	if (prop == "children")
		return HandleSimChildrenRequest(action, format, kv, response);

	if (prop == "tree")
		return HandleSimTreeRequest(action, format, kv, response);

	if (prop == "treeNode")
		return HandleSimTreeNodeRequest(action, format, kv, response);

	/*if (prop == "treePosition")
		return HandleSimTreePositionRequest(action, format, kv, response);*/

	if (prop == "greet")
		return HandleSimGreetRequest(action, format, kv, response);

	if (prop == "homepage")
		return HandleSimHomepageRequest(action, format, kv, response);

	if (prop == "viz")
		return HandleSimVisualizationRequest(action, format, kv, response);

	/*
	 if (prop == "pva")
	 return HandleModelPVARequest(action, format, kv, response);
	 */

	response = "ERROR: Unknown property " + prop
			+ " for sim. Candidates are: clock factory greet tree.";
	return false;
}

bool WebSim::HandleSimVisualizationRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {

	response =
			"<HTML><HEAD><TITLE>Websim Visualization</TITLE></HEAD>"
					"<BODY><iframe id=\"visualization\" frameborder=0 width=\"1000\" height=\"1000\" "
					"src=\"http://192.168.1.210/viz.html\"></iframe></BODY></HTML>";

	/*response =
	 "<HTML><HEAD><TITLE>A Simple Program</TITLE></HEAD>"
	 "<BODY><CENTER>"
	 "<APPLET CODE=\"http://192.168.1.210/Main.class\" WIDTH=\"1000\" HEIGHT=\"1000\" id=\"TheApplet\"></APPLET>"
	 "</CENTER></BODY></HTML>";
	 */

	return true;

}

bool WebSim::HandleSimHomepageRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {

	std::vector<std::string> links;
	GenerateSampleUrls(links);

	std::string clockuri = "http://" + hostportname + "/sim/clock/get";

	response = "<html><head><title>WebSim</title></head>"
			"<body>"
			"<h1>Welcome to " + IdentificationString() + "</h1>"
			"<p>" + package + "-" + version + " HTTP interface to "
			+ IdentificationString() + "-" + VersionString() + "</h1>"
					"<hr>"
					"Example URIs available at this server:<br>"
					"<ul>"
					"<li><a href=" + clockuri + ">" + clockuri + "</a>";

	for (unsigned int k = 0; k < links.size(); k++)
		response += "<li>" + links[k] + "</li>";

	response +=
			"</ul><hr>"
					"<p><a href=\"http://playerstage.org/index.php?src=websim\">WebSim</a>"
					" is part of the <a href=\"http://playerstage.org\">Player Project</a>"
					"</body></html>";

	return true;
}

bool WebSim::HandleSimGeometryRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {

	double x, y, z;
	Pose center;
	if (GetModelGeometry("sim", x, y, z, center, response)) {
		this->GetGeometryData("sim", x, y, z, center, format, response, NULL);
	} else {
		response = "Warning: geometry not yet implemented";
		return false;
	}
	return true;
}

bool WebSim::HandleSimChildrenRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	if (action == "")
		action = "get"; // default action

	if (action == "get") {
		std::vector<std::string> children;
		GetModelChildren("", children);

		for (unsigned int i = 0; i < children.size(); i++)
			response += "-" + children[i];

		return true;
	}

	response = "ERROR: Unknown action " + action
			+ " for sim/children. Candidates are: get (default).";
	return false;
}

bool WebSim::HandleSimTreePositionRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	if (action == "get") {
		GetModelTree("position", format, response, false);
		return true;
	}
	response = "ERROR: Unknown action " + action
			+ " for sim/tree. Candidates are: get (default).";
	return false;
}

bool WebSim::HandleSimTreeNodeRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	countPosition=0;
	if (action == "")
		action = "get"; // default action

	if (action == "get") {
		GetModelTreeNode("", format, response, false);
		std::string tmp=",\"position0\":";
				tmp+="\"";
				tmp.append(1,countPosition+'0');
				tmp+="\"";
		response +=tmp;
		response="{"+response+"}";
		return true;
	}

	response = "ERROR: Unknown action " + action
			+ " for sim/tree. Candidates are: get (default).";
	return false;
}

bool WebSim::HandleSimTreeRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	if (action == "")
		action = "get"; // default action

	if (action == "get") {
		GetModelTree("", format, response, false);
		return true;
	}

	response = "ERROR: Unknown action " + action
			+ " for sim/tree. Candidates are: get (default).";
	return false;
}

bool WebSim::HandleSimGreetRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	if (action == "")
		action = "get"; // default action

	if (action == "get") {
		// TODO - check to see if this server was anticipated
		response = "Greetings " + action;
		printf("[websim] WebSim peer %s connected\n", action.c_str());
		return true;
	}

	response = "ERROR: Unknown action " + action
			+ " for sim/greet. Candidates are: get (default).";
	return false;
}

bool WebSim::HandleSimFactoryRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	if (action == "create") {
		std::string name, type;
		if (!GetValue(name, kv, "name")) {
			response = "ERROR: Missing name argument for sim/factory/create";
			return false;
		}

		if (!GetValue(type, kv, "type")) {
			response =
					"ERROR: Missing name type argument for sim/factory/create";
			return false;
		}

		return (CreateModel(name, type, response));
	}

	if (action == "destroy") {
		std::string name, type;
		if (!GetValue(name, kv, "name")) {
			response = "ERROR: Missing name argument for sim/factory/destroy";
			return false;
		}

		return (DeleteModel(name, response));
	}

	response = "ERROR: Unknown action " + action
			+ " for sim/factory. Candidates are: create destroy";
	return false;
}

bool WebSim::HandleSimClockRequest(std::string action, Format format,
		struct evkeyvalq* kv, std::string& response) {
	if (action == "") // DEFAULT ACTION
		action = "get";

	if (action == "get") {
		if (format == TEXT) {
			response = "Current time: " + GetTime().String() + " seconds.";
		} else if (format == XML) {
			response = "<Time value=\"" + GetTime().String() + "\" />";
		}
		return true;
	}

	if (action == "tick") {
		if (ClockRunFor(TICK_INTERVAL)) {
			ticks_remaining--;
			response = "Ticked the clock";
		} else
			response = "Failed to tick the clock";
		return true;
	}

	if (action == "start") {
		if (ClockStart())
			response = "Started the clock";
		else
			response = "Failed to start the clock";
		return true;
	}

	if (action == "stop") {
		if (ClockStop())
			response = "Stopped the clock";
		else
			response = "Failed to stop the clock";
		return true;
	}

	response = "ERROR: Unknown action " + action
			+ " for sim/clock. Candidates are: get (default) start stop.";
	return false;
}

void WebSim::GenerateSampleUrls(std::vector<std::string> &list) {
	std::vector<std::string> sensors;
	std::vector<std::string> temp;

	GetModelChildren("", temp);

	while (temp.size() > 0 && sensors.size() < 50) {
		std::string m = temp.at(0);
		temp.erase(temp.begin());

		unsigned int size = temp.size();
		GetModelChildren(m, temp);

		if (temp.size() == size)
			sensors.push_back(m);
	}

	const char* names[] = { "laser", "ranger", "fiducial" };
	unsigned int index = 0;

	for (unsigned int i = 0; i < sensors.size(); i++)
		if (sensors[i].find(names[index]) != std::string::npos) {
			std::string uri = "http://" + hostportname + "/" + sensors[i]
					+ "/data/get";
			list.push_back("<a href=" + uri + ">" + uri + "</a> <br>");
			index++;
		}
}

