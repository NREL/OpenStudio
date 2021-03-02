/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../SiteGroundReflectance.hpp"
#include "../SiteGroundReflectance_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SiteGroundReflectance_GettersSetters) {
  Model m;

  SiteGroundReflectance siteGroundReflectance = m.getUniqueModelObject<SiteGroundReflectance>();

  // January Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isJanuaryGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.januaryGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setJanuaryGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isJanuaryGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.januaryGroundReflectance());
  // Test reset
  siteGroundReflectance.resetJanuaryGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.januaryGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isJanuaryGroundReflectanceDefaulted());

  // February Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isFebruaryGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.februaryGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setFebruaryGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isFebruaryGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.februaryGroundReflectance());
  // Test reset
  siteGroundReflectance.resetFebruaryGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.februaryGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isFebruaryGroundReflectanceDefaulted());

  // March Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isMarchGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.marchGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setMarchGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isMarchGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.marchGroundReflectance());
  // Test reset
  siteGroundReflectance.resetMarchGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.marchGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isMarchGroundReflectanceDefaulted());

  // April Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isAprilGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.aprilGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setAprilGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isAprilGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.aprilGroundReflectance());
  // Test reset
  siteGroundReflectance.resetAprilGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.aprilGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isAprilGroundReflectanceDefaulted());

  // May Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isMayGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.mayGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setMayGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isMayGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.mayGroundReflectance());
  // Test reset
  siteGroundReflectance.resetMayGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.mayGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isMayGroundReflectanceDefaulted());

  // June Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isJuneGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.juneGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setJuneGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isJuneGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.juneGroundReflectance());
  // Test reset
  siteGroundReflectance.resetJuneGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.juneGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isJuneGroundReflectanceDefaulted());

  // July Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isJulyGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.julyGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setJulyGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isJulyGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.julyGroundReflectance());
  // Test reset
  siteGroundReflectance.resetJulyGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.julyGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isJulyGroundReflectanceDefaulted());

  // August Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isAugustGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.augustGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setAugustGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isAugustGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.augustGroundReflectance());
  // Test reset
  siteGroundReflectance.resetAugustGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.augustGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isAugustGroundReflectanceDefaulted());

  // September Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isSeptemberGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.septemberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setSeptemberGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isSeptemberGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.septemberGroundReflectance());
  // Test reset
  siteGroundReflectance.resetSeptemberGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.septemberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isSeptemberGroundReflectanceDefaulted());

  // October Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isOctoberGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.octoberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setOctoberGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isOctoberGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.octoberGroundReflectance());
  // Test reset
  siteGroundReflectance.resetOctoberGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.octoberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isOctoberGroundReflectanceDefaulted());

  // November Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isNovemberGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.novemberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setNovemberGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isNovemberGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.novemberGroundReflectance());
  // Test reset
  siteGroundReflectance.resetNovemberGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.novemberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isNovemberGroundReflectanceDefaulted());

  // December Ground Reflectance:  Double
  // Check Idd default: 0.2
  EXPECT_TRUE(siteGroundReflectance.isDecemberGroundReflectanceDefaulted());
  EXPECT_EQ(0.2, siteGroundReflectance.decemberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.setDecemberGroundReflectance(0.1));
  EXPECT_FALSE(siteGroundReflectance.isDecemberGroundReflectanceDefaulted());
  EXPECT_EQ(0.1, siteGroundReflectance.decemberGroundReflectance());
  // Test reset
  siteGroundReflectance.resetDecemberGroundReflectance();
  EXPECT_EQ(0.2, siteGroundReflectance.decemberGroundReflectance());
  EXPECT_TRUE(siteGroundReflectance.isDecemberGroundReflectanceDefaulted());
}
