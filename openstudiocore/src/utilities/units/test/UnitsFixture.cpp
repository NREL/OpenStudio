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
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <utilities/units/test/UnitsFixture.hpp>

#include <utilities/units/ScaleFactory.hpp>
#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/SIUnit.hpp>

#include <utilities/data/Vector.hpp>

#include <utilities/core/Containers.hpp>

#include <cmath>

#include <boost/foreach.hpp>

using openstudio::ScaleFactory;
using openstudio::UnitFactory;
using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::path;
using openstudio::toPath;

// initiallize for each test
void UnitsFixture::SetUp() {
}

// tear down after for each test
void UnitsFixture::TearDown() {
}

// initiallize static members
void UnitsFixture::SetUpTestCase() 
{
  logFile = FileLogSink(toPath("./UnitsFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();
  
  tol = 1.0E-8;

  openstudio::DoubleVector vals = openstudio::toStandardVector(openstudio::randVector(0.0,1000.0,8760u));
  openstudio::Unit u = openstudio::createSIPower();
  BOOST_FOREACH(double val,vals) {
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
