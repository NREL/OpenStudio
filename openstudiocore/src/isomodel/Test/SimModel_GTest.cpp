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
#include <isomodel/Test/ISOModelFixture.hpp>
#include <isomodel/SimModel.hpp>
#include <resources.hxx>
#include <sstream>

using namespace openstudio::isomodel;
using namespace openstudio;

void testGenericFunctions() {
  double scalar = 2, dResults=0;
  double test[] = {0,1,2,3,4,5,6,7,8,9,10,11};
  Vector vTest = Vector(12), vTest2 = Vector(12);
  for(unsigned int i = 0;i<vTest.size();i++){
    vTest[i] = i;
    vTest2[i] = 11-i;
  }
  for(unsigned int i = 0;i<12;i++){
    EXPECT_EQ(i, test[i]);
    EXPECT_EQ(i, vTest[i]);
    EXPECT_EQ(11 - i, vTest2[i]);
  }


  Vector results = mult(test,scalar,12);
  for(unsigned int i = 0;i<12;i++){
    EXPECT_EQ(i*scalar, results[i]);
  }

  results = mult(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i * scalar, results[i]);
  }

  results = mult(vTest, test);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i * i, results[i]);
  }

  results = mult(vTest, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i * i, results[i]);
  }

  results = div(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i / scalar, results[i]);
  }

  results = div(scalar, vTest);
  EXPECT_EQ(DBL_MAX, results[0]);
  for(unsigned int i = 1;i<vTest.size();i++){
    EXPECT_EQ(scalar / i, results[i]);
  }

  results = div(vTest, vTest);
  EXPECT_EQ(DBL_MAX, results[0]);  //0 / 0
  for(unsigned int i = 1;i<vTest.size();i++){
    EXPECT_EQ(1, results[i]);
  }

  results = sum(vTest, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i + i, results[i]);
  }

  results = sum(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i + scalar, results[i]);
  }

  results = dif(vTest, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(0, results[i]);
  }

  results = dif(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i - scalar, results[i]);
  }

  results = dif(scalar, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(scalar - i, results[i]);
  }

  EXPECT_EQ(11, max(vTest));

  results = max(vTest,vTest2);
  EXPECT_EQ(11, results[0]);
  EXPECT_EQ(10, results[1]);
  EXPECT_EQ(9, results[2]);
  EXPECT_EQ(8, results[3]);
  EXPECT_EQ(7, results[4]);
  EXPECT_EQ(6, results[5]);
  EXPECT_EQ(6, results[6]);
  EXPECT_EQ(7, results[7]);
  EXPECT_EQ(8, results[8]);
  EXPECT_EQ(9, results[9]);
  EXPECT_EQ(10, results[10]);
  EXPECT_EQ(11, results[11]);

  for(unsigned int i = 0;i<12;i++){
    EXPECT_EQ(11-i, vTest2[i]);
  }

  results = max(vTest2,10);
  EXPECT_EQ(11, results[0]);  
  for(unsigned int i = 1;i<vTest2.size();i++){
    EXPECT_EQ(10, results[i]);
  }
  
  EXPECT_EQ(0, min(vTest));

  results = min(vTest,1);
  EXPECT_EQ(0, results[0]);  
  for(unsigned int i = 1;i<vTest.size();i++){
    EXPECT_EQ(1, results[i]);
  }
  
  results = pow(vTest, 3);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i*i*i, results[i]);
  }
  for(unsigned int i = 0;i<vTest.size();i++){
    if(i%2==0){
      vTest[i] = -1 * vTest[i];
    }
  }
  results = abs(vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i, results[i]);
  }
  
}
TEST_F(ISOModelFixture, SimModel)
{
  SimModel simModel;  
  testGenericFunctions();


}
