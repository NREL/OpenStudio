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
#include "AnalysisFixture.hpp"

#include "../Algorithm.hpp"
#include "../AlgorithmOptions.hpp"
#include "../DesignOfExperiments.hpp"
#include "../DesignOfExperimentsOptions.hpp"
#include "../SamplingAlgorithm.hpp"
#include "../SamplingAlgorithmOptions.hpp"

#include "../../runmanager/lib/Job.hpp"

#include "../../utilities/core/FileReference.hpp"

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture,MaxIterSimsIsComplete_Connection_OSAlg) {
  // maxIter clear
  DesignOfExperimentsOptions options(DesignOfExperimentsType::FullFactorial);
  options.setMaxIter(0);
  DesignOfExperiments algorithm(createUUID(),
                                createUUID(),
                                std::string(),
                                std::string(),
                                true,
                                false,
                                -1,
                                options);
  EXPECT_TRUE(algorithm.isComplete());
  algorithm.options().clearMaxIter();
  EXPECT_FALSE(algorithm.isComplete());
  EXPECT_EQ(-1,algorithm.iter());

  // maxSims bump up
  options = DesignOfExperimentsOptions(DesignOfExperimentsType::FullFactorial);
  options.setMaxSims(10);
  algorithm = DesignOfExperiments(createUUID(),
                                  createUUID(),
                                  std::string(),
                                  std::string(),
                                  true,
                                  false,
                                  0,
                                  options);
  EXPECT_TRUE(algorithm.isComplete());
  // shares data
  options.setMaxSims(20);
  EXPECT_EQ(20,algorithm.options().maxSims());
  EXPECT_FALSE(algorithm.isComplete());
  EXPECT_EQ(0,algorithm.iter());
}

TEST_F(AnalysisFixture,MaxIterSimsIsComplete_Connection_DakotaAlg) {
  // maxIter bump up
  SamplingAlgorithmOptions options;
  options.setMaxIter(0);
  SamplingAlgorithm algorithm(createUUID(),
                              createUUID(),
                              std::string(),
                              std::string(),
                              true,
                              false,
                              -1,
                              options,
                              boost::none,
                              boost::none,
                              boost::none); //DLM: Elaine is this ok?
  EXPECT_TRUE(algorithm.isComplete());
  EXPECT_FALSE(algorithm.restartFileReference());
  algorithm.options().setMaxIter(3);
  EXPECT_FALSE(algorithm.isComplete());
  EXPECT_EQ(-1,algorithm.iter());
  EXPECT_FALSE(algorithm.job());

  // maxSims clear
  options = SamplingAlgorithmOptions();
  options.setMaxSims(100);
  algorithm = SamplingAlgorithm(createUUID(),
                                createUUID(),
                                std::string(),
                                std::string(),
                                true,
                                false,
                                0,
                                options,
                                FileReference(toPath("dakota.rst")),
                                FileReference(toPath("dakota.out")),
                                boost::none); //DLM: Elaine is this ok?
  EXPECT_TRUE(algorithm.isComplete());
  EXPECT_TRUE(algorithm.outFileReference());
  options.clearMaxSims();
  EXPECT_FALSE(algorithm.options().maxSims());
  EXPECT_FALSE(algorithm.isComplete()); // DLM: Elaine, shouldn't isComplete be tied to the job status?
  EXPECT_FALSE(algorithm.failed());
  EXPECT_EQ(0,algorithm.iter());
  EXPECT_TRUE(algorithm.restartFileReference());
  EXPECT_FALSE(algorithm.job()); 
}
