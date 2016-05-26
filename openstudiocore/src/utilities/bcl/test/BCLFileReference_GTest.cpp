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

#include <gtest/gtest.h>
#include "BCLFixture.hpp"
#include <resources.hxx>

#include "../BCLFileReference.hpp"

#include <QTextStream>

using namespace openstudio;

TEST_F(BCLFixture, BCLFileReference)
{
  openstudio::path path = toPath("./BCLFileReference.txt");
  if (exists(path)){
    remove(path);
  }
  ASSERT_FALSE(exists(path));

  BCLFileReference ref(path, true);
  EXPECT_FALSE(ref.checkForUpdate());

  EXPECT_EQ("BCLFileReference.txt", ref.fileName());
  EXPECT_EQ("txt", ref.fileType());
  
  openstudio::filesystem::ofstream file(path);
  ASSERT_TRUE(file.is_open());
  file << "Hi";
  file.close();

  EXPECT_TRUE(ref.checkForUpdate());

  EXPECT_FALSE(ref.checkForUpdate());

  file.open(path);
  ASSERT_TRUE(file.is_open());
  file << "Bye";
  file.close();

  EXPECT_TRUE(ref.checkForUpdate());

  EXPECT_FALSE(ref.checkForUpdate());

  ASSERT_TRUE(exists(path));
  remove(path);
  ASSERT_FALSE(exists(path));

  EXPECT_TRUE(ref.checkForUpdate());

  EXPECT_FALSE(ref.checkForUpdate());
}
