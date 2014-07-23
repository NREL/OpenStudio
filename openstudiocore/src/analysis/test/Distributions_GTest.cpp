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

#include "../NormalDistribution.hpp"
#include "../LognormalDistribution.hpp"
#include "../UniformDistribution.hpp"
#include "../LoguniformDistribution.hpp"
#include "../TriangularDistribution.hpp"
#include "../ExponentialDistribution.hpp"
#include "../BetaDistribution.hpp"
#include "../GammaDistribution.hpp"
#include "../GumbelDistribution.hpp"
#include "../FrechetDistribution.hpp"
#include "../WeibullDistribution.hpp"
#include "../HistogramBinDistribution.hpp"
#include "../PoissonDistribution.hpp"
#include "../BinomialDistribution.hpp"
#include "../NegativeBinomialDistribution.hpp"
#include "../GeometricDistribution.hpp"
#include "../HypergeometricDistribution.hpp"
#include "../HistogramPointDistribution.hpp"

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, NormalDistribution) {

  //Check default constructor
  NormalDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.mean(),0.0);
  EXPECT_EQ(defaultDistribution.standardDeviation(),1.0);
  EXPECT_EQ(defaultDistribution.lowerBound(),boost::none);

  //Check normal constructor
  NormalDistribution distribution(2.0,3.0);
  distribution.setLowerBound(0.0);
  distribution.setUpperBound(4.0);
  EXPECT_EQ(distribution.mean(),2.0);
  EXPECT_EQ(distribution.standardDeviation(),3.0);
  EXPECT_EQ(distribution.lowerBound(),0.0);
  EXPECT_EQ(distribution.upperBound(),4.0);
  
}

TEST_F(AnalysisFixture, LognormalDistribution) {

  //Check default constructor
  LognormalDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.mean(),1.0);
  EXPECT_EQ(defaultDistribution.standardDeviation(),1.0);
  EXPECT_EQ(defaultDistribution.lowerBound(),boost::none);

  defaultDistribution.setMean(1.0);
  defaultDistribution.setStandardDeviation(2.0);
  defaultDistribution.setErrorFactor(3.0);
  defaultDistribution.setLambda(4.0);
  defaultDistribution.setZeta(5.0);
  defaultDistribution.setLowerBound(1.0);
  defaultDistribution.setUpperBound(6.0);
  EXPECT_EQ(defaultDistribution.mean(),1.0);
  EXPECT_EQ(defaultDistribution.standardDeviation(),2.0);
  EXPECT_EQ(defaultDistribution.errorFactor(),3.0);
  EXPECT_EQ(defaultDistribution.lambda(),4.0);
  EXPECT_EQ(defaultDistribution.zeta(),5.0);
  EXPECT_EQ(defaultDistribution.lowerBound(),1.0);
  EXPECT_EQ(defaultDistribution.upperBound(),6.0);
  
}

TEST_F(AnalysisFixture, UniformDistribution) {

  //Check default constructor
  UniformDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.lowerBound(),0.0);
  EXPECT_EQ(defaultDistribution.upperBound(),1.0);

  //Normal constructor
  UniformDistribution distribution(1.0,2.0);
  EXPECT_EQ(distribution.lowerBound(),1.0);
  EXPECT_EQ(distribution.upperBound(),2.0);
  
}

TEST_F(AnalysisFixture, LoguniformDistribution) {

  //Check default constructor
  LoguniformDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.lowerBound(),0.1);
  EXPECT_EQ(defaultDistribution.upperBound(),1.0);

  //Normal constructor
  LoguniformDistribution distribution(1.0,2.0);
  EXPECT_EQ(distribution.lowerBound(),1.0);
  EXPECT_EQ(distribution.upperBound(),2.0);
  
}

