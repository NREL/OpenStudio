/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
