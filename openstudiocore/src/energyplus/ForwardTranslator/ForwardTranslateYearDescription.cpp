/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"

#include <utilities/idd/Output_PreprocessorMessage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>


using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateYearDescription( YearDescription & modelObject )
{
  // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in 
  // the final file. 
  IdfObject idfObject(IddObjectType::Output_PreprocessorMessage);
  m_idfObjects.push_back(idfObject);
  m_map.insert(std::make_pair(modelObject.handle(), idfObject));

  std::stringstream ss;
  ss << "Assumed Year = " << modelObject.assumedYear();

  idfObject.setString(Output_PreprocessorMessageFields::PreprocessorName, "OpenStudio YearDescription");
  idfObject.setString(Output_PreprocessorMessageFields::ErrorSeverity, "Warning");
  idfObject.setString(Output_PreprocessorMessageFields::MessageLine1, ss.str());
 
  return idfObject;
}

} // energyplus

} // openstudio

