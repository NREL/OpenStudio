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

#include "../FileReference.hpp"

#include <resources.hxx>

using openstudio::toPath;
using openstudio::FileReference;
using openstudio::FileReferenceType;

TEST(FileReference,Constructor) {
  FileReference fileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/dummyname.osm"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::OSM);

  fileReference = FileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/eplusout.sql"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::SQL);

  fileReference = FileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.epw"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::EPW);

  fileReference = FileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.idf"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::IDF);
}
