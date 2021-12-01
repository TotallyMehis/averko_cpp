#pragma once

#include <string>
#include <map>

class OptionsState
{
public:
    static bool GetOption(const std::string& option_name);
    static void SetOption(const std::string& option_name, bool value);
    
private:
    static OptionsState& GetInstance();

    void ReadOptionsFile();
    void SaveOptionsFile();

    std::map<std::string, std::string> options;
};
