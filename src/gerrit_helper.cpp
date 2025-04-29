#include <unistd.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "gerrit_helper.hpp"
#include "common_macros.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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

    cpr::Response r = cpr::Get(cpr::Url(gerrit_url+"/accounts/?q=name:"+user),
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

void GerritHelper::Info(const std::vector<std::string>& ids, bool detail) const {
    cpr::Response r;

    for(auto id: ids){
        if(std::all_of(id.begin(), id.end(), ::isdigit)){
            r = cpr::Get(cpr::Url(gerrit_url+"/a/changes/"+id+"/detail"), 
                    cpr::Authentication(user, passwd, cpr::AuthMode::BASIC));
        }else if(std::all_of(id.begin(), id.end(), ::isalnum)){

        }

        if(r.status_code==200){
            if(r.header["content-type"].find("application/json")!=std::string::npos){
                try {
                    json json_obj = json::parse(r.text.substr(5));
                    std::cout << "---------- change number:" << json_obj["_number"] << " ----------\n";
                    std::cout << "change id:" << json_obj["change_id"] << "\n";
                    std::cout << "project:" << json_obj["project"] << "\n";
                    std::cout << "branch:" << json_obj["branch"] << "\n";
                    std::cout << "subject:" << json_obj["subject"] << "\n";

                    std::string status = json_obj["status"].get<std::string>();
                    if(status=="NEW") {
                        std::cout << "status: NEW" << std::endl;
                    }else if(status=="MERGED"){
                        std::cout << "status:" << OUTPUT_GREEN << "MERGED" << COLOR_END << std::endl;
                    }else if(status=="ABANDONED"){
                        std::cout << "status:" << OUTPUT_RED << "ABANDONED" << COLOR_END << "\n";
                    }

                    std::cout << "owner:" << json_obj["owner"]["name"] << "\n";
                    std::cout << "created:" << json_obj["created"] << "\n";
                    std::cout << "updated:" << json_obj["updated"] << "\n";

                    if(detail){

                    }

                    std::cout << std::endl;
                } catch (const json::parse_error& e) {
                    std::cerr << "JSON 解析错误: " << e.what() << std::endl;
                }
            }else{
                std::cout << r.text << std::endl;
            }
        }else if(r.status_code==401 && r.text=="Unauthorized"){
            std::cout << OUTPUT_RED << "Unauthorized! execute --auth first" <<  COLOR_END << std::endl;
        }
    }
};

bool GerritHelper::Pick(std::string id){
    return true;
};

}