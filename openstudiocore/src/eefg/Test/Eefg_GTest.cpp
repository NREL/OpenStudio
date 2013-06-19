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

#include <gtest/gtest.h>

#include <eefg/Eefg.hpp>

#include <utilities/core/String.hpp>
#include <utilities/core/Path.hpp>

#include <resources.hxx>

using openstudio::toString;
using openstudio::toPath;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN TESTS ***
///////////////////////////////////////////////////////////////////////////////

TEST(EEFG, Test)
{
  openstudio::path iddPath = resourcesPath()/toPath("energyplus/Energy+.idd");

  openstudio::path idfPath = resourcesPath()/toPath("energyplus/5ZoneAirCooled/in.idf");
  openstudio::path outPath = resourcesPath()/toPath("energyplus/5ZoneAirCooled/eefg.idf");
  eefgValidateIdf(toString(iddPath).c_str(), toString(idfPath).c_str(), toString(outPath).c_str());

  idfPath = resourcesPath()/toPath("energyplus/Daylighting_School/in.idf");
  outPath = resourcesPath()/toPath("energyplus/Daylighting_School/eefg.idf");
  eefgValidateIdf(toString(iddPath).c_str(), toString(idfPath).c_str(), toString(outPath).c_str());
}

TEST(EEFG, DummyTest)
{
  openstudio::path iddPath = resourcesPath()/toPath("eefg/dummy.idd");

  openstudio::path idfPath = resourcesPath()/toPath("eefg/dummy.idf");
  openstudio::path outPath = resourcesPath()/toPath("eefg/eefg.idf");
  eefgValidateIdf(toString(iddPath).c_str(), toString(idfPath).c_str(), toString(outPath).c_str());
}

