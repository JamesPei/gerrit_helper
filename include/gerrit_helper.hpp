#pragma once
#ifndef __GERRIT_HELPER__
#define __GERRIT_HELPER__

#include <string>
#include <vector>
#include <cpr/cpr.h>

namespace GerritHelper{

class GerritHelper{
public:
    GerritHelper(std::string url="https://gerrit.senseauto.com");

    bool Auth(std::string username, std::string passwd);

    void Info(const std::vector<std::string>& ids, bool detail=false) const;

    bool Pick(std::string id);

private:
    bool access_check();


private:
    std::string user;
    std::string passwd;
    std::string gerrit_url;
};

}

#endif