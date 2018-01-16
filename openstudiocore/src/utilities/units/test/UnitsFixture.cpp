/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
