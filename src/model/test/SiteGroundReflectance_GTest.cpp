/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SiteGroundReflectance.hpp"
#include "../SiteGroundReflectance_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SiteGroundReflectance_GettersSetters) {
  Model m;

  auto siteGroundReflectance = m.getUniqueModelObject<SiteGroundReflectance>();

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
