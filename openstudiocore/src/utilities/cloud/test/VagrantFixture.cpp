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

#include <utilities/cloud/test/VagrantFixture.hpp>

using namespace openstudio;

void VagrantFixture::SetUp() {}

void VagrantFixture::TearDown() {}

void VagrantFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./VagrantFixture.log"));
  logFile->setLogLevel(Debug);

  // start VirtualBox
  virtualBox = new QProcess();
  virtualBox->start("VirtualBox.exe",QStringList());
  virtualBox->waitForStarted();
}

void VagrantFixture::TearDownTestCase() {
  // stop logging
  logFile->disable();

  // stop VirtualBox
  virtualBox->close();
  virtualBox->waitForFinished();
  virtualBox->deleteLater();
}

boost::optional<openstudio::FileLogSink> VagrantFixture::logFile;
QProcess* VagrantFixture::virtualBox;