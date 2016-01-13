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
#include "../../model/RefrigerationSystem.hpp"
#include "../../model/ThermalZone.hpp"
//#include <model/RefrigerationCondenserAirCooled.hpp>
#include "../../model/RefrigerationAirChiller.hpp"
#include "../../model/RefrigerationCondenserCascade.hpp"
#include "../../model/RefrigerationCase.hpp"
#include "../../model/RefrigerationCompressor.hpp"
#include "../../model/RefrigerationSecondarySystem.hpp"
#include "../../model/RefrigerationWalkIn.hpp"
#include "../../model/RefrigerationSubcoolerLiquidSuction.hpp"
#include "../../model/RefrigerationSubcoolerMechanical.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Refrigeration_System_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CaseAndWalkInList_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CompressorList_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_TransferLoadList_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationSystem( RefrigerationSystem & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;
  std::string name;

// Name
  IdfObject refrigerationSystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_System,
                                                          modelObject);

//Refrigerated Case or Walkin or CaseAndWalkInList Name
  std::vector<RefrigerationCase> cases = modelObject.cases();
  std::vector<RefrigerationWalkIn> walkins = modelObject.walkins();
  std::vector<RefrigerationAirChiller> airChillers = modelObject.airChillers();

  if( !cases.empty() || !walkins.empty() || !airChillers.empty() )
  {
    // Name
    name = " Case and Walkin List";
    refrigerationSystem.setString(Refrigeration_SystemFields::RefrigeratedCaseorWalkinorCaseAndWalkInListName, refrigerationSystem.name().get() + name);

    IdfObject _caseAndWalkinList(IddObjectType::Refrigeration_CaseAndWalkInList);

    m_idfObjects.push_back(_caseAndWalkinList);

    _caseAndWalkinList.setName(refrigerationSystem.name().get() + name);

    for( auto & elem : cases )
    {
      boost::optional<IdfObject> _case = translateAndMapModelObject(elem);

      if( _case )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_case->name().get()); 
      }
    }

    for( auto & elem : walkins )
    {
      boost::optional<IdfObject> _walkin = translateAndMapModelObject(elem);

      if( _walkin )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_walkin->name().get()); 
      }
    }

    for( auto & elem : airChillers )
    {
      boost::optional<IdfObject> _airChiller = translateAndMapModelObject(elem);

      if( _airChiller )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_airChiller->name().get()); 
      }
    }
  }

//Refrigeration Transfer Load or TransferLoad List Name
  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = modelObject.secondarySystemLoads();
  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = modelObject.cascadeCondenserLoads();

  if( !secondarySystemLoads.empty() || !cascadeCondenserLoads.empty() )
  {
    // Name
    name = " Transfer Load List";
    refrigerationSystem.setString(Refrigeration_SystemFields::RefrigerationTransferLoadorTransferLoadListName, refrigerationSystem.name().get() + name);

    IdfObject _transferLoadList(IddObjectType::Refrigeration_TransferLoadList);

    m_idfObjects.push_back(_transferLoadList);

    _transferLoadList.setName(refrigerationSystem.name().get() + name);

    for( auto & elem : secondarySystemLoads )
    {
      boost::optional<IdfObject> _secondarySystemLoad = translateAndMapModelObject(elem);

      if( _secondarySystemLoad )
      {
        IdfExtensibleGroup eg = _transferLoadList.pushExtensibleGroup();

        eg.setString(Refrigeration_TransferLoadListExtensibleFields::CascadeCondenserNameorSecondarySystemName,_secondarySystemLoad->name().get()); 
      }
    }

    for( auto & elem : cascadeCondenserLoads )
    {
      boost::optional<IdfObject> _cascadeCondenserLoad = translateAndMapModelObject(elem);

      if( _cascadeCondenserLoad )
      {
        IdfExtensibleGroup eg = _transferLoadList.pushExtensibleGroup();

        eg.setString(Refrigeration_TransferLoadListExtensibleFields::CascadeCondenserNameorSecondarySystemName,_cascadeCondenserLoad->name().get()); 
      }
    }
  }

//Refrigeration Condenser Name
  boost::optional<ModelObject> refrigerationCondenser = modelObject.refrigerationCondenser();

  if( refrigerationCondenser )
  {
    boost::optional<IdfObject> _refrigerationCondenser = translateAndMapModelObject(refrigerationCondenser.get());

    if( _refrigerationCondenser && _refrigerationCondenser->name() )
    {
      refrigerationSystem.setString(Refrigeration_SystemFields::RefrigerationCondenserName,_refrigerationCondenser->name().get());
    }
  }

//Compressor or CompressorList Name
  std::vector<RefrigerationCompressor> compressors = modelObject.compressors();

  if( !compressors.empty() )
  {
    // Name
    name = " Compressor List";
    refrigerationSystem.setString(Refrigeration_SystemFields::CompressororCompressorListName, refrigerationSystem.name().get() + name);

    IdfObject _compressorList(IddObjectType::Refrigeration_CompressorList);

    m_idfObjects.push_back(_compressorList);

    _compressorList.setName(refrigerationSystem.name().get() + name);

    for( auto & elem : compressors )
    {
      boost::optional<IdfObject> _compressor = translateAndMapModelObject(elem);

      if( _compressor )
      {
        IdfExtensibleGroup eg = _compressorList.pushExtensibleGroup();

        eg.setString(Refrigeration_CompressorListExtensibleFields::RefrigerationCompressorName,_compressor->name().get()); 
      }
    }
  }

