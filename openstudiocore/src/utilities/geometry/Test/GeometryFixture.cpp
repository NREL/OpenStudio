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

#include "GeometryFixture.hpp"

#include "../Point3d.hpp"
#include "../Vector3d.hpp"

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;
using openstudio::Point3d;
using openstudio::Vector3d;

bool pointEqual(const openstudio::Point3d& a, const openstudio::Point3d& b)
{
  Vector3d diff = a-b;
  return diff.length() <= 0.0001;
}

bool pointsEqual(const std::vector<openstudio::Point3d>& a, const std::vector<openstudio::Point3d>& b)
{
  if (a.size() != b.size()){
    return false;
  }

  for (unsigned i = 0; i < a.size(); ++i){
    if (!pointEqual(a[i], b[i])){
      return false;
    }
  }

  return true;
}

bool vectorEqual(const openstudio::Vector3d& a, const openstudio::Vector3d& b)
{
  Vector3d diff = a-b;
  return diff.length() <= 0.0001;
}

// initialize for each test
void GeometryFixture::SetUp() {
}

// tear down after for each test
void GeometryFixture::TearDown() {
}

// initialize static members
void GeometryFixture::SetUpTestCase() 
{
  logFile = FileLogSink(toPath("./GeometryFixture.log"));
  logFile->setLogLevel(Debug);
}

// tear down static members
void GeometryFixture::TearDownTestCase() 
{
  logFile->disable();
}

boost::optional<openstudio::FileLogSink> GeometryFixture::logFile;
