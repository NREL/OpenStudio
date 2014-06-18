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

#include "UnitsFixture.hpp"

#include "../ScaleFactory.hpp"
#include "../UnitFactory.hpp"
#include "../SIUnit.hpp"

#include "../../data/Vector.hpp"

#include "../../core/Containers.hpp"

#include <cmath>

using openstudio::ScaleFactory;
using openstudio::UnitFactory;
using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::path;
using openstudio::toPath;

// initialize for each test
void UnitsFixture::SetUp() {
}

// tear down after for each test
void UnitsFixture::TearDown() {
}

// initialize static members
void UnitsFixture::SetUpTestCase() 
{
  logFile = FileLogSink(toPath("./UnitsFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();
  
  tol = 1.0E-8;

  openstudio::DoubleVector vals = openstudio::toStandardVector(openstudio::randVector(0.0,1000.0,8760u));
  openstudio::Unit u = openstudio::createSIPower();
  for (double val : vals) {
    testQuantityVector.push_back(openstudio::Quantity(val,u));
  }
  testOSQuantityVector = openstudio::OSQuantityVector(u,vals);
}

// tear down static members
void UnitsFixture::TearDownTestCase() {
  logFile->disable();
}

boost::optional<openstudio::FileLogSink> UnitsFixture::logFile;
double UnitsFixture::tol;
openstudio::QuantityVector UnitsFixture::testQuantityVector;
openstudio::OSQuantityVector UnitsFixture::testOSQuantityVector;
