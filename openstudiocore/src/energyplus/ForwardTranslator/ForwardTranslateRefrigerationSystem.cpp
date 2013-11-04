/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/RefrigerationSystem.hpp>
#include <model/ThermalZone.hpp>
//#include <model/RefrigerationCondenserAirCooled.hpp>
#include <model/RefrigerationCase.hpp>
#include <model/RefrigerationCompressor.hpp>
//#include <model/RefrigerationWalkin.hpp>
//#include <model/RefrigerationSubcooler.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <utilities/idd/Refrigeration_System_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CaseAndWalkInList_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CompressorList_FieldEnums.hxx>
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
  //std::vector<RefrigerationWalkin> walkins = modelObject.walkins();

  if( !cases.empty() ) //|| !walkins.empty() )
  {
    // Name
    name = " Case List";
    refrigerationSystem.setString(Refrigeration_SystemFields::RefrigeratedCaseorWalkinorCaseAndWalkInListName, refrigerationSystem.name().get() + name);

    IdfObject _caseAndWalkinList(IddObjectType::Refrigeration_CaseAndWalkInList);

    m_idfObjects.push_back(_caseAndWalkinList);

    _caseAndWalkinList.setName(refrigerationSystem.name().get() + name);

    for( std::vector<RefrigerationCase>::iterator it = cases.begin();
       it != cases.end();
       it++ )
    {
      boost::optional<IdfObject> _case = translateAndMapModelObject(*it);

      if( _case )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_case->name().get()); 
      }
    }

    /*for( std::vector<RefrigerationWalkin>::iterator it = walkins.begin();
       it != walkins.end();
       it++ )
    {
      boost::optional<IdfObject> _walkin = translateAndMapModelObject(*it);

      if( _walkin )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_walkin->name().get()); 
      }
    }*/
  }

//Refrigeration Transfer Load or TransferLoad List Name


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

    for( std::vector<RefrigerationCompressor>::iterator it = compressors.begin();
       it != compressors.end();
       it++ )
    {
      boost::optional<IdfObject> _compressor = translateAndMapModelObject(*it);

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
    refrigerationSystem.setString(Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType,s.get());
  }

//Suction Temperature Control Type
  s = modelObject.suctionTemperatureControlType();
  if (s) {
    refrigerationSystem.setString(Refrigeration_SystemFields::SuctionTemperatureControlType,s.get());
  }

//Mechanical Subcooler Name
  /*boost::optional<ModelObject> mechanicalSubcooler = modelObject.mechanicalSubcooler();

  if( mechanicalSubcooler )
  {
    boost::optional<IdfObject> _mechanicalSubcooler = translateAndMapModelObject(mechanicalSubcooler.get());

    if( _mechanicalSubcooler && _mechanicalSubcooler->name() )
    {
      refrigerationSystem.setString(Refrigeration_SystemFields::MechanicalSubcoolerName,_mechanicalSubcooler->name().get());
    }
  }*/

//Liquid Suction Heat Exchanger Subcooler Name
  /*boost::optional<ModelObject> liquidSuctionHeatExchangerSubcooler = modelObject.liquidSuctionHeatExchangerSubcooler();

  if( liquidSuctionHeatExchangerSubcooler )
  {
    boost::optional<IdfObject> _liquidSuctionHeatExchangerSubcooler = translateAndMapModelObject(liquidSuctionHeatExchangerSubcooler.get());

    if( _liquidSuctionHeatExchangerSubcooler && _liquidSuctionHeatExchangerSubcooler->name() )
    {
      refrigerationSystem.setString(Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName,_liquidSuctionHeatExchangerSubcooler->name().get());
    }
  }*/

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
  /*s = modelObject.numberofCompressorStages();
  if (s) {
    refrigerationSystem.setString(Refrigeration_SystemFields::NumberofCompressorStages,s.get());
  }*/
  refrigerationSystem.setString(Refrigeration_SystemFields::NumberofCompressorStages,"1");

//Intercooler Type 
  /*s = modelObject.intercoolerType();
  if (s) {
    refrigerationSystem.setString(Refrigeration_SystemFields::IntercoolerType,s.get());
  }*/
  refrigerationSystem.setString(Refrigeration_SystemFields::IntercoolerType,"None");

//Shell-and-Coil Intercooler Effectiveness 
  /*d = modelObject.shellandCoilIntercoolerEffectiveness();
  if (d) {
    refrigerationSystem.setDouble(Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness,d.get());
  }*/
  refrigerationSystem.setDouble(Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness,0.8);

//High-Stage Compressor or CompressorList Name
  /*std::vector<ModelObject> highStageCompressorList = modelObject.highStageCompressorList();

  if( !highStageCompressorList.empty() )
  {
    refrigerationSystem.setString(Refrigeration_SystemFields::HighStageCompressororCompressorListName, refrigerationSystem.name() + " High Stage Compressor List";

    IdfObject _highStageCompressorListList(IddObjectType::Refrigeration_CompressorList);

    m_idfObjects.push_back(_highStageCompressorListList);

    _highStageCompressorListList.setName(refrigerationSystem.name() + " High Stage Compressor List");

    for( std::vector<ModelObject>::iterator it = highStageCompressorList.begin();
       it != highStageCompressorList.end();
       it++ )
    {
      boost::optional<IdfObject> _compressor = translateAndMapModelObject(*it);

      if( _compressor )
      {
        IdfExtensibleGroup eg = _highStageCompressorListList.pushExtensibleGroup();

        eg.setString(Refrigeration_CompressorListExtensibleFields::RefrigerationCompressorName,_compressor->name().get()); 
      }
    }
  }*/

  return refrigerationSystem;

}
}
}
