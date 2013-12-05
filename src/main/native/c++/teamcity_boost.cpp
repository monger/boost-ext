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

#include "boost-ext/test/teamcity_boost.hpp"

namespace JetBrains {

// Formatter implementation
string toString(const_string bstr) {
    stringstream ss;
    
    ss << bstr;
    
    return ss.str();
}

TeamcityBoostLogFormatter::TeamcityBoostLogFormatter(const std::string &_flowId)
: flowId(_flowId)
{}

TeamcityBoostLogFormatter::TeamcityBoostLogFormatter()
: flowId(getFlowIdFromEnvironment())
{}

void TeamcityBoostLogFormatter::log_start(ostream &out, counter_t test_cases_amount)
{}

void TeamcityBoostLogFormatter::log_finish(ostream &out)
{}

void TeamcityBoostLogFormatter::log_build_info(ostream &out)
{}

void TeamcityBoostLogFormatter::test_unit_start(ostream &out, test_unit const& tu) {
    messages.setOutput(out);

    if (tu.p_type == tut_case) {
        messages.testStarted(tu.p_name, flowId);
    } else {
        messages.suiteStarted(tu.p_name, flowId);
    }
    
    currentDetails.clear();
}

void TeamcityBoostLogFormatter::test_unit_finish(ostream &out, test_unit const& tu, unsigned long elapsed) {
    messages.setOutput(out);

    test_results const& tr = results_collector.results(tu.p_id);
    if (tu.p_type == tut_case) {
        if(!tr.passed()) {
            if(tr.p_skipped) {
                messages.testIgnored(tu.p_name, "ignored", flowId);
            } else if (tr.p_aborted) {
                messages.testFailed(tu.p_name, "aborted", currentDetails, flowId);
            } else {
                messages.testFailed(tu.p_name, "failed", currentDetails, flowId);
            }
        }
        
        messages.testFinished(tu.p_name, elapsed / 1000, flowId);
    } else {
        messages.suiteFinished(tu.p_name, flowId);
    }
}

void TeamcityBoostLogFormatter::test_unit_skipped(ostream &out, test_unit const& tu)
{}

void TeamcityBoostLogFormatter::log_exception(ostream &out, log_checkpoint_data const&, const_string explanation) {
    string what = toString(explanation);
    
    out << what << endl;
    currentDetails += what + "\n";
}

void TeamcityBoostLogFormatter::log_entry_start(ostream&, log_entry_data const&, log_entry_types let)
{}

void TeamcityBoostLogFormatter::log_entry_value(ostream &out, const_string value) {
    out << value;
    currentDetails += toString(value);
}

void TeamcityBoostLogFormatter::log_entry_finish(ostream &out) {
    out << endl;
    currentDetails += "\n";
}

}
