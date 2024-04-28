#include "Global.h"
#include <GMLIB/Files/Config/JsonConfig.h>
#include <GMLIB/Files/JsonFile.h>

namespace GMLIB::Files {

JsonConfig::JsonConfig(std::string const& filePath, std::string const& defaultJson) : mFilePath(filePath) {
    auto json_value = nlohmann::ordered_json::parse(defaultJson, nullptr, true, true);
    mValue          = json_value;
}

JsonConfig::JsonConfig(std::string const& filePath, nlohmann::ordered_json const& defaultJson)
: mFilePath(filePath),
  mValue(defaultJson) {}

JsonConfig::~JsonConfig() { writeFile(); }

bool JsonConfig::init() {
    auto dirPath = std::filesystem::path(mFilePath).parent_path();
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
    if (std::filesystem::exists(mFilePath)) {
        std::ifstream inputFile(mFilePath);
        std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();
        try {
            auto dataJson = nlohmann::ordered_json::parse(fileContent, nullptr, true, true);
            mValue.merge_patch(dataJson);
        } catch (...) {
            auto backupPath = mFilePath + "_old";
            std::filesystem::rename(mFilePath, backupPath);
        }
    }
    return this->writeFile();
}

bool JsonConfig::reload() {
    auto newFile = JsonFile::readFromOrderedFile(mFilePath);
    mValue.merge_patch(newFile);
    return this->writeFile();
}

bool JsonConfig::writeFile() {
    try {
        std::ofstream newFile(mFilePath);
        if (!newFile.is_open()) {
            return false;
        }
        newFile << mValue.dump(4);
        newFile.close();
        return true;
    } catch (...) {
        return false;
    }
}

nlohmann::ordered_json JsonConfig::getSelf() { return mValue; }

bool JsonConfig::deleteKey(std::vector<std::string> const& path) {
    try {
        if (path.empty()) {
            return false;
        }
        nlohmann::ordered_json* jsonObj = &mValue;
        for (int i = 0; i <= path.size() - 2; i++) {
            if (jsonObj->is_object() && jsonObj->contains(path[i])) {
                jsonObj = &(*jsonObj)[path[i]];
            } else {
                return false;
            }
        }
        jsonObj->erase(path.back());
        return this->writeFile();
    } catch (...) {
        return false;
    }
}

} // namespace GMLIB::Files
