#include "utils.hpp"

namespace GerritHelper{

void get_json_string_from_raw(const std::string& raw_string, std::vector<std::pair<uint32_t, uint32_t>>& result){
    int bracket_count=-1;
    uint32_t start_index=0;
    uint32_t end_index=0;
    for(int i=0; i< raw_string.size(); ++i){
        if(raw_string[i]=='{'){
            if(bracket_count<0){
                bracket_count = 1;
                start_index = i;
            }else{
                bracket_count++;
            }
        }else if(raw_string[i]=='}'){
            bracket_count--;
        }

        if(bracket_count==0){
            end_index = i;
            result.push_back({start_index, end_index});
            bracket_count = -1;
        }
    }
};

}