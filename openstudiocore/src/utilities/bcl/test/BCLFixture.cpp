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

#include "BCLFixture.hpp"

#include "../LocalBCL.hpp"

using openstudio::LocalBCL;
using openstudio::Logger;
using openstudio::toPath;
using openstudio::FileLogSink;

void BCLFixture::SetUp() {

  if (LocalBCL::instance().prodAuthKey().empty()){
    prodAuthKey = defaultProdAuthKey;
    LocalBCL::instance().setProdAuthKey(prodAuthKey);
  }else{
    prodAuthKey = LocalBCL::instance().prodAuthKey();
  }

  if (LocalBCL::instance().devAuthKey().empty()){
    devAuthKey = defaultDevAuthKey;
    LocalBCL::instance().setDevAuthKey(devAuthKey);
  }else{
    devAuthKey = LocalBCL::instance().devAuthKey();
  }
}

void BCLFixture::TearDown() {}

void BCLFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./BCLFixture.log"));
  logFile->setLogLevel(Info);
}

void BCLFixture::TearDownTestCase() {
  logFile->disable();
}

std::string BCLFixture::prodAuthKey;
std::string BCLFixture::devAuthKey;

// these are Dan's API keys labelled under "Testing", delete when there is a better way to do this
std::string BCLFixture::defaultProdAuthKey("2da842aa2d457703d8fdcb5c53080ace");
std::string BCLFixture::defaultDevAuthKey("e8051bca77787c0df16cbe13452e7580");
boost::optional<openstudio::FileLogSink> BCLFixture::logFile;
