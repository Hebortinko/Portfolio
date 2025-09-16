
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

    if (result.errors.empty()) {
        result.success = true;
    }
    buildGraph(doc, graph);
    return result;
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
    for (SizeType i = 0; i < nodes.Size(); ++i) {
        const auto& n = nodes[i];

        // helper na pomenovanie uzla do správy (ak má name, použijeme ho)
        const std::string nodeTag = n.HasMember("name") && n["name"].IsString()
            ? (std::string("node '") + n["name"].GetString() + "'")
            : (std::string("node[") + std::to_string(i) + "]");

        // id
        if (!n.HasMember("id")) {
            addError(res, io::ErrorType::MissingField_Node_Id,io::to_string(io::ErrorType::MissingField_Node_Id) + std::string(":+ ") + nodeTag);
            return false;
        }
        if (!n["id"].IsInt()) {
            addError(res, io::ErrorType::TypeError_Node_IdNotInt,io::to_string(io::ErrorType::TypeError_Node_IdNotInt) + std::string(":+ ") + nodeTag + " -> 'id'");
            return false;
        }

        // lat
        if (!n.HasMember("lat")) {
            addError(res, io::ErrorType::MissingField_Node_Lat,io::to_string(io::ErrorType::MissingField_Node_Lat) + std::string(":+ ") + nodeTag);
            return false;
        }
        if (!n["lat"].IsDouble()) {
            addError(res, io::ErrorType::TypeError_Node_LatNotDouble,io::to_string(io::ErrorType::TypeError_Node_LatNotDouble) + std::string(":+ ") + nodeTag + " -> 'lat'");
            return false;
        }
        {
            const double lat = n["lat"].GetDouble();
            if (lat < -90.0 || lat > 90.0) {
                addError(res, io::ErrorType::RangeError_Node_Lat,io::to_string(io::ErrorType::RangeError_Node_Lat) + std::string(":+ ") + nodeTag +" -> 'lat'=" + std::to_string(lat) + " (expected -90..90)");
                return false;
            }
        }

        // lon
        if (!n.HasMember("lon")) {
            addError(res, io::ErrorType::MissingField_Node_Lon,io::to_string(io::ErrorType::MissingField_Node_Lon) + std::string(":+ ") + nodeTag);
            return false;
        }
        if (!n["lon"].IsDouble()) {
            addError(res, io::ErrorType::TypeError_Node_LonNotDouble,io::to_string(io::ErrorType::TypeError_Node_LonNotDouble) + std::string(":+ ") + nodeTag + " -> 'lon'");
            return false;
        }
        {
            const double lon = n["lon"].GetDouble();
            if (lon < -180.0 || lon > 180.0) {
                addError(res, io::ErrorType::RangeError_Node_Lon,io::to_string(io::ErrorType::RangeError_Node_Lon) + std::string(":+ ") + nodeTag +" -> 'lon'=" + std::to_string(lon) + " (expected -180..180)");
                return false;
            }
        }

        // name
        if (!n.HasMember("name")) {
            addError(res, io::ErrorType::MissingField_Node_Name,io::to_string(io::ErrorType::MissingField_Node_Name) + std::string(":+ ") + nodeTag);
            return false;
        }
        if (!n["name"].IsString()) {
            addError(res, io::ErrorType::TypeError_Node_NameNotString,io::to_string(io::ErrorType::TypeError_Node_NameNotString) + std::string(":+ ") +std::to_string(i) + "] -> 'name'");
            return false;
        }

        // duplicates id, name
        const int idVal = n["id"].GetInt();
        if (seenNodeIds.find(idVal) != seenNodeIds.end()) {
            addError(res, io::ErrorType::UniqueError_Node_ID,
                     io::to_string(io::ErrorType::UniqueError_Node_ID) + std::string(": duplicate id=") + std::to_string(idVal));
            return false;
        }
        const char* nameVal = n["name"].GetString();
        if (seenNames.find(nameVal) != seenNames.end()) {
            addError(res, io::ErrorType::UniqueError_Node_Name,
                     io::to_string(io::ErrorType::UniqueError_Node_Name) + std::string(": duplicate name '") + std::string(nameVal) + "'");
            return false;
        }
        seenNodeIds.insert(idVal);
        seenNames.insert(nameVal);

    }

    //Deeper validation for edges
    const Value& edges = doc["edges"];

