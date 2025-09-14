
#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <iostream>
#include <core/Graph.h>
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <unordered_set>

struct LoadStat {
    int nodes = 0;
    int edges_in = 0;
    int edges_internal = 0;
    double min_speed_mps = +INFINITY, max_speed_mps = 0.0;
    double min_len_m = +INFINITY, max_len_m = 0.0;
};

struct LoadResults {
    bool success = false;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

using namespace rapidjson;


class MapLoader {
private:
    FILE* fp;
    char readBuffer[1024];
    Document doc;
public:
    LoadResults load(const std::string& filepath, Graph& graph);

    bool parsefile(const std::string& filepath, Document& doc, LoadResults& res);
    bool validateSchema(const Document& doc, LoadResults& res);
    bool buildGraph(const Document& doc, Graph& graph, LoadStat& res);
};



#endif //MAPLOADER_H
