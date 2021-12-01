#include "options.hpp"

#include <fstream>


bool OptionsState::GetOption(const std::string& option_name)
{
    auto& instance = GetInstance();

    if (instance.options.find(option_name) == instance.options.end())
    {
        return false;
    }

    return atoi(instance.options[option_name].c_str()) > 0 ? true : false;
}

void OptionsState::SetOption(const std::string& option_name, bool value)
{
    auto& instance = GetInstance();

    instance.options[option_name] = value ? "1" : "0";

    instance.SaveOptionsFile();
}

void OptionsState::ReadOptionsFile()
{
    std::ifstream stream;
    stream.open("options.file");

    if (stream.is_open())
    {
        std::string line;
        while (std::getline(stream, line)) 
        {
            auto pos = line.find('=');
            if (pos == std::string::npos)
            {
                continue;
            }

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1, std::string::npos);

            options[key] = value;
        }
    }

    stream.close();
}

void OptionsState::SaveOptionsFile()
{
    std::ofstream stream;
    stream.open("options.file");

    if (stream.is_open())
    {
        for (const auto& pair : options) 
        {
            stream << pair.first << "=" << pair.second << std::endl;
        }
    }

    stream.close();
}

OptionsState& OptionsState::GetInstance()
{
    static bool init = false;
    static OptionsState instance;
    if (!init)
    {
        instance.ReadOptionsFile();
        init = true;
    }

    return instance;
}
