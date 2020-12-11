/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
