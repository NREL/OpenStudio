/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <gtest/gtest.h>
#include "ProjectFixture.hpp"

#include "../URLSearchPathRecord.hpp"
#include "../URLSearchPathRecord_Impl.hpp"
#include "../ProjectDatabase.hpp"

#include "../../utilities/idf/URLSearchPath.hpp"

#include <resources.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::project;

TEST_F(ProjectFixture, URLSearchPathRecord)
{
  ProjectDatabase database = getCleanDatabase("URLSearchPathRecord");

  URLSearchPath localSearchPath(toPath("./local/"), URLSearchPath::ToCurrentWorkingDir, IddObjectType::OS_WeatherFile);
  URLSearchPath remoteSearchPath(QUrl("https://www.openstudio.net"), URLSearchPath::ToInputFile);

  URLSearchPathRecord localSearchPathRecord(localSearchPath, database);
  URLSearchPathRecord remoteSearchPathRecord(remoteSearchPath, database);

  std::vector<URLSearchPathRecord> urlSearchPathRecords = URLSearchPathRecord::getURLSearchPathRecords(database);
  ASSERT_EQ(2u, urlSearchPathRecords.size());

  URLSearchPath localSearchPath2 = localSearchPathRecord.urlSearchPath();
  EXPECT_TRUE(localSearchPath.getUrl() == localSearchPath2.getUrl());
  EXPECT_TRUE(localSearchPath.getRelativity() == localSearchPath2.getRelativity());
  ASSERT_TRUE(localSearchPath.getIddObjectType());
  ASSERT_TRUE(localSearchPath2.getIddObjectType());
  EXPECT_TRUE(localSearchPath.getIddObjectType()->value() == localSearchPath2.getIddObjectType()->value());

  URLSearchPath remoteSearchPath2 = remoteSearchPathRecord.urlSearchPath();
  EXPECT_TRUE(remoteSearchPath.getUrl() == remoteSearchPath2.getUrl());
  EXPECT_TRUE(remoteSearchPath.getRelativity() == remoteSearchPath2.getRelativity());
  EXPECT_FALSE(remoteSearchPath.getIddObjectType());
  EXPECT_FALSE(remoteSearchPath2.getIddObjectType());

}
