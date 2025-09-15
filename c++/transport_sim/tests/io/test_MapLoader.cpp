// Copyright 2005, Google Inc.
// All rights reserved.

#include "gtest/gtest.h"
#include "io/MapLoader.h"

#include <fstream>
#include <filesystem>
#include <string>
#include "rapidjson/document.h"

using rapidjson::Document;
namespace fs = std::filesystem;

static fs::path testData(const std::string& filename) {
    // __FILE__ = absolútna cesta k TOMUTO .cpp testu
    return fs::path(__FILE__).parent_path() / "data" / filename;
}

TEST(MaploaderTest, ParseFile) {
    // tests/io/test_MapLoader.cpp  ->  tests/io/data/test_parseFile.json
    const fs::path path = testData("test_parseFile.json");
    ASSERT_TRUE(fs::exists(path)) << "Test data file not found: " << path;

    MapLoader loader;
    Document doc;
    LoadResults results;

    bool success = loader.parsefile(path.string(), doc, results);
    ASSERT_TRUE(success);
    ASSERT_TRUE(doc.IsObject());
    ASSERT_TRUE(doc.HasMember("nodes"));
    ASSERT_TRUE(doc.HasMember("edges"));
    ASSERT_TRUE(doc["nodes"].IsArray());
    ASSERT_TRUE(doc["edges"].IsArray());
}