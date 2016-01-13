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
#include "../../model/RefrigerationTranscriticalSystem.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/RefrigerationCase.hpp"
#include "../../model/RefrigerationCompressor.hpp"
#include "../../model/RefrigerationGasCoolerAirCooled.hpp"
#include "../../model/RefrigerationWalkIn.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Refrigeration_TranscriticalSystem_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CaseAndWalkInList_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CompressorList_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationTranscriticalSystem( RefrigerationTranscriticalSystem & modelObject )
{
  OptionalModelObject temp;
  boost::optional<std::string> s;
  boost::optional<double> d;
  std::string name;

// Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_TranscriticalSystem, modelObject);

//MediumTemperatureRefrigeratedCaseAndWalkInListName
  std::vector<RefrigerationCase> mediumTemperatureCases = modelObject.mediumTemperatureCases();
  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins = modelObject.mediumTemperatureWalkins();

  if( !mediumTemperatureCases.empty() || !mediumTemperatureWalkins.empty() )
  {
    // Name
    name = " Medium Temperature Case and Walkin List";
    idfObject.setString(Refrigeration_TranscriticalSystemFields::MediumTemperatureRefrigeratedCaseorWalkinorCaseAndWalkInListName, idfObject.name().get() + name);

    IdfObject _caseAndWalkinList(IddObjectType::Refrigeration_CaseAndWalkInList);

    m_idfObjects.push_back(_caseAndWalkinList);

    _caseAndWalkinList.setName(idfObject.name().get() + name);

    for( auto & mediumTemperatureCase : mediumTemperatureCases )
    {
      boost::optional<IdfObject> _case = translateAndMapModelObject(mediumTemperatureCase);

      if( _case )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_case->name().get()); 
      }
    }

    for( auto & mediumTemperatureWalkin : mediumTemperatureWalkins )
    {
      boost::optional<IdfObject> _walkin = translateAndMapModelObject(mediumTemperatureWalkin);

      if( _walkin )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_walkin->name().get()); 
      }
    }
  }

//LowTemperatureRefrigeratedCaseAndWalkInListName
  std::vector<RefrigerationCase> lowTemperatureCases = modelObject.lowTemperatureCases();
  std::vector<RefrigerationWalkIn> lowTemperatureWalkins = modelObject.lowTemperatureWalkins();

  if( !lowTemperatureCases.empty() || !lowTemperatureWalkins.empty() )
  {
    // Name
    name = " Low Temperature Case and Walkin List";
    idfObject.setString(Refrigeration_TranscriticalSystemFields::LowTemperatureRefrigeratedCaseorWalkinorCaseAndWalkInListName, idfObject.name().get() + name);

    IdfObject _caseAndWalkinList(IddObjectType::Refrigeration_CaseAndWalkInList);

    m_idfObjects.push_back(_caseAndWalkinList);

    _caseAndWalkinList.setName(idfObject.name().get() + name);

    for( auto & lowTemperatureCase : lowTemperatureCases )
    {
      boost::optional<IdfObject> _case = translateAndMapModelObject(lowTemperatureCase);

      if( _case )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_case->name().get()); 
      }
    }

    for( auto & lowTemperatureWalkin : lowTemperatureWalkins )
    {
      boost::optional<IdfObject> _walkin = translateAndMapModelObject(lowTemperatureWalkin);

      if( _walkin )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_walkin->name().get()); 
      }
    }
  }

// SystemType
  if ( (!mediumTemperatureCases.empty() || !mediumTemperatureWalkins.empty()) &&
       (!lowTemperatureCases.empty() || !lowTemperatureWalkins.empty()) ) {
    idfObject.setString(Refrigeration_TranscriticalSystemFields::SystemType,"TwoStage");
  }
  else {
    idfObject.setString(Refrigeration_TranscriticalSystemFields::SystemType,"SingleStage");
  }

// RefrigerationGasCoolerName
  if( boost::optional<RefrigerationGasCoolerAirCooled> refrigerationGasCooler = modelObject.refrigerationGasCooler() )
  {
    boost::optional<IdfObject> _refrigerationGasCooler = translateAndMapModelObject(refrigerationGasCooler.get());

    if( _refrigerationGasCooler && _refrigerationGasCooler->name() )
    {
      idfObject.setString(Refrigeration_TranscriticalSystemFields::RefrigerationGasCoolerName,_refrigerationGasCooler->name().get());
    }
  }

