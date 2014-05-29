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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/TableMultiVariableLookup.hpp>
#include <model/TableMultiVariableLookup_Impl.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idd/Table_MultiVariableLookup_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

namespace multivariablelookup
{
  typedef std::vector<double> Combination;
  
  std::vector<Combination> combinations(const TableMultiVariableLookup & table,
                                        std::vector<Combination> & allCombinations,
                                        Combination & thisCombination,
                                        boost::optional<double> value,
                                        int variableIndex)
  {
    if(value)
    {
      thisCombination.push_back(value.get());
    }
  
    if( variableIndex >= table.numberofIndependentVariables() )
    {
      allCombinations.push_back(thisCombination);
      thisCombination = Combination();
    }
    else
    {
      std::vector<double> xValues = table.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(variableIndex);
      for(std::vector<double>::iterator it = xValues.begin();
          it != xValues.end();
          ++it)
      {
        combinations(table,allCombinations,thisCombination,*it,++variableIndex);
      }
    }
  
    return allCombinations;
  }
} // multivariablelookup

boost::optional<IdfObject> ForwardTranslator::translateTableMultiVariableLookup( TableMultiVariableLookup& modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  // Create a new IddObjectType::Table_MultiVariableLookup
  IdfObject idfObject(IddObjectType::Table_MultiVariableLookup);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  unsigned t_numNonextensibleFields = idfObject.numNonextensibleFields();
  unsigned t_currentFieldIndex = t_numNonextensibleFields;
  unsigned t_numberofIndependentVariables = modelObject.numberofIndependentVariables();

  // Set the number of xValues for each independent variable
  for(unsigned i = 0; i != t_numberofIndependentVariables; ++i)
  {
    unsigned xValueCount = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(i).size();
    idfObject.setUnsigned(t_currentFieldIndex,xValueCount);
    ++t_currentFieldIndex;
  }

  // Set the xValues for each independent variable
  for(unsigned i = 0; i != t_numberofIndependentVariables; ++i)
  {
    std::vector<double> xValues = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(i);
    for(std::vector<double>::iterator it = xValues.begin();
        it != xValues.end();
        ++it)
    {
      idfObject.setDouble(t_currentFieldIndex,*it);
      ++t_currentFieldIndex;
    }
  }

  // Set the table data
  if(t_numberofIndependentVariables == 1u)
  {
    std::vector<double> xValues = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(0);

    for(std::vector<double>::iterator it = xValues.begin();
        it != xValues.end();
        ++it)
    {
      std::vector<double> point;
      point.push_back(*it);

      boost::optional<double> yValue = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->yValue(point);
      if(yValue)
      {
        idfObject.setDouble(t_currentFieldIndex,yValue.get());
      }
      else
      {
        idfObject.setString(t_currentFieldIndex,"");
      }
      ++t_currentFieldIndex;
    }
  }
  else if(t_numberofIndependentVariables == 2u)
  {
    std::vector<double> x1Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(0);
    std::vector<double> x2Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(1);

    for(std::vector<double>::iterator it2 = x2Values.begin();
        it2 != x2Values.end();
        ++it2)
    {
      for(std::vector<double>::iterator it1 = x1Values.begin();
          it1 != x1Values.end();
          ++it1)
      {
        std::vector<double> point;
        point.push_back(*it1);
        point.push_back(*it2);

        boost::optional<double> yValue = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->yValue(point);
        if(yValue)
        {
          idfObject.setDouble(t_currentFieldIndex,yValue.get());
        }
        else
        {
          idfObject.setString(t_currentFieldIndex,"");
        }
        ++t_currentFieldIndex;
      }
    }
  }
  else
  {
    std::vector<multivariablelookup::Combination> combinations;
    multivariablelookup::Combination combination;
    
    combinations = multivariablelookup::combinations(modelObject,
                                                     combinations,
                                                     combination,
                                                     boost::none,
                                                     2);

    for(std::vector<multivariablelookup::Combination>::const_iterator it = combinations.begin();
        it != combinations.end();
        ++it)
    {
      std::vector<boost::optional<double> > yValues;

      std::vector<double> x1Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(0);
      std::vector<double> x2Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(1);

      for(std::vector<double>::iterator it2 = x2Values.begin();
          it2 != x2Values.end();
          ++it2)
      {
        for(std::vector<double>::iterator it1 = x1Values.begin();
            it1 != x1Values.end();
            ++it1)
        {
          std::vector<double> point;
          point.push_back(*it1);
          point.push_back(*it2);

          boost::optional<double> yValue = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->yValue(point);
          yValues.push_back(yValue);
        }
      }
      if( ! yValues.empty() )
      {
        for(multivariablelookup::Combination::const_iterator combinationIt = it->begin();
            combinationIt != it->end();
            ++combinationIt)
        {
          idfObject.setDouble(t_currentFieldIndex,*combinationIt);
          ++t_currentFieldIndex;
        } 

        for(std::vector<boost::optional<double> >::const_iterator yIt = yValues.begin();
            yIt != yValues.end();
            ++yIt)
        {
          if(*yIt)
          {
            idfObject.setDouble(t_currentFieldIndex,(*yIt).get());
          }
          else
          {
            idfObject.setString(t_currentFieldIndex,"");
          }
          ++t_currentFieldIndex;
        }
      }
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