TEST_F(AnalysisFixture, TriangularDistribution) {

  //Check default constructor
  TriangularDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.mode(),0.5);
  EXPECT_EQ(defaultDistribution.lowerBound(),0.0);
  EXPECT_EQ(defaultDistribution.upperBound(),1.0);

  //Normal constructor
  TriangularDistribution distribution(1.5,1.0,2.0);
  EXPECT_EQ(distribution.mode(),1.5);
  EXPECT_EQ(distribution.lowerBound(),1.0);
  EXPECT_EQ(distribution.upperBound(),2.0);
  
}

TEST_F(AnalysisFixture, ExponentialDistribution) {

  //Check default constructor
  ExponentialDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.beta(),1.0);


  //Normal constructor
  ExponentialDistribution distribution(1.5);
  EXPECT_EQ(distribution.beta(),1.5);
  
}

TEST_F(AnalysisFixture, BetaDistribution) {

  //Check default constructor
  BetaDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.alpha(),2.0);
  EXPECT_EQ(defaultDistribution.beta(),2.0);
  EXPECT_EQ(defaultDistribution.lowerBound(),0.0);
  EXPECT_EQ(defaultDistribution.upperBound(),1.0);


  //Normal constructor
  BetaDistribution distribution(2.5,3.0,1.0,2.0);
  EXPECT_EQ(distribution.alpha(),2.5);
  EXPECT_EQ(distribution.beta(),3.0);
  EXPECT_EQ(distribution.lowerBound(),1.0);
  EXPECT_EQ(distribution.upperBound(),2.0);
  
}

TEST_F(AnalysisFixture, GammaDistribution) {

  //Check default constructor
  GammaDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.alpha(),2.0);
  EXPECT_EQ(defaultDistribution.beta(),0.5);

  //Normal constructor
  GammaDistribution distribution(2.5,3.0);
  EXPECT_EQ(distribution.alpha(),2.5);
  EXPECT_EQ(distribution.beta(),3.0);
  
}

TEST_F(AnalysisFixture, GumbelDistribution) {

  //Check default constructor
  GumbelDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.alpha(),1.0);
  EXPECT_EQ(defaultDistribution.beta(),2.0);

  //Normal constructor
  GumbelDistribution distribution(2.5,3.0);
  EXPECT_EQ(distribution.alpha(),2.5);
  EXPECT_EQ(distribution.beta(),3.0);
  
}

TEST_F(AnalysisFixture, FrechetDistribution) {

  //Check default constructor
  FrechetDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.alpha(),2.0);
  EXPECT_EQ(defaultDistribution.beta(),1.0);

  //Normal constructor
  FrechetDistribution distribution(2.5,3.0);
  EXPECT_EQ(distribution.alpha(),2.5);
  EXPECT_EQ(distribution.beta(),3.0);
  
}

TEST_F(AnalysisFixture, WeibullDistribution) {

  //Check default constructor
  WeibullDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.alpha(),1.0);
  EXPECT_EQ(defaultDistribution.beta(),1.0);

  //Normal constructor
  WeibullDistribution distribution(2.5,3.0);
  EXPECT_EQ(distribution.alpha(),2.5);
  EXPECT_EQ(distribution.beta(),3.0);
  
}

TEST_F(AnalysisFixture, HistogramBinDistribution) {

  std::vector<double> vectorOne(2);
  std::vector<double> vectorTwo(2);
  vectorOne[0] = 0.0;
  vectorOne[1] = 1.0;
  vectorTwo[0] = 1.0;
  vectorTwo[1] = 0.0;

  //Check default constructor
  HistogramBinDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.abscissas(),vectorOne);
  EXPECT_EQ(defaultDistribution.counts(),vectorTwo);

  //Normal constructor
  HistogramBinDistribution distribution(vectorOne, vectorTwo);
  EXPECT_EQ(distribution.abscissas(),vectorOne);
  EXPECT_EQ(distribution.counts(),vectorTwo);
  EXPECT_EQ(distribution.numPairs(),2);
  distribution.setOrdinates(vectorTwo);
  EXPECT_EQ(distribution.ordinates(),vectorTwo);
  EXPECT_EQ(distribution.numPairs(),2);

  EXPECT_FALSE(distribution.setAbscissas(vectorTwo));
  EXPECT_FALSE(distribution.setCounts(vectorOne));
  EXPECT_FALSE(distribution.setOrdinates(vectorOne));
  
}

