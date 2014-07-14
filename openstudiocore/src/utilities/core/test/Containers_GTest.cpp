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

#include <gtest/gtest.h>

#include "../Containers.hpp"

#include <resources.hxx>

using openstudio::StringVector;
using openstudio::eraseEmptyElements;

TEST(Containers,StringVector)
{
  StringVector sv;
  sv.push_back("");
  sv.push_back("Hello");
  sv.push_back("Guten Tag");
  sv.push_back("");
  sv.push_back("Bonjour");
  sv = eraseEmptyElements(sv);
  ASSERT_EQ(static_cast<unsigned>(3),sv.size());
  EXPECT_EQ("Hello",sv[0]);
  EXPECT_EQ("Guten Tag",sv[1]);
  EXPECT_EQ("Bonjour",sv[2]);
}
