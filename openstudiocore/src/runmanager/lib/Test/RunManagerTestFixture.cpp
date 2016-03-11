/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "RunManagerTestFixture.hpp"

using openstudio::FileLogSink;
using openstudio::toPath;

void RunManagerTestFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./RunManagerTestFixture.log"));
  logFile->setLogLevel(Trace);
  openstudio::Logger::instance().standardOutLogger().disable();
}

void RunManagerTestFixture::TearDownTestCase() {
  logFile->disable();
}

boost::optional<openstudio::FileLogSink> RunManagerTestFixture::logFile;
