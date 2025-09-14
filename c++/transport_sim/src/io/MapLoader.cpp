
#include "MapLoader.h"

#include <core/Graph.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/rapidjson.h>

LoadResults MapLoader::load(const std::string& filepath, Graph& graph) {
    LoadResults result;
    Document doc;

    if (!parsefile(filepath, doc, result)) {
        return result;
    }

    if (!validateSchema(doc, result)) {
        return result;
    }

}

bool MapLoader::parsefile(const std::string &filepath, Document &doc, LoadResults &res) {
    std::ifstream ifs(filepath);

    if (!ifs.is_open()) {
        res.errors.push_back("MapLoader: cannot open file " + filepath);
        return false;
    }

    IStreamWrapper isw(ifs);
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        res.errors.push_back("MapLoader: JSON parse error in " + filepath);
        return false;
    }

    return true;
}

bool MapLoader::validateSchema(const Document& doc, LoadResults &res) {
    //Basic validation
    if (!doc.HasMember("nodes") || !doc["nodes"].IsArray()) {
        res.errors.push_back(("MapLoader: Missing or invalid 'nodes' array"));
        return false;
    }

    if (!doc.HasMember("edges") || !doc["edges"].IsArray()) {
        res.errors.push_back(("MapLoader: Missing or invalid 'edges' array"));
        return false;
    }

    // Deeper validation for nodes
    std::unordered_set<int> seenNodeIds;
    std::unordered_set<std::string> seenNames;
    const Value& nodes = doc["nodes"];
    for (SizeType i = 0; i < nodes.Size(); i++) {

        if (!nodes[i].HasMember("id") || !nodes[i]["id"].IsInt()) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].id must be int"));
            return false;
        }

        if (!nodes[i].HasMember("lat") || !nodes[i]["lat"].IsDouble()) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].lat must be double"));
            return false;
        }

        if (!nodes[i]["lat"].GetDouble() < -90 || nodes[i]["lat"].GetDouble() > 90) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].lat must be between -90 and 90"));
            return false;
        }

        if (!nodes[i].HasMember("lon") || !nodes[i]["lon"].IsDouble()) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].lon must be double"));
            return false;
        }

        if (!nodes[i]["lon"].GetDouble() < -180 || nodes[i]["lon"].GetDouble() > 180) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].lon must be between -180 and 180"));
            return false;
        }

        if (!nodes[i].HasMember("name") || !nodes[i]["name"].IsString()) {
            res.errors.push_back("MapLoader: nodes[" + std::to_string(i) + "].name must be string");
            return false;
        }

        auto it_ids = seenNodeIds.find(nodes[i].GetInt());
        if (it_ids != seenNodeIds.end()) {
            res.errors.push_back("MapLoader: duplicate node id= " + nodes[i].GetInt());
            return false;
        }

        auto it_names = seenNames.find(nodes[i].GetString());
        if (it_names != seenNames.end()) {
            res.errors.push_back("MapLoader: duplicate name id= " + nodes[i].GetInt());
            return false;
        }
        seenNodeIds.insert(nodes[i].GetInt());
        seenNames.insert(nodes[i].GetString());
        
    }

    //Deeper validation for edges
    const Value& edges = doc["edges"];
    for (SizeType i = 0; i < nodes.Size(); i++) {

        if (!nodes[i].HasMember("id") || !nodes[i]["id"].IsInt()) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].id must be int"));
        }

        if (!nodes[i].HasMember("from") || !nodes[i]["from"].IsInt()) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].from must be int"));
        }

        if (!nodes[i].HasMember("to") || !nodes[i]["to"].IsInt()) {
            res.errors.push_back(("MapLoader: nodes[" + std::to_string(i) + "].to must be int"));
        }

        if (!nodes[i].HasMember("lenght_m") || !nodes[i]["lenght_m"].IsDouble()) {
            res.errors.push_back("MapLoader: nodes[" + std::to_string(i) + "].lenght_m must be double");
        }

        if (!nodes[i].HasMember("speed_kmh") || !nodes[i]["speed_kmh"].IsDouble()) {
            res.errors.push_back("MapLoader: nodes[" + std::to_string(i) + "].speed_kmh must be double");
        }

        if (!nodes[i].HasMember("max_tonnage_t") || !nodes[i]["max_tonnage_m"].IsDouble()) {
            res.errors.push_back("MapLoader: nodes[" + std::to_string(i) + "].max_tonnage_m must be double");
        }
    }
}

/*bool MapLoader::buildGraph(const Document& doc, Graph& graph, LoadStat &res) {
    Value& nodes = doc["nodes"];
    Value& edges = doc["edges"];

    for (SizeType i = 0; i < nodes.Size(); i++) {
        int id = nodes[i]["id"].GetInt();
        double lat = nodes[i]["lat"].GetDouble();
        double lon = nodes[i]["lon"].GetDouble();
        std::string name = nodes[i]["name"].GetString();
        graph.addNode(id, lat, lon, name);

    }

    for (SizeType i = 0; i < edges.Size(); i++) {
        int id = edges[i]["id"].GetInt();
        int from = edges[i]["from"].GetInt();
        int to = edges[i]["to"].GetInt();
        double length_m = edges[i]["length_m"].GetDouble();
        double speed_kmh = edges[i]["speed_kmh"].GetDouble();
        double max_tonnage_t = edges[i]["max_tonnage_t"].GetDouble();
        graph.addEdge(id, from, to, length_m, speed_kmh, max_tonnage_t);

    }
}
*/