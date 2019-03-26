/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idd/IddFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <resources.hxx>

#include <sstream>

using openstudio::model::OptionalModel;
using openstudio::energyplus::ForwardTranslator;
using openstudio::energyplus::ReverseTranslator;
using openstudio::energyplus::ErrorFile;
using openstudio::IdfFile;
using openstudio::OptionalIdfFile;
using openstudio::Workspace;
using openstudio::OptionalWorkspace;
using openstudio::SqlFile;
using openstudio::OptionalInt;
using openstudio::IddFileType;

TEST_F(EnergyPlusFixture,IddFile)
{
  boost::optional<openstudio::IddFile> iddFile = openstudio::IddFile::load(resourcesPath() / openstudio::toPath("energyplus/ProposedEnergy+.idd"));
  ASSERT_TRUE(iddFile);
}


/*
TEST_F(EnergyPlusFixture,Translator_RoundTrip)
{
  // this is typical operation of translator

  openstudio::path epDir = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/");
  openstudio::path idfPath = epDir / openstudio::toPath("in.idf");
  openstudio::path sqlPath = epDir / openstudio::toPath("eplusout.sql");
  openstudio::path errorPath = epDir / openstudio::toPath("eplusout.err");

  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  SqlFile sqlFile(sqlPath);
  ErrorFile errorFile(errorPath);

  ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel model = reverseTranslator.convert();
  ASSERT_TRUE(model);

  model->setSqlFile(sqlFile);

  ForwardTranslator forwardTranslator(*model);
  OptionalWorkspace outWorkspace = forwardTranslator.convert();
  ASSERT_TRUE(outWorkspace);
}
*/
