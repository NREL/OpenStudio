#ifndef OPENSTUDIO_RUNMANAGER_LINEARAPPROXIMATION_HPP
#define OPENSTUDIO_RUNMANAGER_LINEARAPPROXIMATION_HPP

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

#endif
