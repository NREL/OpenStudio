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

#include "../ProgressBar.hpp"
#include "../../core/Application.hpp"
#include "../../core/System.hpp"
#include "../../idf/Workspace.hpp"
#include "../../idf/WorkspaceObject.hpp"
#include "../../idf/IdfObject.hpp"
#include "../../idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;

///////////////////////////////////////////////////////////////////////////////
// *** HELPER CLASSES ***
///////////////////////////////////////////////////////////////////////////////


class ProgressBar2 : public ProgressBar
{
public:
  ProgressBar2()
    : m_called(false)
  {}

  virtual void onPercentageUpdated(double percentage) override
  {
    m_called = true;
  }

  bool called() const {return m_called;}

private:
  bool m_called;
};

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN TESTS ***
///////////////////////////////////////////////////////////////////////////////

TEST(ProgressBar, BasicTest)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){
	{
      ProgressBar pb;
      for (int i = 0; i <= 100; ++i){
        pb.setValue(i);
        EXPECT_EQ(i, pb.value());
        System::msleep(10);
      }
    }
    System::msleep(10);
  }
  
}

TEST(ProgressBar, AdvancedTest)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){
    {
      ProgressBar pb1;
      ProgressBar pb2;
      ProgressBar pb3;

      for (unsigned i = 0; i <= 100; ++i){
        pb1.setValue(i);
        pb2.setValue(100 - i);
        pb3.setValue((10 * i) % 100);
        System::msleep(10);
      }
    }
    System::msleep(10);
  }
}

TEST(ProgressBar, WorkspaceTest)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){
    ProgressBar2 pb;
    EXPECT_FALSE(pb.called());

    StrictnessLevel level(StrictnessLevel::Draft);
    IddFileType fileType(IddFileType::EnergyPlus);
    Workspace workspace(level, fileType);
    workspace.connectProgressBar(pb);

    IdfObjectVector objects;
    objects.push_back(IdfObject(IddObjectType::Zone));
    workspace.addObjects(objects);

    System::msleep(10);

    EXPECT_TRUE(pb.called());
  }
}

