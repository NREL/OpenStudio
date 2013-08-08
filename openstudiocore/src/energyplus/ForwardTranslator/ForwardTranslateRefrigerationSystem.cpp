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

#include <utilities/idd/OS_Refrigeration_System_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CaseAndWalkInList_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CompressorList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idf/IdfFile.hpp>
#include <utilities/core/Logger.hpp>

#include <QFile>
#include <QTextStream>
#include <QString>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

typedef std::map<const std::string, const boost::optional<IdfFile> > FluidPropertiesMap;

typedef std::map<const openstudio::Handle, const IdfObject> ModelObjectMap;

//FluidPropertiesMap fluidPropertiesMap;

boost::optional<IdfFile> findIdfFile(QString fileName) {
  QFile file(fileName);
  bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);
  BOOST_ASSERT(opened);

  QTextStream in(&file);
  std::stringstream ss;
  ss << in.readAll().toStdString();

  return IdfFile::load(ss, IddFileType::EnergyPlus);
}

FluidPropertiesMap createFluidPropertiesMap()
{
  FluidPropertiesMap m;
  m.insert(make_pair("R11", findIdfFile(":/Resources/R11_FluidPropertiesDataSet.idf")));
  //m.insert(make_pair("R22", findIdfFile(":/Resources/R22_FluidPropertiesDataSet.idf")));
  return m;
}

FluidPropertiesMap m_fluidPropertiesMap = createFluidPropertiesMap();

IdfObject mapCreateAndRegisterIdfObject(const IddObjectType& idfObjectType, const IdfObject& idfObject, const std::string& name,
                                            std::vector<IdfObject>& m_idfObjects, ModelObjectMap& m_map) {
  // if already translated then exit
  ModelObjectMap::const_iterator objInMap = m_map.find( idfObject.handle() );
  if( objInMap != m_map.end() )
  {
    return objInMap->second;
  }

  //IdfObject idfObject(idfObjectType);
  m_idfObjects.push_back(idfObject);
  m_map.insert(std::make_pair(idfObject.handle(),idfObject));
  //if (OptionalString moName = modelObject.name()) {
  //  idfObject.setName(*moName);
  //}
  //idfObject.setName(name);
  return idfObject;
}

boost::optional<IdfObject> addFluid(const std::string& fluidType, std::vector<IdfObject>& m_idfObjects, ModelObjectMap& m_map) {

  boost::optional<IdfObject> idfObject;
  boost::optional<IdfFile> idfFile;
  FluidPropertiesMap::const_iterator objInMap = m_fluidPropertiesMap.find( fluidType );
  if( objInMap != m_fluidPropertiesMap.end() )
  {
    idfFile = boost::optional<IdfFile>(objInMap->second);
  }
  else
  {
    return boost::none;
  }

  std::vector<IdfObject> fluidObjects = idfFile->objects();

  for( std::vector<IdfObject>::iterator it = fluidObjects.begin();
     it != fluidObjects.end();
     it++ )
  {
    switch(it->iddObject().type().value())
    {
    case openstudio::IddObjectType::FluidProperties_Name :
      {
        idfObject = mapCreateAndRegisterIdfObject(openstudio::IddObjectType::FluidProperties_Name, *it, fluidType, m_idfObjects, m_map);
        break;
      }
    case openstudio::IddObjectType::FluidProperties_Temperatures :
      {
        mapCreateAndRegisterIdfObject(openstudio::IddObjectType::FluidProperties_Temperatures, *it, fluidType + "world", m_idfObjects, m_map);
        break;
      }
    case openstudio::IddObjectType::FluidProperties_Saturated :
      {
        mapCreateAndRegisterIdfObject(openstudio::IddObjectType::FluidProperties_Saturated, *it, fluidType + "world", m_idfObjects, m_map);
        break;
      }
    case openstudio::IddObjectType::FluidProperties_Superheated :
      {
        mapCreateAndRegisterIdfObject(openstudio::IddObjectType::FluidProperties_Superheated, *it, fluidType + "world", m_idfObjects, m_map);
        break;
      }
    case openstudio::IddObjectType::FluidProperties_GlycolConcentration :
      {
        mapCreateAndRegisterIdfObject(openstudio::IddObjectType::FluidProperties_GlycolConcentration, *it, fluidType + "world", m_idfObjects, m_map);
        break;
      }
    case openstudio::IddObjectType::FluidProperties_Concentration :
      {
        mapCreateAndRegisterIdfObject(openstudio::IddObjectType::FluidProperties_Concentration, *it, fluidType + "world", m_idfObjects, m_map);
        break;
      }
    //If no case statement log a warning
    default:
      {
        //LOG(Warn, "Unknown IddObjectType: '" << it->iddObject().name() << "'");
      }
    }
  }

  /*if(istringEqual(fluidType,"R11"))
  {

  }*/
  return idfObject;
}

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationSystem( RefrigerationSystem & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;
  std::string name;

// Name
  IdfObject refrigerationSystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::OS_Refrigeration_System,
                                                          modelObject);

  //m_idfObjects.push_back(refrigerationSystem);

