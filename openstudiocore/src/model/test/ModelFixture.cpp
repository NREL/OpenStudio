/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../../utilities/idd/IddKey.hpp"
#include "../../utilities/core/Finder.hpp"

#include <resources.hxx>

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void ModelFixture::SetUp() {}

void ModelFixture::TearDown() {}

void ModelFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./ModelTestFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();

  // create resources folder for scratch files
  openstudio::filesystem::create_directory(resourcesPath()/toPath("model"));
}

void ModelFixture::TearDownTestCase()
{
  logFile->disable();
}

void ModelFixture::testBooleanIddField(const openstudio::IddField& iddField,
                                       const boost::optional<std::string>& defaultValue)
{
  EXPECT_TRUE(iddField.properties().type == openstudio::IddFieldType::ChoiceType);
  openstudio::IddKeyVector keys = iddField.keys();
  EXPECT_EQ(2u,keys.size());
  openstudio::NameFinder<openstudio::IddKey> finder("Yes");
  openstudio::IddKeyVector::const_iterator it = std::find_if(keys.begin(),keys.end(),finder);
  EXPECT_FALSE(it == keys.end());
  finder = openstudio::NameFinder<openstudio::IddKey>("No");
  it = std::find_if(keys.begin(),keys.end(),finder);
  EXPECT_FALSE(it == keys.end());
  if (defaultValue) {
    ASSERT_TRUE(iddField.properties().stringDefault);
    EXPECT_EQ(*defaultValue,iddField.properties().stringDefault.get());
  }
}

double ModelFixture::tol(1.0E-5);
boost::optional<openstudio::FileLogSink> ModelFixture::logFile;
