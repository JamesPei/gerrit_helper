#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include "gerrit_helper.hpp"

int main(int argc, char* argv[]){
    cxxopts::Options options("gerrit_helper", "gerrit_helper <command> [OPTION...]");

    options.add_options()
        ("h,help", "print help messages")
        ("a,auth", "authenticate", cxxopts::value<bool>(), "auth user identify")    // default true
        ("u,user", "username", cxxopts::value<std::string>()->default_value(""), "username")
        ("p,password", "password", cxxopts::value<std::string>()->default_value(""), "password")
        ("i,info", "look change/commit info", cxxopts::value<std::vector<std::string>>()->default_value({}), "list of change-id or commit hash")
        ("f,file", "assign a file path", cxxopts::value<std::string>()->default_value(""), "file path")
        ("t,topic", "query by topic", cxxopts::value<bool>(), "topic")
        ("d,detail", "display more details", cxxopts::value<bool>(), "show details");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    GerritHelper::GerritHelper helper= GerritHelper::GerritHelper();

    if (result.count("auth")) {
        if(!result.count("user") || !result.count("password")){
            exit(0);
        }
        std::string user=result["user"].as<std::string>();
        std::string passwd=result["password"].as<std::string>();

        if(helper.Auth(user, passwd)){
            std::cout << "Authentication Success!" << std::endl;
        }else{
            std::cout << "Authentication Fail!" << std::endl;
        };
    }

    if (result.count("info")) {
        auto ids = result["info"].as<std::vector<std::string>>();

        GerritHelper::GerritHelper::ID_TYPE id_type=GerritHelper::GerritHelper::ID_TYPE::CHANGE_NUM;  // change_num
        if(result.count("topic")){
            id_type = GerritHelper::GerritHelper::ID_TYPE::TOPIC;
        }
        if(result.count("detail")){
            helper.Info(ids, id_type, true);
        }else{
            helper.Info(ids, id_type);
        }
    }

    return 0;
}