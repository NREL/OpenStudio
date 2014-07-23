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

#include "IddFixture.hpp"

using openstudio::FileLogSink;
using openstudio::toPath;

void IddFixture::SetUpTestCase()
{
  // set up logging
  logFile = FileLogSink(toPath("./IddFixture.log"));
  logFile->setLogLevel(Debug);

  // load from factory and time it
  openstudio::Time start = openstudio::Time::currentTime();
  epIddFile = openstudio::IddFactory::instance().getIddFile(openstudio::IddFileType::EnergyPlus);
  iddLoadTime = openstudio::Time::currentTime() - start;

  LOG(Info, "EnergyPlus IddFile load time (from IddFactory) = " << iddLoadTime);

  start = openstudio::Time::currentTime();
  osIddFile = openstudio::IddFactory::instance().getIddFile(openstudio::IddFileType::OpenStudio);
  iddLoadTime = openstudio::Time::currentTime() - start;

  LOG(Info, "OpenStudio IddFile load time (from IddFactory) = " << iddLoadTime);
}

void IddFixture::TearDownTestCase() {
  logFile->disable();
}

openstudio::IddFile IddFixture::epIddFile;
openstudio::IddFile IddFixture::osIddFile;
openstudio::Time IddFixture::iddLoadTime;
boost::optional<openstudio::FileLogSink> IddFixture::logFile;
