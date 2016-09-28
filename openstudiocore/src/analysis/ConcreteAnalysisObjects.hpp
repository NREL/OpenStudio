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

#ifndef ANALYSIS_CONCRETEANALYSISOBJECTS_HPP
#define ANALYSIS_CONCRETEANALYSISOBJECTS_HPP

#include "Analysis.hpp"
#include "BetaDistribution.hpp"
#include "BinomialDistribution.hpp"
#include "ContinuousVariable.hpp"
#include "DataPoint.hpp"
#include "DDACEAlgorithm.hpp"
#include "DDACEAlgorithmOptions.hpp"
#include "DesignOfExperiments.hpp"
#include "DesignOfExperimentsOptions.hpp"
#include "ExponentialDistribution.hpp"
#include "FrechetDistribution.hpp"
#include "FSUDaceAlgorithm.hpp"
#include "FSUDaceAlgorithmOptions.hpp"
#include "Function.hpp"
#include "GammaDistribution.hpp"
#include "GeometricDistribution.hpp"
#include "GumbelDistribution.hpp"
#include "HistogramBinDistribution.hpp"
#include "HistogramPointDistribution.hpp"
#include "HypergeometricDistribution.hpp"
#include "LinearFunction.hpp"
#include "LognormalDistribution.hpp"
#include "LoguniformDistribution.hpp"
#include "MeasureGroup.hpp"
#include "NegativeBinomialDistribution.hpp"
#include "NormalDistribution.hpp"
#include "NullMeasure.hpp"
#include "OptimizationDataPoint.hpp"
#include "OptimizationProblem.hpp"
#include "OutputAttributeVariable.hpp"
#include "Problem.hpp"
#include "ParameterStudyAlgorithm.hpp"
#include "ParameterStudyAlgorithmOptions.hpp"
#include "PoissonDistribution.hpp"
#include "PSUADEDaceAlgorithm.hpp"
#include "PSUADEDaceAlgorithmOptions.hpp"
#include "RubyContinuousVariable.hpp"
#include "RubyMeasure.hpp"
#include "SamplingAlgorithm.hpp"
#include "SamplingAlgorithmOptions.hpp"
#include "SequentialSearch.hpp"
#include "SequentialSearchOptions.hpp"
#include "TriangularDistribution.hpp"
#include "UncertaintyDescription.hpp"
#include "UniformDistribution.hpp"
#include "WeibullDistribution.hpp"
#include "WorkflowStep.hpp"

#include "Analysis_Impl.hpp"
#include "ContinuousVariable_Impl.hpp"
#include "DataPoint_Impl.hpp"
#include "DDACEAlgorithm_Impl.hpp"
#include "DDACEAlgorithmOptions_Impl.hpp"
#include "DesignOfExperiments_Impl.hpp"
#include "DesignOfExperimentsOptions_Impl.hpp"
#include "FSUDaceAlgorithm_Impl.hpp"
#include "FSUDaceAlgorithmOptions_Impl.hpp"
#include "Function_Impl.hpp"
#include "LinearFunction_Impl.hpp"
#include "MeasureGroup_Impl.hpp"
#include "NullMeasure_Impl.hpp"
#include "OptimizationDataPoint_Impl.hpp"
#include "OptimizationProblem_Impl.hpp"
#include "OutputAttributeVariable_Impl.hpp"
#include "Problem_Impl.hpp"
#include "ParameterStudyAlgorithm_Impl.hpp"
#include "ParameterStudyAlgorithmOptions_Impl.hpp"
#include "PSUADEDaceAlgorithm_Impl.hpp"
#include "PSUADEDaceAlgorithmOptions_Impl.hpp"
#include "RubyContinuousVariable_Impl.hpp"
#include "RubyMeasure_Impl.hpp"
#include "SamplingAlgorithm_Impl.hpp"
#include "SamplingAlgorithmOptions_Impl.hpp"
#include "SequentialSearch_Impl.hpp"
#include "SequentialSearchOptions_Impl.hpp"
#include "UncertaintyDescription_Impl.hpp"
#include "WorkflowStep_Impl.hpp"

#endif // ANALYSIS_CONCRETEANALYSISOBJECTS_HPP
