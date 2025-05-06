#include <unistd.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "gerrit_helper.hpp"
#include "common_macros.hpp"
#include "utils.hpp"


namespace GerritHelper{

GerritHelper::GerritHelper(){
    if(access("/tmp/.gerrit_user", F_OK)!=-1){
        std::ifstream input_file("/tmp/.gerrit_user");
        if(input_file.good()){
            input_file >> user;
            input_file >> passwd;
            input_file >> gerrit_url;
        }
        input_file.close();
    }
};

bool GerritHelper::Auth(std::string user, std::string passwd, std::string url){
    user = user;
    passwd = passwd;
    gerrit_url = url;

    cpr::Response r = cpr::Get(cpr::Url(gerrit_url+"/accounts/?q=name:"+user),
                      cpr::Authentication(user, passwd, cpr::AuthMode::BASIC));

    if(r.status_code==200){
        std::ofstream output_file("/tmp/.gerrit_user", std::ios::out);
        output_file << user << "\n";
        output_file << passwd << "\n";
        output_file << gerrit_url;

        output_file.close();

        return true;
    }else{
        return false;
    }
};

void GerritHelper::Info(const std::vector<std::string>& ids, ID_TYPE id_type, bool detail) const {
    for(auto id: ids){
        if(id_type==ID_TYPE::TOPIC){
            std::vector<json> json_objs;
            get_change_by_topic(id, json_objs, detail);
            for(json obj:json_objs){
                print_change_info(obj, detail);
            }
        }else{
            json json_obj;
            get_change_by_id(id, json_obj, detail);
            print_change_info(json_obj, detail);
        }
    }
};


void GerritHelper::print_change_info(const json& json_obj, bool detail) const {
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

    if (json_obj["owner"].contains("name") && json_obj["owner"]["name"].is_string()) {
        std::cout << "owner:" << json_obj["owner"]["name"] << "\n";
    } else if (json_obj["owner"].contains("_account_id") && json_obj["owner"]["_account_id"].is_number()) {
        json account_info;
        get_account(std::to_string(json_obj["owner"]["_account_id"].get<int>()), account_info);
        if(account_info.contains("name") && account_info["name"].is_string()) {
            std::cout << "owner:" << account_info["name"] << "\n";
        }
    }

    std::cout << "created:" << json_obj["created"] << "\n";
    std::cout << "updated:" << json_obj["updated"] << "\n";

    if(detail){

    }

    std::cout << std::endl;
}

bool GerritHelper::Pick(std::string id){
    return true;
};

void GerritHelper::get_change_by_id(const std::string& id, json& change_info, bool detail) const {
    cpr::Response r;
    r = cpr::Get(cpr::Url(gerrit_url+"/a/changes/"+id+"/detail"), 
            cpr::Authentication(user, passwd, cpr::AuthMode::BASIC));

    if(r.status_code==200){
        if(r.header["content-type"].find("application/json")!=std::string::npos){
            const std::string& raw_string{r.text.substr(5)};
            change_info = json::parse(raw_string); 
        }else{
            std::cout << r.text << std::endl;
        }
    }else if(r.status_code==401 && r.text=="Unauthorized"){
        std::cout << OUTPUT_RED << "Unauthorized! execute auth first" <<  COLOR_END << std::endl;
        exit(0);
    }
};

void GerritHelper::get_change_by_commit(const std::string& commit, json& change_info, bool detail) const {
    cpr::Response r;
};

void GerritHelper::get_change_by_topic(const std::string& topic, std::vector<json>& changes_info, bool detail) const {
    cpr::Response r;
    r = cpr::Get(cpr::Url(gerrit_url+"/a/changes/?q=topic:"+topic), 
            cpr::Authentication(user, passwd, cpr::AuthMode::BASIC));
    if(r.status_code==200){
        if(r.header["content-type"].find("application/json")!=std::string::npos){
                const std::string& raw_string{r.text.substr(5)};
                if(raw_string[0]!='{'){
                    std::vector<std::pair<uint32_t, uint32_t>> result;
                    get_json_string_from_raw(raw_string, result);

                    for(auto index_pair: result){
                        std::string json_string = raw_string.substr(index_pair.first, index_pair.second-index_pair.first+1);
                        json json_obj = json::parse(json_string);
                        changes_info.push_back(json_obj);
                    }
                }else{
                    json json_obj = json::parse(raw_string);
                    changes_info.push_back(json_obj);
                }

        }else{
            std::cout << r.text << std::endl;
        }
    }else if(r.status_code==401 && r.text=="Unauthorized"){
        std::cout << OUTPUT_RED << "Unauthorized! execute auth first" <<  COLOR_END << std::endl;
        exit(0);
    }
};

void GerritHelper::get_account(const std::string& id, json& account_info) const {
    cpr::Response r;
    r = cpr::Get(cpr::Url(gerrit_url+"/a/accounts/"+id), 
            cpr::Authentication(user, passwd, cpr::AuthMode::BASIC));
    if(r.status_code==200){
        if(r.header["content-type"].find("application/json")!=std::string::npos){
            const std::string& raw_string{r.text.substr(5)};
            try
            {
                account_info = json::parse(raw_string);
            }catch (const json::parse_error& e) {
                std::cerr << OUTPUT_RED << "JSON 解析错误: " << e.what() << COLOR_END << std::endl;
            }
        }else{
            std::cout << r.text << std::endl;
        }
    }else if(r.status_code==401 && r.text=="Unauthorized"){
        std::cout << OUTPUT_RED << "Unauthorized! execute auth first" <<  COLOR_END << std::endl;
        exit(0);
    }
};

}