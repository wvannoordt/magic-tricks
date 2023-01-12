#pragma once
#include <vector>
#include <string>
#include <sstream>
static inline void get_format_substrings(std::vector<std::string>& subStrings, const std::string& templateStr)
    {
        std::string delimiter = "{}";
        std::string templateStrCopy = templateStr;
        size_t pos = 0;
        std::string token;
        while ((pos = templateStrCopy.find(delimiter)) != std::string::npos)
        {
            token = templateStrCopy.substr(0, pos);
            subStrings.push_back(token);
            templateStrCopy.erase(0, pos + delimiter.length());
        }
        subStrings.push_back(templateStrCopy);
    }

    static inline std::vector<std::string> string_split(std::string templateStr, std::string delimiter)
    {
        std::vector<std::string> subStrings;
        std::string templateStrCopy = templateStr;
        size_t pos = 0;
        std::string token;
        while ((pos = templateStrCopy.find(delimiter)) != std::string::npos)
        {
            token = templateStrCopy.substr(0, pos);
            subStrings.push_back(token);
            templateStrCopy.erase(0, pos + delimiter.length());
        }
        subStrings.push_back(templateStrCopy);
        return subStrings;
    }
    
    template <typename T> static inline void strformat_recursive (const std::string& templateStr, std::ostringstream& strstream, std::vector<std::string>& subStrings, int& lev, const T& t)
    {
        strstream << subStrings[lev] << t;
        lev++;
    }
    
    template <typename T, typename... Ts> static inline void strformat_recursive (const std::string& templateStr, std::ostringstream& strstream, std::vector<std::string>& subStrings, int& lev, const T& t, Ts... ts)
    {
        strstream << subStrings[lev] << t;
        lev++;
        strformat_recursive(templateStr, strstream, subStrings, lev, ts...);
    }
    
    static inline std::string strformat (std::string templateStr) { return templateStr; }
    
    template <typename T, typename... Ts> static inline std::string strformat (const std::string& templateStr, const T& t, Ts... ts)
    {
        std::ostringstream strstream;
        std::vector<std::string> subStrings;
        get_format_substrings(subStrings, templateStr);
        if (((sizeof...(Ts))+1)!=(subStrings.size()-1))
        {
            std::string err_string = "strformat invocation with template string \"" + templateStr +"\" and wrong number of arguments (" +
                std::to_string((sizeof...(Ts))+1) + " args instead of " + std::to_string(subStrings.size()-1) + ")";
            throw std::runtime_error(err_string.c_str());
        }
        int lev = 0;
        strformat_recursive(templateStr, strstream, subStrings, lev, t, ts...);
        strstream << subStrings[lev];
        return strstream.str();
    }