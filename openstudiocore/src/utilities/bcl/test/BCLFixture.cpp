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
