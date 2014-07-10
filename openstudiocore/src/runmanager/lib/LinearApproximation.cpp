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

#include "LinearApproximation.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>


LinearApproximation::LinearApproximation(const size_t t_numVars)
  : m_numVars(t_numVars)
{
}

LinearApproximation::~LinearApproximation()
{
}

double LinearApproximation::average() const
{
  double sum = 0;
  for (const auto & lhsitr : m_values)
  {
    sum += lhsitr[m_numVars];
  }
  return sum / m_numVars;
}

LinearApproximation LinearApproximation::operator-(const LinearApproximation &t_rhs) const
{
  if (m_numVars != t_rhs.m_numVars)
  {
    throw std::runtime_error("Unable to subtract, different number of variables");
  }

  LinearApproximation retval(m_numVars);

  for (const auto & lhsitr : m_values)
  {
    for (const auto & rhsitr : t_rhs.m_values)
    {
      std::vector<double> vals(m_numVars, 0);

      bool match = true;
      for (size_t i = 0; i < m_numVars; ++i)
      {
        if ( lhsitr[i] != rhsitr[i] )
        {
          match = false;
          break;
        } else {
          vals[i] = lhsitr[i];
        }
      }

      if (match)
      {
        double value = lhsitr[m_numVars] - rhsitr[m_numVars];
        retval.addVals(vals, value);
        break;
      }
    }
  }

  return retval;
}

void LinearApproximation::addVals(std::vector<double> t_vals, const double t_result)
{
  validateVariableSize(t_vals);

  t_vals.push_back(t_result);

  for (std::vector<std::vector<double> >::const_iterator lhsitr = m_values.begin();
      lhsitr != m_values.end();
      ++lhsitr)
  {
    bool match = true;
    for (size_t i = 0; i < m_numVars; ++i)
    {
      if ( (*lhsitr)[i] != t_vals[i] )
      {
        match = false;
        break;
      }
    }

    if (match)
    {
      if ((*lhsitr)[m_numVars] != t_result)
      {
        throw std::runtime_error("Data already exists with a different result value");
      } else {
        // it already exists
        return;
      }
    }
  }

  // it didn't already exist, add it now
  m_values.push_back(t_vals);

}




std::vector<std::vector<double> > LinearApproximation::findMinimalDifferences(
    size_t t_numDifferences,
    const std::vector<std::vector<double> > &t_vals,
    const std::vector<std::vector<double> > &t_data) const
{
  // t_vals includes the baseline requested value, so we want one more than that
  if (t_data.empty() || t_numDifferences + 2 == t_vals.size())
  {
    // this is as good as we're going to get / can use right now
    std::vector<std::vector<double> > retval = t_vals;

    if (!t_vals.empty())
    {
      retval.erase(retval.begin());
    }
    return retval;
  }

  std::vector<std::vector<double> > candidates;
  std::vector<std::vector<double> > remainder;

  for (size_t i = 0; i < t_data.size(); ++i)
  {
    size_t numDifferences = 0;

    for (size_t j = 0; j < m_numVars; ++j)
    {
      for (size_t i2 = 0; i2 < t_vals.size(); ++i2)
      {
        if (t_data[i][j] != t_vals[i2][j])
        {
          ++numDifferences;
          break; // don't check any further input rows
        }
      }

      if (numDifferences > t_numDifferences)
      {
        // don't continue with this data row
        break;
      }
    }

    if (numDifferences <= t_numDifferences)
    {
      candidates.push_back(t_data[i]);
    } else {
      remainder.push_back(t_data[i]);
    }
  }

  std::vector<std::vector<double> > keptresult = t_vals;
  keptresult.erase(keptresult.begin());

  for (size_t i = 0; i < candidates.size(); ++i)
  {
    std::vector<std::vector<double> > newvals(t_vals);
    newvals.push_back(candidates[i]);

    std::vector<std::vector<double> > newdata(t_data);
    newdata.erase(std::remove(newdata.begin(), newdata.end(), candidates[i]), newdata.end());

    std::vector<std::vector<double> > result = findMinimalDifferences(t_numDifferences, newvals, newdata);
    if (result.size() > keptresult.size())
    {
      keptresult = result;
    }
  }

  return keptresult;
}

