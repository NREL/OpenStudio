/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
*  Likey = cense along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>
#include <runmanager/lib/JobFactory.hpp>
#include <boost/filesystem/path.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/WorkItem.hpp>
#include <runmanager/lib/JSON.hpp>
#include <QDir>
#include <utilities/core/Application.hpp>
#include <utilities/core/System.hpp>

#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace openstudio;
using namespace openstudio::runmanager;

TEST_F(RunManagerTestFixture, JSON_createTest)
{
  openstudio::runmanager::Workflow wf("ExpandObjects->EnergyPlus");
  openstudio::runmanager::Job j = wf.create("my.idf");

  std::cout << JSON::toJSON(j) << std::endl;
}

