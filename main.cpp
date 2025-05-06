#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include "gerrit_helper.hpp"

int main(int argc, char* argv[]){
    cxxopts::Options options("gerrit_helper", "gerrit_helper <command> [OPTION...]");

    options.add_options()
        ("command", "command type", cxxopts::value<std::string>(), "info|pick|trace")
        ("h,help", "print help messages")
        ("a,auth", "authenticate", cxxopts::value<bool>(), "auth user identify")    // default true
        ("u,user", "username", cxxopts::value<std::string>()->default_value(""), "username")
        ("url", "gerrit url", cxxopts::value<std::string>()->default_value(""), "gerrit url")
        ("p,password", "password", cxxopts::value<std::string>()->default_value(""), "password")
        ("i,info", "look change/commit info", cxxopts::value<std::vector<std::string>>()->default_value({}), "list of change-id or commit hash")
        ("f,file", "assign a file path", cxxopts::value<std::string>()->default_value(""), "file path")
        ("t,topic", "query by topic", cxxopts::value<bool>(), "topic")
        ("d,detail", "display more details", cxxopts::value<bool>(), "show details");

    // 设置第一个位置参数为操作类型
    options.parse_positional({"command", "info"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    std::string command;
    if(result.count("command")){
        command = result["command"].as<std::string>();
    }else{
        std::cout << "Error: No operation specified. Use 'info' or 'trace'.\n";
        std::cout << "Use --help for more information.\n";
        exit(1);
    }

    GerritHelper::GerritHelper helper= GerritHelper::GerritHelper();

    if (command == "auth") {
        if(!result.count("user") || !result.count("password") || !result.count("url")){
            exit(0);
        }
        std::string user=result["user"].as<std::string>();
        std::string passwd=result["password"].as<std::string>();
        std::string url=result["url"].as<std::string>();

        if(helper.Auth(user, passwd, url)){
            std::cout << "Authentication Success!" << std::endl;
        }else{
            std::cout << "Authentication Fail!" << std::endl;
        };
    }else if (command == "info") {
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
    }else if(command == "pick") {
        auto ids = result["info"].as<std::vector<std::string>>();
        for(auto id: ids){
            helper.Pick(id);
        }

    }

    return 0;
}