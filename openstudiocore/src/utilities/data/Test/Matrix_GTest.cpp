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
#include "DataFixture.hpp"

#include "../Matrix.hpp"
#include "../Vector.hpp"

#include <exception>
#include <iostream>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(DataFixture,Matrix_Operators)
{
  Matrix A(2,3);
  Matrix B(2,2);

  for (unsigned i = 0; i < A.size1(); ++i) {
    for (unsigned j = 0; j < A.size2(); ++j) {
      A(i,j) = i + j;
    }
  }

  for (unsigned i = 0; i < B.size1(); ++i) {
    for (unsigned j = 0; j < B.size2(); ++j) {
      B(i,j) = i*j;
    }
  }

  Matrix C(A);

  ASSERT_TRUE(A == C);
  ASSERT_FALSE(A == B);
  ASSERT_TRUE(A != B);

}

TEST_F(DataFixture,Matrix_Interp1)
{
  Vector x(2);
  Vector y(2);
  Matrix v(2,2);

  x(0) = 0.0;
  x(1) = 1.0;
  y(0) = 0.0;
  y(1) = 1.0;
  v(0,0) = 0.0;
  v(1,0) = 1.0;
  v(0,1) = -1.0;
  v(1,1) = 2.0;

  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, 0.0, 0.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.0, interp(x, y, v, 1.0, 0.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(-1.0, interp(x, y, v, 0.0, 1.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, v, 1.0, 1.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.5, interp(x, y, v, 0.5, 0.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(-0.5, interp(x, y, v, 0.0, 0.5, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.5, interp(x, y, v, 0.5, 1.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.5, interp(x, y, v, 1.0, 0.5, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.5, interp(x, y, v, 0.5, 0.5, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, 1.5, 0.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, -1.5, 0.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, 0.0, 1.5, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, 0.0, -1.5, LinearInterp, NoneExtrap));
}

TEST_F(DataFixture,Matrix_Interp2)
{
  Matrix v(10,10);
  for (unsigned i = 0; i < v.size1(); ++i){
    for (unsigned j = 0; j < v.size2(); ++j){
      v(i,j) = 10*i+j;
    }
  }

  Vector x(linspace(-100, 0, 10));
  Vector y(linspace(0, 100, 10));

  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, -100.0, 0.0, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, -105.5, 0.0, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, v, -105.5, -5.5, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(9.0, interp(x, y, v, -100.0, 100.0, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(9.0, interp(x, y, v, -100.0, 105.5, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(90.0, interp(x, y, v, 0.0, 0.0, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(90.0, interp(x, y, v, 5.5, 0.0, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(99.0, interp(x, y, v, -0.1, 99.9, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(99.0, interp(x, y, v, 0.0, 100.0, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(99.0, interp(x, y, v, 0.0, 105.5, NearestInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(99.0, interp(x, y, v, 5.5, 105.5, NearestInterp, NearestExtrap));
}

TEST_F(DataFixture,Matrix_Prod)
{
  Matrix A(3,2);
  Matrix B(2,2);
  Vector x(2,1);

  A(0,0) = 1.0; A(0,1) = 1.0;
  A(1,0) = -1.0; A(1,1) = 0.0;
  A(2,0) = 0.0; A(2,1) = 2.0;

  B(0,0) = 0.0; B(0,1) = 1.0;
  B(1,0) = -2.0; B(1,1) = 1.0;

  x(0) = 1.0;
  x(1) = 0.5;
  
  Matrix C = prod(A,B);
  Vector y = prod(B,x);

  ASSERT_EQ(A.size1(),C.size1());
  ASSERT_EQ(B.size2(),C.size2());
  EXPECT_DOUBLE_EQ(-2.0,C(0,0)); EXPECT_DOUBLE_EQ(2.0,C(0,1));
  EXPECT_DOUBLE_EQ(0.0,C(1,0)); EXPECT_DOUBLE_EQ(-1.0,C(1,1));
  EXPECT_DOUBLE_EQ(-4.0,C(2,0)); EXPECT_DOUBLE_EQ(2.0,C(2,1));

  ASSERT_EQ(B.size1(),y.size());
  EXPECT_DOUBLE_EQ(0.5,y(0));
  EXPECT_DOUBLE_EQ(-1.5,y(1));

}

TEST_F(DataFixture,Matrix_OuterProd)
{
  Vector x(2);
  Vector y(3);

  x(0) = 1.0; x(1) = 2.0;
  y(0) = 0.0; y(1) = 1.0; y(2) = 2.0;

  Matrix A = outerProd(x,y);

  EXPECT_DOUBLE_EQ(0.0,A(0,0));
  EXPECT_DOUBLE_EQ(1.0,A(0,1)); 
  EXPECT_DOUBLE_EQ(2.0,A(0,2));
  EXPECT_DOUBLE_EQ(0.0,A(1,0)); 
  EXPECT_DOUBLE_EQ(2.0,A(1,1)); 
  EXPECT_DOUBLE_EQ(4.0,A(1,2));

}

TEST_F(DataFixture,Matrix_Log)
{
  Matrix m(3,3);
  Matrix n(3,3);
  for (unsigned i = 0; i < m.size1(); ++i){
    for (unsigned j = 0; j < m.size2(); ++j){
      m(i,j) = std::pow(10.0, (double)(i+j));
      n(i,j) = std::exp((double)(i+j));
    }
  }

  Matrix logM = openstudio::log(m, 10.0);
  Matrix logN = openstudio::log(n);

  ASSERT_EQ(m.size1(), logM.size1());
  ASSERT_EQ(m.size2(), logM.size2());
  ASSERT_EQ(n.size1(), logN.size1());
  ASSERT_EQ(n.size2(), logN.size2());
  for (unsigned i = 0; i < m.size1(); ++i){
    for (unsigned j = 0; j < m.size2(); ++j){
      EXPECT_DOUBLE_EQ((double)(i+j), logM(i,j));
      EXPECT_DOUBLE_EQ((double)(i+j), logN(i,j));
    }
  }
}

TEST_F(DataFixture,Matrix_RandMatrix)
{
  Matrix m = randMatrix(1.0,1.0,10,10);
  EXPECT_EQ(1.0, maximum(m));
  EXPECT_EQ(1.0, minimum(m));

  m = randMatrix(-1.0,1.0,10,10);
  EXPECT_TRUE(1.0 >= maximum(m));
  EXPECT_TRUE(-1.0 <= minimum(m));
}

TEST_F(DataFixture,Matrix_ReturnsDoubleFunctions)
{
  Matrix A(2,2);
  Matrix B(3,2);

  A(0,0) = 1.0; A(0,1) = 2.0;
  A(1,0) = 3.0; A(1,1) = 4.0;

  B(0,0) = 3.0; B(0,1) = -1.0;
  B(1,0) = 1.0; B(1,1) = 0.0;
  B(2,0) = 0.0; B(2,1) = 2.0;

  EXPECT_DOUBLE_EQ(10.0,sum(A));
  EXPECT_DOUBLE_EQ(4.0,maximum(A));
  EXPECT_DOUBLE_EQ(1.0,minimum(A));
  EXPECT_DOUBLE_EQ(10.0/4.0,mean(A));

  EXPECT_DOUBLE_EQ(5.0,sum(B));
  EXPECT_DOUBLE_EQ(3.0,maximum(B));
  EXPECT_DOUBLE_EQ(-1.0,minimum(B));
  EXPECT_DOUBLE_EQ(5.0/6.0,mean(B));
}


TEST_F(DataFixture, Matrix_FindConnectedComponents)
{
  std::vector<std::vector<unsigned> > result;

  // non-symetric
  {
    Matrix A(2,3,0.0);
    result = findConnectedComponents(A);
    EXPECT_TRUE(result.empty());
  }
    
  // empty
  {
    Matrix A(0,0,0.0);
    result = findConnectedComponents(A);
    EXPECT_TRUE(result.empty());
  }

  // single
  {
    Matrix A(1,1,0.0);
    A(0,0) = 1.0;
    result = findConnectedComponents(A);
    ASSERT_EQ(1u, result.size());
    ASSERT_EQ(1u, result[0].size());
    EXPECT_EQ(0u, result[0][0]);
  }

  // 2x2 identity
  {
    Matrix A(2,2,0.0);
    A(0,0) = 1.0;
    A(1,1) = 1.0;
    result = findConnectedComponents(A);
    ASSERT_EQ(2u, result.size());
    ASSERT_EQ(1u, result[0].size());
    EXPECT_EQ(0u, result[0][0]);
    ASSERT_EQ(1u, result[1].size());
    EXPECT_EQ(1u, result[1][0]);
  }

  // 3x3 identity
  {
    Matrix A(3,3,0.0);
    A(0,0) = 1.0;
    A(1,1) = 1.0;
    A(2,2) = 1.0;
    result = findConnectedComponents(A);
    ASSERT_EQ(3u, result.size());
    ASSERT_EQ(1u, result[0].size());
    EXPECT_EQ(0u, result[0][0]);
    ASSERT_EQ(1u, result[1].size());
    EXPECT_EQ(1u, result[1][0]);
    ASSERT_EQ(1u, result[2].size());
    EXPECT_EQ(2u, result[2][0]);
  }

  // 3x3 1-2 symetric, 3
  {
    Matrix A(3,3,0.0);
    A(0,0) = 1.0;
    A(0,1) = 1.0;
    A(1,0) = 1.0;
    A(1,1) = 1.0;
    A(2,2) = 1.0;
    result = findConnectedComponents(A);
    ASSERT_EQ(2u, result.size());
    ASSERT_EQ(2u, result[0].size());
    EXPECT_EQ(0u, result[0][0]);
    EXPECT_EQ(1u, result[0][1]);
    ASSERT_EQ(1u, result[1].size());
    EXPECT_EQ(2u, result[1][0]);
  }

  // 3x3 1-2 asymetric, 3
  {
    Matrix A(3,3,0.0);
    A(0,0) = 1.0;
    A(0,1) = 1.0;
    A(1,1) = 1.0;
    A(2,2) = 1.0;
    result = findConnectedComponents(A);
    ASSERT_EQ(2u, result.size());
    ASSERT_EQ(2u, result[0].size());
    EXPECT_EQ(0u, result[0][0]);
    EXPECT_EQ(1u, result[0][1]);
    ASSERT_EQ(1u, result[1].size());
    EXPECT_EQ(2u, result[1][0]);
  }

  // 3x3 1-2 asymetric, 3
  {
    Matrix A(3,3,0.0);
    A(0,0) = 1.0;
    A(1,0) = 1.0;
    A(1,1) = 1.0;
    A(2,2) = 1.0;
    result = findConnectedComponents(A);
    ASSERT_EQ(2u, result.size());
    ASSERT_EQ(2u, result[0].size());
    EXPECT_EQ(0u, result[0][0]);
    EXPECT_EQ(1u, result[0][1]);
    ASSERT_EQ(1u, result[1].size());
    EXPECT_EQ(2u, result[1][0]);
  }
}
