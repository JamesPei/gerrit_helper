#pragma once
#ifndef __GERRIT_HELPER__
#define __GERRIT_HELPER__

#include <string>
#include <vector>
#include <cpr/cpr.h>

namespace GerritHelper{

class GerritHelper{
public:
    enum class ID_TYPE{
        CHANGE_ID=1,
        CHANGE_NUM,
        HASH,
        TOPIC
    };

    GerritHelper(std::string url="https://gerrit.senseauto.com");

    bool Auth(std::string username, std::string passwd);

    void Info(const std::vector<std::string>& ids, ID_TYPE type, bool detail=false) const;

    bool Pick(std::string id);

private:
    bool access_check();

    void parse_print(const std::string& json_string ,bool detail=false) const;


private:
    std::string user;
    std::string passwd;
    std::string gerrit_url;
};

}

#endif