for (SizeType i = 0; i < edges.Size(); ++i) {
    const auto& e = edges[i];
    const std::string edgeTag = std::string("edge[") + std::to_string(i) + "]";

    // id
    if (!e.HasMember("id")) {
        addError(res, io::ErrorType::MissingField_Edge_Id,io::to_string(io::ErrorType::MissingField_Edge_Id) + std::string(": ") + edgeTag);
        return false;
    }
    if (!e["id"].IsInt()) {
        addError(res, io::ErrorType::TypeError_Edge_IdNotInt,io::to_string(io::ErrorType::TypeError_Edge_IdNotInt) + std::string(": ") + edgeTag + " -> 'id'");
        return false;
    }
    const int eid = e["id"].GetInt();

    // from
    if (!e.HasMember("from")) {
        addError(res, io::ErrorType::MissingField_Edge_From,io::to_string(io::ErrorType::MissingField_Edge_From) + std::string(": ") + edgeTag);
        return false;
    }
    if (!e["from"].IsInt()) {
        addError(res, io::ErrorType::TypeError_Edge_FromNotInt,io::to_string(io::ErrorType::TypeError_Edge_FromNotInt) + std::string(": ") + edgeTag + " -> 'from'");
        return false;
    }
    const int from = e["from"].GetInt();

    // to
    if (!e.HasMember("to")) {
        addError(res, io::ErrorType::MissingField_Edge_To,io::to_string(io::ErrorType::MissingField_Edge_To) + std::string(": ") + edgeTag);
        return false;
    }
    if (!e["to"].IsInt()) {
        addError(res, io::ErrorType::TypeError_Edge_ToNotInt,io::to_string(io::ErrorType::TypeError_Edge_ToNotInt) + std::string(": ") + edgeTag + " -> 'to'");
        return false;
    }
    const int to = e["to"].GetInt();

    // length_m (JSON key), enum has "Lenght"
    if (!e.HasMember("length_m")) {
        addError(res, io::ErrorType::MissingField_Edge_Lenght_m,io::to_string(io::ErrorType::MissingField_Edge_Lenght_m) + std::string(": ") + edgeTag);
        return false;
    }
    if (!e["length_m"].IsDouble()) {
        addError(res, io::ErrorType::TypeError_Edge_LenghtNotDouble,io::to_string(io::ErrorType::TypeError_Edge_LenghtNotDouble) + std::string(": ") + edgeTag + " -> 'length_m'");
        return false;
    }
    const double length_m = e["length_m"].GetDouble();

    // speed_kmh
    if (!e.HasMember("speed_kmh")) {
        addError(res, io::ErrorType::MissingField_Edge_Speed_mps,io::to_string(io::ErrorType::MissingField_Edge_Speed_mps) + std::string(": ") + edgeTag);
        return false;
    }
    if (!e["speed_kmh"].IsDouble()) {
        addError(res, io::ErrorType::TypeError_Edge_SpeedNotDouble,io::to_string(io::ErrorType::TypeError_Edge_SpeedNotDouble) + std::string(": ") + edgeTag + " -> 'speed_kmh'");
        return false;
    }
    const double speed_kmh = e["speed_kmh"].GetDouble();

    // max_tonnage_t
    if (!e.HasMember("max_tonnage_t")) {
        addError(res, io::ErrorType::MissingField_Edge_MaxTonnage,io::to_string(io::ErrorType::MissingField_Edge_MaxTonnage) + std::string(": ") + edgeTag);
        return false;
    }
    if (!e["max_tonnage_t"].IsDouble()) {
        addError(res, io::ErrorType::TypeError_Edge_MaxTonnageNotDouble,io::to_string(io::ErrorType::TypeError_Edge_MaxTonnageNotDouble) + std::string(": ") + edgeTag + " -> 'max_tonnage_t'");
        return false;
        }
    }

    return true;
}

void MapLoader::addError(LoadResults& res, io::ErrorType type, const std::string& message, std::optional<int> node_id, std::optional<int> edge_id) {
    res.errors_codes.push_back(LoadError{type,message,node_id,edge_id});
    res.errors.push_back(message);
}


bool MapLoader::buildGraph(const Document& doc, Graph& graph) {
    const Value& nodes = doc["nodes"];
    const Value& edges = doc["edges"];

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
