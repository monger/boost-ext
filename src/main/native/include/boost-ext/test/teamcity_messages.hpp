/* Copyright 2011 JetBrains s.r.o.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * $Revision: 88625 $
*/

#ifndef H_TEAMCITY_MESSAGES
#define H_TEAMCITY_MESSAGES

#include <string>
#include <iostream>

namespace JetBrains {

inline static std::string getFlowIdFromEnvironment() {
    const char *flowId = getenv("TEAMCITY_PROCESS_FLOW_ID");
    return flowId == NULL ? "" : flowId;
}
inline static bool underTeamcity() { return getenv("TEAMCITY_PROJECT_NAME") != NULL; }

class TeamcityMessages {
    std::ostream *m_out;
    
protected:
    std::string escape(std::string s) {
        std::string result;
    
        for (size_t i = 0; i < s.length(); i++) {
            char c = s[i];
        
            switch (c) {
            case '\n': result.append("|n"); break;
            case '\r': result.append("|r"); break;
            case '\'': result.append("|'"); break;
            case '|':  result.append("||"); break;
            case ']':  result.append("|]"); break;
            default:   result.append(&c, 1);
            }
        }
    
        return result;
    }

    void openMsg(const std::string &name) { *m_out << std::endl << "##teamcity[" << name; }
    void writeProperty(std::string name, std::string value) {
        *m_out << " " << name << "='" << escape(value) << "'";
    }
    void closeMsg() { *m_out << "]"; *m_out << std::endl; m_out->flush(); }

public:
    TeamcityMessages() : m_out(&std::cout) {}
    
    void setOutput(std::ostream &out) { m_out = &out; }
    
    void suiteStarted(std::string name, std::string flowid = "") {
        openMsg("testSuiteStarted");
        writeProperty("name", name);
        if(flowid.length() > 0) {
            writeProperty("flowId", flowid);
        }
    
        closeMsg();
    }
    void suiteFinished(std::string name, std::string flowid = "") {
        openMsg("testSuiteFinished");
        writeProperty("name", name);
        if(flowid.length() > 0) {
            writeProperty("flowId", flowid);
        }
    
        closeMsg();
    }
    
    void testStarted(std::string name, std::string flowid = "") {
        openMsg("testStarted");
        writeProperty("name", name);
        if(flowid.length() > 0) {
            writeProperty("flowId", flowid);
        }
    
        closeMsg();
    }
    void testFailed(std::string name, std::string message, std::string details, std::string flowid = "") {
        openMsg("testFailed");
        writeProperty("name", name);
        writeProperty("message", message);
        writeProperty("details", details);
        if(flowid.length() > 0) {
            writeProperty("flowId", flowid);
        }
    
        closeMsg();
    }
    void testIgnored(std::string name, std::string message, std::string flowid = "") {
        openMsg("testIgnored");
        writeProperty("name", name);
        writeProperty("message", message);
        if(flowid.length() > 0) {
            writeProperty("flowId", flowid);
        }
    
        closeMsg();
    }
    void testFinished(std::string name, int durationMs = -1, std::string flowid = "") {
        openMsg("testFinished");

        writeProperty("name", name);

        if(flowid.length() > 0) {
            writeProperty("flowId", flowid);
        }

        if(durationMs >= 0) {
            std::stringstream out;
            out << durationMs;
            writeProperty("duration", out.str());
        }
    
        closeMsg();
    } 
};

}

#endif /* H_TEAMCITY_MESSAGES */
