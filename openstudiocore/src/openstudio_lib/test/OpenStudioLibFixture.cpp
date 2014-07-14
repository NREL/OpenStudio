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

#include "OpenStudioLibFixture.hpp"

#include "../../utilities/core/Application.hpp"
#include "../../utilities/core/Path.hpp"

void OpenStudioLibFixture::SetUp() {
  openstudio::Application::instance().application();
}

void OpenStudioLibFixture::TearDown() {}

void OpenStudioLibFixture::SetUpTestCase() {
  // set up logging
  logFile = openstudio::FileLogSink(openstudio::toPath("./OpenStudioLibFixture.log"));
  logFile->setLogLevel(Debug);
  openstudio::Logger::instance().standardOutLogger().disable();
}

void OpenStudioLibFixture::TearDownTestCase(){}

// static variables
boost::optional<openstudio::FileLogSink> OpenStudioLibFixture::logFile;
