/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PerformancePrecisionTradeoffs.hpp"
#include "../../model/PerformancePrecisionTradeoffs_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/PerformancePrecisionTradeoffs_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PerformancePrecisionTradeoffs) {

  ForwardTranslator ft;

  Model model;

  // Get the unique object
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();
  EXPECT_TRUE(performancePrecisionTradeoffs.isUseCoilDirectSolutionsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isOverrideModeDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxZoneTempDiffDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isUseRepresentativeSurfacesforCalculationsDefaulted());

  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::PerformancePrecisionTradeoffs);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idf_perf(idfObjs[0]);

  EXPECT_FALSE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions));
  EXPECT_EQ("No", idf_perf.getString(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions).get());
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm));
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::OverrideMode));
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::MaxZoneTempDiff));
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp));
  EXPECT_FALSE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations));
}
