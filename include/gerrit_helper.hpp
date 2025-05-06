#pragma once
#ifndef __GERRIT_HELPER__
#define __GERRIT_HELPER__

#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace GerritHelper{

class GerritHelper{
public:
    enum class ID_TYPE{
        CHANGE_ID=1,
        CHANGE_NUM,
        HASH,
        TOPIC
    };

    GerritHelper();

    bool Auth(std::string username, std::string passwd, std::string url);

    void Info(const std::vector<std::string>& ids, ID_TYPE type, bool detail=false) const;

    bool Pick(std::string id);

private:
    bool access_check();

    void print_change_info(const json& json_obj, bool detail=false) const;

    void get_change_by_id(const std::string& id, json& json_obj, bool detail=false) const;
    void get_change_by_commit(const std::string& commit, json& json_obj, bool detail=false) const;
    void get_change_by_topic(const std::string& topic, std::vector<json>& json_objs, bool detail=false) const;

    void get_account(const std::string& id, json& json_obj) const;

private:
    std::string user;
    std::string passwd;
    std::string gerrit_url;
};

}

#endif