#include "AuthParser.hpp"

#include <string>
using namespace std;

namespace LogSupervisor::LogParser{

Auth::Auth(const string& filePath) :
    logFile(filePath)
{
    if(logFile.fail()){
        throw runtime_error(string("Unable to open ") + filePath + ": " + strerror(errno));
    }
}

std::string Auth::humanReadableLogType() const{
    return "SSH log checks";
}

std::string Auth::shortName(){
    return "auth";
}


void Auth::parseLog(){
    string currentLine;
    while(std::getline(logFile, currentLine)){
        optional<Authentication> auth = parseLine(currentLine);
        if(auth){
            m_auths.push_back(*auth);
        }
    }

}

optional<LogSupervisor::Authentication> Auth::parseLine(const std::string& line){
    // Regexes can be expensive (both at building and at using)
    // Fail early if it can. Luckily enough, we can filter interesting lines quite easily
    if(line.find("from") == string::npos){
        return {};
    }

    // Build the regex objects, only once if the lifetime of the program!
    //                                 TS     host   proc   PID                                user              IP
    static regex re_success_pk(R"raw(^(.*)\s+[^\s]+\s+.*\[[0-9]+\]: Accepted publickey for\s+([^\s]+)\s+from\s+([^\s]+)\s.*)raw");
    static regex re_success_pw(R"raw(^(.*)\s+[^\s]+\s+.*\[[0-9]+\]: Accepted password for\s+([^\s]+)\s+from\s+([^\s]+)\s.*)raw");

    //                                 TS     host   proc   PID                                          user              IP
    static regex re_failed(    R"raw(^(.*)\s+[^\s]+\s+.*\[[0-9]+\]: Failed password for invalid user\s+([^\s]+)\s+from\s+([^\s]+)\s.*)raw");

    smatch matches;
    if(regex_search(line, matches, re_success_pk)){
        return authFromMatches(matches, true);
    }else if(regex_search(line, matches, re_success_pw)){
        return authFromMatches(matches, true);
    }else if(regex_search(line, matches, re_failed)){
        return authFromMatches(matches, false);
    }else{
        return {};
    }
}

LogSupervisor::Authentication Auth::authFromMatches(const smatch& matches, bool success){
    Timestamp ts = matches.str(1);
    User user = matches.str(2);
    Address origin = matches.str(3);
    return Authentication(user, origin, ts, success);
}


vector<LogSupervisor::Authentication> Auth::all(){
    return m_auths;
}


} // namespace