std::vector<std::vector<double> > LinearApproximation::filterForProblemReduction(const std::vector<double> &t_vals,
    const std::vector<std::vector<double> > &t_data) const
{
  std::vector<std::vector<double> > testData;
  testData.push_back(t_vals);

  for (size_t i = 1; i < m_numVars; ++i)
  {
    std::vector<std::vector<double> > results = findMinimalDifferences(i, testData, t_data);
    if (results.size() > i)
    {
      return results;
    }
  }

  return t_data;
}


double LinearApproximation::approximate(const std::vector<double> &t_vals) const
{
  // Linear Approximation
  // A(x-x0) + B(y-y0) + C(z-z0) = 0
  validateVariableSize(t_vals);

//  print("Approximating: ", t_vals);

  std::vector<std::vector<double> > sortedPoints = sortByDistance(t_vals, m_values);

  if (!sortedPoints.empty() && distance(t_vals, sortedPoints[0]) == 0)
  {
    //return exact match
    return sortedPoints[0].back();
  }

  std::vector<std::vector<double> > optimalPoints = filterForProblemReduction(t_vals, sortedPoints);

//  print("Starting points: ", m_values);

//  print("Sorted Points: ", sortedPoints);
  std::vector<std::vector<double> > chosenPoints = filterForSimilarity(t_vals, optimalPoints);

//  print("Chosen points: ", chosenPoints);


  std::vector<std::vector<std::vector<double> > > coefficientMatrices = buildCoefficientMatrices(chosenPoints);

//  print("Coefficients: ", coefficientMatrices);


  std::vector<double> coefficients = solveDeterminates(coefficientMatrices);

//  print("Coefficients: ", coefficients);
  assert(coefficients.size() == m_numVars + 1);



  double approximation = 0;
  for (size_t i = 0; i < m_numVars; ++i)
  {
    // A(x-x0) + B(y-y0) + C(z-z0) ... 
    approximation += coefficients[i] * (t_vals[i] - chosenPoints[0][i]);
  }

  // the last variable is the one we are solving for 
  // z=(z0*C+(y0-y)*B+(x0-x)*A)/C
  if (coefficients[m_numVars] == 0)
  {
    throw std::runtime_error("Unabled to approximate, not enough data");
  }

  approximation = (approximation + chosenPoints[0][m_numVars] * coefficients[m_numVars]) / coefficients[m_numVars];

//  std::cout << "final approximation: " << approximation << std::endl;

  return approximation;
}


void LinearApproximation::validateVariableSize(const std::vector<double> &t_vals) const
{
  if (t_vals.size() != m_numVars)
  {
    throw std::range_error("Unexpected number of variables supplied");
  }
}

const std::vector<std::vector<double> > LinearApproximation::filterForSimilarity(const std::vector<double> &t_point, const std::vector<std::vector<double> > &t_vals) const
{
  std::vector<std::vector<double> > goodPoints = t_vals;
  int pointsNeeded = m_numVars + 1;

  for (size_t i = 0; i < m_numVars; ++i)
  {
    bool diversityFound = false;
    bool allTheSame = true;

    int differencePosition = -1;

    int rowCount = 0;
    for (const auto & vals : t_vals)
    {
      if (t_vals[0][i] != vals[i])
      {
        diversityFound = true; // not all of the inputs match each other
      }

      if (t_point[i] != vals[i])
      {
        allTheSame = false; // not all of the inputs for this position match the requested approximation
        differencePosition = rowCount;
      }

      ++rowCount;
    }

    if (!diversityFound && !allTheSame)
    {
      // There is not enough data available to calculate the value for this position
      std::stringstream ss;
      ss << "Not enough data available for the " << i << " variable position";
      throw std::runtime_error(ss.str());
    }

    if (allTheSame || (differencePosition >= pointsNeeded))
    {
      if (pointsNeeded > 2)
      {
        --pointsNeeded;
        goodPoints.resize(std::min(pointsNeeded, static_cast<int>(goodPoints.size()))); // never expand it
      }
    }
  }

  goodPoints.resize(std::min(pointsNeeded, static_cast<int>(goodPoints.size())));
  return goodPoints;
}

