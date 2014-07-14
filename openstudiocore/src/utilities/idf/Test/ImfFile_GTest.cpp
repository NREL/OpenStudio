/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
#include "IdfFixture.hpp"
#include "../ImfFile.hpp"
#include "../../core/Path.hpp"
#include "../../core/Optional.hpp"
#include "../../core/Logger.hpp"

#include <resources.hxx>

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture, ImfFile)
{
  EXPECT_TRUE(imfFile.numSections() > 0);
  StringVector sectionNames = imfFile.sectionNames();
  unsigned i = 1;
  LOG(Info,"Sections in the IdfFixture ImfFile:");
  for (const std::string& name : sectionNames) {
    LOG(Info,"  " << i << ": " << name << ", " << imfFile.section(name).size() << " objects");
    ++i;
  }
}

