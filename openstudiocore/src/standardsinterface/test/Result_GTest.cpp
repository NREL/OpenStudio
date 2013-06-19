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
#include <standardsinterface/test/StandardsInterfaceFixture.hpp>

#include <standardsinterface/Result.hpp>

#include <utilities/core/Containers.hpp>

#include <string>

using openstudio::StringVector;
using openstudio::standardsinterface::Result;
using namespace openstudio::standardsinterface::result;

TEST_F(StandardsInterfaceFixture,Result_NoValue) {

  Result<none> r(Good);
  EXPECT_TRUE(r);

  r = Result<none>(DataDictionaryError);
  EXPECT_FALSE(r);
  EXPECT_TRUE(r.code() == DataDictionaryError);

  r = Result<none>(ModelError,"Model object is not the correct type.");
  EXPECT_FALSE(r);
  EXPECT_TRUE(r.code() == ModelError);
  EXPECT_EQ("Model object is not the correct type.",r.what());

}

TEST_F(StandardsInterfaceFixture,Result_Double) {

  Result<double> r(3.3);
  ASSERT_TRUE(r);
  double my_value = (*r); // best practice is to store good value in new variable.
  EXPECT_DOUBLE_EQ(3.3,my_value);
  my_value = my_value*(*r);
  EXPECT_DOUBLE_EQ(3.3*3.3,my_value);

  r = Result<double>(ModelError,"Reason for error.");
  EXPECT_FALSE(r);
  EXPECT_TRUE(r.code() == ModelError);
  ASSERT_THROW(*r,std::exception);

  r = Result<double>();
  EXPECT_FALSE(r);
  EXPECT_TRUE(r.code() == RequestUnmapped);

}

TEST_F(StandardsInterfaceFixture,Result_String) {
  Result<std::string> r("hello there.");
  ASSERT_TRUE(r);
  std::string my_str = *r;
  EXPECT_EQ("hello there.",my_str);

  r = Result<std::string>(ModelError,"Message passed from the model.");
  EXPECT_FALSE(r);
  EXPECT_TRUE(r.code() == ModelError);
  EXPECT_EQ("Message passed from the model.",r.what());
  ASSERT_THROW(*r,std::exception);

  r = Result<std::string>();
  EXPECT_FALSE(r);
  EXPECT_TRUE(r.code() == RequestUnmapped);
}

TEST_F(StandardsInterfaceFixture,Result_StringVector) {
  StringVector sv;
  sv.push_back("A");
  sv.push_back("B");
  sv.push_back("C");
  Result<StringVector> r(sv);
  ASSERT_TRUE(r);
  StringVector copy = *r;
  EXPECT_TRUE(sv == copy);

  r = Result<StringVector>();
  EXPECT_FALSE(r);
  EXPECT_TRUE(r.code() == RequestUnmapped);
}
