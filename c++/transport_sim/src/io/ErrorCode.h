#ifndef ERRORCODE_H
#define ERRORCODE_H
#include <string>
namespace io {
    enum class ErrorType {
        // I/O /JSON
        FileOpenFailed,
        JsonParseFailed,
        // Structure
        MissingKey_Nodes,
        MissingKey_Edges,
        TypeError_NodesNotArray,
        TypeError_EdgesNotArray,

        // Nodes errors
            //MissingField Errors
        MissingField_Node_Id,
        MissingField_Node_Lat,
        MissingField_Node_Lon,
        MissingField_Node_Name,
            //TypeErrors
        TypeError_Node_IdNotInt,
        TypeError_Node_LatNotDouble,
        TypeError_Node_LonNotDouble,
        TypeError_Node_NameNotString,
            // Logic Errors
        RangeError_Node_Lat,
        RangeError_Node_Lon,
        UniqueError_Node_ID,
        UniqueError_Node_Name,

        // Edges errors
            //MissingField Errors
        MissingField_Edge_Id,
        MissingField_Edge_From,
        MissingField_Edge_To,
        MissingField_Edge_Lenght_m,
        MissingField_Edge_Speed_mps,
        MissingField_Edge_MaxTonnage,
            //TypeErrors
        TypeError_Edge_IdNotInt,
        TypeError_Edge_FromNotInt,
        TypeError_Edge_ToNotInt,
        TypeError_Edge_LenghtNotDouble,
        TypeError_Edge_SpeedNotDouble,
        TypeError_Edge_MaxTonnageNotDouble,
            //LogicErrors

    };

    const char* to_string(ErrorType e){
      switch (e) {
        case ErrorType::FileOpenFailed: return "FileOpenFailed";
        case ErrorType::JsonParseFailed: return "JsonParseFailed";
        case ErrorType::MissingKey_Nodes: return "MissingKey_Nodes";
        case ErrorType::MissingKey_Edges: return "MissingKey_Edges";
        case ErrorType::TypeError_NodesNotArray: return "TypeError_NodesNotArray";
        case ErrorType::TypeError_EdgesNotArray: return "TypeError_EdgesNotArray";
        case ErrorType::MissingField_Node_Id: return "MissingField_Node_Id";
        case ErrorType::MissingField_Node_Lat: return "MissingField_Node_Lat";
        case ErrorType::MissingField_Node_Lon: return "MissingField_Node_Lon";
        case ErrorType::MissingField_Node_Name: return "MissingField_Node_Name";
        case ErrorType::TypeError_Node_IdNotInt: return "TypeError_Node_IdNotInt";
        case ErrorType::TypeError_Node_LonNotDouble: return "TypeError_Node_LonNotDouble";
        case ErrorType::TypeError_Node_LatNotDouble: return "TypeError_Node_LatNotDouble";
        case ErrorType::TypeError_Node_NameNotString: return "TypeError_Node_NameNotString";
        case ErrorType::RangeError_Node_Lat: return "RangeError_Node_Lat";
        case ErrorType::RangeError_Node_Lon: return "RangeError_Node_Lon";
        case ErrorType::UniqueError_Node_ID: return "UniqueError_Node_ID";
        case ErrorType::UniqueError_Node_Name: return "UniqueError_Node_Name";
        case ErrorType::MissingField_Edge_Id: return "MissingField_Edge_Id";
        case ErrorType::MissingField_Edge_From: return "MissingField_Edge_From";
        case ErrorType::MissingField_Edge_To: return "MissingField_Edge_To";
        case ErrorType::MissingField_Edge_Lenght_m: return "MissingField_Edge_Lenght_m";
        case ErrorType::MissingField_Edge_Speed_mps: return "MissingField_Edge_Speed_mps";
        case ErrorType::MissingField_Edge_MaxTonnage: return "MissingField_Edge_MaxTonnage";
        case ErrorType::TypeError_Edge_IdNotInt: return "TypeError_Edge_IdNotInt";
        case ErrorType::TypeError_Edge_FromNotInt: return "TypeError_Edge_FromNotInt";
        case ErrorType::TypeError_Edge_ToNotInt: return "TypeError_Edge_ToNotInt";
        case ErrorType::TypeError_Edge_LenghtNotDouble: return "TypeError_Edge_LenghtNotDouble";
        case ErrorType::TypeError_Edge_SpeedNotDouble: return "TypeError_Edge_SpeedNotDouble";
        case ErrorType::TypeError_Edge_MaxTonnageNotDouble: return "TypeError_Edge_MaxTonnageNotDouble";
        default: return "Unknown";
      }
    }
}

#endif //ERRORCODE_H
