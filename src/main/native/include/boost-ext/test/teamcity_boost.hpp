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

#ifndef H_TEAMCITY_BOOST
#define H_TEAMCITY_BOOST

#include <boost/test/results_collector.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log_formatter.hpp>

#include "boost-ext/test/teamcity_messages.hpp"

using namespace boost::unit_test;

namespace JetBrains {

// Custom formatter for TeamCity messages
class TeamcityBoostLogFormatter: public boost::unit_test::unit_test_log_formatter {
    TeamcityMessages messages;
    std::string currentDetails;
    std::string flowId;
    
    std::string toString(const_string bstr) { std::stringstream ss; ss << bstr; return ss.str(); }
    
public:
    TeamcityBoostLogFormatter(const std::string &_flowId) : flowId(_flowId) {}
    TeamcityBoostLogFormatter() : flowId(getFlowIdFromEnvironment()) {}
    
    void log_start(std::ostream&, boost::unit_test::counter_t test_cases_amount) {}
    void log_finish(std::ostream&) {}
    void log_build_info(std::ostream&) {}

    void test_unit_start(std::ostream &out, boost::unit_test::test_unit const& tu) {
        messages.setOutput(out);

        if (tu.p_type == tut_case) {
            messages.testStarted(tu.p_name, flowId);
        } else {
            messages.suiteStarted(tu.p_name, flowId);
        }
    
        currentDetails.clear();
    }
    void test_unit_finish(std::ostream &out, boost::unit_test::test_unit const& tu, unsigned long elapsed) {
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
    void test_unit_skipped(std::ostream&, boost::unit_test::test_unit const& tu) {}

    void log_exception(std::ostream &out,
                       boost::unit_test::log_checkpoint_data const&,
                       boost::unit_test::const_string explanation) {
        std::string what = toString(explanation);
    
        out << what << std::endl;
        currentDetails += what + "\n";
    }

    void log_entry_start(std::ostream&, boost::unit_test::log_entry_data const&, log_entry_types let) {}
    void log_entry_value(std::ostream &out, boost::unit_test::const_string value) {
        out << value;
        currentDetails += toString(value);
    }
    void log_entry_finish(std::ostream &out) {
        out << std::endl;
        currentDetails += "\n";
    }
};

// Fake fixture to register formatter
struct TeamcityFormatterRegistrar {
    TeamcityFormatterRegistrar() {
        if (JetBrains::underTeamcity()) {
            boost::unit_test::unit_test_log.set_formatter(new JetBrains::TeamcityBoostLogFormatter());
            boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_successful_tests);
        }
    }
};
#ifdef BOOST_TEST_MODULE
    BOOST_GLOBAL_FIXTURE(TeamcityFormatterRegistrar);
#endif

}

#endif /* H_TEAMCITY_BOOST */
