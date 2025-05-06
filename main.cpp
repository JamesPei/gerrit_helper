#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include "gerrit_helper.hpp"

int main(int argc, char* argv[]){
    cxxopts::Options options("gerrit_helper", "gerrit_helper <command> [OPTION...]");

    options.add_options()
        ("command", "command type", cxxopts::value<std::string>(), "info|pick|trace")
        ("value", "value for info or pick", cxxopts::value<std::string>(), "value") // 通用参数
        ("h,help", "print help messages")
        ("u,user", "username", cxxopts::value<std::string>()->default_value(""), "username")
        ("p,password", "password", cxxopts::value<std::string>()->default_value(""), "password")
        ("i,info", "look change/commit info", cxxopts::value<std::vector<std::string>>()->default_value({}), "list of change-id or commit hash")
        ("f,file", "assign a file path", cxxopts::value<std::string>()->default_value(""), "file path")
        ("t,topic", "set topic", cxxopts::value<bool>(), "topic")
        ("c,commit", "set commit", cxxopts::value<bool>(), "commit id")
        ("branch", "which branches to pick", cxxopts::value<std::vector<std::string>>()->default_value({}), "branch name")
        ("d,detail", "display more details", cxxopts::value<bool>(), "show details");

    // 设置第一个位置参数为操作类型，第二个位置参数为通用值
    options.parse_positional({"command", "value"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    std::string command;
    if (result.count("command")) {
        command = result["command"].as<std::string>();
    } else {
        std::cout << "Error: No operation specified. Use 'info' or 'pick'.\n";
        std::cout << "Use --help for more information.\n";
        exit(1);
    }

    std::string value;
    if (result.count("value")) {
        value = result["value"].as<std::string>();
    }

    GerritHelper::GerritHelper helper = GerritHelper::GerritHelper();

    if (command == "info") {
        std::vector<std::string> ids;
        if (!value.empty()) {
            ids.push_back(value); // 将第二个位置参数作为 info 的值
        }
        if (result.count("i")) {
            auto additional_ids = result["i"].as<std::vector<std::string>>();
            ids.insert(ids.end(), additional_ids.begin(), additional_ids.end());
        }

        GerritHelper::GerritHelper::ID_TYPE id_type = GerritHelper::GerritHelper::ID_TYPE::CHANGE_NUM;
        if (result.count("topic")) {
            id_type = GerritHelper::GerritHelper::ID_TYPE::TOPIC;
        } else if (result.count("commit")) {
            id_type = GerritHelper::GerritHelper::ID_TYPE::COMMIT_ID;
        }

        if (result.count("detail")) {
            helper.Info(ids, id_type, true);
        } else {
            helper.Info(ids, id_type);
        }
    } else if (command == "pick") {
        if (!value.empty() && result.count("branch")) {
            std::string commit_id = value; // 将第二个位置参数作为 pick 的值
            auto branches = result["branch"].as<std::vector<std::string>>();
            helper.Pick(commit_id, branches);
        } else {
            std::cout << "Error: No commit id or branch specified.\n";
            std::cout << "Use --help for more information.\n";
            exit(1);
        }
    } else {
        std::cout << "Error: Invalid operation '" << command << "'. Use 'info' or 'pick'.\n";
        std::cout << "Use --help for more information.\n";
        exit(1);
    }

    return 0;
}