std::vector<std::vector<std::vector<double> > > LinearApproximation::buildCoefficientMatrices(
    const std::vector<std::vector<double> > &t_points) const
{
  std::set<size_t> discardedColumns;

  for (size_t i = 0; i <= m_numVars; ++i)
  {
    bool allthesame = true;
    for (const auto & point : t_points)
    {
      if (point[i] != t_points[0][i])
      {
        allthesame = false;
      }
    }

    if (allthesame)
    {
      discardedColumns.insert(i);
    }
  }

  std::vector<std::vector<std::vector<double> > > retvals;

  for (size_t i = 0; i <= m_numVars; ++i)
  {
    if (discardedColumns.count(i) == 1)
    {
      // this is a discarded column its value is 0
      std::vector<double> v(1,0); // a vector of one element of value 0
      std::vector<std::vector<double> > m;
      m.push_back(v);
      retvals.push_back(m);
    } else {
      std::vector<std::vector<double> > m;
      for (size_t curRow = 0; curRow < m_numVars - discardedColumns.size(); ++curRow)
      {
        std::vector<double> row;
        for (size_t j = 0; j <= m_numVars; ++j)
        {
          if (j != i && discardedColumns.count(j) == 0)
          {
            // This is a column we want to work with
            row.push_back(t_points[curRow+1][j] - t_points[0][j]);
          }
        }
        m.push_back(row);
      }
      retvals.push_back(m);
    }
  }

  return retvals;
}

std::vector<std::vector<double> > LinearApproximation::removeCol(const std::vector<std::vector<double> > &t_matrix, const size_t col) const
{
  std::vector<std::vector<double> > retval(t_matrix.size()-1, std::vector<double>(t_matrix.size()-1, 0));

  for (size_t i = 1; i < t_matrix.size(); ++i)
  {
    size_t colnum = 0;
    for (size_t j = 0; j < t_matrix[i].size(); ++j)
    {
      if (j != col)
      {
        retval[i-1][colnum] = t_matrix[i][j];
        ++colnum;
      }
    }
  }

  return retval;
}

