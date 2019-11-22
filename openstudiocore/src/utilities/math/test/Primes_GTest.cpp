/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../Primes.hpp"

#include <iostream>

using namespace openstudio;

TEST(Primes, isPrime)
{
  EXPECT_FALSE(isPrime<int>(0));
  EXPECT_FALSE(isPrime<int>(1));
  EXPECT_TRUE(isPrime<int>(2));
  EXPECT_TRUE(isPrime<int>(3));
  EXPECT_FALSE(isPrime<int>(4));
  EXPECT_TRUE(isPrime<int>(5));
  EXPECT_FALSE(isPrime<int>(6));
  EXPECT_TRUE(isPrime<int>(7));
  EXPECT_FALSE(isPrime<int>(8));
  EXPECT_FALSE(isPrime<int>(9));
  EXPECT_TRUE(isPrime<int>(11));
  EXPECT_TRUE(isPrime<int>(13));
  EXPECT_FALSE(isPrime<int>(15));
  EXPECT_TRUE(isPrime<int>(17));
  EXPECT_TRUE(isPrime<int>(19));
  EXPECT_FALSE(isPrime<int>(21));
  EXPECT_TRUE(isPrime<int>(23));
  EXPECT_FALSE(isPrime<int>(25));
}
