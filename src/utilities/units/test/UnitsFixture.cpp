/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
void UnitsFixture::SetUp() {}

// tear down after for each test
void UnitsFixture::TearDown() {}

// initialize static members
void UnitsFixture::SetUpTestSuite() {
  logFile = FileLogSink(toPath("./UnitsFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();

  tol = 1.0E-8;

  openstudio::DoubleVector vals = openstudio::toStandardVector(openstudio::randVector(0.0, 1000.0, 8760u));
  openstudio::Unit u = openstudio::createSIPower();
  for (double val : vals) {
    testQuantityVector.push_back(openstudio::Quantity(val, u));
  }
  testOSQuantityVector = openstudio::OSQuantityVector(u, vals);
}

// tear down static members
void UnitsFixture::TearDownTestSuite() {
  logFile->disable();
}

boost::optional<openstudio::FileLogSink> UnitsFixture::logFile;
double UnitsFixture::tol;
openstudio::QuantityVector UnitsFixture::testQuantityVector;
openstudio::OSQuantityVector UnitsFixture::testOSQuantityVector;