double LinearApproximation::determinate(const std::vector<std::vector<double> > &t_matrix) const
{
/*
  bool docache = false;

  if (docache)
  {
    std::map<std::vector<std::vector<double> >, double>::const_iterator itr = m_cache.find(t_matrix);
    if (itr != m_cache.end())
    {
      std::cout << "Cache hit" << std::endl; 
      return itr->second;
    }
  }

  if (t_matrix.size() == 1)
  {
    return t_matrix[0][0];
  } else if (t_matrix.size() == 2) {
    return t_matrix[0][0] * t_matrix[1][1] - t_matrix[0][1] * t_matrix[1][0];
  } else {
    double d = 0;

    for (size_t i = 0; i < t_matrix.size(); ++i)
    {
      //          d += pow(-1, i) * t_matrix[0][i] * determinate(removeCol(t_matrix, i));
      d += (i%2==1?-1:1) * t_matrix[0][i] * determinate(removeCol(t_matrix, i));
    }

    if (docache)
    {
      m_cache[t_matrix] = d;
    }
    return d;
  }
  */

  namespace bnu = boost::numeric::ublas;

  class Determinant 
  {

    static int determinant_sign(const bnu::permutation_matrix<std::size_t>& pm)
    {
      int pm_sign=1;
      std::size_t size = pm.size();
      for (std::size_t i = 0; i < size; ++i)
      {
        if (i != pm(i)) {
          pm_sign *= -1; // swap_rows would swap a pair of rows here, so we change sign
        }
      }
      return pm_sign;
    }

    static double determinant( bnu::matrix<double>& m ) {
      bnu::permutation_matrix<std::size_t> pm(m.size1());
      double det = 1.0;
      if( bnu::lu_factorize(m,pm) ) {
        det = 0.0;
      } else {
        for(size_t i = 0; i < m.size1(); i++) {
          det *= m(i,i); // multiply by elements on diagonal
        }

        det = det * determinant_sign( pm );
      }
      return det;
    }

    public:
    static double determinant(const std::vector<std::vector<double> > &t_matrix)
    {
      bnu::matrix<double> matrix(t_matrix.size(), t_matrix.size());

      for (size_t i = 0; i < t_matrix.size(); ++i)
      {
        for (size_t j = 0; j < t_matrix[i].size(); ++j)
        {
          matrix(i, j) = t_matrix[i][j];
        }
      }

      return determinant(matrix);
    }
  };

  double retval = Determinant::determinant(t_matrix);

  /*
  if (docache)
  {
    m_cache[t_matrix] = retval;
  }
  */

  return retval;
}

std::vector<double> LinearApproximation::solveDeterminates(const std::vector<std::vector<std::vector<double> > > &t_matrices) const
{
  std::vector<double> retval;

  for (const auto & matrix : t_matrices)
  {
    retval.push_back(determinate(matrix));
  }

  return retval; 
}

std::vector<std::vector<double> > LinearApproximation::sortByDistance(const std::vector<double> &t_point, 
    const std::vector<std::vector<double> > &t_values) const
{
  std::set<std::pair<double, std::vector<double> > > distanceSorted;

  for (const auto & vals : t_values)
  {
    distanceSorted.insert(std::make_pair(distance(t_point, vals), vals));
  }

  std::vector<std::vector<double> > retval;

  for (const auto & vals : distanceSorted)
  {
    retval.push_back(vals.second);
  }

  return retval;
}

double LinearApproximation::distance(const std::vector<double> &t_p1, const std::vector<double> &t_p2) const
{
  double result = 0;
  for (size_t i = 0; i < m_numVars; ++i)
  {
    double part = t_p1[i] - t_p2[i];
    part *= part;

    result += part;
  }

  return sqrt(result);
}

void LinearApproximation::print(const std::string &t_str, const std::vector<double> &t_vals)
{
  if (!t_str.empty())
  {
    std::cout << t_str << std::endl;
  }

  std::cout << "[ ";
  for (const auto & vals : t_vals)
  {
    std::cout << std::setw(10) << vals << " ";
  }
  std::cout << "]" << std::endl;
}

void LinearApproximation::print(const std::string &t_str, const std::vector<std::vector<double> > &t_vals)
{
  if (!t_str.empty())
  {
    std::cout << t_str << std::endl;
  }

  for (const auto & vals : t_vals)
  {
    print("", vals);
  }
}

std::pair<double, double> LinearApproximation::nearestFurthestNeighborDistances(const std::vector<double> &t_vals) const
{
  std::vector<std::vector<double> > sorted = sortByDistance(t_vals, m_values);

  if (sorted.size() < 1)
  {
    throw std::range_error("no neighbors");
  }

  return std::make_pair(distance(t_vals, sorted.front()), distance(t_vals, sorted.back()));
}

void LinearApproximation::print(const std::string &t_str, const std::vector<std::vector<std::vector<double> > > &t_vals)
{
  if (!t_str.empty())
  {
    std::cout << t_str << std::endl;
  }

  for (const auto & vals : t_vals)
  {
    print("", vals);
    std::cout << std::endl;
  }
}



