
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
  std::cout << "Cache size: " << m_cache.size();
}

double LinearApproximation::average() const
{
  double sum = 0;
  for (std::vector<std::vector<double> >::const_iterator lhsitr = m_values.begin();
      lhsitr != m_values.end();
      ++lhsitr)
  {
    sum += (*lhsitr)[m_numVars];
  }
  return sum / m_numVars;
}

LinearApproximation LinearApproximation::operator-(const LinearApproximation &t_rhs) const
{
  print("Subtracting", t_rhs.m_values);
  print("From", m_values);

  if (m_numVars != t_rhs.m_numVars)
  {
    throw std::runtime_error("Unable to subtract, different number of variables");
  }

  LinearApproximation retval(m_numVars);

  for (std::vector<std::vector<double> >::const_iterator lhsitr = m_values.begin();
      lhsitr != m_values.end();
      ++lhsitr)
  {
    for (std::vector<std::vector<double> >::const_iterator rhsitr = t_rhs.m_values.begin();
        rhsitr != t_rhs.m_values.end();
        ++rhsitr)
    {
      std::vector<double> vals(m_numVars, 0);

      bool match = true;
      for (size_t i = 0; i < m_numVars; ++i)
      {
        if ( (*lhsitr)[i] != (*rhsitr)[i] )
        {
          match = false;
          break;
        } else {
          vals[i] = (*lhsitr)[i];
        }
      }

      if (match)
      {
        double value = (*lhsitr)[m_numVars] - (*rhsitr)[m_numVars];
        std::cout << " Found a match for our subtraction " << (*lhsitr)[m_numVars] << " " << (*rhsitr)[m_numVars] << std::endl;
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


std::set<std::pair<size_t, std::vector<double> > > LinearApproximation::sortByCommonality(const std::vector<double> &t_vals, 
    const std::vector<std::vector<double> > &t_data) const
{
  std::set<std::pair<size_t, std::vector<double> > > sorted;

  for (std::vector<std::vector<double> >::const_iterator itr = t_data.begin();
       itr != t_data.end();
       ++itr)
  {
    size_t commonality = 0;
    for (size_t i = 0; i < m_numVars; ++i)
    {
      if (t_vals[i] == (*itr)[i])
      {
        ++commonality;
      }
    }

    sorted.insert(std::make_pair(m_numVars - commonality, *itr));
  }

  return sorted;
}

// right now we only know how to filter int the case that all but one variable is the same
std::vector<std::vector<double> > LinearApproximation::filterForProblemReduction(const std::vector<double> &t_vals,
    const std::vector<std::vector<double> > &t_data) const
{
  std::set<std::pair<size_t, std::vector<double> > > sorted = sortByCommonality(t_vals, t_data);
  

  std::vector<std::vector<double> > retval;

  for (std::set<std::pair<size_t, std::vector<double> > >::const_iterator itr = sorted.begin();
       itr != sorted.end();
       ++itr)
  {
    if (itr->first <= 1)
    {
      retval.push_back(itr->second);
    }
  }


  if (retval.size() >= 2)
  {
    // we need at least two points that are this similar to do what we need to do
    return retval;
  } else {
    return t_data;
  }
  return filterForSimilarity(t_vals, retval);
}


double LinearApproximation::approximate(const std::vector<double> &t_vals) const
{
  // Linear Approximation
  // A(x-x0) + B(y-y0) + C(z-z0) = 0
  validateVariableSize(t_vals);

  print("Approximating: ", t_vals);

  std::vector<std::vector<double> > optimalPoints = filterForProblemReduction(t_vals, m_values);

//  print("Starting points: ", m_values);
  std::vector<std::vector<double> > sortedPoints = sortByDistance(t_vals, optimalPoints);

  if (!sortedPoints.empty() && distance(t_vals, sortedPoints[0]) == 0)
  {
    //return exact match
    return sortedPoints[0].back();
  }

//  print("Sorted Points: ", sortedPoints);
  std::vector<std::vector<double> > chosenPoints = filterForSimilarity(t_vals, sortedPoints);

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

  std::cout << "final approximation: " << approximation << std::endl;

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
    for (std::vector<std::vector<double> >::const_iterator itr = t_vals.begin();
        itr != t_vals.end();
        ++itr, ++rowCount)
    {
      if (t_vals[0][i] != (*itr)[i])
      {
        diversityFound = true; // not all of the inputs match each other
      }

      if (t_point[i] != (*itr)[i])
      {
        allTheSame = false; // not all of the inputs for this position match the requested approximation
        differencePosition = rowCount;
      }
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
    for (std::vector<std::vector<double> >::const_iterator itr = t_points.begin();
        itr != t_points.end();
        ++itr)
    {
      if ((*itr)[i] != t_points[0][i])
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
          pm_sign *= -1.0; // swap_rows would swap a pair of rows here, so we change sign
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

  for (std::vector<std::vector<std::vector<double> > >::const_iterator itr = t_matrices.begin();
      itr != t_matrices.end();
      ++itr)
  {
    retval.push_back(determinate(*itr));
  }

  return retval; 
}

std::vector<std::vector<double> > LinearApproximation::sortByDistance(const std::vector<double> &t_point, 
    const std::vector<std::vector<double> > &t_values) const
{
  std::set<std::pair<double, std::vector<double> > > distanceSorted;

  for (std::vector<std::vector<double> >::const_iterator itr = t_values.begin();
      itr != t_values.end();
      ++itr)
  {
    distanceSorted.insert(std::make_pair(distance(t_point, *itr), *itr));
  }

  std::vector<std::vector<double> > retval;

  for (std::set<std::pair<double, std::vector<double> > >::const_iterator itr = distanceSorted.begin();
      itr != distanceSorted.end();
      ++itr)
  {
    retval.push_back(itr->second);
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
  for (std::vector<double>::const_iterator itr = t_vals.begin();
      itr != t_vals.end();
      ++itr)
  {
    std::cout << std::setw(10) << *itr << " ";
  }
  std::cout << "]" << std::endl;
}

void LinearApproximation::print(const std::string &t_str, const std::vector<std::vector<double> > &t_vals)
{
  if (!t_str.empty())
  {
    std::cout << t_str << std::endl;
  }

  for (std::vector<std::vector<double> >::const_iterator itr = t_vals.begin();
      itr != t_vals.end();
      ++itr)
  {
    print("", *itr);
  }
}

void LinearApproximation::print(const std::string &t_str, const std::vector<std::vector<std::vector<double> > > &t_vals)
{
  if (!t_str.empty())
  {
    std::cout << t_str << std::endl;
  }

  for (std::vector<std::vector<std::vector<double> > >::const_iterator itr = t_vals.begin();
      itr != t_vals.end();
      ++itr)
  {
    print("", *itr);
    std::cout << std::endl;
  }
}



