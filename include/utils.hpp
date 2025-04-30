#pragma once
#ifndef __GERRIT_HELPER_UTILS__
#define __GERRIT_HELPER_UTILS__

#include <string>
#include <vector>
#include <utility>

namespace GerritHelper{

void get_json_string_from_raw(const std::string& raw_string, std::vector<std::pair<uint32_t, uint32_t>>& result);

}

#endif