//HighPressureCompressorListName
  std::vector<RefrigerationCompressor> highPressureCompressors = modelObject.highPressureCompressors();

  if( !highPressureCompressors.empty() )
  {
    // Name
    name = " High Pressure Compressor List";
    idfObject.setString(Refrigeration_TranscriticalSystemFields::HighPressureCompressororCompressorListName, idfObject.name().get() + name);

    IdfObject _compressorList(IddObjectType::Refrigeration_CompressorList);

    m_idfObjects.push_back(_compressorList);

    _compressorList.setName(idfObject.name().get() + name);

    for( auto & highPressureCompressor :  highPressureCompressors )
    {
      boost::optional<IdfObject> _compressor = translateAndMapModelObject(highPressureCompressor);

      if( _compressor )
      {
        IdfExtensibleGroup eg = _compressorList.pushExtensibleGroup();

        eg.setString(Refrigeration_CompressorListExtensibleFields::RefrigerationCompressorName,_compressor->name().get()); 
      }
    }
  }

//LowPressureCompressorListName
  std::vector<RefrigerationCompressor> lowPressureCompressors = modelObject.lowPressureCompressors();

  if( !lowPressureCompressors.empty() )
  {
    // Name
    name = " Low Pressure Compressor List";
    idfObject.setString(Refrigeration_TranscriticalSystemFields::LowPressureCompressororCompressorListName, idfObject.name().get() + name);

    IdfObject _compressorList(IddObjectType::Refrigeration_CompressorList);

    m_idfObjects.push_back(_compressorList);

    _compressorList.setName(idfObject.name().get() + name);

    for( auto & lowPressureCompressor : lowPressureCompressors )
    {
      boost::optional<IdfObject> _compressor = translateAndMapModelObject(lowPressureCompressor);

      if( _compressor )
      {
        IdfExtensibleGroup eg = _compressorList.pushExtensibleGroup();

        eg.setString(Refrigeration_CompressorListExtensibleFields::RefrigerationCompressorName,_compressor->name().get()); 
      }
    }
  }

// ReceiverPressure
  d = modelObject.receiverPressure();
  if (d) {
    idfObject.setDouble(Refrigeration_TranscriticalSystemFields::ReceiverPressure,d.get());
  }

// SubcoolerEffectiveness
  d = modelObject.subcoolerEffectiveness();
  if (d) {
    idfObject.setDouble(Refrigeration_TranscriticalSystemFields::SubcoolerEffectiveness,d.get());
  }

// RefrigerationSystemWorkingFluidType
  s = modelObject.refrigerationSystemWorkingFluidType();
  if (s) {
    boost::optional<IdfObject> fluidProperties = createFluidProperties(s.get());
    if( fluidProperties ) {
      boost::optional<std::string> value = fluidProperties.get().getString(FluidProperties_NameFields::FluidName,true);
      if( value ) {
        idfObject.setString(Refrigeration_TranscriticalSystemFields::RefrigerationSystemWorkingFluidType,value.get());
      }
    }
  }

// SumUASuctionPipingforMediumTemperatureLoads
  d = modelObject.sumUASuctionPipingforMediumTemperatureLoads();
  if (d) {
    idfObject.setDouble(Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforMediumTemperatureLoads,d.get());
  }

// MediumTemperatureSuctionPipingZoneName
  if( boost::optional<ThermalZone> mediumTemperatureSuctionPipingZone = modelObject.mediumTemperatureSuctionPipingZone() )
  {
    boost::optional<IdfObject> _mediumTemperatureSuctionPipingZone = translateAndMapModelObject(mediumTemperatureSuctionPipingZone.get());

    if( _mediumTemperatureSuctionPipingZone && _mediumTemperatureSuctionPipingZone->name() )
    {
      idfObject.setString(Refrigeration_TranscriticalSystemFields::MediumTemperatureSuctionPipingZoneName,_mediumTemperatureSuctionPipingZone->name().get());
    }
  }

// SumUASuctionPipingforLowTemperatureLoads
  d = modelObject.sumUASuctionPipingforLowTemperatureLoads();
  if (d) {
    idfObject.setDouble(Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforLowTemperatureLoads,d.get());
  }

// LowTemperatureSuctionPipingZoneName
  if( boost::optional<ThermalZone> lowTemperatureSuctionPipingZone = modelObject.lowTemperatureSuctionPipingZone() )
  {
    boost::optional<IdfObject> _lowTemperatureSuctionPipingZone = translateAndMapModelObject(lowTemperatureSuctionPipingZone.get());

    if( _lowTemperatureSuctionPipingZone && _lowTemperatureSuctionPipingZone->name() )
    {
      idfObject.setString(Refrigeration_TranscriticalSystemFields::LowTemperatureSuctionPipingZoneName,_lowTemperatureSuctionPipingZone->name().get());
    }
  }

// EndUseSubcategory
  s = modelObject.endUseSubcategory();
  if (s) {
    idfObject.setString(Refrigeration_TranscriticalSystemFields::EndUseSubcategory,s.get());
  }

  return idfObject;

}
}
}
