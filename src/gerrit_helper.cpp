#include <unistd.h>
#include <fstream>
#include "gerrit_helper.hpp"

namespace GerritHelper{

GerritHelper::GerritHelper(std::string gerrit_url):gerrit_url(gerrit_url){
    if(access("/tmp/.gerrit_user", F_OK)!=-1){
        std::ifstream input_file("/tmp/.gerrit_user");
        if(input_file.good()){
            input_file >> user;
            input_file >> passwd;
        }
        input_file.close();
    }
};

bool GerritHelper::Auth(std::string user, std::string passwd){
    user = user;
    passwd = passwd;

    cpr::Response r = cpr::Get(cpr::Url(gerrit_url+"/path/to/api/"),
                      cpr::Authentication(user, passwd, cpr::AuthMode::BASIC));

    if(r.status_code==200){
        std::ofstream output_file("/tmp/.gerrit_user", std::ios::out);
        output_file << user << "\n";
        output_file << passwd;

        return true;
    }else{
        return false;
    }
};

void GerritHelper::Info(std::string id){

};

bool GerritHelper::Pick(std::string id){
    return true;
};

}