//Minimum Condensing Temperature
  d = modelObject.minimumCondensingTemperature();
  if (d) {
    refrigerationSystem.setDouble(Refrigeration_SystemFields::MinimumCondensingTemperature,d.get());
  }

//Refrigeration System Working Fluid Type
  s = modelObject.refrigerationSystemWorkingFluidType();
  if (s) {
    boost::optional<IdfObject> fluidProperties = createFluidProperties(s.get());
    if( fluidProperties ) {
      boost::optional<std::string> value = fluidProperties.get().getString(FluidProperties_NameFields::FluidName,true);
      if( value ) {
        refrigerationSystem.setString(Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType,value.get());
      }
    }
  }

//Suction Temperature Control Type
  s = modelObject.suctionTemperatureControlType();
  if (s) {
    refrigerationSystem.setString(Refrigeration_SystemFields::SuctionTemperatureControlType,s.get());
  }

//Mechanical Subcooler Name
  boost::optional<RefrigerationSubcoolerMechanical> mechanicalSubcooler = modelObject.mechanicalSubcooler();

  if( mechanicalSubcooler )
  {
    boost::optional<IdfObject> _mechanicalSubcooler = translateAndMapModelObject(mechanicalSubcooler.get());

    if( _mechanicalSubcooler && _mechanicalSubcooler->name() )
    {
      refrigerationSystem.setString(Refrigeration_SystemFields::MechanicalSubcoolerName,_mechanicalSubcooler->name().get());
    }
  }

//Liquid Suction Heat Exchanger Subcooler Name
  boost::optional<RefrigerationSubcoolerLiquidSuction> liquidSuctionHeatExchangerSubcooler = modelObject.liquidSuctionHeatExchangerSubcooler();

  if( liquidSuctionHeatExchangerSubcooler )
  {
    boost::optional<IdfObject> _liquidSuctionHeatExchangerSubcooler = translateAndMapModelObject(liquidSuctionHeatExchangerSubcooler.get());

    if( _liquidSuctionHeatExchangerSubcooler && _liquidSuctionHeatExchangerSubcooler->name() )
    {
      refrigerationSystem.setString(Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName,_liquidSuctionHeatExchangerSubcooler->name().get());
    }
  }

//Sum UA Suction Piping
  d = modelObject.sumUASuctionPiping();
  if (d) {
    refrigerationSystem.setDouble(Refrigeration_SystemFields::SumUASuctionPiping,d.get());
  }

//Suction Piping Zone Name
  boost::optional<ThermalZone> suctionPipingZone = modelObject.suctionPipingZone();

  if( suctionPipingZone )
  {
    boost::optional<IdfObject> _suctionPipingZone = translateAndMapModelObject(suctionPipingZone.get());

    if( _suctionPipingZone && _suctionPipingZone->name() )
    {
      refrigerationSystem.setString(Refrigeration_SystemFields::SuctionPipingZoneName,_suctionPipingZone->name().get());
    }
  }

//End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if (s) {
    refrigerationSystem.setString(Refrigeration_SystemFields::EndUseSubcategory,s.get());
  }

//Number of Compressor Stages 
  s = modelObject.numberofCompressorStages();
  if (s) {
    refrigerationSystem.setString(Refrigeration_SystemFields::NumberofCompressorStages,s.get());
  }

//Intercooler Type 
  s = modelObject.intercoolerType();
  if (s) {
    refrigerationSystem.setString(Refrigeration_SystemFields::IntercoolerType,s.get());
  }

//Shell-and-Coil Intercooler Effectiveness 
  d = modelObject.shellandCoilIntercoolerEffectiveness();
  if (d) {
    refrigerationSystem.setDouble(Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness,d.get());
  }

//High-Stage Compressor or CompressorList Name
  std::vector<RefrigerationCompressor> highStageCompressors = modelObject.highStageCompressors();

  if( !highStageCompressors.empty() )
  {
    // Name
    name = " High Stage Compressor List";
    refrigerationSystem.setString(Refrigeration_SystemFields::HighStageCompressororCompressorListName, refrigerationSystem.name().get() + name);

    IdfObject _highStageCompressorList(IddObjectType::Refrigeration_CompressorList);

    m_idfObjects.push_back(_highStageCompressorList);

    _highStageCompressorList.setName(refrigerationSystem.name().get() + name);

    for( auto & elem : highStageCompressors )
    {
      boost::optional<IdfObject> _highStageCompressor = translateAndMapModelObject(elem);

      if( _highStageCompressor )
      {
        IdfExtensibleGroup eg = _highStageCompressorList.pushExtensibleGroup();

        eg.setString(Refrigeration_CompressorListExtensibleFields::RefrigerationCompressorName,_highStageCompressor->name().get()); 
      }
    }
  }

  return refrigerationSystem;

}
}
}