//Refrigerated Case or Walkin or CaseAndWalkInList Name
  std::vector<RefrigerationCase> cases = modelObject.cases();
  //std::vector<RefrigerationWalkin> walkins = modelObject.walkins();

  if( !cases.empty() ) //|| !walkins.empty() )
  {
  	// Name
  	name = " Case List";
	refrigerationSystem.setString(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName, refrigerationSystem.name().get() + name);

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
      refrigerationSystem.setString(OS_Refrigeration_SystemFields::RefrigerationCondenserName,_refrigerationCondenser->name().get());
    }
  }

//Compressor or CompressorList Name
  std::vector<RefrigerationCompressor> compressors = modelObject.compressors();

  if( !compressors.empty() )
  {
  	// Name
  	name = " Compressor List";
  	refrigerationSystem.setString(OS_Refrigeration_SystemFields::CompressorListName, refrigerationSystem.name().get() + name);

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
  	refrigerationSystem.setDouble(OS_Refrigeration_SystemFields::MinimumCondensingTemperature,d.get());
  }

//Refrigeration System Working Fluid Type
  s = modelObject.refrigerationSystemWorkingFluidType();
  if (s) {
    boost::optional<IdfObject> fluidProperties = addFluid(s.get(), m_idfObjects, m_map);
    refrigerationSystem.setString(OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType,s.get());
  }

//Suction Temperature Control Type
  s = modelObject.suctionTemperatureControlType();
  if (s) {
  	refrigerationSystem.setString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType,s.get());
  }

//Mechanical Subcooler Name
  /*boost::optional<ModelObject> mechanicalSubcooler = modelObject.mechanicalSubcooler();

  if( mechanicalSubcooler )
  {
    boost::optional<IdfObject> _mechanicalSubcooler = translateAndMapModelObject(mechanicalSubcooler.get());

    if( _mechanicalSubcooler && _mechanicalSubcooler->name() )
    {
      refrigerationSystem.setString(OS_Refrigeration_SystemFields::MechanicalSubcoolerName,_mechanicalSubcooler->name().get());
    }
  }*/

//Liquid Suction Heat Exchanger Subcooler Name
  /*boost::optional<ModelObject> liquidSuctionHeatExchangerSubcooler = modelObject.liquidSuctionHeatExchangerSubcooler();

  if( liquidSuctionHeatExchangerSubcooler )
  {
    boost::optional<IdfObject> _liquidSuctionHeatExchangerSubcooler = translateAndMapModelObject(liquidSuctionHeatExchangerSubcooler.get());

    if( _liquidSuctionHeatExchangerSubcooler && _liquidSuctionHeatExchangerSubcooler->name() )
    {
      refrigerationSystem.setString(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName,_liquidSuctionHeatExchangerSubcooler->name().get());
    }
  }*/

//Sum UA Suction Piping
  d = modelObject.sumUASuctionPiping();
  if (d) {
  	refrigerationSystem.setDouble(OS_Refrigeration_SystemFields::SumUASuctionPiping,d.get());
  }

//Suction Piping Zone Name
  boost::optional<ThermalZone> suctionPipingZone = modelObject.suctionPipingZone();

  if( suctionPipingZone )
  {
    boost::optional<IdfObject> _suctionPipingZone = translateAndMapModelObject(suctionPipingZone.get());

    if( _suctionPipingZone && _suctionPipingZone->name() )
    {
      refrigerationSystem.setString(OS_Refrigeration_SystemFields::SuctionPipingZoneName,_suctionPipingZone->name().get());
    }
  }

//End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if (s) {
  	refrigerationSystem.setString(OS_Refrigeration_SystemFields::EndUseSubcategory,s.get());
  }

//Number of Compressor Stages 
  /*s = modelObject.numberofCompressorStages();
  if (s) {
  	refrigerationSystem.setString(OS_Refrigeration_SystemFields::NumberofCompressorStages,s.get());
  }*/
  refrigerationSystem.setString(OS_Refrigeration_SystemFields::NumberofCompressorStages,"1");

//Intercooler Type 
  /*s = modelObject.intercoolerType();
  if (s) {
  	refrigerationSystem.setString(OS_Refrigeration_SystemFields::IntercoolerType,s.get());
  }*/
  refrigerationSystem.setString(OS_Refrigeration_SystemFields::IntercoolerType,"None");

//Shell-and-Coil Intercooler Effectiveness 
  /*d = modelObject.shellandCoilIntercoolerEffectiveness();
  if (d) {
  	refrigerationSystem.setDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness,d.get());
  }*/
  refrigerationSystem.setDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness,0.8);

//High-Stage Compressor or CompressorList Name
  /*std::vector<ModelObject> highStageCompressorList = modelObject.highStageCompressorList();

  if( !highStageCompressorList.empty() )
  {
	refrigerationSystem.setString(OS_Refrigeration_SystemFields::HighStageCompressorListName, refrigerationSystem.name() + " High Stage Compressor List";

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