#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include "gerrit_helper.hpp"
#include "common_macros.hpp"

int main(int argc, char* argv[]){
    cxxopts::Options options("gerrit_helper", "gerrit_helper <command> [OPTION...]");

    options.add_options()
        ("command", "command type", cxxopts::value<std::string>(), "info|pick|trace")
        ("value", "value for info|pick|auth", cxxopts::value<std::vector<std::string>>()->default_value({}), "value") // 通用参数
        ("h,help", "print help messages")
        ("u,user", "username", cxxopts::value<std::string>()->default_value(""), "username")
        ("url", "gerrit url", cxxopts::value<std::string>()->default_value(""), "gerrit url")
        ("p,password", "password", cxxopts::value<std::string>()->default_value(""), "password")
        ("f,file", "assign a file path", cxxopts::value<std::string>()->default_value(""), "file path")
        ("t,topic", "set topic", cxxopts::value<bool>(), "topic")
        ("c,commit", "set commit", cxxopts::value<bool>(), "commit id")
        ("branch", "which branches to pick", cxxopts::value<std::vector<std::string>>()->default_value({}), "branch name")
        ("d,detail", "display more details", cxxopts::value<bool>(), "show details")
        ("o,output", "output result to a file", cxxopts::value<std::string>(), "output file");

    // 设置第一个位置参数为操作类型，第二个位置参数为通用值
    options.parse_positional({"command", "value"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << "gerrit_helper <command> [OPTION...]\n";
        std::cout << "Usage:\n";
        std::cout << "   command: can be auth|info|pick \n";
        std::stringstream help_text(options.help());
        std::string line_content;

        int i=0;
        while(std::getline(help_text, line_content)){
            if(i<3){
                i++;
                continue;
            }
            std::cout << line_content << std::endl;
        }
        exit(0);
    }

    std::string command;
    if (result.count("command")) {
        command = result["command"].as<std::string>();
    } else {
        std::cout << "Error: No operation specified. Use 'info|pick|auth'.\n";
        std::cout << "Use --help for more information.\n";
        exit(1);
    }

    std::vector<std::string> values;
    if (result.count("value")) {
        values = result["value"].as<std::vector<std::string>>();
    }

    GerritHelper::GerritHelper helper = GerritHelper::GerritHelper();

    std::string output_path;
    if(result.count("output")){
        output_path = result["output"].as<std::string>();
    }
    
    std::ifstream file;
    if (result.count("file")) {
        std::string file_path = result["file"].as<std::string>();
        file.open(file_path);
        if (!file.is_open()) {
            std::cout << "Error: Unable to open file " << file_path << "\n";
            exit(1);
        }
    }

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
        std::vector<std::string> ids;
        if (!values.empty()) {
            ids.insert(ids.end(), values.begin(), values.end());
        }

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                // strim line to remove leading and trailing whitespace
                size_t head = line.find_first_not_of(" \t\n\r\f\v");
                size_t tail = line.find_last_not_of(" \t\n\r\f\v");
                if(head == std::string::npos || tail == std::string::npos) {
                    continue; // skip empty lines
                }
                ids.push_back(line.substr(head, tail-head+1));
            }
        }

        GerritHelper::GerritHelper::ID_TYPE id_type = GerritHelper::GerritHelper::ID_TYPE::CHANGE_NUM;
        if (result.count("topic")) {
            id_type = GerritHelper::GerritHelper::ID_TYPE::TOPIC;
        } else if (result.count("commit")) {
            id_type = GerritHelper::GerritHelper::ID_TYPE::COMMIT_ID;
        }

        if (result.count("detail")) {
            helper.Info(ids, id_type, true, &output_path);
        } else {
            helper.Info(ids, id_type, false, &output_path);
        }
    } else if (command == "pick") {
        if (!values.empty() && result.count("branch")) {
            std::vector<std::string> commit_hashes;
            std::vector<GerritHelper::PickResult> picked_result;
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    // strim line to remove leading and trailing whitespace
                    size_t head = line.find_first_not_of(" \t\n\r\f\v");
                    size_t tail = line.find_last_not_of(" \t\n\r\f\v");
                    if(head == std::string::npos || tail == std::string::npos) {
                        continue; // skip empty lines
                    }
                    commit_hashes.push_back(line.substr(head, tail-head+1));
                }
            }else{
                std::string commit_id = values[0]; // 将第二个位置参数作为 pick 的值
                commit_hashes.push_back(commit_id);
            }
            auto branches = result["branch"].as<std::vector<std::string>>();
            helper.Pick(commit_hashes, branches, picked_result);
            for(const auto& picked_info: picked_result){
                std::cout << picked_info.commit_hash << ":\n";
                std::cout << "      " << picked_info.branch << ":" << "Picked ";
                if(picked_info.success){
                    std::cout << OUTPUT_GREEN << "success" << COLOR_END << std::endl;
                } else {
                    std::cout << OUTPUT_RED << "fail" << COLOR_END << std::endl;
                    std::cout << "          message:" << picked_info.message << std::endl;
                }
            }
        } else {
            std::cout << "Error: No commit_hash or branch specified.\n";
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