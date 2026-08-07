#pragma once
#include <string>
class Config { public: bool load(const std::string& path); std::string systemName; std::string version; std::string intelligence; std::string personality; };
