/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
    ProgressBar pb;
    for (int i = 0; i <= 100; ++i){
      pb.setValue(i);
      EXPECT_EQ(i, pb.value());
      System::msleep(10);
    }
  }
}

TEST(ProgressBar, AdvancedTest)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){
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

