/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/TableMultiVariableLookup.hpp"
#include "../../model/TableMultiVariableLookup_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/Table_MultiVariableLookup_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateTableMultiVariableLookup( TableMultiVariableLookup& modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;
  OptionalInt n;  

  // Create a new IddObjectType::Table_MultiVariableLookup
  IdfObject idfObject(IddObjectType::Table_MultiVariableLookup);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // InterpolationMethod
  if( (s = modelObject.interpolationMethod()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::InterpolationMethod,s.get());
  }

  // NumberofInterpolationPoints
  if( (n = modelObject.numberofInterpolationPoints()) )
  {
    idfObject.setInt(Table_MultiVariableLookupFields::NumberofInterpolationPoints,n.get());
  }

  // CurveType
  if( (s = modelObject.curveType()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::CurveType,s.get());
  }
  
  // TableDataFormat
  if( (s = modelObject.tableDataFormat()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::TableDataFormat,s.get());
  }

  // ExternalFileName
  // Not supported
  
  // X1SortOrder
  idfObject.setString(Table_MultiVariableLookupFields::X1SortOrder,"Ascending");
  
  // X2SortOrder
  idfObject.setString(Table_MultiVariableLookupFields::X2SortOrder,"Ascending");
  
  // NormalizationReference
  if( (d = modelObject.normalizationReference()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::NormalizationReference,d.get());
  }
  
  // MinimumValueofX1
  if( (d = modelObject.minimumValueofX1()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MinimumValueofX1,d.get());
  }
  
  // MaximumValueofX1
  if( (d = modelObject.maximumValueofX1()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MaximumValueofX1,d.get());
  }
  
  // MinimumValueofX2
  if( (d = modelObject.minimumValueofX2()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MinimumValueofX2,d.get());
  }
  
  // MaximumValueofX2
  if( (d = modelObject.maximumValueofX2()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MaximumValueofX2,d.get());
  }
  
  // MinimumValueofX3
  if( (d = modelObject.minimumValueofX3()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MinimumValueofX3,d.get());
  }
  
  // MaximumValueofX3
  if( (d = modelObject.maximumValueofX3()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MaximumValueofX3,d.get());
  }
  
  // MinimumValueofX4
  if( (d = modelObject.minimumValueofX4()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MinimumValueofX4,d.get());
  }
  
  // MaximumValueofX4
  if( (d = modelObject.maximumValueofX4()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MaximumValueofX4,d.get());
  }
  
  // MinimumValueofX5
  if( (d = modelObject.minimumValueofX5()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MinimumValueofX5,d.get());
  }
  
  // MaximumValueofX5
  if( (d = modelObject.maximumValueofX5()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MaximumValueofX5,d.get());
  }
  
  // MinimumTableOutput
  if( (d = modelObject.minimumTableOutput()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MinimumTableOutput,d.get());
  }
  
  // MaximumTableOutput
  if( (d = modelObject.maximumTableOutput()) )
  {
    idfObject.setDouble(Table_MultiVariableLookupFields::MaximumTableOutput,d.get());
  }
  
  // InputUnitTypeforX1
  if( (s = modelObject.inputUnitTypeforX1()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::InputUnitTypeforX1,s.get());
  }
  
  // InputUnitTypeforX2
  if( (s = modelObject.inputUnitTypeforX2()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::InputUnitTypeforX2,s.get());
  }
  
  // InputUnitTypeforX3
  if( (s = modelObject.inputUnitTypeforX3()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::InputUnitTypeforX3,s.get());
  }
  
  // InputUnitTypeforX4
  if( (s = modelObject.inputUnitTypeforX4()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::InputUnitTypeforX4,s.get());
  }
  
  // InputUnitTypeforX5
  if( (s = modelObject.inputUnitTypeforX5()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::InputUnitTypeforX5,s.get());
  }
  
  // OutputUnitType
  if( (s = modelObject.outputUnitType()) )
  {
    idfObject.setString(Table_MultiVariableLookupFields::OutputUnitType,s.get());
  }
  
  // NumberofIndependentVariables
  if( (n = modelObject.numberofIndependentVariables()) )
  {
    idfObject.setInt(Table_MultiVariableLookupFields::NumberofIndependentVariables,n.get());
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
    for(auto it = xValues.begin();
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
    // If there is just one variable then we just make a list of the y values.
    std::vector<double> xValues = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(0);

    for(auto it = xValues.begin();
        it != xValues.end();
        ++it)
    {
      std::vector<double> coord(1);
      coord[0] = *it;

      boost::optional<double> yValue = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->yValue(coord);
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
    // If there are two variables we make a list of the y values corresponding to x1 and x2,
    // iterating over x2 in the outer layer
    std::vector<double> x1Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(0);
    std::vector<double> x2Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(1);

    for(auto it2 = x2Values.begin();
        it2 != x2Values.end();
        ++it2)
    {
      for(auto it1 = x1Values.begin();
          it1 != x1Values.end();
          ++it1)
      {
        std::vector<double> coord(2);
        coord[0] = *it1;
        coord[1] = *it2;

        boost::optional<double> yValue = modelObject.yValue(coord);
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
    std::vector<std::pair<std::vector<double>,double> > points = modelObject.points();

    // Slice the first and second x values off the coordinates
    std::vector<std::vector<double> > slices;
    for(std::vector<std::pair<std::vector<double>,double> >::const_iterator it = points.begin();
        it != points.end();
        ++it)
    {
      OS_ASSERT(it->first.size() == t_numberofIndependentVariables);
      std::vector<double> slice(it->first.begin() + 2,it->first.begin() + t_numberofIndependentVariables);
      slices.push_back(slice);
    }

    // Remove duplicate slices
    std::sort(slices.begin(),slices.end());
    slices.erase(std::unique(slices.begin(),slices.end()),slices.end());

    // Iterate over each slice that is left, creating a 2D table for each one 
    for(std::vector<std::vector<double> >::const_iterator it = slices.begin();
        it != slices.end();
        ++it)
    {
      for(auto it2 = it->begin();
          it2 != it->end();
          ++it2)
      {
        idfObject.setDouble(t_currentFieldIndex,*it2);
        ++t_currentFieldIndex; 
      }

      std::vector<double> x1Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(0);
      std::vector<double> x2Values = modelObject.getImpl<model::detail::TableMultiVariableLookup_Impl>()->xValues(1);

      for(auto x2It = x2Values.begin();
          x2It != x2Values.end();
          ++x2It)
      {
        for(auto x1It = x1Values.begin();
            x1It != x1Values.end();
            ++x1It)
        {
          std::vector<double> coord(2);
          coord[0] = *x1It;
          coord[1] = *x2It;
          coord.insert(coord.end(),it->begin(),it->end());

          boost::optional<double> yValue = modelObject.yValue(coord);
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
  }

  return idfObject;
}

} // energyplus

} // openstudio

