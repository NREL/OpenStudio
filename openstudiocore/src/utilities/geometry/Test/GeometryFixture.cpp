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

double totalArea(const std::vector<std::vector<Point3d> >& polygons)
{
  double result = 0.0;
  for (auto polygon : polygons){
    boost::optional<double> a = getArea(polygon);
    if (a){
      result += a.get();
    }
  }
  return result;
}

bool checkNormals(const openstudio::Vector3d& normal, const std::vector<std::vector<Point3d> >& polygons)
{
  for (auto polygon : polygons){
    boost::optional<Vector3d> a = getOutwardNormal(polygon);
    if (a){
      if (!vectorEqual(normal, *a)){
        return false;
      }
    }else{
      return false;
    }
  }
  return true;
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
