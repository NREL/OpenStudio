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

#include <utilities/document/DocumentRegex.hpp>

#include <utilities/core/Path.hpp>

#include <resources.hxx>

#include <iomanip>

using openstudio::toPath;

TEST(DocumentRegex, UnitStringFormatting)
{
  std::string unitString("W/m^2*K");
  std::string formattedString = openstudio::formatUnitString(unitString,openstudio::DocumentFormat::XHTML);
  EXPECT_EQ("W/m<sup>2</sup>&middot;K",formattedString);

  formattedString = openstudio::formatUnitString(unitString,openstudio::DocumentFormat::LaTeX);
  EXPECT_EQ("W/m\\ensuremath{^2}\\ensuremath{\\cdot}K",formattedString);

  formattedString = openstudio::formatUnitString(unitString,openstudio::DocumentFormat::COUT);
  EXPECT_EQ("W/m^2*K",formattedString);
}


