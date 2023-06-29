/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IddFixture.hpp"
#include "../IddRegex.hpp"

#include <string>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;
using namespace openstudio;

using boost::algorithm::trim;

TEST_F(IddFixture, IddRegex_FieldExtraction) {
  std::string iddFieldText(" A1,  \\field Name\
        \\required-field\
  A2 , \\field Inlet Node Name\
        \\required-field\
  A3 , \\field Outlet Node Name\
        \\required-field\
  N1 , \\field Rated Flow Rate\
        \\units m3/s\
        \\autosizable\
        \\ip-units gal/min\
  N2 , \\field Rated Pump Head\
        \\units Pa\
        \\default 179352\
        \\note default head is 60 feet\
        \\ip-units ftH2O\
  N3 , \\field Rated Power Consumption\
        \\units W\
        \\autosizable\
        \\ip-units W\
  N4 , \\field Motor Efficiency\
        \\type real\
        \\minimum> 0.0\
        \\maximum 1.0\
        \\default 0.9\
  N5 , \\field Fraction of Motor Inefficiencies to Fluid Stream\
        \\minimum 0.0\
        \\maximum 1.0\
        \\default 0.0\
  A4 , \\field Pump Control Type\
        \\type choice\
        \\key Continuous\
        \\key Intermittent\
        \\default Continuous\
  A5 ; \\field Pump Flow Rate Schedule Name\
        \\note Modifies the rated flow rate of the pump on a time basis. Default is\
        \\note that the pump is on and runs according to its other operational requirements\
        \\note specified above.  The schedule is for special pump operations.\
        \\type object-list\
        \\object-list ScheduleNames");
  trim(iddFieldText);

  std::string iddLastField("A5 ; \\field Pump Flow Rate Schedule Name\
        \\note Modifies the rated flow rate of the pump on a time basis. Default is\
        \\note that the pump is on and runs according to its other operational requirements\
        \\note specified above.  The schedule is for special pump operations.\
        \\type object-list\
        \\object-list ScheduleNames");

  std::string iddLastFieldLeftovers("A1,  \\field Name\
        \\required-field\
  A2 , \\field Inlet Node Name\
        \\required-field\
  A3 , \\field Outlet Node Name\
        \\required-field\
  N1 , \\field Rated Flow Rate\
        \\units m3/s\
        \\autosizable\
        \\ip-units gal/min\
  N2 , \\field Rated Pump Head\
        \\units Pa\
        \\default 179352\
        \\note default head is 60 feet\
        \\ip-units ftH2O\
  N3 , \\field Rated Power Consumption\
        \\units W\
        \\autosizable\
        \\ip-units W\
  N4 , \\field Motor Efficiency\
        \\type real\
        \\minimum> 0.0\
        \\maximum 1.0\
        \\default 0.9\
  N5 , \\field Fraction of Motor Inefficiencies to Fluid Stream\
        \\minimum 0.0\
        \\maximum 1.0\
        \\default 0.0\
  A4 , \\field Pump Control Type\
        \\type choice\
        \\key Continuous\
        \\key Intermittent\
        \\default Continuous");

  smatch matches;
  std::string firstMatch;
  std::string secondMatch;

  EXPECT_TRUE(boost::regex_search(iddFieldText, matches, iddRegex::lastField()));
  firstMatch = std::string(matches[1].first, matches[1].second);
  trim(firstMatch);
  secondMatch = std::string(matches[2].first, matches[2].second);
  trim(secondMatch);
  EXPECT_EQ(iddLastFieldLeftovers, firstMatch);
  EXPECT_EQ(iddLastField, secondMatch);
}