TEST_F(AnalysisFixture, PoissonDistribution) {

  //Check default constructor
  PoissonDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.lambda(),1.0);

  //Normal constructor
  PoissonDistribution distribution(1.5);
  EXPECT_EQ(distribution.lambda(),1.5);

  EXPECT_FALSE(distribution.setLambda(-2.0));
  
}

TEST_F(AnalysisFixture, BinomialDistribution) {

  //Check default constructor
  BinomialDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.probabilityPerTrial(),0.5);
  EXPECT_EQ(defaultDistribution.numTrials(),20);

  //Normal constructor
  BinomialDistribution distribution(0.6,25);
  EXPECT_EQ(distribution.probabilityPerTrial(),0.6);
  EXPECT_EQ(distribution.numTrials(),25);

  EXPECT_FALSE(distribution.setProbabilityPerTrial(-2.0));
  EXPECT_FALSE(distribution.setProbabilityPerTrial(1.1));
  
}

TEST_F(AnalysisFixture, NegativeBinomialDistribution) {

  //Check default constructor
  NegativeBinomialDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.probabilityPerTrial(),0.5);
  EXPECT_EQ(defaultDistribution.numTrials(),20);

  //Normal constructor
  NegativeBinomialDistribution distribution(0.6,25);
  EXPECT_EQ(distribution.probabilityPerTrial(),0.6);
  EXPECT_EQ(distribution.numTrials(),25);

  EXPECT_FALSE(distribution.setProbabilityPerTrial(-2.0));
  EXPECT_FALSE(distribution.setProbabilityPerTrial(1.1));
  
}

TEST_F(AnalysisFixture, GeometricDistribution) {

  //Check default constructor
  GeometricDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.probabilityPerTrial(),0.5);

  //Normal constructor
  GeometricDistribution distribution(0.5);
  EXPECT_EQ(distribution.probabilityPerTrial(),0.5);

  EXPECT_FALSE(distribution.setProbabilityPerTrial(-2.0));
  EXPECT_FALSE(distribution.setProbabilityPerTrial(1.1));
  
}

TEST_F(AnalysisFixture, HypergeometricDistribution) {

  //Check default constructor
  HypergeometricDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.totalPopulation(),20);
  EXPECT_EQ(defaultDistribution.selectedPopulation(),10);
  EXPECT_EQ(defaultDistribution.numDrawn(),10);

  //Normal constructor
  HypergeometricDistribution distribution(21,11,12);
  EXPECT_EQ(distribution.totalPopulation(),21);
  EXPECT_EQ(distribution.selectedPopulation(),11);
  EXPECT_EQ(distribution.numDrawn(),12);
  
}

TEST_F(AnalysisFixture, HistogramPointDistribution) {

  std::vector<double> vectorOne(2);
  std::vector<double> vectorTwo(2);
  std::vector<double> vectorThree(2);
  vectorOne[0] = 0.0;
  vectorOne[1] = 1.0;
  vectorTwo[0] = 1.0;
  vectorTwo[1] = 1.0;
  vectorThree[0] = -1.0;
  vectorThree[1] = 1.0;

  //Check default constructor
  HistogramPointDistribution defaultDistribution;
  EXPECT_EQ(defaultDistribution.abscissas(),vectorOne);
  EXPECT_EQ(defaultDistribution.counts(),vectorTwo);

  //Normal constructor
  HistogramPointDistribution distribution(vectorOne,vectorTwo);
  EXPECT_EQ(distribution.abscissas(),vectorOne);
  EXPECT_EQ(distribution.counts(),vectorTwo);
  EXPECT_EQ(distribution.numPairs(),2);

  EXPECT_FALSE(distribution.setAbscissas(vectorTwo));
  EXPECT_FALSE(distribution.setCounts(vectorThree));
  
}
