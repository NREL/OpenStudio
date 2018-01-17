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

#ifndef RUNMANAGER_LIB_LINEARAPPROXIMATION_HPP
#define RUNMANAGER_LIB_LINEARAPPROXIMATION_HPP

#include <vector>
#include <set>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <utility>
#include <cmath>
#include <cassert>
#include <map>
#include "RunManagerAPI.hpp"

class RUNMANAGER_API LinearApproximation
{
  public:
    explicit LinearApproximation(const size_t t_numVars);

    ~LinearApproximation();

    double average() const;

    LinearApproximation operator-(const LinearApproximation &t_rhs) const;

    void addVals(std::vector<double> t_vals, const double t_result);


    double approximate(const std::vector<double> &t_vals) const;

    std::pair<double, double> nearestFurthestNeighborDistances(const std::vector<double> &t_vals) const;

  private:
    std::vector<std::vector<double> > findMinimalDifferences(
        size_t t_numDifferences,
        const std::vector<std::vector<double> > &t_vals,
        const std::vector<std::vector<double> > &t_data) const;

    void validateVariableSize(const std::vector<double> &t_vals) const;
    const std::vector<std::vector<double> > filterForSimilarity(const std::vector<double> &t_point, const std::vector<std::vector<double> > &t_vals) const;
    std::vector<std::vector<double> > filterForProblemReduction(const std::vector<double> &t_vals,
        const std::vector<std::vector<double> > &t_data) const;


    std::vector<std::vector<std::vector<double> > > buildCoefficientMatrices(
        const std::vector<std::vector<double> > &t_points) const;
    std::vector<std::vector<double> > removeCol(const std::vector<std::vector<double> > &t_matrix, const size_t col) const;
    double determinate(const std::vector<std::vector<double> > &t_matrix) const;
    std::vector<double> solveDeterminates(const std::vector<std::vector<std::vector<double> > > &t_matrices) const;
    std::vector<std::vector<double> > sortByDistance(const std::vector<double> &t_point, 
        const std::vector<std::vector<double> > &t_values) const;

    double distance(const std::vector<double> &t_p1, const std::vector<double> &t_p2) const;

    static void print(const std::string &t_str, const std::vector<double> &t_vals);

    static void print(const std::string &t_str, const std::vector<std::vector<double> > &t_vals);

    static void print(const std::string &t_str, const std::vector<std::vector<std::vector<double> > > &t_vals);

    size_t m_numVars;
    mutable std::map<std::vector<std::vector<double> >, double> m_cache;
    std::vector<std::vector<double> > m_values;
};

#endif // RUNMANAGER_LIB_LINEARAPPROXIMATION_HPP
