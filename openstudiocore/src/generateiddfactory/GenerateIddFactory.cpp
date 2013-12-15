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

#include <generateiddfactory/GenerateIddFactory.hpp>

#include <boost/foreach.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <exception>

namespace openstudio {

IddFileFactoryDataVector constructIddFileObjects(const std::vector<std::string>& iddArgs) {
  IddFileFactoryDataVector result;
  // Construct one IddFileFactoryData object per Idd file input by the user. Constructor will 
  // throw if it is unable to parse iddArg or is unable to validate the path or name.
  BOOST_FOREACH(const std::string& iddArg,iddArgs) {
    result.push_back(IddFileFactoryData(iddArg));
  }
  return result;
}

void initializeOutFiles(GenerateIddFactoryOutFiles& outFiles,
                        const std::vector<IddFileFactoryData>& iddFiles) 
{
  std::stringstream ss;

  // start IddEnums.hxx
  outFiles.iddEnumsHxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDENUMS_HXX" << std::endl
    << "#define UTILITIES_IDD_IDDENUMS_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "#include <boost/optional.hpp>" << std::endl
    << std::endl
    << "#include <vector>" << std::endl
    << "#include <set>" << std::endl
    << std::endl
    << "#include <QMetaType>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl;

  // start IddFieldEnums.hxx
  outFiles.iddFieldEnumsHxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDFIELDENUMS_HXX" << std::endl
    << "#define UTILITIES_IDD_IDDFIELDENUMS_HXX" << std::endl
    << std::endl
    << "/** \\file IddFieldEnums.hxx" << std::endl
    << " *  IddFieldEnums.hxx includes one file per IddObjectType (excluding Catchall, UserCustom, " << std::endl
    << " *  and CommentOnly). Each file so included contains one or two OPENSTUDIO_ENUM macros that " << std::endl
    << " *  provide enum and string access to the non-extensible fields and extensible groups of "  << std::endl
    << " *  the corresponding IddObjects. This file should not be included directly, but rather, " << std::endl
    << " *  used as a reference to help locate the individual field enum files that should be " << std::endl
    << " *  included in a particular source file. */" << std::endl
    << std::endl;

  // start IddFieldEnums.ixx
  outFiles.iddFieldEnumsIxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDFIELDENUMS_IXX" << std::endl
    << "#define UTILITIES_IDD_IDDFIELDENUMS_IXX" << std::endl
    << std::endl;

  // write IddFactory.hxx
  outFiles.iddFactoryHxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDFACTORY_HXX" << std::endl
    << "#define UTILITIES_IDD_IDDFACTORY_HXX" << std::endl
    << std::endl
    << "#include <utilities/UtilitiesAPI.hpp>" << std::endl
    << std::endl
    << "#include <utilities/idd/IddObject.hpp>" << std::endl
    << "#include <utilities/idd/IddFile.hpp>" << std::endl
    << "#include <utilities/idd/IddEnums.hxx>" << std::endl
    << std::endl
    << "#include <utilities/core/Singleton.hpp>" << std::endl
    << "#include <utilities/core/Compare.hpp>" << std::endl
    << "#include <utilities/core/Logger.hpp>" << std::endl
    << "#include <QMutex>" << std::endl
    << std::endl
    << "#include <map>" << std::endl
    << std::endl
    << "namespace openstudio{" << std::endl
    << std::endl
    << "/** IddFactorySingleton can return \\link IddObject IddObjects\\endlink and \\link IddFile" << std::endl
    << " *  IddFiles\\endlink for the current version of EnergyPlus and OpenStudio. It can return \\link IddFile " << std::endl
    << " *  IddFiles\\endlink only (no link with other methods in this class, for instance, no " << std::endl
    << " *  by-IddObjectType access to individual \\link IddObject IddObjects\\endlink) for " << std::endl
    << " *  previous versions of OpenStudio, back to version 0.7.0. It also supports the default IddObject " << std::endl
    << " *  type, Catchall, which is provided to enable the display of IdfObjects with misspelled type " << std::endl
    << " *  names, and a CommentOnly object. Its primary function is to establish a single set of IDD schema " << std::endl
    << " *  across all of OpenStudio, thereby ensuring consistency and reducing file I-O. " << std::endl
    << " *  " << std::endl
    << " *  IddFactorySingleton should be used through the IddFactory typedef as in" << std::endl
    << " *  \\code" << std::endl
    << " *  IddFile osIddFile = IddFactory::instance().iddFile(IddFileType::OpenStudio);" << std::endl
    << " *  \\endcode */" << std::endl
    << "class UTILITIES_API IddFactorySingleton  {" << std::endl
    << std::endl
    << "  friend class Singleton<IddFactorySingleton>;" << std::endl
    << std::endl
    << " public:" << std::endl
    << "  /** @name Getters */" << std::endl
    << "  //@{ " << std::endl
    << std::endl
    << "  /** Get the file version. Throws if fileType == IddFileType::UserCustom or " << std::endl
    << "   *  IddFileType::WholeFactory. */" << std::endl
    << "  std::string getVersion(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Get the file header. Throws if fileType == IddFileType::UserCustom or " << std::endl
    << "   *  IddFileType::WholeFactory. */" << std::endl
    << "  std::string getHeader(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink registered in factory. */" << std::endl
    << "  std::vector<IddObject> objects() const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink in IddFileType type. */" << std::endl
    << "  std::vector<IddObject> getObjects(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all groups in the factory (e.g. \"\" and \"Simulation Parameters\"). */" << std::endl
    << "  std::vector<std::string> groups() const;" << std::endl
    << std::endl
    << "  /** Return all groups in fileType. */" << std::endl
    << "  std::vector<std::string> getGroups(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink in group (e.g. \"Simulation Parameters\"). */" << std::endl
    << "  std::vector<IddObject> getObjectsInGroup(const std::string& group) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink in group and fileType. */" << std::endl
    << "  std::vector<IddObject> getObjectsInGroup(const std::string& group, IddFileType fileType) const;" << std::endl
    << std::endl
    //<< "  /** Return all \\link IddObject IddObjects\\endlink that match objectRegex. */" << std::endl
    //<< "  std::vector<IddObject> getObjects(const boost::regex& objectRegex) const;" << std::endl
    << std::endl
    //<< "  /** Return all \\link IddObject IddObjects\\endlink that match objectRegex and are in fileType. */" << std::endl
    //<< "  std::vector<IddObject> getObjects(const boost::regex& objectRegex, IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Returns the version IddObject for fileType. Throws if fileType == IddFileType::UserCustom " << std::endl
    << "   *  or IddFileType::WholeFactory. */" << std::endl
    << "  IddObject getVersionObject(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return the IddObject with .name() objectName, if it exists in the factory. */" << std::endl
    << "  boost::optional<IddObject> getObject(const std::string& objectName) const;" << std::endl
    << std::endl
    << "  /** Return the IddObject corresponding to objectType. Returns false if type == " << std::endl
    << "   *  iddobjectname::UserCustom. */" << std::endl
    << "  boost::optional<IddObject> getObject(IddObjectType objectType) const;" << std::endl
    << std::endl
    << "  /** Return all objects in the factory that are required. */" << std::endl
    << "  std::vector<IddObject> requiredObjects() const;" << std::endl
    << std::endl
    << "  /** Return all objects in IddFile fileType that are required. */" << std::endl
    << "  std::vector<IddObject> getRequiredObjects(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all objects in the factory that are unique. */" << std::endl
    << "  std::vector<IddObject> uniqueObjects() const;" << std::endl
    << std::endl
    << "  /** Return all objects in IddFile fileType that are unique. */" << std::endl
    << "  std::vector<IddObject> getUniqueObjects(IddFileType fileType) const;" << std::endl    
    << std::endl
    << "  /** Return the (current) IddFile corresponding to type. */" << std::endl
    << "  IddFile getIddFile(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return the IddFile corresponding to type and version. Unless version is equal to " << std::endl
    << "   *  VersionString(getVersion(fileType)), files are loaded from disk and returned as " << std::endl
    << "   *  IddFileType::UserCustom, to distinguish them from the IDD information wrapped by " << std::endl
    << "   *  the IddFactory. At this time, only IddFileType::OpenStudio files are supported, " << std::endl
    << "   *  and only for valid version identifiers >= VersionString(\"0.7.0\"). Returns false " << std::endl
    << "   *  in all other cases. */" << std::endl
    << "  boost::optional<IddFile> getIddFile(IddFileType fileType, const VersionString& version) const;" << std::endl
    << std::endl
    << "  //@}" << std::endl
    << "  /** @name Queries */" << std::endl
    << "  //@{" << std::endl
    << std::endl
    << "  /** Returns true if IddObject of objectType belongs to IddFile of fileType. " << std::endl
    << "   *  iddobjectname::Catchall is in no \\link IddFile IddFiles\\endlink; " << std::endl
    << "   *  iddobjectname::CommentOnly is in all \\link IddFile IddFiles\\endlink. */" << std::endl
    << "  bool isInFile(IddObjectType objectType, IddFileType fileType) const;" << std::endl
    << std::endl
    << "  //@}" << std::endl
    << " private:" << std::endl
    << std::endl
    << "  IddFactorySingleton();" << std::endl;
  BOOST_FOREACH(const IddFileFactoryData& iddFile,iddFiles) {
    outFiles.iddFactoryHxx.tempFile
      << "  void register" << iddFile.fileName() << "ObjectsInCallbackMap();" << std::endl;
  }
  outFiles.iddFactoryHxx.tempFile
    << std::endl
    << "  REGISTER_LOGGER(\"utilities.idd.IddFactory\");" << std::endl
    << std::endl
    << "  typedef boost::function<IddObject ()> CreateIddObjectCallback;" << std::endl
    << "  typedef std::map<IddObjectType,CreateIddObjectCallback> IddObjectCallbackMap;" << std::endl
    << "  IddObjectCallbackMap m_callbackMap;" << std::endl
    << "  mutable QMutex m_callbackmutex;" << std::endl
    << std::endl
    << "  typedef std::multimap<IddObjectType,IddFileType> IddObjectSourceFileMap;" << std::endl
    << "  IddObjectSourceFileMap m_sourceFileMap;" << std::endl
    << std::endl
    << "  mutable std::map<VersionString,IddFile> m_osIddFiles;" << std::endl
    << "};" << std::endl
    << std::endl
    << "#if _WIN32 || _MSC_VER" << std::endl
    << "  // Explicitly instantiate and export IddFactorySingleton Singleton template instance" << std::endl
    << "  // so that the same instance is shared between DLLs that link to Utilities.dll." << std::endl
    << "  UTILITIES_TEMPLATE_EXT template class UTILITIES_API openstudio::Singleton<IddFactorySingleton>;" << std::endl
    << "#endif" << std::endl
    << std::endl
    << "/** Convenience typedef for accessing IddFactorySingleton. Usage:" << std::endl
    << " *  \\code" << std::endl
    << " *  unsigned n = IddFactory::instance().objects().size();" << std::endl
    << " *  \\endcode" << std::endl
    << " *  " << std::endl
    << " *  \\relates IddFactorySingleton */" << std::endl
    << "typedef openstudio::Singleton<IddFactorySingleton> IddFactory;" << std::endl
    << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif //UTILITIES_IDD_IDDFACTORY_HXX" << std::endl;

  // start IddFactory.cxx
  outFiles.iddFactoryCxx.tempFile
    << "#include <utilities/idd/IddFactory.hxx>" << std::endl
    << "#include <utilities/idd/IddRegex.hpp>" << std::endl
    << std::endl
    << "#include <utilities/core/Assert.hpp>" << std::endl
    << "#include <utilities/core/Compare.hpp>" << std::endl
    << "#include <utilities/core/Containers.hpp>" << std::endl
    << "#include <utilities/core/ApplicationPathHelpers.hpp>" << std::endl
    << std::endl
    << "#include <OpenStudio.hxx>" << std::endl
    << std::endl
    << "#include <boost/bind.hpp>" << std::endl
    << "#include <boost/foreach.hpp>" << std::endl
    << std::endl
    << "#include <QMutexLocker>" << std::endl
    << "#include <QMetaType>" << std::endl
    << std::endl
    << "int _IddObjectType_id = qRegisterMetaType<openstudio::IddObjectType>(\"openstudio::IddObjectType\");" << std::endl
    << "int _IddFileType_id = qRegisterMetaType<openstudio::IddFileType>(\"openstudio::IddFileType\");" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << std::endl
    << "IddObject createCatchallIddObject() {" << std::endl
    << std::endl
    << "  static IddObject object;" << std::endl
    << std::endl
    << "  // Catchall is the type of IddObject returned by the default constructor." << std::endl
    << "  OS_ASSERT(object.type() == iddobjectname::Catchall);" << std::endl
    << "  return object;" << std::endl
    << "}" << std::endl;

  // start other IddFactory cxx files
  BOOST_FOREACH(boost::shared_ptr<IddFactoryOutFile>& cxxFile,outFiles.iddFactoryIddFileCxxs) {
    cxxFile->tempFile
      << "#include <utilities/idd/IddFactory.hxx>" << std::endl
      << std::endl
      << "#include <utilities/core/Assert.hpp>" << std::endl
      << "#include <utilities/core/Compare.hpp>" << std::endl
      << std::endl
      << "#include <boost/bind.hpp>" << std::endl
      << "#include <boost/foreach.hpp>" << std::endl
      << "#include <QMutexLocker>" << std::endl
      << std::endl
      << "namespace openstudio {" << std::endl;
  }

  std::cout << "IddFactory files initialized." << std::endl << std::endl;
}

void completeOutFiles(const IddFileFactoryDataVector& iddFiles,
                      GenerateIddFactoryOutFiles& outFiles) {

  // calculate total number of objects for purpose of splitting up IddObjectType OPENSTUDIO_ENUM
  unsigned numObjects = 3; // Catchall, UserCustom, CommentOnly
  BOOST_FOREACH(const IddFileFactoryData& idd,iddFiles) {
    numObjects += idd.objectNames().size();
  }

  std::stringstream tempSS;

  // complete and close IddEnums.hxx
  tempSS
    << "OPENSTUDIO_ENUM( IddFileType," << std::endl
    << "  ((UserCustom))" << std::endl
    << "  ((WholeFactory))";
  BOOST_FOREACH(const IddFileFactoryData& idd,iddFiles) {
    tempSS
      << std::endl
      << "  ((" << idd.fileName() << "))";
  }
  tempSS << " );" << std::endl;
  outFiles.iddEnumsHxx.tempFile
    << std::endl
    << "/** \\class IddFileType" << std::endl
    << " *  \\brief Enumeration of the types of \\link openstudio::IddFile IddFile\\endlink available" << std::endl
    << " *  through OpenStudio." << std::endl
    << " *  \\details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro" << std::endl
    << " *  call is:" << std::endl
    << " *" << std::endl
    << " *  \\code" << std::endl
    << tempSS.str()
    << " *  \\endcode" << std::endl
    << " *" << std::endl
    << " *  UserCustom \\link openstudio::IddFile IddFiles\\endlink are loaded directly from disk, and" << std::endl 
    << " *  typically correspond to old or under-development versions of EnergyPlus or OpenStudio. The" << std::endl
    << " *  rest of the enumeration values designate subsets of the \\link openstudio::IddFactorySingleton" << std::endl
    << " *  IddFactory\\endlink (the current versions of the EnergyPlus and OpenStudio IDDs, and all" << std::endl
    << " *  objects in the factory). */" << std::endl
    << tempSS.str();
  tempSS.str("");

  //// write IddObjectType enum. is very large, so split into 7 groups.
  //unsigned groupSize = static_cast<unsigned>(std::ceil(static_cast<double>(numObjects)/7.0));
  //unsigned n = 2; // number of objects written so far--will start with Catchall and UserCustom
  //tempSS
  //  << "OPENSTUDIO_ENUM7( IddObjectType," << std::endl
  //  << "  ((Catchall))" << std::endl
  //  << "  ((UserCustom))" << std::endl;
  //// loop through each IDD file
  //BOOST_FOREACH(const IddFileFactoryData& idd,iddFiles) {
  //  // write out an IddObjectType enum value for each object in the IDD file
  //  BOOST_FOREACH(const StringPair& objectName,idd.objectNames()) {
  //    // splits the enum values into seven groups
  //    if (n % groupSize == 0) {
  //      tempSS << "  ," << std::endl;
  //    }
  //    // writes the enum value (name and description)
  //    tempSS
  //      << "  ((" << objectName.first << ")(" << objectName.second << "))" << std::endl;
  //    ++n;
  //  }
  //}
  //tempSS
  //  << "  ((CommentOnly)) );" << std::endl;

  tempSS
  << "  static const IddObjectType UserCustom;" << std::endl
  << "  static const IddObjectType Catchall;" << std::endl
  << "  static const IddObjectType CommentOnly;" << std::endl
  << "  static const IddObjectType LeadInput;" << std::endl
  << "  static const IddObjectType EndLeadInput;" << std::endl
  << "  static const IddObjectType SimulationData;" << std::endl
  << "  static const IddObjectType EndSimulationData;" << std::endl
  << "  static const IddObjectType Version;" << std::endl
  << "  static const IddObjectType SimulationControl;" << std::endl
  << "  static const IddObjectType Building;" << std::endl
  << "  static const IddObjectType ShadowCalculation;" << std::endl
  << "  static const IddObjectType SurfaceConvectionAlgorithm_Inside;" << std::endl
  << "  static const IddObjectType SurfaceConvectionAlgorithm_Outside;" << std::endl
  << "  static const IddObjectType HeatBalanceAlgorithm;" << std::endl
  << "  static const IddObjectType HeatBalanceSettings_ConductionFiniteDifference;" << std::endl
  << "  static const IddObjectType ZoneAirHeatBalanceAlgorithm;" << std::endl
  << "  static const IddObjectType ZoneAirContaminantBalance;" << std::endl
  << "  static const IddObjectType ZoneCapacitanceMultiplier_ResearchSpecial;" << std::endl
  << "  static const IddObjectType Timestep;" << std::endl
  << "  static const IddObjectType ConvergenceLimits;" << std::endl
  << "  static const IddObjectType ProgramControl;" << std::endl
  << "  static const IddObjectType Compliance_Building;" << std::endl
  << "  static const IddObjectType Site_Location;" << std::endl
  << "  static const IddObjectType SizingPeriod_DesignDay;" << std::endl
  << "  static const IddObjectType SizingPeriod_WeatherFileDays;" << std::endl
  << "  static const IddObjectType SizingPeriod_WeatherFileConditionType;" << std::endl
  << "  static const IddObjectType RunPeriod;" << std::endl
  << "  static const IddObjectType RunPeriod_CustomRange;" << std::endl
  << "  static const IddObjectType RunPeriodControl_SpecialDays;" << std::endl
  << "  static const IddObjectType RunPeriodControl_DaylightSavingTime;" << std::endl
  << "  static const IddObjectType WeatherProperty_SkyTemperature;" << std::endl
  << "  static const IddObjectType Site_WeatherStation;" << std::endl
  << "  static const IddObjectType Site_HeightVariation;" << std::endl
  << "  static const IddObjectType Site_GroundTemperature_BuildingSurface;" << std::endl
  << "  static const IddObjectType Site_GroundTemperature_FCfactorMethod;" << std::endl
  << "  static const IddObjectType Site_GroundTemperature_Shallow;" << std::endl
  << "  static const IddObjectType Site_GroundTemperature_Deep;" << std::endl
  << "  static const IddObjectType Site_GroundReflectance;" << std::endl
  << "  static const IddObjectType Site_GroundReflectance_SnowModifier;" << std::endl
  << "  static const IddObjectType Site_WaterMainsTemperature;" << std::endl
  << "  static const IddObjectType Site_Precipitation;" << std::endl
  << "  static const IddObjectType RoofIrrigation;" << std::endl
  << "  static const IddObjectType ScheduleTypeLimits;" << std::endl
  << "  static const IddObjectType Schedule_Day_Hourly;" << std::endl
  << "  static const IddObjectType Schedule_Day_Interval;" << std::endl
  << "  static const IddObjectType Schedule_Day_List;" << std::endl
  << "  static const IddObjectType Schedule_Week_Daily;" << std::endl
  << "  static const IddObjectType Schedule_Week_Compact;" << std::endl
  << "  static const IddObjectType Schedule_Year;" << std::endl
  << "  static const IddObjectType Schedule_Compact;" << std::endl
  << "  static const IddObjectType Schedule_Constant;" << std::endl
  << "  static const IddObjectType Schedule_File;" << std::endl
  << "  static const IddObjectType Material;" << std::endl
  << "  static const IddObjectType Material_NoMass;" << std::endl
  << "  static const IddObjectType Material_InfraredTransparent;" << std::endl
  << "  static const IddObjectType Material_AirGap;" << std::endl
  << "  static const IddObjectType Material_RoofVegetation;" << std::endl
  << "  static const IddObjectType WindowMaterial_SimpleGlazingSystem;" << std::endl
  << "  static const IddObjectType WindowMaterial_Glazing;" << std::endl
  << "  static const IddObjectType WindowMaterial_GlazingGroup_Thermochromic;" << std::endl
  << "  static const IddObjectType WindowMaterial_Glazing_RefractionExtinctionMethod;" << std::endl
  << "  static const IddObjectType WindowMaterial_Gas;" << std::endl
  << "  static const IddObjectType WindowGap_SupportPillar;" << std::endl
  << "  static const IddObjectType WindowGap_DeflectionState;" << std::endl
  << "  static const IddObjectType WindowMaterial_GasMixture;" << std::endl
  << "  static const IddObjectType WindowMaterial_Gap;" << std::endl
  << "  static const IddObjectType WindowMaterial_Shade;" << std::endl
  << "  static const IddObjectType WindowMaterial_ComplexShade;" << std::endl
  << "  static const IddObjectType WindowMaterial_Blind;" << std::endl
  << "  static const IddObjectType WindowMaterial_Screen;" << std::endl
  << "  static const IddObjectType MaterialProperty_MoisturePenetrationDepth_Settings;" << std::endl
  << "  static const IddObjectType MaterialProperty_PhaseChange;" << std::endl
  << "  static const IddObjectType MaterialProperty_VariableThermalConductivity;" << std::endl
  << "  static const IddObjectType MaterialProperty_HeatAndMoistureTransfer_Settings;" << std::endl
  << "  static const IddObjectType MaterialProperty_HeatAndMoistureTransfer_SorptionIsotherm;" << std::endl
  << "  static const IddObjectType MaterialProperty_HeatAndMoistureTransfer_Suction;" << std::endl
  << "  static const IddObjectType MaterialProperty_HeatAndMoistureTransfer_Redistribution;" << std::endl
  << "  static const IddObjectType MaterialProperty_HeatAndMoistureTransfer_Diffusion;" << std::endl
  << "  static const IddObjectType MaterialProperty_HeatAndMoistureTransfer_ThermalConductivity;" << std::endl
  << "  static const IddObjectType MaterialProperty_GlazingSpectralData;" << std::endl
  << "  static const IddObjectType Construction;" << std::endl
  << "  static const IddObjectType Construction_CfactorUndergroundWall;" << std::endl
  << "  static const IddObjectType Construction_FfactorGroundFloor;" << std::endl
  << "  static const IddObjectType Construction_InternalSource;" << std::endl
  << "  static const IddObjectType WindowThermalModel_Params;" << std::endl
  << "  static const IddObjectType Construction_ComplexFenestrationState;" << std::endl
  << "  static const IddObjectType Construction_WindowDataFile;" << std::endl
  << "  static const IddObjectType GlobalGeometryRules;" << std::endl
  << "  static const IddObjectType GeometryTransform;" << std::endl
  << "  static const IddObjectType Zone;" << std::endl
  << "  static const IddObjectType ZoneList;" << std::endl
  << "  static const IddObjectType ZoneGroup;" << std::endl
  << "  static const IddObjectType BuildingSurface_Detailed;" << std::endl
  << "  static const IddObjectType Wall_Detailed;" << std::endl
  << "  static const IddObjectType RoofCeiling_Detailed;" << std::endl
  << "  static const IddObjectType Floor_Detailed;" << std::endl
  << "  static const IddObjectType Wall_Exterior;" << std::endl
  << "  static const IddObjectType Wall_Adiabatic;" << std::endl
  << "  static const IddObjectType Wall_Underground;" << std::endl
  << "  static const IddObjectType Wall_Interzone;" << std::endl
  << "  static const IddObjectType Roof;" << std::endl
  << "  static const IddObjectType Ceiling_Adiabatic;" << std::endl
  << "  static const IddObjectType Ceiling_Interzone;" << std::endl
  << "  static const IddObjectType Floor_GroundContact;" << std::endl
  << "  static const IddObjectType Floor_Adiabatic;" << std::endl
  << "  static const IddObjectType Floor_Interzone;" << std::endl
  << "  static const IddObjectType FenestrationSurface_Detailed;" << std::endl
  << "  static const IddObjectType Window;" << std::endl
  << "  static const IddObjectType Door;" << std::endl
  << "  static const IddObjectType GlazedDoor;" << std::endl
  << "  static const IddObjectType Window_Interzone;" << std::endl
  << "  static const IddObjectType Door_Interzone;" << std::endl
  << "  static const IddObjectType GlazedDoor_Interzone;" << std::endl
  << "  static const IddObjectType WindowProperty_ShadingControl;" << std::endl
  << "  static const IddObjectType WindowProperty_FrameAndDivider;" << std::endl
  << "  static const IddObjectType WindowProperty_AirflowControl;" << std::endl
  << "  static const IddObjectType WindowProperty_StormWindow;" << std::endl
  << "  static const IddObjectType InternalMass;" << std::endl
  << "  static const IddObjectType Shading_Site;" << std::endl
  << "  static const IddObjectType Shading_Building;" << std::endl
  << "  static const IddObjectType Shading_Site_Detailed;" << std::endl
  << "  static const IddObjectType Shading_Building_Detailed;" << std::endl
  << "  static const IddObjectType Shading_Overhang;" << std::endl
  << "  static const IddObjectType Shading_Overhang_Projection;" << std::endl
  << "  static const IddObjectType Shading_Fin;" << std::endl
  << "  static const IddObjectType Shading_Fin_Projection;" << std::endl
  << "  static const IddObjectType Shading_Zone_Detailed;" << std::endl
  << "  static const IddObjectType ShadingProperty_Reflectance;" << std::endl
  << "  static const IddObjectType SurfaceProperty_HeatTransferAlgorithm;" << std::endl
  << "  static const IddObjectType SurfaceProperty_HeatTransferAlgorithm_MultipleSurface;" << std::endl
  << "  static const IddObjectType SurfaceProperty_HeatTransferAlgorithm_SurfaceList;" << std::endl
  << "  static const IddObjectType SurfaceProperty_HeatTransferAlgorithm_Construction;" << std::endl
  << "  static const IddObjectType SurfaceControl_MovableInsulation;" << std::endl
  << "  static const IddObjectType SurfaceProperty_OtherSideCoefficients;" << std::endl
  << "  static const IddObjectType SurfaceProperty_OtherSideConditionsModel;" << std::endl
  << "  static const IddObjectType SurfaceConvectionAlgorithm_Inside_AdaptiveModelSelections;" << std::endl
  << "  static const IddObjectType SurfaceConvectionAlgorithm_Outside_AdaptiveModelSelections;" << std::endl
  << "  static const IddObjectType SurfaceConvectionAlgorithm_Inside_UserCurve;" << std::endl
  << "  static const IddObjectType SurfaceConvectionAlgorithm_Outside_UserCurve;" << std::endl
  << "  static const IddObjectType SurfaceProperty_ConvectionCoefficients;" << std::endl
  << "  static const IddObjectType SurfaceProperty_ConvectionCoefficients_MultipleSurface;" << std::endl
  << "  static const IddObjectType SurfaceProperties_VaporCoefficients;" << std::endl
  << "  static const IddObjectType SurfaceProperty_ExteriorNaturalVentedCavity;" << std::endl
  << "  static const IddObjectType ZoneProperty_UserViewFactors_bySurfaceName;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Control;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_Materials;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_MatlProps;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_BoundConds;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_BldgProps;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_Insulation;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_EquivalentSlab;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_AutoGrid;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_ManualGrid;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_XFACE;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_YFACE;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Slab_ZFACE;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_SimParameters;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_MatlProps;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_Insulation;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_SurfaceProps;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_BldgData;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_Interior;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_ComBldg;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_EquivSlab;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_EquivAutoGrid;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_AutoGrid;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_ManualGrid;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_XFACE;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_YFACE;" << std::endl
  << "  static const IddObjectType GroundHeatTransfer_Basement_ZFACE;" << std::endl
  << "  static const IddObjectType RoomAirModelType;" << std::endl
  << "  static const IddObjectType RoomAir_TemperaturePattern_UserDefined;" << std::endl
  << "  static const IddObjectType RoomAir_TemperaturePattern_ConstantGradient;" << std::endl
  << "  static const IddObjectType RoomAir_TemperaturePattern_TwoGradient;" << std::endl
  << "  static const IddObjectType RoomAir_TemperaturePattern_NondimensionalHeight;" << std::endl
  << "  static const IddObjectType RoomAir_TemperaturePattern_SurfaceMapping;" << std::endl
  << "  static const IddObjectType RoomAir_Node;" << std::endl
  << "  static const IddObjectType RoomAirSettings_OneNodeDisplacementVentilation;" << std::endl
  << "  static const IddObjectType RoomAirSettings_ThreeNodeDisplacementVentilation;" << std::endl
  << "  static const IddObjectType RoomAirSettings_CrossVentilation;" << std::endl
  << "  static const IddObjectType RoomAirSettings_UnderFloorAirDistributionInterior;" << std::endl
  << "  static const IddObjectType RoomAirSettings_UnderFloorAirDistributionExterior;" << std::endl
  << "  static const IddObjectType People;" << std::endl
  << "  static const IddObjectType ComfortViewFactorAngles;" << std::endl
  << "  static const IddObjectType Lights;" << std::endl
  << "  static const IddObjectType ElectricEquipment;" << std::endl
  << "  static const IddObjectType GasEquipment;" << std::endl
  << "  static const IddObjectType HotWaterEquipment;" << std::endl
  << "  static const IddObjectType SteamEquipment;" << std::endl
  << "  static const IddObjectType OtherEquipment;" << std::endl
  << "  static const IddObjectType ZoneBaseboard_OutdoorTemperatureControlled;" << std::endl
  << "  static const IddObjectType ZoneContaminantSourceAndSink_CarbonDioxide;" << std::endl
  << "  static const IddObjectType ZoneContaminantSourceAndSink_Generic_Constant;" << std::endl
  << "  static const IddObjectType SurfaceContaminantSourceAndSink_Generic_PressureDriven;" << std::endl
  << "  static const IddObjectType ZoneContaminantSourceAndSink_Generic_CutoffModel;" << std::endl
  << "  static const IddObjectType ZoneContaminantSourceAndSink_Generic_DecaySource;" << std::endl
  << "  static const IddObjectType SurfaceContaminantSourceAndSink_Generic_BoundaryLayerDiffusion;" << std::endl
  << "  static const IddObjectType SurfaceContaminantSourceAndSink_Generic_DepositionVelocitySink;" << std::endl
  << "  static const IddObjectType ZoneContaminantSourceAndSink_Generic_DepositionRateSink;" << std::endl
  << "  static const IddObjectType Daylighting_Controls;" << std::endl
  << "  static const IddObjectType Daylighting_DELight_Controls;" << std::endl
  << "  static const IddObjectType Daylighting_DELight_ReferencePoint;" << std::endl
  << "  static const IddObjectType Daylighting_DELight_ComplexFenestration;" << std::endl
  << "  static const IddObjectType DaylightingDevice_Tubular;" << std::endl
  << "  static const IddObjectType DaylightingDevice_Shelf;" << std::endl
  << "  static const IddObjectType DaylightingDevice_LightWell;" << std::endl
  << "  static const IddObjectType Output_DaylightFactors;" << std::endl
  << "  static const IddObjectType Output_IlluminanceMap;" << std::endl
  << "  static const IddObjectType OutputControl_IlluminanceMap_Style;" << std::endl
  << "  static const IddObjectType ZoneInfiltration_DesignFlowRate;" << std::endl
  << "  static const IddObjectType ZoneInfiltration_EffectiveLeakageArea;" << std::endl
  << "  static const IddObjectType ZoneInfiltration_FlowCoefficient;" << std::endl
  << "  static const IddObjectType ZoneVentilation_DesignFlowRate;" << std::endl
  << "  static const IddObjectType ZoneVentilation_WindandStackOpenArea;" << std::endl
  << "  static const IddObjectType ZoneAirBalance_OutdoorAir;" << std::endl
  << "  static const IddObjectType ZoneMixing;" << std::endl
  << "  static const IddObjectType ZoneCrossMixing;" << std::endl
  << "  static const IddObjectType ZoneRefrigerationDoorMixing;" << std::endl
  << "  static const IddObjectType ZoneEarthtube;" << std::endl
  << "  static const IddObjectType ZoneCoolTower_Shower;" << std::endl
  << "  static const IddObjectType ZoneThermalChimney;" << std::endl
  << "  static const IddObjectType AirflowNetwork_SimulationControl;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Zone;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Surface;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_ReferenceCrackConditions;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Surface_Crack;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Surface_EffectiveLeakageArea;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Component_DetailedOpening;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Component_SimpleOpening;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Component_HorizontalOpening;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_Component_ZoneExhaustFan;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_ExternalNode;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_WindPressureCoefficientArray;" << std::endl
  << "  static const IddObjectType AirflowNetwork_MultiZone_WindPressureCoefficientValues;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Node;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_Leak;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_LeakageRatio;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_Duct;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_Fan;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_Coil;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_HeatExchanger;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_TerminalUnit;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Component_ConstantPressureDrop;" << std::endl
  << "  static const IddObjectType AirflowNetwork_Distribution_Linkage;" << std::endl
  << "  static const IddObjectType Exterior_Lights;" << std::endl
  << "  static const IddObjectType Exterior_FuelEquipment;" << std::endl
  << "  static const IddObjectType Exterior_WaterEquipment;" << std::endl
  << "  static const IddObjectType HVACTemplate_Thermostat;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_IdealLoadsAirSystem;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_FanCoil;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_PTAC;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_PTHP;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_Unitary;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_VAV;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_VAV_FanPowered;" << std::endl
  << "  static const IddObjectType HVACTemplate_Zone_WaterToAirHeatPump;" << std::endl
  << "  static const IddObjectType HVACTemplate_System_Unitary;" << std::endl
  << "  static const IddObjectType HVACTemplate_System_UnitaryHeatPump_AirToAir;" << std::endl
  << "  static const IddObjectType HVACTemplate_System_VAV;" << std::endl
  << "  static const IddObjectType HVACTemplate_System_PackagedVAV;" << std::endl
  << "  static const IddObjectType HVACTemplate_System_DedicatedOutdoorAir;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_ChilledWaterLoop;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_Chiller;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_Chiller_ObjectReference;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_Tower;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_Tower_ObjectReference;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_HotWaterLoop;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_Boiler;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_Boiler_ObjectReference;" << std::endl
  << "  static const IddObjectType HVACTemplate_Plant_MixedWaterLoop;" << std::endl
  << "  static const IddObjectType DesignSpecification_OutdoorAir;" << std::endl
  << "  static const IddObjectType DesignSpecification_ZoneAirDistribution;" << std::endl
  << "  static const IddObjectType Sizing_Parameters;" << std::endl
  << "  static const IddObjectType Sizing_Zone;" << std::endl
  << "  static const IddObjectType Sizing_System;" << std::endl
  << "  static const IddObjectType Sizing_Plant;" << std::endl
  << "  static const IddObjectType OutputControl_Sizing_Style;" << std::endl
  << "  static const IddObjectType ZoneControl_Humidistat;" << std::endl
  << "  static const IddObjectType ZoneControl_Thermostat;" << std::endl
  << "  static const IddObjectType ZoneControl_Thermostat_OperativeTemperature;" << std::endl
  << "  static const IddObjectType ZoneControl_Thermostat_ThermalComfort;" << std::endl
  << "  static const IddObjectType ZoneControl_Thermostat_TemperatureAndHumidity;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_SingleHeating;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_SingleCooling;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_SingleHeatingOrCooling;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_DualSetpoint;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_ThermalComfort_Fanger_SingleHeating;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_ThermalComfort_Fanger_SingleCooling;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_ThermalComfort_Fanger_SingleHeatingOrCooling;" << std::endl
  << "  static const IddObjectType ThermostatSetpoint_ThermalComfort_Fanger_DualSetpoint;" << std::endl
  << "  static const IddObjectType ZoneControl_ContaminantController;" << std::endl
  << "  static const IddObjectType ZoneHVAC_IdealLoadsAirSystem;" << std::endl
  << "  static const IddObjectType ZoneHVAC_FourPipeFanCoil;" << std::endl
  << "  static const IddObjectType ZoneHVAC_WindowAirConditioner;" << std::endl
  << "  static const IddObjectType ZoneHVAC_PackagedTerminalAirConditioner;" << std::endl
  << "  static const IddObjectType ZoneHVAC_PackagedTerminalHeatPump;" << std::endl
  << "  static const IddObjectType ZoneHVAC_WaterToAirHeatPump;" << std::endl
  << "  static const IddObjectType ZoneHVAC_Dehumidifier_DX;" << std::endl
  << "  static const IddObjectType ZoneHVAC_EnergyRecoveryVentilator;" << std::endl
  << "  static const IddObjectType ZoneHVAC_EnergyRecoveryVentilator_Controller;" << std::endl
  << "  static const IddObjectType ZoneHVAC_UnitVentilator;" << std::endl
  << "  static const IddObjectType ZoneHVAC_UnitHeater;" << std::endl
  << "  static const IddObjectType ZoneHVAC_OutdoorAirUnit;" << std::endl
  << "  static const IddObjectType ZoneHVAC_OutdoorAirUnit_EquipmentList;" << std::endl
  << "  static const IddObjectType ZoneHVAC_TerminalUnit_VariableRefrigerantFlow;" << std::endl
  << "  static const IddObjectType ZoneHVAC_Baseboard_RadiantConvective_Water;" << std::endl
  << "  static const IddObjectType ZoneHVAC_Baseboard_RadiantConvective_Steam;" << std::endl
  << "  static const IddObjectType ZoneHVAC_Baseboard_RadiantConvective_Electric;" << std::endl
  << "  static const IddObjectType ZoneHVAC_Baseboard_Convective_Water;" << std::endl
  << "  static const IddObjectType ZoneHVAC_Baseboard_Convective_Electric;" << std::endl
  << "  static const IddObjectType ZoneHVAC_LowTemperatureRadiant_VariableFlow;" << std::endl
  << "  static const IddObjectType ZoneHVAC_LowTemperatureRadiant_ConstantFlow;" << std::endl
  << "  static const IddObjectType ZoneHVAC_LowTemperatureRadiant_Electric;" << std::endl
  << "  static const IddObjectType ZoneHVAC_LowTemperatureRadiant_SurfaceGroup;" << std::endl
  << "  static const IddObjectType ZoneHVAC_HighTemperatureRadiant;" << std::endl
  << "  static const IddObjectType ZoneHVAC_VentilatedSlab;" << std::endl
  << "  static const IddObjectType ZoneHVAC_VentilatedSlab_SlabGroup;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_Uncontrolled;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_ConstantVolume_Reheat;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_VAV_NoReheat;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_VAV_Reheat;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_VAV_Reheat_VariableSpeedFan;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_SeriesPIU_Reheat;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_ParallelPIU_Reheat;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_ConstantVolume_CooledBeam;" << std::endl
  << "  static const IddObjectType AirTerminal_DualDuct_ConstantVolume;" << std::endl
  << "  static const IddObjectType AirTerminal_DualDuct_VAV;" << std::endl
  << "  static const IddObjectType AirTerminal_DualDuct_VAV_OutdoorAir;" << std::endl
  << "  static const IddObjectType ZoneHVAC_AirDistributionUnit;" << std::endl
  << "  static const IddObjectType ZoneHVAC_EquipmentList;" << std::endl
  << "  static const IddObjectType ZoneHVAC_EquipmentConnections;" << std::endl
  << "  static const IddObjectType Fan_ConstantVolume;" << std::endl
  << "  static const IddObjectType Fan_VariableVolume;" << std::endl
  << "  static const IddObjectType Fan_OnOff;" << std::endl
  << "  static const IddObjectType Fan_ZoneExhaust;" << std::endl
  << "  static const IddObjectType FanPerformance_NightVentilation;" << std::endl
  << "  static const IddObjectType Fan_ComponentModel;" << std::endl
  << "  static const IddObjectType Coil_Cooling_Water;" << std::endl
  << "  static const IddObjectType Coil_Cooling_Water_DetailedGeometry;" << std::endl
  << "  static const IddObjectType Coil_Cooling_DX_SingleSpeed;" << std::endl
  << "  static const IddObjectType Coil_Cooling_DX_TwoSpeed;" << std::endl
  << "  static const IddObjectType Coil_Cooling_DX_MultiSpeed;" << std::endl
  << "  static const IddObjectType Coil_Cooling_DX_VariableSpeed;" << std::endl
  << "  static const IddObjectType Coil_Cooling_DX_TwoStageWithHumidityControlMode;" << std::endl
  << "  static const IddObjectType CoilPerformance_DX_Cooling;" << std::endl
  << "  static const IddObjectType Coil_Cooling_DX_VariableRefrigerantFlow;" << std::endl
  << "  static const IddObjectType Coil_Heating_DX_VariableRefrigerantFlow;" << std::endl
  << "  static const IddObjectType Coil_Heating_Water;" << std::endl
  << "  static const IddObjectType Coil_Heating_Steam;" << std::endl
  << "  static const IddObjectType Coil_Heating_Electric;" << std::endl
  << "  static const IddObjectType Coil_Heating_Electric_MultiStage;" << std::endl
  << "  static const IddObjectType Coil_Heating_Gas;" << std::endl
  << "  static const IddObjectType Coil_Heating_Gas_MultiStage;" << std::endl
  << "  static const IddObjectType Coil_Heating_Desuperheater;" << std::endl
  << "  static const IddObjectType Coil_Heating_DX_SingleSpeed;" << std::endl
  << "  static const IddObjectType Coil_Heating_DX_MultiSpeed;" << std::endl
  << "  static const IddObjectType Coil_Heating_DX_VariableSpeed;" << std::endl
  << "  static const IddObjectType Coil_Cooling_WaterToAirHeatPump_ParameterEstimation;" << std::endl
  << "  static const IddObjectType Coil_Heating_WaterToAirHeatPump_ParameterEstimation;" << std::endl
  << "  static const IddObjectType Coil_Cooling_WaterToAirHeatPump_EquationFit;" << std::endl
  << "  static const IddObjectType Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit;" << std::endl
  << "  static const IddObjectType Coil_Heating_WaterToAirHeatPump_EquationFit;" << std::endl
  << "  static const IddObjectType Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit;" << std::endl
  << "  static const IddObjectType Coil_WaterHeating_AirToWaterHeatPump;" << std::endl
  << "  static const IddObjectType Coil_WaterHeating_Desuperheater;" << std::endl
  << "  static const IddObjectType CoilSystem_Cooling_DX;" << std::endl
  << "  static const IddObjectType CoilSystem_Heating_DX;" << std::endl
  << "  static const IddObjectType CoilSystem_Cooling_Water_HeatExchangerAssisted;" << std::endl
  << "  static const IddObjectType CoilSystem_Cooling_DX_HeatExchangerAssisted;" << std::endl
  << "  static const IddObjectType EvaporativeCooler_Direct_CelDekPad;" << std::endl
  << "  static const IddObjectType EvaporativeCooler_Indirect_CelDekPad;" << std::endl
  << "  static const IddObjectType EvaporativeCooler_Indirect_WetCoil;" << std::endl
  << "  static const IddObjectType EvaporativeCooler_Indirect_ResearchSpecial;" << std::endl
  << "  static const IddObjectType EvaporativeCooler_Direct_ResearchSpecial;" << std::endl
  << "  static const IddObjectType Humidifier_Steam_Electric;" << std::endl
  << "  static const IddObjectType Dehumidifier_Desiccant_NoFans;" << std::endl
  << "  static const IddObjectType Dehumidifier_Desiccant_System;" << std::endl
  << "  static const IddObjectType HeatExchanger_AirToAir_FlatPlate;" << std::endl
  << "  static const IddObjectType HeatExchanger_AirToAir_SensibleAndLatent;" << std::endl
  << "  static const IddObjectType HeatExchanger_Desiccant_BalancedFlow;" << std::endl
  << "  static const IddObjectType HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_Unitary_Furnace_HeatOnly;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_Unitary_Furnace_HeatCool;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_UnitaryHeatOnly;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_UnitaryHeatCool;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_UnitaryHeatPump_AirToAir;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_UnitaryHeatPump_WaterToAir;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed;" << std::endl
  << "  static const IddObjectType AirConditioner_VariableRefrigerantFlow;" << std::endl
  << "  static const IddObjectType ZoneTerminalUnitList;" << std::endl
  << "  static const IddObjectType Controller_WaterCoil;" << std::endl
  << "  static const IddObjectType Controller_OutdoorAir;" << std::endl
  << "  static const IddObjectType Controller_MechanicalVentilation;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_ControllerList;" << std::endl
  << "  static const IddObjectType AirLoopHVAC;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_OutdoorAirSystem_EquipmentList;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_OutdoorAirSystem;" << std::endl
  << "  static const IddObjectType OutdoorAir_Mixer;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_ZoneSplitter;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_SupplyPlenum;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_SupplyPath;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_ZoneMixer;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_ReturnPlenum;" << std::endl
  << "  static const IddObjectType AirLoopHVAC_ReturnPath;" << std::endl
  << "  static const IddObjectType Branch;" << std::endl
  << "  static const IddObjectType BranchList;" << std::endl
  << "  static const IddObjectType Connector_Splitter;" << std::endl
  << "  static const IddObjectType Connector_Mixer;" << std::endl
  << "  static const IddObjectType ConnectorList;" << std::endl
  << "  static const IddObjectType NodeList;" << std::endl
  << "  static const IddObjectType OutdoorAir_Node;" << std::endl
  << "  static const IddObjectType OutdoorAir_NodeList;" << std::endl
  << "  static const IddObjectType Pipe_Adiabatic;" << std::endl
  << "  static const IddObjectType Pipe_Adiabatic_Steam;" << std::endl
  << "  static const IddObjectType Pipe_Indoor;" << std::endl
  << "  static const IddObjectType Pipe_Outdoor;" << std::endl
  << "  static const IddObjectType Pipe_Underground;" << std::endl
  << "  static const IddObjectType PipingSystem_Underground_Domain;" << std::endl
  << "  static const IddObjectType PipingSystem_Underground_PipeCircuit;" << std::endl
  << "  static const IddObjectType PipingSystem_Underground_PipeSegment;" << std::endl
  << "  static const IddObjectType Duct;" << std::endl
  << "  static const IddObjectType Pump_VariableSpeed;" << std::endl
  << "  static const IddObjectType Pump_ConstantSpeed;" << std::endl
  << "  static const IddObjectType Pump_VariableSpeed_Condensate;" << std::endl
  << "  static const IddObjectType HeaderedPumps_ConstantSpeed;" << std::endl
  << "  static const IddObjectType HeaderedPumps_VariableSpeed;" << std::endl
  << "  static const IddObjectType TemperingValve;" << std::endl
  << "  static const IddObjectType LoadProfile_Plant;" << std::endl
  << "  static const IddObjectType SolarCollectorPerformance_FlatPlate;" << std::endl
  << "  static const IddObjectType SolarCollector_FlatPlate_Water;" << std::endl
  << "  static const IddObjectType SolarCollector_FlatPlate_PhotovoltaicThermal;" << std::endl
  << "  static const IddObjectType SolarCollector_IntegralCollectorStorage;" << std::endl
  << "  static const IddObjectType SolarCollectorPerformance_IntegralCollectorStorage;" << std::endl
  << "  static const IddObjectType SolarCollectorPerformance_PhotovoltaicThermal_Simple;" << std::endl
  << "  static const IddObjectType SolarCollector_UnglazedTranspired;" << std::endl
  << "  static const IddObjectType SolarCollector_UnglazedTranspired_Multisystem;" << std::endl
  << "  static const IddObjectType Boiler_HotWater;" << std::endl
  << "  static const IddObjectType Boiler_Steam;" << std::endl
  << "  static const IddObjectType Chiller_Electric_EIR;" << std::endl
  << "  static const IddObjectType Chiller_Electric_ReformulatedEIR;" << std::endl
  << "  static const IddObjectType Chiller_Electric;" << std::endl
  << "  static const IddObjectType Chiller_Absorption_Indirect;" << std::endl
  << "  static const IddObjectType Chiller_Absorption;" << std::endl
  << "  static const IddObjectType Chiller_ConstantCOP;" << std::endl
  << "  static const IddObjectType Chiller_EngineDriven;" << std::endl
  << "  static const IddObjectType Chiller_CombustionTurbine;" << std::endl
  << "  static const IddObjectType ChillerHeater_Absorption_DirectFired;" << std::endl
  << "  static const IddObjectType ChillerHeater_Absorption_DoubleEffect;" << std::endl
  << "  static const IddObjectType HeatPump_WaterToWater_EquationFit_Heating;" << std::endl
  << "  static const IddObjectType HeatPump_WaterToWater_EquationFit_Cooling;" << std::endl
  << "  static const IddObjectType HeatPump_WaterToWater_ParameterEstimation_Cooling;" << std::endl
  << "  static const IddObjectType HeatPump_WaterToWater_ParameterEstimation_Heating;" << std::endl
  << "  static const IddObjectType DistrictCooling;" << std::endl
  << "  static const IddObjectType DistrictHeating;" << std::endl
  << "  static const IddObjectType PlantComponent_TemperatureSource;" << std::endl
  << "  static const IddObjectType CentralHeatPumpSystem;" << std::endl
  << "  static const IddObjectType ChillerHeaterPerformance_Electric_EIR;" << std::endl
  << "  static const IddObjectType CoolingTower_SingleSpeed;" << std::endl
  << "  static const IddObjectType CoolingTower_TwoSpeed;" << std::endl
  << "  static const IddObjectType CoolingTower_VariableSpeed;" << std::endl
  << "  static const IddObjectType CoolingTowerPerformance_CoolTools;" << std::endl
  << "  static const IddObjectType CoolingTowerPerformance_YorkCalc;" << std::endl
  << "  static const IddObjectType EvaporativeFluidCooler_SingleSpeed;" << std::endl
  << "  static const IddObjectType EvaporativeFluidCooler_TwoSpeed;" << std::endl
  << "  static const IddObjectType FluidCooler_SingleSpeed;" << std::endl
  << "  static const IddObjectType FluidCooler_TwoSpeed;" << std::endl
  << "  static const IddObjectType GroundHeatExchanger_Vertical;" << std::endl
  << "  static const IddObjectType GroundHeatExchanger_Pond;" << std::endl
  << "  static const IddObjectType GroundHeatExchanger_Surface;" << std::endl
  << "  static const IddObjectType GroundHeatExchanger_HorizontalTrench;" << std::endl
  << "  static const IddObjectType HeatExchanger_FluidToFluid;" << std::endl
  << "  static const IddObjectType WaterHeater_Mixed;" << std::endl
  << "  static const IddObjectType WaterHeater_Stratified;" << std::endl
  << "  static const IddObjectType WaterHeater_Sizing;" << std::endl
  << "  static const IddObjectType WaterHeater_HeatPump;" << std::endl
  << "  static const IddObjectType ThermalStorage_Ice_Simple;" << std::endl
  << "  static const IddObjectType ThermalStorage_Ice_Detailed;" << std::endl
  << "  static const IddObjectType ThermalStorage_ChilledWater_Mixed;" << std::endl
  << "  static const IddObjectType ThermalStorage_ChilledWater_Stratified;" << std::endl
  << "  static const IddObjectType PlantLoop;" << std::endl
  << "  static const IddObjectType CondenserLoop;" << std::endl
  << "  static const IddObjectType PlantEquipmentList;" << std::endl
  << "  static const IddObjectType CondenserEquipmentList;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_Uncontrolled;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_CoolingLoad;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_HeatingLoad;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_OutdoorDryBulb;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_OutdoorWetBulb;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_OutdoorRelativeHumidity;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_OutdoorDewpoint;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_ComponentSetpoint;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_OutdoorDryBulbDifference;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_OutdoorWetBulbDifference;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperation_OutdoorDewpointDifference;" << std::endl
  << "  static const IddObjectType PlantEquipmentOperationSchemes;" << std::endl
  << "  static const IddObjectType CondenserEquipmentOperationSchemes;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_Sensor;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_Actuator;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_ProgramCallingManager;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_Program;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_Subroutine;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_GlobalVariable;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_OutputVariable;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_MeteredOutputVariable;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_TrendVariable;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_InternalVariable;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_CurveOrTableIndexVariable;" << std::endl
  << "  static const IddObjectType EnergyManagementSystem_ConstructionIndexVariable;" << std::endl
  << "  static const IddObjectType ExternalInterface;" << std::endl
  << "  static const IddObjectType ExternalInterface_Schedule;" << std::endl
  << "  static const IddObjectType ExternalInterface_Variable;" << std::endl
  << "  static const IddObjectType ExternalInterface_Actuator;" << std::endl
  << "  static const IddObjectType ExternalInterface_FunctionalMockupUnitImport;" << std::endl
  << "  static const IddObjectType ExternalInterface_FunctionalMockupUnitImport_From_Variable;" << std::endl
  << "  static const IddObjectType ExternalInterface_FunctionalMockupUnitImport_To_Schedule;" << std::endl
  << "  static const IddObjectType ExternalInterface_FunctionalMockupUnitImport_To_Actuator;" << std::endl
  << "  static const IddObjectType ExternalInterface_FunctionalMockupUnitImport_To_Variable;" << std::endl
  << "  static const IddObjectType ZoneHVAC_ForcedAir_UserDefined;" << std::endl
  << "  static const IddObjectType AirTerminal_SingleDuct_UserDefined;" << std::endl
  << "  static const IddObjectType Coil_UserDefined;" << std::endl
  << "  static const IddObjectType PlantComponent_UserDefined;" << std::endl
  << "  static const IddObjectType AvailabilityManager_Scheduled;" << std::endl
  << "  static const IddObjectType AvailabilityManager_ScheduledOn;" << std::endl
  << "  static const IddObjectType AvailabilityManager_ScheduledOff;" << std::endl
  << "  static const IddObjectType AvailabilityManager_NightCycle;" << std::endl
  << "  static const IddObjectType AvailabilityManager_DifferentialThermostat;" << std::endl
  << "  static const IddObjectType AvailabilityManager_HighTemperatureTurnOff;" << std::endl
  << "  static const IddObjectType AvailabilityManager_HighTemperatureTurnOn;" << std::endl
  << "  static const IddObjectType AvailabilityManager_LowTemperatureTurnOff;" << std::endl
  << "  static const IddObjectType AvailabilityManager_LowTemperatureTurnOn;" << std::endl
  << "  static const IddObjectType AvailabilityManager_NightVentilation;" << std::endl
  << "  static const IddObjectType AvailabilityManager_HybridVentilation;" << std::endl
  << "  static const IddObjectType AvailabilityManagerAssignmentList;" << std::endl
  << "  static const IddObjectType SetpointManager_Scheduled;" << std::endl
  << "  static const IddObjectType SetpointManager_Scheduled_DualSetpoint;" << std::endl
  << "  static const IddObjectType SetpointManager_OutdoorAirReset;" << std::endl
  << "  static const IddObjectType SetpointManager_SingleZone_Reheat;" << std::endl
  << "  static const IddObjectType SetpointManager_SingleZone_Heating;" << std::endl
  << "  static const IddObjectType SetpointManager_SingleZone_Cooling;" << std::endl
  << "  static const IddObjectType SetpointManager_SingleZone_Humidity_Minimum;" << std::endl
  << "  static const IddObjectType SetpointManager_SingleZone_Humidity_Maximum;" << std::endl
  << "  static const IddObjectType SetpointManager_MixedAir;" << std::endl
  << "  static const IddObjectType SetpointManager_OutdoorAirPretreat;" << std::endl
  << "  static const IddObjectType SetpointManager_Warmest;" << std::endl
  << "  static const IddObjectType SetpointManager_Coldest;" << std::endl
  << "  static const IddObjectType SetpointManager_ReturnAirBypassFlow;" << std::endl
  << "  static const IddObjectType SetpointManager_WarmestTemperatureFlow;" << std::endl
  << "  static const IddObjectType SetpointManager_MultiZone_Heating_Average;" << std::endl
  << "  static const IddObjectType SetpointManager_MultiZone_Cooling_Average;" << std::endl
  << "  static const IddObjectType SetpointManager_MultiZone_MinimumHumidity_Average;" << std::endl
  << "  static const IddObjectType SetpointManager_MultiZone_MaximumHumidity_Average;" << std::endl
  << "  static const IddObjectType SetpointManager_MultiZone_Humidity_Minimum;" << std::endl
  << "  static const IddObjectType SetpointManager_MultiZone_Humidity_Maximum;" << std::endl
  << "  static const IddObjectType SetpointManager_FollowOutdoorAirTemperature;" << std::endl
  << "  static const IddObjectType SetpointManager_FollowSystemNodeTemperature;" << std::endl
  << "  static const IddObjectType SetpointManager_FollowGroundTemperature;" << std::endl
  << "  static const IddObjectType SetpointManager_CondenserEnteringReset;" << std::endl
  << "  static const IddObjectType SetpointManager_CondenserEnteringReset_Ideal;" << std::endl
  << "  static const IddObjectType Refrigeration_Case;" << std::endl
  << "  static const IddObjectType Refrigeration_CompressorRack;" << std::endl
  << "  static const IddObjectType Refrigeration_CaseAndWalkInList;" << std::endl
  << "  static const IddObjectType Refrigeration_Condenser_AirCooled;" << std::endl
  << "  static const IddObjectType Refrigeration_Condenser_EvaporativeCooled;" << std::endl
  << "  static const IddObjectType Refrigeration_Condenser_WaterCooled;" << std::endl
  << "  static const IddObjectType Refrigeration_Condenser_Cascade;" << std::endl
  << "  static const IddObjectType Refrigeration_GasCooler_AirCooled;" << std::endl
  << "  static const IddObjectType Refrigeration_TransferLoadList;" << std::endl
  << "  static const IddObjectType Refrigeration_Subcooler;" << std::endl
  << "  static const IddObjectType Refrigeration_Compressor;" << std::endl
  << "  static const IddObjectType Refrigeration_CompressorList;" << std::endl
  << "  static const IddObjectType Refrigeration_System;" << std::endl
  << "  static const IddObjectType Refrigeration_TranscriticalSystem;" << std::endl
  << "  static const IddObjectType Refrigeration_SecondarySystem;" << std::endl
  << "  static const IddObjectType Refrigeration_WalkIn;" << std::endl
  << "  static const IddObjectType Refrigeration_AirChiller;" << std::endl
  << "  static const IddObjectType ZoneHVAC_RefrigerationChillerSet;" << std::endl
  << "  static const IddObjectType DemandManagerAssignmentList;" << std::endl
  << "  static const IddObjectType DemandManager_ExteriorLights;" << std::endl
  << "  static const IddObjectType DemandManager_Lights;" << std::endl
  << "  static const IddObjectType DemandManager_ElectricEquipment;" << std::endl
  << "  static const IddObjectType DemandManager_Thermostats;" << std::endl
  << "  static const IddObjectType Generator_InternalCombustionEngine;" << std::endl
  << "  static const IddObjectType Generator_CombustionTurbine;" << std::endl
  << "  static const IddObjectType Generator_MicroTurbine;" << std::endl
  << "  static const IddObjectType Generator_Photovoltaic;" << std::endl
  << "  static const IddObjectType PhotovoltaicPerformance_Simple;" << std::endl
  << "  static const IddObjectType PhotovoltaicPerformance_EquivalentOneDiode;" << std::endl
  << "  static const IddObjectType PhotovoltaicPerformance_Sandia;" << std::endl
  << "  static const IddObjectType Generator_FuelCell;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_PowerModule;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_AirSupply;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_WaterSupply;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_AuxiliaryHeater;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_ExhaustGasToWaterHeatExchanger;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_ElectricalStorage;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_Inverter;" << std::endl
  << "  static const IddObjectType Generator_FuelCell_StackCooler;" << std::endl
  << "  static const IddObjectType Generator_MicroCHP;" << std::endl
  << "  static const IddObjectType Generator_MicroCHP_NonNormalizedParameters;" << std::endl
  << "  static const IddObjectType Generator_FuelSupply;" << std::endl
  << "  static const IddObjectType Generator_WindTurbine;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Generators;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Inverter_Simple;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Inverter_FunctionOfPower;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Inverter_LookUpTable;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Storage_Simple;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Storage_Battery;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Transformer;" << std::endl
  << "  static const IddObjectType ElectricLoadCenter_Distribution;" << std::endl
  << "  static const IddObjectType WaterUse_Equipment;" << std::endl
  << "  static const IddObjectType WaterUse_Connections;" << std::endl
  << "  static const IddObjectType WaterUse_Storage;" << std::endl
  << "  static const IddObjectType WaterUse_Well;" << std::endl
  << "  static const IddObjectType WaterUse_RainCollector;" << std::endl
  << "  static const IddObjectType Matrix_TwoDimension;" << std::endl
  << "  static const IddObjectType Curve_Linear;" << std::endl
  << "  static const IddObjectType Curve_QuadLinear;" << std::endl
  << "  static const IddObjectType Curve_Quadratic;" << std::endl
  << "  static const IddObjectType Curve_Cubic;" << std::endl
  << "  static const IddObjectType Curve_Quartic;" << std::endl
  << "  static const IddObjectType Curve_Exponent;" << std::endl
  << "  static const IddObjectType Curve_Bicubic;" << std::endl
  << "  static const IddObjectType Curve_Biquadratic;" << std::endl
  << "  static const IddObjectType Curve_QuadraticLinear;" << std::endl
  << "  static const IddObjectType Curve_Triquadratic;" << std::endl
  << "  static const IddObjectType Curve_Functional_PressureDrop;" << std::endl
  << "  static const IddObjectType Curve_FanPressureRise;" << std::endl
  << "  static const IddObjectType Curve_ExponentialSkewNormal;" << std::endl
  << "  static const IddObjectType Curve_Sigmoid;" << std::endl
  << "  static const IddObjectType Curve_RectangularHyperbola1;" << std::endl
  << "  static const IddObjectType Curve_RectangularHyperbola2;" << std::endl
  << "  static const IddObjectType Curve_ExponentialDecay;" << std::endl
  << "  static const IddObjectType Curve_DoubleExponentialDecay;" << std::endl
  << "  static const IddObjectType Table_OneIndependentVariable;" << std::endl
  << "  static const IddObjectType Table_TwoIndependentVariables;" << std::endl
  << "  static const IddObjectType Table_MultiVariableLookup;" << std::endl
  << "  static const IddObjectType FluidProperties_Name;" << std::endl
  << "  static const IddObjectType FluidProperties_GlycolConcentration;" << std::endl
  << "  static const IddObjectType FluidProperties_Temperatures;" << std::endl
  << "  static const IddObjectType FluidProperties_Saturated;" << std::endl
  << "  static const IddObjectType FluidProperties_Superheated;" << std::endl
  << "  static const IddObjectType FluidProperties_Concentration;" << std::endl
  << "  static const IddObjectType CurrencyType;" << std::endl
  << "  static const IddObjectType ComponentCost_Adjustments;" << std::endl
  << "  static const IddObjectType ComponentCost_Reference;" << std::endl
  << "  static const IddObjectType ComponentCost_LineItem;" << std::endl
  << "  static const IddObjectType UtilityCost_Tariff;" << std::endl
  << "  static const IddObjectType UtilityCost_Qualify;" << std::endl
  << "  static const IddObjectType UtilityCost_Charge_Simple;" << std::endl
  << "  static const IddObjectType UtilityCost_Charge_Block;" << std::endl
  << "  static const IddObjectType UtilityCost_Ratchet;" << std::endl
  << "  static const IddObjectType UtilityCost_Variable;" << std::endl
  << "  static const IddObjectType UtilityCost_Computation;" << std::endl
  << "  static const IddObjectType LifeCycleCost_Parameters;" << std::endl
  << "  static const IddObjectType LifeCycleCost_RecurringCosts;" << std::endl
  << "  static const IddObjectType LifeCycleCost_NonrecurringCost;" << std::endl
  << "  static const IddObjectType LifeCycleCost_UsePriceEscalation;" << std::endl
  << "  static const IddObjectType LifeCycleCost_UseAdjustment;" << std::endl
  << "  static const IddObjectType Parametric_SetValueForRun;" << std::endl
  << "  static const IddObjectType Parametric_Logic;" << std::endl
  << "  static const IddObjectType Parametric_RunControl;" << std::endl
  << "  static const IddObjectType Parametric_FileNameSuffix;" << std::endl
  << "  static const IddObjectType Output_VariableDictionary;" << std::endl
  << "  static const IddObjectType Output_Surfaces_List;" << std::endl
  << "  static const IddObjectType Output_Surfaces_Drawing;" << std::endl
  << "  static const IddObjectType Output_Schedules;" << std::endl
  << "  static const IddObjectType Output_Constructions;" << std::endl
  << "  static const IddObjectType Output_EnergyManagementSystem;" << std::endl
  << "  static const IddObjectType OutputControl_SurfaceColorScheme;" << std::endl
  << "  static const IddObjectType Output_Table_SummaryReports;" << std::endl
  << "  static const IddObjectType Output_Table_TimeBins;" << std::endl
  << "  static const IddObjectType Output_Table_Monthly;" << std::endl
  << "  static const IddObjectType OutputControl_Table_Style;" << std::endl
  << "  static const IddObjectType OutputControl_ReportingTolerances;" << std::endl
  << "  static const IddObjectType Output_Variable;" << std::endl
  << "  static const IddObjectType Output_Meter;" << std::endl
  << "  static const IddObjectType Output_Meter_MeterFileOnly;" << std::endl
  << "  static const IddObjectType Output_Meter_Cumulative;" << std::endl
  << "  static const IddObjectType Output_Meter_Cumulative_MeterFileOnly;" << std::endl
  << "  static const IddObjectType Meter_Custom;" << std::endl
  << "  static const IddObjectType Meter_CustomDecrement;" << std::endl
  << "  static const IddObjectType Output_SQLite;" << std::endl
  << "  static const IddObjectType Output_EnvironmentalImpactFactors;" << std::endl
  << "  static const IddObjectType EnvironmentalImpactFactors;" << std::endl
  << "  static const IddObjectType FuelFactors;" << std::endl
  << "  static const IddObjectType Output_Diagnostics;" << std::endl
  << "  static const IddObjectType Output_DebuggingData;" << std::endl
  << "  static const IddObjectType Output_PreprocessorMessage;" << std::endl
  << "  static const IddObjectType OS_Version;" << std::endl
  << "  static const IddObjectType OS_ComponentData;" << std::endl
  << "  static const IddObjectType OS_ConvergenceLimits;" << std::endl
  << "  static const IddObjectType OS_ProgramControl;" << std::endl
  << "  static const IddObjectType OS_HeatBalanceAlgorithm;" << std::endl
  << "  static const IddObjectType OS_RunPeriod;" << std::endl
  << "  static const IddObjectType OS_RunPeriodControl_DaylightSavingTime;" << std::endl
  << "  static const IddObjectType OS_RunPeriodControl_SpecialDays;" << std::endl
  << "  static const IddObjectType OS_ShadowCalculation;" << std::endl
  << "  static const IddObjectType OS_SimulationControl;" << std::endl
  << "  static const IddObjectType OS_Sizing_Parameters;" << std::endl
  << "  static const IddObjectType OS_SurfaceConvectionAlgorithm_Inside;" << std::endl
  << "  static const IddObjectType OS_SurfaceConvectionAlgorithm_Outside;" << std::endl
  << "  static const IddObjectType OS_Timestep;" << std::endl
  << "  static const IddObjectType OS_YearDescription;" << std::endl
  << "  static const IddObjectType OS_ZoneAirContaminantBalance;" << std::endl
  << "  static const IddObjectType OS_ZoneAirHeatBalanceAlgorithm;" << std::endl
  << "  static const IddObjectType OS_ZoneCapacitanceMultiplier_ResearchSpecial;" << std::endl
  << "  static const IddObjectType OS_RadianceParameters;" << std::endl
  << "  static const IddObjectType OS_ClimateZones;" << std::endl
  << "  static const IddObjectType OS_Site;" << std::endl
  << "  static const IddObjectType OS_Site_GroundTemperature_BuildingSurface;" << std::endl
  << "  static const IddObjectType OS_Site_GroundReflectance;" << std::endl
  << "  static const IddObjectType OS_Site_WaterMainsTemperature;" << std::endl
  << "  static const IddObjectType OS_SizingPeriod_DesignDay;" << std::endl
  << "  static const IddObjectType OS_SizingPeriod_WeatherFileConditionType;" << std::endl
  << "  static const IddObjectType OS_SizingPeriod_WeatherFileDays;" << std::endl
  << "  static const IddObjectType OS_TimeDependentValuation;" << std::endl
  << "  static const IddObjectType OS_UtilityCost_Charge_Block;" << std::endl
  << "  static const IddObjectType OS_UtilityCost_Charge_Simple;" << std::endl
  << "  static const IddObjectType OS_UtilityCost_Computation;" << std::endl
  << "  static const IddObjectType OS_UtilityCost_Qualify;" << std::endl
  << "  static const IddObjectType OS_UtilityCost_Ratchet;" << std::endl
  << "  static const IddObjectType OS_UtilityCost_Tariff;" << std::endl
  << "  static const IddObjectType OS_UtilityCost_Variable;" << std::endl
  << "  static const IddObjectType OS_WeatherFile;" << std::endl
  << "  static const IddObjectType OS_WeatherProperty_SkyTemperature;" << std::endl
  << "  static const IddObjectType OS_BuildingStory;" << std::endl
  << "  static const IddObjectType OS_DefaultConstructionSet;" << std::endl
  << "  static const IddObjectType OS_DefaultScheduleSet;" << std::endl
  << "  static const IddObjectType OS_DefaultSubSurfaceConstructions;" << std::endl
  << "  static const IddObjectType OS_DefaultSurfaceConstructions;" << std::endl
  << "  static const IddObjectType OS_Rendering_Color;" << std::endl
  << "  static const IddObjectType OS_SpaceType;" << std::endl
  << "  static const IddObjectType OS_Material;" << std::endl
  << "  static const IddObjectType OS_Material_AirGap;" << std::endl
  << "  static const IddObjectType OS_Material_AirWall;" << std::endl
  << "  static const IddObjectType OS_Material_InfraredTransparent;" << std::endl
  << "  static const IddObjectType OS_Material_NoMass;" << std::endl
  << "  static const IddObjectType OS_Material_RoofVegetation;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_Blind;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_Gas;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_GasMixture;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_Glazing;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_GlazingGroup_Thermochromic;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_Glazing_RefractionExtinctionMethod;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_Screen;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_Shade;" << std::endl
  << "  static const IddObjectType OS_WindowMaterial_SimpleGlazingSystem;" << std::endl
  << "  static const IddObjectType OS_Construction;" << std::endl
  << "  static const IddObjectType OS_Construction_CfactorUndergroundWall;" << std::endl
  << "  static const IddObjectType OS_Construction_FfactorGroundFloor;" << std::endl
  << "  static const IddObjectType OS_Construction_InternalSource;" << std::endl
  << "  static const IddObjectType OS_Construction_WindowDataFile;" << std::endl
  << "  static const IddObjectType OS_StandardsInformation_Construction;" << std::endl
  << "  static const IddObjectType OS_InternalMass_Definition;" << std::endl
  << "  static const IddObjectType OS_People_Definition;" << std::endl
  << "  static const IddObjectType OS_Lights_Definition;" << std::endl
  << "  static const IddObjectType OS_Luminaire_Definition;" << std::endl
  << "  static const IddObjectType OS_ElectricEquipment_Definition;" << std::endl
  << "  static const IddObjectType OS_GasEquipment_Definition;" << std::endl
  << "  static const IddObjectType OS_HotWaterEquipment_Definition;" << std::endl
  << "  static const IddObjectType OS_SteamEquipment_Definition;" << std::endl
  << "  static const IddObjectType OS_OtherEquipment_Definition;" << std::endl
  << "  static const IddObjectType OS_Exterior_Lights_Definition;" << std::endl
  << "  static const IddObjectType OS_Schedule_Compact;" << std::endl
  << "  static const IddObjectType OS_Schedule_Day;" << std::endl
  << "  static const IddObjectType OS_Schedule_Week;" << std::endl
  << "  static const IddObjectType OS_Schedule_Year;" << std::endl
  << "  static const IddObjectType OS_Schedule_Rule;" << std::endl
  << "  static const IddObjectType OS_Schedule_Ruleset;" << std::endl
  << "  static const IddObjectType OS_Schedule_Constant;" << std::endl
  << "  static const IddObjectType OS_Schedule_FixedInterval;" << std::endl
  << "  static const IddObjectType OS_Schedule_VariableInterval;" << std::endl
  << "  static const IddObjectType OS_ScheduleTypeLimits;" << std::endl
  << "  static const IddObjectType OS_Building;" << std::endl
  << "  static const IddObjectType OS_DaylightingDevice_Shelf;" << std::endl
  << "  static const IddObjectType OS_Facility;" << std::endl
  << "  static const IddObjectType OS_InteriorPartitionSurface;" << std::endl
  << "  static const IddObjectType OS_InteriorPartitionSurfaceGroup;" << std::endl
  << "  static const IddObjectType OS_ShadingSurface;" << std::endl
  << "  static const IddObjectType OS_ShadingSurfaceGroup;" << std::endl
  << "  static const IddObjectType OS_Space;" << std::endl
  << "  static const IddObjectType OS_StandardsInformation_Building;" << std::endl
  << "  static const IddObjectType OS_Surface;" << std::endl
  << "  static const IddObjectType OS_SubSurface;" << std::endl
  << "  static const IddObjectType OS_ShadingControl;" << std::endl
  << "  static const IddObjectType OS_InternalMass;" << std::endl
  << "  static const IddObjectType OS_People;" << std::endl
  << "  static const IddObjectType OS_Lights;" << std::endl
  << "  static const IddObjectType OS_Luminaire;" << std::endl
  << "  static const IddObjectType OS_ElectricEquipment;" << std::endl
  << "  static const IddObjectType OS_GasEquipment;" << std::endl
  << "  static const IddObjectType OS_HotWaterEquipment;" << std::endl
  << "  static const IddObjectType OS_SteamEquipment;" << std::endl
  << "  static const IddObjectType OS_OtherEquipment;" << std::endl
  << "  static const IddObjectType OS_SpaceInfiltration_DesignFlowRate;" << std::endl
  << "  static const IddObjectType OS_SpaceInfiltration_EffectiveLeakageArea;" << std::endl
  << "  static const IddObjectType OS_Exterior_Lights;" << std::endl
  << "  static const IddObjectType OS_Daylighting_Control;" << std::endl
  << "  static const IddObjectType OS_Glare_Sensor;" << std::endl
  << "  static const IddObjectType OS_IlluminanceMap;" << std::endl
  << "  static const IddObjectType OS_LightingDesignDay;" << std::endl
  << "  static const IddObjectType OS_LightingSimulationControl;" << std::endl
  << "  static const IddObjectType OS_LightingSimulationZone;" << std::endl
  << "  static const IddObjectType OS_AirLoopHVAC;" << std::endl
  << "  static const IddObjectType OS_AirLoopHVAC_ControllerList;" << std::endl
  << "  static const IddObjectType OS_AirLoopHVAC_OutdoorAirSystem;" << std::endl
  << "  static const IddObjectType OS_AirLoopHVAC_UnitaryCoolOnly;" << std::endl
  << "  static const IddObjectType OS_AirLoopHVAC_UnitaryHeatPump_AirToAir;" << std::endl
  << "  static const IddObjectType OS_AirLoopHVAC_ZoneMixer;" << std::endl
  << "  static const IddObjectType OS_AirLoopHVAC_ZoneSplitter;" << std::endl
  << "  static const IddObjectType OS_AirTerminal_SingleDuct_ConstantVolume_Reheat;" << std::endl
  << "  static const IddObjectType OS_AirTerminal_SingleDuct_ParallelPIU_Reheat;" << std::endl
  << "  static const IddObjectType OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam;" << std::endl
  << "  static const IddObjectType OS_AirTerminal_SingleDuct_Uncontrolled;" << std::endl
  << "  static const IddObjectType OS_AirTerminal_SingleDuct_VAV_NoReheat;" << std::endl
  << "  static const IddObjectType OS_AirTerminal_SingleDuct_VAV_Reheat;" << std::endl
  << "  static const IddObjectType OS_AvailabilityManagerAssignmentList;" << std::endl
  << "  static const IddObjectType OS_AvailabilityManager_Scheduled;" << std::endl
  << "  static const IddObjectType OS_AvailabilityManager_NightCycle;" << std::endl
  << "  static const IddObjectType OS_Boiler_HotWater;" << std::endl
  << "  static const IddObjectType OS_Boiler_Steam;" << std::endl
  << "  static const IddObjectType OS_Chiller_Electric_EIR;" << std::endl
  << "  static const IddObjectType OS_CoolingTower_SingleSpeed;" << std::endl
  << "  static const IddObjectType OS_CoolingTower_VariableSpeed;" << std::endl
  << "  static const IddObjectType OS_DistrictCooling;" << std::endl
  << "  static const IddObjectType OS_DistrictHeating;" << std::endl
  << "  static const IddObjectType OS_GroundHeatExchanger_Vertical;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_CooledBeam;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_DX_SingleSpeed;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_DX_TwoSpeed;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_Water;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_WaterToAirHeatPump_EquationFit;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_DX_SingleSpeed;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_Electric;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_Gas;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_LowTemperatureRadiant_VariableFlow;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_Water;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_Water_Baseboard;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_WaterToAirHeatPump_EquationFit;" << std::endl
  << "  static const IddObjectType OS_Connection;" << std::endl
  << "  static const IddObjectType OS_Connector_Mixer;" << std::endl
  << "  static const IddObjectType OS_Connector_Splitter;" << std::endl
  << "  static const IddObjectType OS_Controller_MechanicalVentilation;" << std::endl
  << "  static const IddObjectType OS_Controller_OutdoorAir;" << std::endl
  << "  static const IddObjectType OS_Controller_WaterCoil;" << std::endl
  << "  static const IddObjectType OS_Curve_Linear;" << std::endl
  << "  static const IddObjectType OS_Curve_Quadratic;" << std::endl
  << "  static const IddObjectType OS_Curve_Cubic;" << std::endl
  << "  static const IddObjectType OS_Curve_Quartic;" << std::endl
  << "  static const IddObjectType OS_Curve_Exponent;" << std::endl
  << "  static const IddObjectType OS_Curve_Bicubic;" << std::endl
  << "  static const IddObjectType OS_Curve_Biquadratic;" << std::endl
  << "  static const IddObjectType OS_Curve_QuadraticLinear;" << std::endl
  << "  static const IddObjectType OS_Curve_Triquadratic;" << std::endl
  << "  static const IddObjectType OS_Curve_Functional_PressureDrop;" << std::endl
  << "  static const IddObjectType OS_Curve_FanPressureRise;" << std::endl
  << "  static const IddObjectType OS_Curve_ExponentialSkewNormal;" << std::endl
  << "  static const IddObjectType OS_Curve_Sigmoid;" << std::endl
  << "  static const IddObjectType OS_Curve_RectangularHyperbola1;" << std::endl
  << "  static const IddObjectType OS_Curve_RectangularHyperbola2;" << std::endl
  << "  static const IddObjectType OS_Curve_ExponentialDecay;" << std::endl
  << "  static const IddObjectType OS_Curve_DoubleExponentialDecay;" << std::endl
  << "  static const IddObjectType OS_DesignSpecification_OutdoorAir;" << std::endl
  << "  static const IddObjectType OS_DesignSpecification_ZoneAirDistribution;" << std::endl
  << "  static const IddObjectType OS_EvaporativeCooler_Direct_ResearchSpecial;" << std::endl
  << "  static const IddObjectType OS_EvaporativeFluidCooler_SingleSpeed;" << std::endl
  << "  static const IddObjectType OS_Fan_ConstantVolume;" << std::endl
  << "  static const IddObjectType OS_Fan_VariableVolume;" << std::endl
  << "  static const IddObjectType OS_Fan_OnOff;" << std::endl
  << "  static const IddObjectType OS_HeatExchanger_AirToAir_SensibleAndLatent;" << std::endl
  << "  static const IddObjectType OS_Node;" << std::endl
  << "  static const IddObjectType OS_PortList;" << std::endl
  << "  static const IddObjectType OS_Pipe_Adiabatic;" << std::endl
  << "  static const IddObjectType OS_PlantLoop;" << std::endl
  << "  static const IddObjectType OS_Pump_ConstantSpeed;" << std::endl
  << "  static const IddObjectType OS_Pump_VariableSpeed;" << std::endl
  << "  static const IddObjectType OS_SetpointManager_FollowOutdoorAirTemperature;" << std::endl
  << "  static const IddObjectType OS_SetpointManager_MixedAir;" << std::endl
  << "  static const IddObjectType OS_SetpointManager_OutdoorAirReset;" << std::endl
  << "  static const IddObjectType OS_SetpointManager_SingleZone_Reheat;" << std::endl
  << "  static const IddObjectType OS_SetpointManager_Scheduled;" << std::endl
  << "  static const IddObjectType OS_SetpointManager_Warmest;" << std::endl
  << "  static const IddObjectType OS_Sizing_Plant;" << std::endl
  << "  static const IddObjectType OS_Sizing_System;" << std::endl
  << "  static const IddObjectType OS_Sizing_Zone;" << std::endl
  << "  static const IddObjectType OS_Splitter;" << std::endl
  << "  static const IddObjectType OS_ThermalZone;" << std::endl
  << "  static const IddObjectType OS_ThermostatSetpoint_DualSetpoint;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_EquipmentList;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_Baseboard_Convective_Electric;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_Baseboard_Convective_Water;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_LowTemperatureRadiant_Electric;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_IdealLoadsAirSystem;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_FourPipeFanCoil;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_PackagedTerminalHeatPump;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_PackagedTerminalAirConditioner;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_WaterToAirHeatPump;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_UnitHeater;" << std::endl
  << "  static const IddObjectType OS_WaterHeater_Mixed;" << std::endl
  << "  static const IddObjectType OS_WaterUse_Equipment;" << std::endl
  << "  static const IddObjectType OS_WaterUse_Equipment_Definition;" << std::endl
  << "  static const IddObjectType OS_WaterUse_Connections;" << std::endl
  << "  static const IddObjectType OS_ComponentCost_Adjustments;" << std::endl
  << "  static const IddObjectType OS_LifeCycleCost;" << std::endl
  << "  static const IddObjectType OS_CurrencyType;" << std::endl
  << "  static const IddObjectType OS_LifeCycleCost_Parameters;" << std::endl
  << "  static const IddObjectType OS_LifeCycleCost_UsePriceEscalation;" << std::endl
  << "  static const IddObjectType OS_UtilityBill;" << std::endl
  << "  static const IddObjectType OS_OutputControl_ReportingTolerances;" << std::endl
  << "  static const IddObjectType OS_Meter;" << std::endl
  << "  static const IddObjectType OS_Output_Variable;" << std::endl
  << "  static const IddObjectType OS_ModelObjectList;" << std::endl
  << "  static const IddObjectType OS_AirConditioner_VariableRefrigerantFlow;" << std::endl
  << "  static const IddObjectType OS_Coil_Cooling_DX_VariableRefrigerantFlow;" << std::endl
  << "  static const IddObjectType OS_Coil_Heating_DX_VariableRefrigerantFlow;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Case;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Condenser_AirCooled;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Condenser_EvaporativeCooled;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Condenser_WaterCooled;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Condenser_Cascade;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Compressor;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Subcooler_Mechanical;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_Subcooler_LiquidSuction;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_System;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_SecondarySystem;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_WalkIn;" << std::endl
  << "  static const IddObjectType OS_Refrigeration_WalkIn_ZoneBoundary;" << std::endl
  << "  static const IddObjectType OS_CoolingTowerPerformance_CoolTools;" << std::endl
  << "  static const IddObjectType OS_CoolingTowerPerformance_YorkCalc;" << std::endl
  << "  static const IddObjectType OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow;" << std::endl;

  outFiles.iddEnumsHxx.tempFile
    << std::endl

    << std::endl
    << "class IddObjectType" << std::endl
    << "{" << std::endl
    << "  public:" << std::endl
    << std::endl
    << "  IddObjectType(const IddObjectType & otherType)" << std::endl
    << "  {" << std::endl
    << "    m_name = otherType.m_name;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  IddObjectType()" << std::endl
    << "  {" << std::endl
    << "    m_name = \"UserCustom\";" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  std::string valueDescription() const" << std::endl
    << "  {" << std::endl
    << "    return m_name;" << std::endl
    << "  }" << std::endl
    << "  std::string valueName() const" << std::endl
    << "  {" << std::endl
    << "    return m_name;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  IddObjectType(const std::string & name)" << std::endl
    << "  {" << std::endl
    << "    m_name = name;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  bool operator==(const IddObjectType &other) const" << std::endl
    << "  {" << std::endl
    << "    return (m_name == other.m_name);" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  bool operator==(const std::string &other) const" << std::endl
    << "  {" << std::endl
    << "    return (m_name == other);" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  bool operator!=(const IddObjectType &other) const" << std::endl
    << "  {" << std::endl
    << "    return (m_name != other.m_name);" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  bool operator!=(const std::string &other) const" << std::endl
    << "  {" << std::endl
    << "    return (m_name != other);" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  bool operator<(const IddObjectType &other) const" << std::endl
    << "  {" << std::endl
    << "    return (m_name < other.m_name);" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  bool operator<(const std::string &other) const" << std::endl
    << "  {" << std::endl
    << "    return (m_name < other);" << std::endl
    << "  }" << std::endl
    << std::endl
    << tempSS.str()
    << std::endl
    << "  private:" << std::endl
    << std::endl
    << "  std::string m_name;" << std::endl
    << "};" << std::endl
    << std::endl
    
    << std::endl
    << "namespace iddobjectname {" << std::endl
    << std::endl
    << "  const std::string UserCustom = \"UserCustom\";" << std::endl
    << "  const std::string Catchall = \"CatchAll\";" << std::endl
    << "  const std::string CommentOnly = \"CommentOnly\";" << std::endl
    //<< "  const std::string Version = \"Version\";" << std::endl
    //<< "  const std::string OS_Version = \"OS:Version\";" << std::endl
    << std::endl
    << "}" << std::endl

    //<< std::endl
    //<< "#define IddObjectType::UserCustom IddObjectType(\"UserCustom\")" << std::endl
    //<< "#define IddObjectType::CatchAll IddObjectType(\"CatchAll\")" << std::endl
    //<< "#define IddObjectType::CommentOnly IddObjectType(\"CommentOnly\")" << std::endl
    //<< "#define IddObjectType::Version IddObjectType(\"Version\")" << std::endl
    //<< "#define IddObjectType::OS_Version IddObjectType(\"OS:Version\")" << std::endl
    //<< std::endl

    //<< "/** \\class IddObjectType" << std::endl
    //<< " *  \\brief Enumeration of the \\link openstudio::IddObject IddObject\\endlink types available" << std::endl
    //<< " *  through the \\link openstudio::IddFactorySingleton IddFactory\\endlink. " << std::endl
    //<< " *  \\details Catchall is the default constructed \\link openstudio::IddObject IddObject\\endlink" << std::endl
    //<< " *  type. UserCustom is the default type for \\link openstudio::IddObject IddObjects\\endlink" << std::endl
    //<< " *  constructed by \\link openstudio::IddObject::load IddObject::load\\endlink. UserCustom objects" << std::endl
    //<< " *  must be referenced by name through an \\link openstudio::IddFile IddFile\\endlink or \\link" << std::endl
    //<< " *  openstudio::IddFileAndFactoryWrapper IddFileAndFactoryWrapper\\endlink. They cannot be" << std::endl
    //<< " *  accessed through the \\link openstudio::IddFactorySingleton IddFactory\\endlink (by name or" << std::endl
    //<< " *  type). CommentOnly is a convenience object for capturing standalone comments in IDFs. All other" << std::endl
    //<< " *  types are derived from the IDD files used to create \\link openstudio::IddFactorySingleton" << std::endl
    //<< " *  IddFactory\\endlink. See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The" << std::endl
    //<< " *  actual macro call is:" << std::endl
    //<< " *" << std::endl
    //<< " *  \\code" << std::endl
    //<< tempSS.str()
    //<< " *  \\endcode */" << std::endl
    //<< tempSS.str()
    //<< std::endl

    << "/** \\relates IddObjectType */" << std::endl
    << "typedef std::vector<IddObjectType> IddObjectTypeVector;" << std::endl
    << "/** \\relates IddObjectType */" << std::endl
    << "typedef std::set<IddObjectType> IddObjectTypeSet;" << std::endl
    << std::endl
    << "/** \\relates IddObjectType */" << std::endl
    << "typedef boost::optional<std::vector<IddObjectType> > OptionalIddObjectTypeVector;" << std::endl
    << "/** \\relates IddObjectType */" << std::endl
    << "typedef boost::optional<std::set<IddObjectType> > OptionalIddObjectTypeSet;" << std::endl
    << "typedef boost::optional<IddObjectType> OptionalIddObjectType;" << std::endl
    << std::endl

    << "} // openstudio" << std::endl
    << std::endl
    << "Q_DECLARE_METATYPE(openstudio::IddFileType)" << std::endl
    << "Q_DECLARE_METATYPE(openstudio::IddObjectType)" << std::endl
    << std::endl
    << "#endif // UTILITIES_IDD_IDDENUMS_HXX" << std::endl;
  tempSS.str("");
  
  // complete and close IddFieldEnums.hxx
  outFiles.iddFieldEnumsHxx.tempFile
    << std::endl
    << "#endif // UTILITIES_IDD_IDDFIELDENUMS_HXX" << std::endl;

  // complete and close IddFieldEnums.ixx
  outFiles.iddFieldEnumsIxx.tempFile
    << std::endl
    << "#endif // UTILITIES_IDD_IDDFIELDENUMS_IXX" << std::endl;

  // complete and close IddFactory.cxx

  outFiles.iddFactoryCxx.tempFile
  << "const IddObjectType  IddObjectType::UserCustom = IddObjectType(\"UserCustom\");" << std::endl
  << "const IddObjectType  IddObjectType::Catchall = IddObjectType(\"CatchAll\");" << std::endl
  << "const IddObjectType  IddObjectType::CommentOnly = IddObjectType(\"CommentOnly\");" << std::endl
  << "const IddObjectType IddObjectType::LeadInput = IddObjectType(\"Lead Input\");" << std::endl
  << "const IddObjectType IddObjectType::EndLeadInput = IddObjectType(\"End Lead Input\");" << std::endl
  << "const IddObjectType IddObjectType::SimulationData = IddObjectType(\"Simulation Data\");" << std::endl
  << "const IddObjectType IddObjectType::EndSimulationData = IddObjectType(\"End Simulation Data\");" << std::endl
  << "const IddObjectType IddObjectType::Version = IddObjectType(\"Version\");" << std::endl
  << "const IddObjectType IddObjectType::SimulationControl = IddObjectType(\"SimulationControl\");" << std::endl
  << "const IddObjectType IddObjectType::Building = IddObjectType(\"Building\");" << std::endl
  << "const IddObjectType IddObjectType::ShadowCalculation = IddObjectType(\"ShadowCalculation\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceConvectionAlgorithm_Inside = IddObjectType(\"SurfaceConvectionAlgorithm:Inside\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceConvectionAlgorithm_Outside = IddObjectType(\"SurfaceConvectionAlgorithm:Outside\");" << std::endl
  << "const IddObjectType IddObjectType::HeatBalanceAlgorithm = IddObjectType(\"HeatBalanceAlgorithm\");" << std::endl
  << "const IddObjectType IddObjectType::HeatBalanceSettings_ConductionFiniteDifference = IddObjectType(\"HeatBalanceSettings:ConductionFiniteDifference\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneAirHeatBalanceAlgorithm = IddObjectType(\"ZoneAirHeatBalanceAlgorithm\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneAirContaminantBalance = IddObjectType(\"ZoneAirContaminantBalance\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneCapacitanceMultiplier_ResearchSpecial = IddObjectType(\"ZoneCapacitanceMultiplier:ResearchSpecial\");" << std::endl
  << "const IddObjectType IddObjectType::Timestep = IddObjectType(\"Timestep\");" << std::endl
  << "const IddObjectType IddObjectType::ConvergenceLimits = IddObjectType(\"ConvergenceLimits\");" << std::endl
  << "const IddObjectType IddObjectType::ProgramControl = IddObjectType(\"ProgramControl\");" << std::endl
  << "const IddObjectType IddObjectType::Compliance_Building = IddObjectType(\"Compliance:Building\");" << std::endl
  << "const IddObjectType IddObjectType::Site_Location = IddObjectType(\"Site:Location\");" << std::endl
  << "const IddObjectType IddObjectType::SizingPeriod_DesignDay = IddObjectType(\"SizingPeriod:DesignDay\");" << std::endl
  << "const IddObjectType IddObjectType::SizingPeriod_WeatherFileDays = IddObjectType(\"SizingPeriod:WeatherFileDays\");" << std::endl
  << "const IddObjectType IddObjectType::SizingPeriod_WeatherFileConditionType = IddObjectType(\"SizingPeriod:WeatherFileConditionType\");" << std::endl
  << "const IddObjectType IddObjectType::RunPeriod = IddObjectType(\"RunPeriod\");" << std::endl
  << "const IddObjectType IddObjectType::RunPeriod_CustomRange = IddObjectType(\"RunPeriod:CustomRange\");" << std::endl
  << "const IddObjectType IddObjectType::RunPeriodControl_SpecialDays = IddObjectType(\"RunPeriodControl:SpecialDays\");" << std::endl
  << "const IddObjectType IddObjectType::RunPeriodControl_DaylightSavingTime = IddObjectType(\"RunPeriodControl:DaylightSavingTime\");" << std::endl
  << "const IddObjectType IddObjectType::WeatherProperty_SkyTemperature = IddObjectType(\"WeatherProperty:SkyTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::Site_WeatherStation = IddObjectType(\"Site:WeatherStation\");" << std::endl
  << "const IddObjectType IddObjectType::Site_HeightVariation = IddObjectType(\"Site:HeightVariation\");" << std::endl
  << "const IddObjectType IddObjectType::Site_GroundTemperature_BuildingSurface = IddObjectType(\"Site:GroundTemperature:BuildingSurface\");" << std::endl
  << "const IddObjectType IddObjectType::Site_GroundTemperature_FCfactorMethod = IddObjectType(\"Site:GroundTemperature:FCfactorMethod\");" << std::endl
  << "const IddObjectType IddObjectType::Site_GroundTemperature_Shallow = IddObjectType(\"Site:GroundTemperature:Shallow\");" << std::endl
  << "const IddObjectType IddObjectType::Site_GroundTemperature_Deep = IddObjectType(\"Site:GroundTemperature:Deep\");" << std::endl
  << "const IddObjectType IddObjectType::Site_GroundReflectance = IddObjectType(\"Site:GroundReflectance\");" << std::endl
  << "const IddObjectType IddObjectType::Site_GroundReflectance_SnowModifier = IddObjectType(\"Site:GroundReflectance:SnowModifier\");" << std::endl
  << "const IddObjectType IddObjectType::Site_WaterMainsTemperature = IddObjectType(\"Site:WaterMainsTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::Site_Precipitation = IddObjectType(\"Site:Precipitation\");" << std::endl
  << "const IddObjectType IddObjectType::RoofIrrigation = IddObjectType(\"RoofIrrigation\");" << std::endl
  << "const IddObjectType IddObjectType::ScheduleTypeLimits = IddObjectType(\"ScheduleTypeLimits\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Day_Hourly = IddObjectType(\"Schedule:Day:Hourly\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Day_Interval = IddObjectType(\"Schedule:Day:Interval\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Day_List = IddObjectType(\"Schedule:Day:List\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Week_Daily = IddObjectType(\"Schedule:Week:Daily\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Week_Compact = IddObjectType(\"Schedule:Week:Compact\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Year = IddObjectType(\"Schedule:Year\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Compact = IddObjectType(\"Schedule:Compact\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_Constant = IddObjectType(\"Schedule:Constant\");" << std::endl
  << "const IddObjectType IddObjectType::Schedule_File = IddObjectType(\"Schedule:File\");" << std::endl
  << "const IddObjectType IddObjectType::Material = IddObjectType(\"Material\");" << std::endl
  << "const IddObjectType IddObjectType::Material_NoMass = IddObjectType(\"Material:NoMass\");" << std::endl
  << "const IddObjectType IddObjectType::Material_InfraredTransparent = IddObjectType(\"Material:InfraredTransparent\");" << std::endl
  << "const IddObjectType IddObjectType::Material_AirGap = IddObjectType(\"Material:AirGap\");" << std::endl
  << "const IddObjectType IddObjectType::Material_RoofVegetation = IddObjectType(\"Material:RoofVegetation\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_SimpleGlazingSystem = IddObjectType(\"WindowMaterial:SimpleGlazingSystem\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_Glazing = IddObjectType(\"WindowMaterial:Glazing\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_GlazingGroup_Thermochromic = IddObjectType(\"WindowMaterial:GlazingGroup:Thermochromic\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_Glazing_RefractionExtinctionMethod = IddObjectType(\"WindowMaterial:Glazing:RefractionExtinctionMethod\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_Gas = IddObjectType(\"WindowMaterial:Gas\");" << std::endl
  << "const IddObjectType IddObjectType::WindowGap_SupportPillar = IddObjectType(\"WindowGap:SupportPillar\");" << std::endl
  << "const IddObjectType IddObjectType::WindowGap_DeflectionState = IddObjectType(\"WindowGap:DeflectionState\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_GasMixture = IddObjectType(\"WindowMaterial:GasMixture\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_Gap = IddObjectType(\"WindowMaterial:Gap\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_Shade = IddObjectType(\"WindowMaterial:Shade\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_ComplexShade = IddObjectType(\"WindowMaterial:ComplexShade\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_Blind = IddObjectType(\"WindowMaterial:Blind\");" << std::endl
  << "const IddObjectType IddObjectType::WindowMaterial_Screen = IddObjectType(\"WindowMaterial:Screen\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_MoisturePenetrationDepth_Settings = IddObjectType(\"MaterialProperty:MoisturePenetrationDepth:Settings\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_PhaseChange = IddObjectType(\"MaterialProperty:PhaseChange\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_VariableThermalConductivity = IddObjectType(\"MaterialProperty:VariableThermalConductivity\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_HeatAndMoistureTransfer_Settings = IddObjectType(\"MaterialProperty:HeatAndMoistureTransfer:Settings\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_HeatAndMoistureTransfer_SorptionIsotherm = IddObjectType(\"MaterialProperty:HeatAndMoistureTransfer:SorptionIsotherm\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_HeatAndMoistureTransfer_Suction = IddObjectType(\"MaterialProperty:HeatAndMoistureTransfer:Suction\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_HeatAndMoistureTransfer_Redistribution = IddObjectType(\"MaterialProperty:HeatAndMoistureTransfer:Redistribution\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_HeatAndMoistureTransfer_Diffusion = IddObjectType(\"MaterialProperty:HeatAndMoistureTransfer:Diffusion\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_HeatAndMoistureTransfer_ThermalConductivity = IddObjectType(\"MaterialProperty:HeatAndMoistureTransfer:ThermalConductivity\");" << std::endl
  << "const IddObjectType IddObjectType::MaterialProperty_GlazingSpectralData = IddObjectType(\"MaterialProperty:GlazingSpectralData\");" << std::endl
  << "const IddObjectType IddObjectType::Construction = IddObjectType(\"Construction\");" << std::endl
  << "const IddObjectType IddObjectType::Construction_CfactorUndergroundWall = IddObjectType(\"Construction:CfactorUndergroundWall\");" << std::endl
  << "const IddObjectType IddObjectType::Construction_FfactorGroundFloor = IddObjectType(\"Construction:FfactorGroundFloor\");" << std::endl
  << "const IddObjectType IddObjectType::Construction_InternalSource = IddObjectType(\"Construction:InternalSource\");" << std::endl
  << "const IddObjectType IddObjectType::WindowThermalModel_Params = IddObjectType(\"WindowThermalModel:Params\");" << std::endl
  << "const IddObjectType IddObjectType::Construction_ComplexFenestrationState = IddObjectType(\"Construction:ComplexFenestrationState\");" << std::endl
  << "const IddObjectType IddObjectType::Construction_WindowDataFile = IddObjectType(\"Construction:WindowDataFile\");" << std::endl
  << "const IddObjectType IddObjectType::GlobalGeometryRules = IddObjectType(\"GlobalGeometryRules\");" << std::endl
  << "const IddObjectType IddObjectType::GeometryTransform = IddObjectType(\"GeometryTransform\");" << std::endl
  << "const IddObjectType IddObjectType::Zone = IddObjectType(\"Zone\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneList = IddObjectType(\"ZoneList\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneGroup = IddObjectType(\"ZoneGroup\");" << std::endl
  << "const IddObjectType IddObjectType::BuildingSurface_Detailed = IddObjectType(\"BuildingSurface:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::Wall_Detailed = IddObjectType(\"Wall:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::RoofCeiling_Detailed = IddObjectType(\"RoofCeiling:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::Floor_Detailed = IddObjectType(\"Floor:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::Wall_Exterior = IddObjectType(\"Wall:Exterior\");" << std::endl
  << "const IddObjectType IddObjectType::Wall_Adiabatic = IddObjectType(\"Wall:Adiabatic\");" << std::endl
  << "const IddObjectType IddObjectType::Wall_Underground = IddObjectType(\"Wall:Underground\");" << std::endl
  << "const IddObjectType IddObjectType::Wall_Interzone = IddObjectType(\"Wall:Interzone\");" << std::endl
  << "const IddObjectType IddObjectType::Roof = IddObjectType(\"Roof\");" << std::endl
  << "const IddObjectType IddObjectType::Ceiling_Adiabatic = IddObjectType(\"Ceiling:Adiabatic\");" << std::endl
  << "const IddObjectType IddObjectType::Ceiling_Interzone = IddObjectType(\"Ceiling:Interzone\");" << std::endl
  << "const IddObjectType IddObjectType::Floor_GroundContact = IddObjectType(\"Floor:GroundContact\");" << std::endl
  << "const IddObjectType IddObjectType::Floor_Adiabatic = IddObjectType(\"Floor:Adiabatic\");" << std::endl
  << "const IddObjectType IddObjectType::Floor_Interzone = IddObjectType(\"Floor:Interzone\");" << std::endl
  << "const IddObjectType IddObjectType::FenestrationSurface_Detailed = IddObjectType(\"FenestrationSurface:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::Window = IddObjectType(\"Window\");" << std::endl
  << "const IddObjectType IddObjectType::Door = IddObjectType(\"Door\");" << std::endl
  << "const IddObjectType IddObjectType::GlazedDoor = IddObjectType(\"GlazedDoor\");" << std::endl
  << "const IddObjectType IddObjectType::Window_Interzone = IddObjectType(\"Window:Interzone\");" << std::endl
  << "const IddObjectType IddObjectType::Door_Interzone = IddObjectType(\"Door:Interzone\");" << std::endl
  << "const IddObjectType IddObjectType::GlazedDoor_Interzone = IddObjectType(\"GlazedDoor:Interzone\");" << std::endl
  << "const IddObjectType IddObjectType::WindowProperty_ShadingControl = IddObjectType(\"WindowProperty:ShadingControl\");" << std::endl
  << "const IddObjectType IddObjectType::WindowProperty_FrameAndDivider = IddObjectType(\"WindowProperty:FrameAndDivider\");" << std::endl
  << "const IddObjectType IddObjectType::WindowProperty_AirflowControl = IddObjectType(\"WindowProperty:AirflowControl\");" << std::endl
  << "const IddObjectType IddObjectType::WindowProperty_StormWindow = IddObjectType(\"WindowProperty:StormWindow\");" << std::endl
  << "const IddObjectType IddObjectType::InternalMass = IddObjectType(\"InternalMass\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Site = IddObjectType(\"Shading:Site\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Building = IddObjectType(\"Shading:Building\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Site_Detailed = IddObjectType(\"Shading:Site:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Building_Detailed = IddObjectType(\"Shading:Building:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Overhang = IddObjectType(\"Shading:Overhang\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Overhang_Projection = IddObjectType(\"Shading:Overhang:Projection\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Fin = IddObjectType(\"Shading:Fin\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Fin_Projection = IddObjectType(\"Shading:Fin:Projection\");" << std::endl
  << "const IddObjectType IddObjectType::Shading_Zone_Detailed = IddObjectType(\"Shading:Zone:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::ShadingProperty_Reflectance = IddObjectType(\"ShadingProperty:Reflectance\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_HeatTransferAlgorithm = IddObjectType(\"SurfaceProperty:HeatTransferAlgorithm\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_HeatTransferAlgorithm_MultipleSurface = IddObjectType(\"SurfaceProperty:HeatTransferAlgorithm:MultipleSurface\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_HeatTransferAlgorithm_SurfaceList = IddObjectType(\"SurfaceProperty:HeatTransferAlgorithm:SurfaceList\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_HeatTransferAlgorithm_Construction = IddObjectType(\"SurfaceProperty:HeatTransferAlgorithm:Construction\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceControl_MovableInsulation = IddObjectType(\"SurfaceControl:MovableInsulation\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_OtherSideCoefficients = IddObjectType(\"SurfaceProperty:OtherSideCoefficients\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_OtherSideConditionsModel = IddObjectType(\"SurfaceProperty:OtherSideConditionsModel\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceConvectionAlgorithm_Inside_AdaptiveModelSelections = IddObjectType(\"SurfaceConvectionAlgorithm:Inside:AdaptiveModelSelections\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceConvectionAlgorithm_Outside_AdaptiveModelSelections = IddObjectType(\"SurfaceConvectionAlgorithm:Outside:AdaptiveModelSelections\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceConvectionAlgorithm_Inside_UserCurve = IddObjectType(\"SurfaceConvectionAlgorithm:Inside:UserCurve\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceConvectionAlgorithm_Outside_UserCurve = IddObjectType(\"SurfaceConvectionAlgorithm:Outside:UserCurve\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_ConvectionCoefficients = IddObjectType(\"SurfaceProperty:ConvectionCoefficients\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_ConvectionCoefficients_MultipleSurface = IddObjectType(\"SurfaceProperty:ConvectionCoefficients:MultipleSurface\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperties_VaporCoefficients = IddObjectType(\"SurfaceProperties:VaporCoefficients\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceProperty_ExteriorNaturalVentedCavity = IddObjectType(\"SurfaceProperty:ExteriorNaturalVentedCavity\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneProperty_UserViewFactors_bySurfaceName = IddObjectType(\"ZoneProperty:UserViewFactors:bySurfaceName\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Control = IddObjectType(\"GroundHeatTransfer:Control\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_Materials = IddObjectType(\"GroundHeatTransfer:Slab:Materials\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_MatlProps = IddObjectType(\"GroundHeatTransfer:Slab:MatlProps\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_BoundConds = IddObjectType(\"GroundHeatTransfer:Slab:BoundConds\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_BldgProps = IddObjectType(\"GroundHeatTransfer:Slab:BldgProps\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_Insulation = IddObjectType(\"GroundHeatTransfer:Slab:Insulation\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_EquivalentSlab = IddObjectType(\"GroundHeatTransfer:Slab:EquivalentSlab\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_AutoGrid = IddObjectType(\"GroundHeatTransfer:Slab:AutoGrid\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_ManualGrid = IddObjectType(\"GroundHeatTransfer:Slab:ManualGrid\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_XFACE = IddObjectType(\"GroundHeatTransfer:Slab:XFACE\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_YFACE = IddObjectType(\"GroundHeatTransfer:Slab:YFACE\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Slab_ZFACE = IddObjectType(\"GroundHeatTransfer:Slab:ZFACE\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_SimParameters = IddObjectType(\"GroundHeatTransfer:Basement:SimParameters\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_MatlProps = IddObjectType(\"GroundHeatTransfer:Basement:MatlProps\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_Insulation = IddObjectType(\"GroundHeatTransfer:Basement:Insulation\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_SurfaceProps = IddObjectType(\"GroundHeatTransfer:Basement:SurfaceProps\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_BldgData = IddObjectType(\"GroundHeatTransfer:Basement:BldgData\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_Interior = IddObjectType(\"GroundHeatTransfer:Basement:Interior\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_ComBldg = IddObjectType(\"GroundHeatTransfer:Basement:ComBldg\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_EquivSlab = IddObjectType(\"GroundHeatTransfer:Basement:EquivSlab\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_EquivAutoGrid = IddObjectType(\"GroundHeatTransfer:Basement:EquivAutoGrid\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_AutoGrid = IddObjectType(\"GroundHeatTransfer:Basement:AutoGrid\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_ManualGrid = IddObjectType(\"GroundHeatTransfer:Basement:ManualGrid\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_XFACE = IddObjectType(\"GroundHeatTransfer:Basement:XFACE\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_YFACE = IddObjectType(\"GroundHeatTransfer:Basement:YFACE\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatTransfer_Basement_ZFACE = IddObjectType(\"GroundHeatTransfer:Basement:ZFACE\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAirModelType = IddObjectType(\"RoomAirModelType\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAir_TemperaturePattern_UserDefined = IddObjectType(\"RoomAir:TemperaturePattern:UserDefined\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAir_TemperaturePattern_ConstantGradient = IddObjectType(\"RoomAir:TemperaturePattern:ConstantGradient\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAir_TemperaturePattern_TwoGradient = IddObjectType(\"RoomAir:TemperaturePattern:TwoGradient\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAir_TemperaturePattern_NondimensionalHeight = IddObjectType(\"RoomAir:TemperaturePattern:NondimensionalHeight\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAir_TemperaturePattern_SurfaceMapping = IddObjectType(\"RoomAir:TemperaturePattern:SurfaceMapping\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAir_Node = IddObjectType(\"RoomAir:Node\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAirSettings_OneNodeDisplacementVentilation = IddObjectType(\"RoomAirSettings:OneNodeDisplacementVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAirSettings_ThreeNodeDisplacementVentilation = IddObjectType(\"RoomAirSettings:ThreeNodeDisplacementVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAirSettings_CrossVentilation = IddObjectType(\"RoomAirSettings:CrossVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAirSettings_UnderFloorAirDistributionInterior = IddObjectType(\"RoomAirSettings:UnderFloorAirDistributionInterior\");" << std::endl
  << "const IddObjectType IddObjectType::RoomAirSettings_UnderFloorAirDistributionExterior = IddObjectType(\"RoomAirSettings:UnderFloorAirDistributionExterior\");" << std::endl
  << "const IddObjectType IddObjectType::People = IddObjectType(\"People\");" << std::endl
  << "const IddObjectType IddObjectType::ComfortViewFactorAngles = IddObjectType(\"ComfortViewFactorAngles\");" << std::endl
  << "const IddObjectType IddObjectType::Lights = IddObjectType(\"Lights\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricEquipment = IddObjectType(\"ElectricEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::GasEquipment = IddObjectType(\"GasEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::HotWaterEquipment = IddObjectType(\"HotWaterEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::SteamEquipment = IddObjectType(\"SteamEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::OtherEquipment = IddObjectType(\"OtherEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneBaseboard_OutdoorTemperatureControlled = IddObjectType(\"ZoneBaseboard:OutdoorTemperatureControlled\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneContaminantSourceAndSink_CarbonDioxide = IddObjectType(\"ZoneContaminantSourceAndSink:CarbonDioxide\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneContaminantSourceAndSink_Generic_Constant = IddObjectType(\"ZoneContaminantSourceAndSink:Generic:Constant\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceContaminantSourceAndSink_Generic_PressureDriven = IddObjectType(\"SurfaceContaminantSourceAndSink:Generic:PressureDriven\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneContaminantSourceAndSink_Generic_CutoffModel = IddObjectType(\"ZoneContaminantSourceAndSink:Generic:CutoffModel\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneContaminantSourceAndSink_Generic_DecaySource = IddObjectType(\"ZoneContaminantSourceAndSink:Generic:DecaySource\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceContaminantSourceAndSink_Generic_BoundaryLayerDiffusion = IddObjectType(\"SurfaceContaminantSourceAndSink:Generic:BoundaryLayerDiffusion\");" << std::endl
  << "const IddObjectType IddObjectType::SurfaceContaminantSourceAndSink_Generic_DepositionVelocitySink = IddObjectType(\"SurfaceContaminantSourceAndSink:Generic:DepositionVelocitySink\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneContaminantSourceAndSink_Generic_DepositionRateSink = IddObjectType(\"ZoneContaminantSourceAndSink:Generic:DepositionRateSink\");" << std::endl
  << "const IddObjectType IddObjectType::Daylighting_Controls = IddObjectType(\"Daylighting:Controls\");" << std::endl
  << "const IddObjectType IddObjectType::Daylighting_DELight_Controls = IddObjectType(\"Daylighting:DELight:Controls\");" << std::endl
  << "const IddObjectType IddObjectType::Daylighting_DELight_ReferencePoint = IddObjectType(\"Daylighting:DELight:ReferencePoint\");" << std::endl
  << "const IddObjectType IddObjectType::Daylighting_DELight_ComplexFenestration = IddObjectType(\"Daylighting:DELight:ComplexFenestration\");" << std::endl
  << "const IddObjectType IddObjectType::DaylightingDevice_Tubular = IddObjectType(\"DaylightingDevice:Tubular\");" << std::endl
  << "const IddObjectType IddObjectType::DaylightingDevice_Shelf = IddObjectType(\"DaylightingDevice:Shelf\");" << std::endl
  << "const IddObjectType IddObjectType::DaylightingDevice_LightWell = IddObjectType(\"DaylightingDevice:LightWell\");" << std::endl
  << "const IddObjectType IddObjectType::Output_DaylightFactors = IddObjectType(\"Output:DaylightFactors\");" << std::endl
  << "const IddObjectType IddObjectType::Output_IlluminanceMap = IddObjectType(\"Output:IlluminanceMap\");" << std::endl
  << "const IddObjectType IddObjectType::OutputControl_IlluminanceMap_Style = IddObjectType(\"OutputControl:IlluminanceMap:Style\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneInfiltration_DesignFlowRate = IddObjectType(\"ZoneInfiltration:DesignFlowRate\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneInfiltration_EffectiveLeakageArea = IddObjectType(\"ZoneInfiltration:EffectiveLeakageArea\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneInfiltration_FlowCoefficient = IddObjectType(\"ZoneInfiltration:FlowCoefficient\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneVentilation_DesignFlowRate = IddObjectType(\"ZoneVentilation:DesignFlowRate\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneVentilation_WindandStackOpenArea = IddObjectType(\"ZoneVentilation:WindandStackOpenArea\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneAirBalance_OutdoorAir = IddObjectType(\"ZoneAirBalance:OutdoorAir\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneMixing = IddObjectType(\"ZoneMixing\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneCrossMixing = IddObjectType(\"ZoneCrossMixing\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneRefrigerationDoorMixing = IddObjectType(\"ZoneRefrigerationDoorMixing\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneEarthtube = IddObjectType(\"ZoneEarthtube\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneCoolTower_Shower = IddObjectType(\"ZoneCoolTower:Shower\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneThermalChimney = IddObjectType(\"ZoneThermalChimney\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_SimulationControl = IddObjectType(\"AirflowNetwork:SimulationControl\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Zone = IddObjectType(\"AirflowNetwork:MultiZone:Zone\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Surface = IddObjectType(\"AirflowNetwork:MultiZone:Surface\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_ReferenceCrackConditions = IddObjectType(\"AirflowNetwork:MultiZone:ReferenceCrackConditions\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Surface_Crack = IddObjectType(\"AirflowNetwork:MultiZone:Surface:Crack\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Surface_EffectiveLeakageArea = IddObjectType(\"AirflowNetwork:MultiZone:Surface:EffectiveLeakageArea\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Component_DetailedOpening = IddObjectType(\"AirflowNetwork:MultiZone:Component:DetailedOpening\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Component_SimpleOpening = IddObjectType(\"AirflowNetwork:MultiZone:Component:SimpleOpening\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Component_HorizontalOpening = IddObjectType(\"AirflowNetwork:MultiZone:Component:HorizontalOpening\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_Component_ZoneExhaustFan = IddObjectType(\"AirflowNetwork:MultiZone:Component:ZoneExhaustFan\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_ExternalNode = IddObjectType(\"AirflowNetwork:MultiZone:ExternalNode\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_WindPressureCoefficientArray = IddObjectType(\"AirflowNetwork:MultiZone:WindPressureCoefficientArray\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_MultiZone_WindPressureCoefficientValues = IddObjectType(\"AirflowNetwork:MultiZone:WindPressureCoefficientValues\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Node = IddObjectType(\"AirflowNetwork:Distribution:Node\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_Leak = IddObjectType(\"AirflowNetwork:Distribution:Component:Leak\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_LeakageRatio = IddObjectType(\"AirflowNetwork:Distribution:Component:LeakageRatio\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_Duct = IddObjectType(\"AirflowNetwork:Distribution:Component:Duct\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_Fan = IddObjectType(\"AirflowNetwork:Distribution:Component:Fan\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_Coil = IddObjectType(\"AirflowNetwork:Distribution:Component:Coil\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_HeatExchanger = IddObjectType(\"AirflowNetwork:Distribution:Component:HeatExchanger\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_TerminalUnit = IddObjectType(\"AirflowNetwork:Distribution:Component:TerminalUnit\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Component_ConstantPressureDrop = IddObjectType(\"AirflowNetwork:Distribution:Component:ConstantPressureDrop\");" << std::endl
  << "const IddObjectType IddObjectType::AirflowNetwork_Distribution_Linkage = IddObjectType(\"AirflowNetwork:Distribution:Linkage\");" << std::endl
  << "const IddObjectType IddObjectType::Exterior_Lights = IddObjectType(\"Exterior:Lights\");" << std::endl
  << "const IddObjectType IddObjectType::Exterior_FuelEquipment = IddObjectType(\"Exterior:FuelEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::Exterior_WaterEquipment = IddObjectType(\"Exterior:WaterEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Thermostat = IddObjectType(\"HVACTemplate:Thermostat\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_IdealLoadsAirSystem = IddObjectType(\"HVACTemplate:Zone:IdealLoadsAirSystem\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_FanCoil = IddObjectType(\"HVACTemplate:Zone:FanCoil\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_PTAC = IddObjectType(\"HVACTemplate:Zone:PTAC\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_PTHP = IddObjectType(\"HVACTemplate:Zone:PTHP\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_Unitary = IddObjectType(\"HVACTemplate:Zone:Unitary\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_VAV = IddObjectType(\"HVACTemplate:Zone:VAV\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_VAV_FanPowered = IddObjectType(\"HVACTemplate:Zone:VAV:FanPowered\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Zone_WaterToAirHeatPump = IddObjectType(\"HVACTemplate:Zone:WaterToAirHeatPump\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_System_Unitary = IddObjectType(\"HVACTemplate:System:Unitary\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_System_UnitaryHeatPump_AirToAir = IddObjectType(\"HVACTemplate:System:UnitaryHeatPump:AirToAir\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_System_VAV = IddObjectType(\"HVACTemplate:System:VAV\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_System_PackagedVAV = IddObjectType(\"HVACTemplate:System:PackagedVAV\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_System_DedicatedOutdoorAir = IddObjectType(\"HVACTemplate:System:DedicatedOutdoorAir\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_ChilledWaterLoop = IddObjectType(\"HVACTemplate:Plant:ChilledWaterLoop\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_Chiller = IddObjectType(\"HVACTemplate:Plant:Chiller\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_Chiller_ObjectReference = IddObjectType(\"HVACTemplate:Plant:Chiller:ObjectReference\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_Tower = IddObjectType(\"HVACTemplate:Plant:Tower\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_Tower_ObjectReference = IddObjectType(\"HVACTemplate:Plant:Tower:ObjectReference\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_HotWaterLoop = IddObjectType(\"HVACTemplate:Plant:HotWaterLoop\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_Boiler = IddObjectType(\"HVACTemplate:Plant:Boiler\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_Boiler_ObjectReference = IddObjectType(\"HVACTemplate:Plant:Boiler:ObjectReference\");" << std::endl
  << "const IddObjectType IddObjectType::HVACTemplate_Plant_MixedWaterLoop = IddObjectType(\"HVACTemplate:Plant:MixedWaterLoop\");" << std::endl
  << "const IddObjectType IddObjectType::DesignSpecification_OutdoorAir = IddObjectType(\"DesignSpecification:OutdoorAir\");" << std::endl
  << "const IddObjectType IddObjectType::DesignSpecification_ZoneAirDistribution = IddObjectType(\"DesignSpecification:ZoneAirDistribution\");" << std::endl
  << "const IddObjectType IddObjectType::Sizing_Parameters = IddObjectType(\"Sizing:Parameters\");" << std::endl
  << "const IddObjectType IddObjectType::Sizing_Zone = IddObjectType(\"Sizing:Zone\");" << std::endl
  << "const IddObjectType IddObjectType::Sizing_System = IddObjectType(\"Sizing:System\");" << std::endl
  << "const IddObjectType IddObjectType::Sizing_Plant = IddObjectType(\"Sizing:Plant\");" << std::endl
  << "const IddObjectType IddObjectType::OutputControl_Sizing_Style = IddObjectType(\"OutputControl:Sizing:Style\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneControl_Humidistat = IddObjectType(\"ZoneControl:Humidistat\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneControl_Thermostat = IddObjectType(\"ZoneControl:Thermostat\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneControl_Thermostat_OperativeTemperature = IddObjectType(\"ZoneControl:Thermostat:OperativeTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneControl_Thermostat_ThermalComfort = IddObjectType(\"ZoneControl:Thermostat:ThermalComfort\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneControl_Thermostat_TemperatureAndHumidity = IddObjectType(\"ZoneControl:Thermostat:TemperatureAndHumidity\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_SingleHeating = IddObjectType(\"ThermostatSetpoint:SingleHeating\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_SingleCooling = IddObjectType(\"ThermostatSetpoint:SingleCooling\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_SingleHeatingOrCooling = IddObjectType(\"ThermostatSetpoint:SingleHeatingOrCooling\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_DualSetpoint = IddObjectType(\"ThermostatSetpoint:DualSetpoint\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_ThermalComfort_Fanger_SingleHeating = IddObjectType(\"ThermostatSetpoint:ThermalComfort:Fanger:SingleHeating\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_ThermalComfort_Fanger_SingleCooling = IddObjectType(\"ThermostatSetpoint:ThermalComfort:Fanger:SingleCooling\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_ThermalComfort_Fanger_SingleHeatingOrCooling = IddObjectType(\"ThermostatSetpoint:ThermalComfort:Fanger:SingleHeatingOrCooling\");" << std::endl
  << "const IddObjectType IddObjectType::ThermostatSetpoint_ThermalComfort_Fanger_DualSetpoint = IddObjectType(\"ThermostatSetpoint:ThermalComfort:Fanger:DualSetpoint\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneControl_ContaminantController = IddObjectType(\"ZoneControl:ContaminantController\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_IdealLoadsAirSystem = IddObjectType(\"ZoneHVAC:IdealLoadsAirSystem\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_FourPipeFanCoil = IddObjectType(\"ZoneHVAC:FourPipeFanCoil\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_WindowAirConditioner = IddObjectType(\"ZoneHVAC:WindowAirConditioner\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_PackagedTerminalAirConditioner = IddObjectType(\"ZoneHVAC:PackagedTerminalAirConditioner\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_PackagedTerminalHeatPump = IddObjectType(\"ZoneHVAC:PackagedTerminalHeatPump\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_WaterToAirHeatPump = IddObjectType(\"ZoneHVAC:WaterToAirHeatPump\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_Dehumidifier_DX = IddObjectType(\"ZoneHVAC:Dehumidifier:DX\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_EnergyRecoveryVentilator = IddObjectType(\"ZoneHVAC:EnergyRecoveryVentilator\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_EnergyRecoveryVentilator_Controller = IddObjectType(\"ZoneHVAC:EnergyRecoveryVentilator:Controller\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_UnitVentilator = IddObjectType(\"ZoneHVAC:UnitVentilator\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_UnitHeater = IddObjectType(\"ZoneHVAC:UnitHeater\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_OutdoorAirUnit = IddObjectType(\"ZoneHVAC:OutdoorAirUnit\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_OutdoorAirUnit_EquipmentList = IddObjectType(\"ZoneHVAC:OutdoorAirUnit:EquipmentList\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow = IddObjectType(\"ZoneHVAC:TerminalUnit:VariableRefrigerantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Water = IddObjectType(\"ZoneHVAC:Baseboard:RadiantConvective:Water\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Steam = IddObjectType(\"ZoneHVAC:Baseboard:RadiantConvective:Steam\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Electric = IddObjectType(\"ZoneHVAC:Baseboard:RadiantConvective:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_Baseboard_Convective_Water = IddObjectType(\"ZoneHVAC:Baseboard:Convective:Water\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_Baseboard_Convective_Electric = IddObjectType(\"ZoneHVAC:Baseboard:Convective:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow = IddObjectType(\"ZoneHVAC:LowTemperatureRadiant:VariableFlow\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_LowTemperatureRadiant_ConstantFlow = IddObjectType(\"ZoneHVAC:LowTemperatureRadiant:ConstantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_LowTemperatureRadiant_Electric = IddObjectType(\"ZoneHVAC:LowTemperatureRadiant:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_LowTemperatureRadiant_SurfaceGroup = IddObjectType(\"ZoneHVAC:LowTemperatureRadiant:SurfaceGroup\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_HighTemperatureRadiant = IddObjectType(\"ZoneHVAC:HighTemperatureRadiant\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_VentilatedSlab = IddObjectType(\"ZoneHVAC:VentilatedSlab\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_VentilatedSlab_SlabGroup = IddObjectType(\"ZoneHVAC:VentilatedSlab:SlabGroup\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_Uncontrolled = IddObjectType(\"AirTerminal:SingleDuct:Uncontrolled\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_ConstantVolume_Reheat = IddObjectType(\"AirTerminal:SingleDuct:ConstantVolume:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_VAV_NoReheat = IddObjectType(\"AirTerminal:SingleDuct:VAV:NoReheat\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_VAV_Reheat = IddObjectType(\"AirTerminal:SingleDuct:VAV:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_VAV_Reheat_VariableSpeedFan = IddObjectType(\"AirTerminal:SingleDuct:VAV:Reheat:VariableSpeedFan\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat = IddObjectType(\"AirTerminal:SingleDuct:VAV:HeatAndCool:NoReheat\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat = IddObjectType(\"AirTerminal:SingleDuct:VAV:HeatAndCool:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_SeriesPIU_Reheat = IddObjectType(\"AirTerminal:SingleDuct:SeriesPIU:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_ParallelPIU_Reheat = IddObjectType(\"AirTerminal:SingleDuct:ParallelPIU:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction = IddObjectType(\"AirTerminal:SingleDuct:ConstantVolume:FourPipeInduction\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_ConstantVolume_CooledBeam = IddObjectType(\"AirTerminal:SingleDuct:ConstantVolume:CooledBeam\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_DualDuct_ConstantVolume = IddObjectType(\"AirTerminal:DualDuct:ConstantVolume\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_DualDuct_VAV = IddObjectType(\"AirTerminal:DualDuct:VAV\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_DualDuct_VAV_OutdoorAir = IddObjectType(\"AirTerminal:DualDuct:VAV:OutdoorAir\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_AirDistributionUnit = IddObjectType(\"ZoneHVAC:AirDistributionUnit\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_EquipmentList = IddObjectType(\"ZoneHVAC:EquipmentList\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_EquipmentConnections = IddObjectType(\"ZoneHVAC:EquipmentConnections\");" << std::endl
  << "const IddObjectType IddObjectType::Fan_ConstantVolume = IddObjectType(\"Fan:ConstantVolume\");" << std::endl
  << "const IddObjectType IddObjectType::Fan_VariableVolume = IddObjectType(\"Fan:VariableVolume\");" << std::endl
  << "const IddObjectType IddObjectType::Fan_OnOff = IddObjectType(\"Fan:OnOff\");" << std::endl
  << "const IddObjectType IddObjectType::Fan_ZoneExhaust = IddObjectType(\"Fan:ZoneExhaust\");" << std::endl
  << "const IddObjectType IddObjectType::FanPerformance_NightVentilation = IddObjectType(\"FanPerformance:NightVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::Fan_ComponentModel = IddObjectType(\"Fan:ComponentModel\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_Water = IddObjectType(\"Coil:Cooling:Water\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_Water_DetailedGeometry = IddObjectType(\"Coil:Cooling:Water:DetailedGeometry\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_DX_SingleSpeed = IddObjectType(\"Coil:Cooling:DX:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_DX_TwoSpeed = IddObjectType(\"Coil:Cooling:DX:TwoSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_DX_MultiSpeed = IddObjectType(\"Coil:Cooling:DX:MultiSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_DX_VariableSpeed = IddObjectType(\"Coil:Cooling:DX:VariableSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode = IddObjectType(\"Coil:Cooling:DX:TwoStageWithHumidityControlMode\");" << std::endl
  << "const IddObjectType IddObjectType::CoilPerformance_DX_Cooling = IddObjectType(\"CoilPerformance:DX:Cooling\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow = IddObjectType(\"Coil:Cooling:DX:VariableRefrigerantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow = IddObjectType(\"Coil:Heating:DX:VariableRefrigerantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_Water = IddObjectType(\"Coil:Heating:Water\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_Steam = IddObjectType(\"Coil:Heating:Steam\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_Electric = IddObjectType(\"Coil:Heating:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_Electric_MultiStage = IddObjectType(\"Coil:Heating:Electric:MultiStage\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_Gas = IddObjectType(\"Coil:Heating:Gas\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_Gas_MultiStage = IddObjectType(\"Coil:Heating:Gas:MultiStage\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_Desuperheater = IddObjectType(\"Coil:Heating:Desuperheater\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_DX_SingleSpeed = IddObjectType(\"Coil:Heating:DX:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_DX_MultiSpeed = IddObjectType(\"Coil:Heating:DX:MultiSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_DX_VariableSpeed = IddObjectType(\"Coil:Heating:DX:VariableSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_WaterToAirHeatPump_ParameterEstimation = IddObjectType(\"Coil:Cooling:WaterToAirHeatPump:ParameterEstimation\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_WaterToAirHeatPump_ParameterEstimation = IddObjectType(\"Coil:Heating:WaterToAirHeatPump:ParameterEstimation\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit = IddObjectType(\"Coil:Cooling:WaterToAirHeatPump:EquationFit\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit = IddObjectType(\"Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit = IddObjectType(\"Coil:Heating:WaterToAirHeatPump:EquationFit\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit = IddObjectType(\"Coil:Heating:WaterToAirHeatPump:VariableSpeedEquationFit\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_WaterHeating_AirToWaterHeatPump = IddObjectType(\"Coil:WaterHeating:AirToWaterHeatPump\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_WaterHeating_Desuperheater = IddObjectType(\"Coil:WaterHeating:Desuperheater\");" << std::endl
  << "const IddObjectType IddObjectType::CoilSystem_Cooling_DX = IddObjectType(\"CoilSystem:Cooling:DX\");" << std::endl
  << "const IddObjectType IddObjectType::CoilSystem_Heating_DX = IddObjectType(\"CoilSystem:Heating:DX\");" << std::endl
  << "const IddObjectType IddObjectType::CoilSystem_Cooling_Water_HeatExchangerAssisted = IddObjectType(\"CoilSystem:Cooling:Water:HeatExchangerAssisted\");" << std::endl
  << "const IddObjectType IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted = IddObjectType(\"CoilSystem:Cooling:DX:HeatExchangerAssisted\");" << std::endl
  << "const IddObjectType IddObjectType::EvaporativeCooler_Direct_CelDekPad = IddObjectType(\"EvaporativeCooler:Direct:CelDekPad\");" << std::endl
  << "const IddObjectType IddObjectType::EvaporativeCooler_Indirect_CelDekPad = IddObjectType(\"EvaporativeCooler:Indirect:CelDekPad\");" << std::endl
  << "const IddObjectType IddObjectType::EvaporativeCooler_Indirect_WetCoil = IddObjectType(\"EvaporativeCooler:Indirect:WetCoil\");" << std::endl
  << "const IddObjectType IddObjectType::EvaporativeCooler_Indirect_ResearchSpecial = IddObjectType(\"EvaporativeCooler:Indirect:ResearchSpecial\");" << std::endl
  << "const IddObjectType IddObjectType::EvaporativeCooler_Direct_ResearchSpecial = IddObjectType(\"EvaporativeCooler:Direct:ResearchSpecial\");" << std::endl
  << "const IddObjectType IddObjectType::Humidifier_Steam_Electric = IddObjectType(\"Humidifier:Steam:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::Dehumidifier_Desiccant_NoFans = IddObjectType(\"Dehumidifier:Desiccant:NoFans\");" << std::endl
  << "const IddObjectType IddObjectType::Dehumidifier_Desiccant_System = IddObjectType(\"Dehumidifier:Desiccant:System\");" << std::endl
  << "const IddObjectType IddObjectType::HeatExchanger_AirToAir_FlatPlate = IddObjectType(\"HeatExchanger:AirToAir:FlatPlate\");" << std::endl
  << "const IddObjectType IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent = IddObjectType(\"HeatExchanger:AirToAir:SensibleAndLatent\");" << std::endl
  << "const IddObjectType IddObjectType::HeatExchanger_Desiccant_BalancedFlow = IddObjectType(\"HeatExchanger:Desiccant:BalancedFlow\");" << std::endl
  << "const IddObjectType IddObjectType::HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1 = IddObjectType(\"HeatExchanger:Desiccant:BalancedFlow:PerformanceDataType1\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_Unitary_Furnace_HeatOnly = IddObjectType(\"AirLoopHVAC:Unitary:Furnace:HeatOnly\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_Unitary_Furnace_HeatCool = IddObjectType(\"AirLoopHVAC:Unitary:Furnace:HeatCool\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_UnitaryHeatOnly = IddObjectType(\"AirLoopHVAC:UnitaryHeatOnly\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_UnitaryHeatCool = IddObjectType(\"AirLoopHVAC:UnitaryHeatCool\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir = IddObjectType(\"AirLoopHVAC:UnitaryHeatPump:AirToAir\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_UnitaryHeatPump_WaterToAir = IddObjectType(\"AirLoopHVAC:UnitaryHeatPump:WaterToAir\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass = IddObjectType(\"AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed = IddObjectType(\"AirLoopHVAC:UnitaryHeatPump:AirToAir:MultiSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::AirConditioner_VariableRefrigerantFlow = IddObjectType(\"AirConditioner:VariableRefrigerantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneTerminalUnitList = IddObjectType(\"ZoneTerminalUnitList\");" << std::endl
  << "const IddObjectType IddObjectType::Controller_WaterCoil = IddObjectType(\"Controller:WaterCoil\");" << std::endl
  << "const IddObjectType IddObjectType::Controller_OutdoorAir = IddObjectType(\"Controller:OutdoorAir\");" << std::endl
  << "const IddObjectType IddObjectType::Controller_MechanicalVentilation = IddObjectType(\"Controller:MechanicalVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_ControllerList = IddObjectType(\"AirLoopHVAC:ControllerList\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC = IddObjectType(\"AirLoopHVAC\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_OutdoorAirSystem_EquipmentList = IddObjectType(\"AirLoopHVAC:OutdoorAirSystem:EquipmentList\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_OutdoorAirSystem = IddObjectType(\"AirLoopHVAC:OutdoorAirSystem\");" << std::endl
  << "const IddObjectType IddObjectType::OutdoorAir_Mixer = IddObjectType(\"OutdoorAir:Mixer\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_ZoneSplitter = IddObjectType(\"AirLoopHVAC:ZoneSplitter\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_SupplyPlenum = IddObjectType(\"AirLoopHVAC:SupplyPlenum\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_SupplyPath = IddObjectType(\"AirLoopHVAC:SupplyPath\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_ZoneMixer = IddObjectType(\"AirLoopHVAC:ZoneMixer\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_ReturnPlenum = IddObjectType(\"AirLoopHVAC:ReturnPlenum\");" << std::endl
  << "const IddObjectType IddObjectType::AirLoopHVAC_ReturnPath = IddObjectType(\"AirLoopHVAC:ReturnPath\");" << std::endl
  << "const IddObjectType IddObjectType::Branch = IddObjectType(\"Branch\");" << std::endl
  << "const IddObjectType IddObjectType::BranchList = IddObjectType(\"BranchList\");" << std::endl
  << "const IddObjectType IddObjectType::Connector_Splitter = IddObjectType(\"Connector:Splitter\");" << std::endl
  << "const IddObjectType IddObjectType::Connector_Mixer = IddObjectType(\"Connector:Mixer\");" << std::endl
  << "const IddObjectType IddObjectType::ConnectorList = IddObjectType(\"ConnectorList\");" << std::endl
  << "const IddObjectType IddObjectType::NodeList = IddObjectType(\"NodeList\");" << std::endl
  << "const IddObjectType IddObjectType::OutdoorAir_Node = IddObjectType(\"OutdoorAir:Node\");" << std::endl
  << "const IddObjectType IddObjectType::OutdoorAir_NodeList = IddObjectType(\"OutdoorAir:NodeList\");" << std::endl
  << "const IddObjectType IddObjectType::Pipe_Adiabatic = IddObjectType(\"Pipe:Adiabatic\");" << std::endl
  << "const IddObjectType IddObjectType::Pipe_Adiabatic_Steam = IddObjectType(\"Pipe:Adiabatic:Steam\");" << std::endl
  << "const IddObjectType IddObjectType::Pipe_Indoor = IddObjectType(\"Pipe:Indoor\");" << std::endl
  << "const IddObjectType IddObjectType::Pipe_Outdoor = IddObjectType(\"Pipe:Outdoor\");" << std::endl
  << "const IddObjectType IddObjectType::Pipe_Underground = IddObjectType(\"Pipe:Underground\");" << std::endl
  << "const IddObjectType IddObjectType::PipingSystem_Underground_Domain = IddObjectType(\"PipingSystem:Underground:Domain\");" << std::endl
  << "const IddObjectType IddObjectType::PipingSystem_Underground_PipeCircuit = IddObjectType(\"PipingSystem:Underground:PipeCircuit\");" << std::endl
  << "const IddObjectType IddObjectType::PipingSystem_Underground_PipeSegment = IddObjectType(\"PipingSystem:Underground:PipeSegment\");" << std::endl
  << "const IddObjectType IddObjectType::Duct = IddObjectType(\"Duct\");" << std::endl
  << "const IddObjectType IddObjectType::Pump_VariableSpeed = IddObjectType(\"Pump:VariableSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Pump_ConstantSpeed = IddObjectType(\"Pump:ConstantSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::Pump_VariableSpeed_Condensate = IddObjectType(\"Pump:VariableSpeed:Condensate\");" << std::endl
  << "const IddObjectType IddObjectType::HeaderedPumps_ConstantSpeed = IddObjectType(\"HeaderedPumps:ConstantSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::HeaderedPumps_VariableSpeed = IddObjectType(\"HeaderedPumps:VariableSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::TemperingValve = IddObjectType(\"TemperingValve\");" << std::endl
  << "const IddObjectType IddObjectType::LoadProfile_Plant = IddObjectType(\"LoadProfile:Plant\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollectorPerformance_FlatPlate = IddObjectType(\"SolarCollectorPerformance:FlatPlate\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollector_FlatPlate_Water = IddObjectType(\"SolarCollector:FlatPlate:Water\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollector_FlatPlate_PhotovoltaicThermal = IddObjectType(\"SolarCollector:FlatPlate:PhotovoltaicThermal\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollector_IntegralCollectorStorage = IddObjectType(\"SolarCollector:IntegralCollectorStorage\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollectorPerformance_IntegralCollectorStorage = IddObjectType(\"SolarCollectorPerformance:IntegralCollectorStorage\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_Simple = IddObjectType(\"SolarCollectorPerformance:PhotovoltaicThermal:Simple\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollector_UnglazedTranspired = IddObjectType(\"SolarCollector:UnglazedTranspired\");" << std::endl
  << "const IddObjectType IddObjectType::SolarCollector_UnglazedTranspired_Multisystem = IddObjectType(\"SolarCollector:UnglazedTranspired:Multisystem\");" << std::endl
  << "const IddObjectType IddObjectType::Boiler_HotWater = IddObjectType(\"Boiler:HotWater\");" << std::endl
  << "const IddObjectType IddObjectType::Boiler_Steam = IddObjectType(\"Boiler:Steam\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_Electric_EIR = IddObjectType(\"Chiller:Electric:EIR\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_Electric_ReformulatedEIR = IddObjectType(\"Chiller:Electric:ReformulatedEIR\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_Electric = IddObjectType(\"Chiller:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_Absorption_Indirect = IddObjectType(\"Chiller:Absorption:Indirect\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_Absorption = IddObjectType(\"Chiller:Absorption\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_ConstantCOP = IddObjectType(\"Chiller:ConstantCOP\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_EngineDriven = IddObjectType(\"Chiller:EngineDriven\");" << std::endl
  << "const IddObjectType IddObjectType::Chiller_CombustionTurbine = IddObjectType(\"Chiller:CombustionTurbine\");" << std::endl
  << "const IddObjectType IddObjectType::ChillerHeater_Absorption_DirectFired = IddObjectType(\"ChillerHeater:Absorption:DirectFired\");" << std::endl
  << "const IddObjectType IddObjectType::ChillerHeater_Absorption_DoubleEffect = IddObjectType(\"ChillerHeater:Absorption:DoubleEffect\");" << std::endl
  << "const IddObjectType IddObjectType::HeatPump_WaterToWater_EquationFit_Heating = IddObjectType(\"HeatPump:WaterToWater:EquationFit:Heating\");" << std::endl
  << "const IddObjectType IddObjectType::HeatPump_WaterToWater_EquationFit_Cooling = IddObjectType(\"HeatPump:WaterToWater:EquationFit:Cooling\");" << std::endl
  << "const IddObjectType IddObjectType::HeatPump_WaterToWater_ParameterEstimation_Cooling = IddObjectType(\"HeatPump:WaterToWater:ParameterEstimation:Cooling\");" << std::endl
  << "const IddObjectType IddObjectType::HeatPump_WaterToWater_ParameterEstimation_Heating = IddObjectType(\"HeatPump:WaterToWater:ParameterEstimation:Heating\");" << std::endl
  << "const IddObjectType IddObjectType::DistrictCooling = IddObjectType(\"DistrictCooling\");" << std::endl
  << "const IddObjectType IddObjectType::DistrictHeating = IddObjectType(\"DistrictHeating\");" << std::endl
  << "const IddObjectType IddObjectType::PlantComponent_TemperatureSource = IddObjectType(\"PlantComponent:TemperatureSource\");" << std::endl
  << "const IddObjectType IddObjectType::CentralHeatPumpSystem = IddObjectType(\"CentralHeatPumpSystem\");" << std::endl
  << "const IddObjectType IddObjectType::ChillerHeaterPerformance_Electric_EIR = IddObjectType(\"ChillerHeaterPerformance:Electric:EIR\");" << std::endl
  << "const IddObjectType IddObjectType::CoolingTower_SingleSpeed = IddObjectType(\"CoolingTower:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::CoolingTower_TwoSpeed = IddObjectType(\"CoolingTower:TwoSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::CoolingTower_VariableSpeed = IddObjectType(\"CoolingTower:VariableSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::CoolingTowerPerformance_CoolTools = IddObjectType(\"CoolingTowerPerformance:CoolTools\");" << std::endl
  << "const IddObjectType IddObjectType::CoolingTowerPerformance_YorkCalc = IddObjectType(\"CoolingTowerPerformance:YorkCalc\");" << std::endl
  << "const IddObjectType IddObjectType::EvaporativeFluidCooler_SingleSpeed = IddObjectType(\"EvaporativeFluidCooler:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::EvaporativeFluidCooler_TwoSpeed = IddObjectType(\"EvaporativeFluidCooler:TwoSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::FluidCooler_SingleSpeed = IddObjectType(\"FluidCooler:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::FluidCooler_TwoSpeed = IddObjectType(\"FluidCooler:TwoSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatExchanger_Vertical = IddObjectType(\"GroundHeatExchanger:Vertical\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatExchanger_Pond = IddObjectType(\"GroundHeatExchanger:Pond\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatExchanger_Surface = IddObjectType(\"GroundHeatExchanger:Surface\");" << std::endl
  << "const IddObjectType IddObjectType::GroundHeatExchanger_HorizontalTrench = IddObjectType(\"GroundHeatExchanger:HorizontalTrench\");" << std::endl
  << "const IddObjectType IddObjectType::HeatExchanger_FluidToFluid = IddObjectType(\"HeatExchanger:FluidToFluid\");" << std::endl
  << "const IddObjectType IddObjectType::WaterHeater_Mixed = IddObjectType(\"WaterHeater:Mixed\");" << std::endl
  << "const IddObjectType IddObjectType::WaterHeater_Stratified = IddObjectType(\"WaterHeater:Stratified\");" << std::endl
  << "const IddObjectType IddObjectType::WaterHeater_Sizing = IddObjectType(\"WaterHeater:Sizing\");" << std::endl
  << "const IddObjectType IddObjectType::WaterHeater_HeatPump = IddObjectType(\"WaterHeater:HeatPump\");" << std::endl
  << "const IddObjectType IddObjectType::ThermalStorage_Ice_Simple = IddObjectType(\"ThermalStorage:Ice:Simple\");" << std::endl
  << "const IddObjectType IddObjectType::ThermalStorage_Ice_Detailed = IddObjectType(\"ThermalStorage:Ice:Detailed\");" << std::endl
  << "const IddObjectType IddObjectType::ThermalStorage_ChilledWater_Mixed = IddObjectType(\"ThermalStorage:ChilledWater:Mixed\");" << std::endl
  << "const IddObjectType IddObjectType::ThermalStorage_ChilledWater_Stratified = IddObjectType(\"ThermalStorage:ChilledWater:Stratified\");" << std::endl
  << "const IddObjectType IddObjectType::PlantLoop = IddObjectType(\"PlantLoop\");" << std::endl
  << "const IddObjectType IddObjectType::CondenserLoop = IddObjectType(\"CondenserLoop\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentList = IddObjectType(\"PlantEquipmentList\");" << std::endl
  << "const IddObjectType IddObjectType::CondenserEquipmentList = IddObjectType(\"CondenserEquipmentList\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_Uncontrolled = IddObjectType(\"PlantEquipmentOperation:Uncontrolled\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_CoolingLoad = IddObjectType(\"PlantEquipmentOperation:CoolingLoad\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_HeatingLoad = IddObjectType(\"PlantEquipmentOperation:HeatingLoad\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_OutdoorDryBulb = IddObjectType(\"PlantEquipmentOperation:OutdoorDryBulb\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_OutdoorWetBulb = IddObjectType(\"PlantEquipmentOperation:OutdoorWetBulb\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_OutdoorRelativeHumidity = IddObjectType(\"PlantEquipmentOperation:OutdoorRelativeHumidity\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_OutdoorDewpoint = IddObjectType(\"PlantEquipmentOperation:OutdoorDewpoint\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_ComponentSetpoint = IddObjectType(\"PlantEquipmentOperation:ComponentSetpoint\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_OutdoorDryBulbDifference = IddObjectType(\"PlantEquipmentOperation:OutdoorDryBulbDifference\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_OutdoorWetBulbDifference = IddObjectType(\"PlantEquipmentOperation:OutdoorWetBulbDifference\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperation_OutdoorDewpointDifference = IddObjectType(\"PlantEquipmentOperation:OutdoorDewpointDifference\");" << std::endl
  << "const IddObjectType IddObjectType::PlantEquipmentOperationSchemes = IddObjectType(\"PlantEquipmentOperationSchemes\");" << std::endl
  << "const IddObjectType IddObjectType::CondenserEquipmentOperationSchemes = IddObjectType(\"CondenserEquipmentOperationSchemes\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_Sensor = IddObjectType(\"EnergyManagementSystem:Sensor\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_Actuator = IddObjectType(\"EnergyManagementSystem:Actuator\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_ProgramCallingManager = IddObjectType(\"EnergyManagementSystem:ProgramCallingManager\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_Program = IddObjectType(\"EnergyManagementSystem:Program\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_Subroutine = IddObjectType(\"EnergyManagementSystem:Subroutine\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_GlobalVariable = IddObjectType(\"EnergyManagementSystem:GlobalVariable\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_OutputVariable = IddObjectType(\"EnergyManagementSystem:OutputVariable\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_MeteredOutputVariable = IddObjectType(\"EnergyManagementSystem:MeteredOutputVariable\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_TrendVariable = IddObjectType(\"EnergyManagementSystem:TrendVariable\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_InternalVariable = IddObjectType(\"EnergyManagementSystem:InternalVariable\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable = IddObjectType(\"EnergyManagementSystem:CurveOrTableIndexVariable\");" << std::endl
  << "const IddObjectType IddObjectType::EnergyManagementSystem_ConstructionIndexVariable = IddObjectType(\"EnergyManagementSystem:ConstructionIndexVariable\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface = IddObjectType(\"ExternalInterface\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_Schedule = IddObjectType(\"ExternalInterface:Schedule\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_Variable = IddObjectType(\"ExternalInterface:Variable\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_Actuator = IddObjectType(\"ExternalInterface:Actuator\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_FunctionalMockupUnitImport = IddObjectType(\"ExternalInterface:FunctionalMockupUnitImport\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_FunctionalMockupUnitImport_From_Variable = IddObjectType(\"ExternalInterface:FunctionalMockupUnitImport:From:Variable\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Schedule = IddObjectType(\"ExternalInterface:FunctionalMockupUnitImport:To:Schedule\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Actuator = IddObjectType(\"ExternalInterface:FunctionalMockupUnitImport:To:Actuator\");" << std::endl
  << "const IddObjectType IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Variable = IddObjectType(\"ExternalInterface:FunctionalMockupUnitImport:To:Variable\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_ForcedAir_UserDefined = IddObjectType(\"ZoneHVAC:ForcedAir:UserDefined\");" << std::endl
  << "const IddObjectType IddObjectType::AirTerminal_SingleDuct_UserDefined = IddObjectType(\"AirTerminal:SingleDuct:UserDefined\");" << std::endl
  << "const IddObjectType IddObjectType::Coil_UserDefined = IddObjectType(\"Coil:UserDefined\");" << std::endl
  << "const IddObjectType IddObjectType::PlantComponent_UserDefined = IddObjectType(\"PlantComponent:UserDefined\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_Scheduled = IddObjectType(\"AvailabilityManager:Scheduled\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_ScheduledOn = IddObjectType(\"AvailabilityManager:ScheduledOn\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_ScheduledOff = IddObjectType(\"AvailabilityManager:ScheduledOff\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_NightCycle = IddObjectType(\"AvailabilityManager:NightCycle\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_DifferentialThermostat = IddObjectType(\"AvailabilityManager:DifferentialThermostat\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_HighTemperatureTurnOff = IddObjectType(\"AvailabilityManager:HighTemperatureTurnOff\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_HighTemperatureTurnOn = IddObjectType(\"AvailabilityManager:HighTemperatureTurnOn\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_LowTemperatureTurnOff = IddObjectType(\"AvailabilityManager:LowTemperatureTurnOff\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_LowTemperatureTurnOn = IddObjectType(\"AvailabilityManager:LowTemperatureTurnOn\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_NightVentilation = IddObjectType(\"AvailabilityManager:NightVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManager_HybridVentilation = IddObjectType(\"AvailabilityManager:HybridVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::AvailabilityManagerAssignmentList = IddObjectType(\"AvailabilityManagerAssignmentList\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_Scheduled = IddObjectType(\"SetpointManager:Scheduled\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_Scheduled_DualSetpoint = IddObjectType(\"SetpointManager:Scheduled:DualSetpoint\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_OutdoorAirReset = IddObjectType(\"SetpointManager:OutdoorAirReset\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_SingleZone_Reheat = IddObjectType(\"SetpointManager:SingleZone:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_SingleZone_Heating = IddObjectType(\"SetpointManager:SingleZone:Heating\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_SingleZone_Cooling = IddObjectType(\"SetpointManager:SingleZone:Cooling\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_SingleZone_Humidity_Minimum = IddObjectType(\"SetpointManager:SingleZone:Humidity:Minimum\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_SingleZone_Humidity_Maximum = IddObjectType(\"SetpointManager:SingleZone:Humidity:Maximum\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_MixedAir = IddObjectType(\"SetpointManager:MixedAir\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_OutdoorAirPretreat = IddObjectType(\"SetpointManager:OutdoorAirPretreat\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_Warmest = IddObjectType(\"SetpointManager:Warmest\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_Coldest = IddObjectType(\"SetpointManager:Coldest\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_ReturnAirBypassFlow = IddObjectType(\"SetpointManager:ReturnAirBypassFlow\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_WarmestTemperatureFlow = IddObjectType(\"SetpointManager:WarmestTemperatureFlow\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_MultiZone_Heating_Average = IddObjectType(\"SetpointManager:MultiZone:Heating:Average\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_MultiZone_Cooling_Average = IddObjectType(\"SetpointManager:MultiZone:Cooling:Average\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_MultiZone_MinimumHumidity_Average = IddObjectType(\"SetpointManager:MultiZone:MinimumHumidity:Average\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_MultiZone_MaximumHumidity_Average = IddObjectType(\"SetpointManager:MultiZone:MaximumHumidity:Average\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_MultiZone_Humidity_Minimum = IddObjectType(\"SetpointManager:MultiZone:Humidity:Minimum\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_MultiZone_Humidity_Maximum = IddObjectType(\"SetpointManager:MultiZone:Humidity:Maximum\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_FollowOutdoorAirTemperature = IddObjectType(\"SetpointManager:FollowOutdoorAirTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_FollowSystemNodeTemperature = IddObjectType(\"SetpointManager:FollowSystemNodeTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_FollowGroundTemperature = IddObjectType(\"SetpointManager:FollowGroundTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_CondenserEnteringReset = IddObjectType(\"SetpointManager:CondenserEnteringReset\");" << std::endl
  << "const IddObjectType IddObjectType::SetpointManager_CondenserEnteringReset_Ideal = IddObjectType(\"SetpointManager:CondenserEnteringReset:Ideal\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_Case = IddObjectType(\"Refrigeration:Case\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_CompressorRack = IddObjectType(\"Refrigeration:CompressorRack\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_CaseAndWalkInList = IddObjectType(\"Refrigeration:CaseAndWalkInList\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_Condenser_AirCooled = IddObjectType(\"Refrigeration:Condenser:AirCooled\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_Condenser_EvaporativeCooled = IddObjectType(\"Refrigeration:Condenser:EvaporativeCooled\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_Condenser_WaterCooled = IddObjectType(\"Refrigeration:Condenser:WaterCooled\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_Condenser_Cascade = IddObjectType(\"Refrigeration:Condenser:Cascade\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_GasCooler_AirCooled = IddObjectType(\"Refrigeration:GasCooler:AirCooled\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_TransferLoadList = IddObjectType(\"Refrigeration:TransferLoadList\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_Subcooler = IddObjectType(\"Refrigeration:Subcooler\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_Compressor = IddObjectType(\"Refrigeration:Compressor\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_CompressorList = IddObjectType(\"Refrigeration:CompressorList\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_System = IddObjectType(\"Refrigeration:System\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_TranscriticalSystem = IddObjectType(\"Refrigeration:TranscriticalSystem\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_SecondarySystem = IddObjectType(\"Refrigeration:SecondarySystem\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_WalkIn = IddObjectType(\"Refrigeration:WalkIn\");" << std::endl
  << "const IddObjectType IddObjectType::Refrigeration_AirChiller = IddObjectType(\"Refrigeration:AirChiller\");" << std::endl
  << "const IddObjectType IddObjectType::ZoneHVAC_RefrigerationChillerSet = IddObjectType(\"ZoneHVAC:RefrigerationChillerSet\");" << std::endl
  << "const IddObjectType IddObjectType::DemandManagerAssignmentList = IddObjectType(\"DemandManagerAssignmentList\");" << std::endl
  << "const IddObjectType IddObjectType::DemandManager_ExteriorLights = IddObjectType(\"DemandManager:ExteriorLights\");" << std::endl
  << "const IddObjectType IddObjectType::DemandManager_Lights = IddObjectType(\"DemandManager:Lights\");" << std::endl
  << "const IddObjectType IddObjectType::DemandManager_ElectricEquipment = IddObjectType(\"DemandManager:ElectricEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::DemandManager_Thermostats = IddObjectType(\"DemandManager:Thermostats\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_InternalCombustionEngine = IddObjectType(\"Generator:InternalCombustionEngine\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_CombustionTurbine = IddObjectType(\"Generator:CombustionTurbine\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_MicroTurbine = IddObjectType(\"Generator:MicroTurbine\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_Photovoltaic = IddObjectType(\"Generator:Photovoltaic\");" << std::endl
  << "const IddObjectType IddObjectType::PhotovoltaicPerformance_Simple = IddObjectType(\"PhotovoltaicPerformance:Simple\");" << std::endl
  << "const IddObjectType IddObjectType::PhotovoltaicPerformance_EquivalentOneDiode = IddObjectType(\"PhotovoltaicPerformance:EquivalentOne-Diode\");" << std::endl
  << "const IddObjectType IddObjectType::PhotovoltaicPerformance_Sandia = IddObjectType(\"PhotovoltaicPerformance:Sandia\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell = IddObjectType(\"Generator:FuelCell\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_PowerModule = IddObjectType(\"Generator:FuelCell:PowerModule\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_AirSupply = IddObjectType(\"Generator:FuelCell:AirSupply\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_WaterSupply = IddObjectType(\"Generator:FuelCell:WaterSupply\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_AuxiliaryHeater = IddObjectType(\"Generator:FuelCell:AuxiliaryHeater\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_ExhaustGasToWaterHeatExchanger = IddObjectType(\"Generator:FuelCell:ExhaustGasToWaterHeatExchanger\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_ElectricalStorage = IddObjectType(\"Generator:FuelCell:ElectricalStorage\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_Inverter = IddObjectType(\"Generator:FuelCell:Inverter\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelCell_StackCooler = IddObjectType(\"Generator:FuelCell:StackCooler\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_MicroCHP = IddObjectType(\"Generator:MicroCHP\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_MicroCHP_NonNormalizedParameters = IddObjectType(\"Generator:MicroCHP:NonNormalizedParameters\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_FuelSupply = IddObjectType(\"Generator:FuelSupply\");" << std::endl
  << "const IddObjectType IddObjectType::Generator_WindTurbine = IddObjectType(\"Generator:WindTurbine\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Generators = IddObjectType(\"ElectricLoadCenter:Generators\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Inverter_Simple = IddObjectType(\"ElectricLoadCenter:Inverter:Simple\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Inverter_FunctionOfPower = IddObjectType(\"ElectricLoadCenter:Inverter:FunctionOfPower\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Inverter_LookUpTable = IddObjectType(\"ElectricLoadCenter:Inverter:LookUpTable\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Storage_Simple = IddObjectType(\"ElectricLoadCenter:Storage:Simple\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Storage_Battery = IddObjectType(\"ElectricLoadCenter:Storage:Battery\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Transformer = IddObjectType(\"ElectricLoadCenter:Transformer\");" << std::endl
  << "const IddObjectType IddObjectType::ElectricLoadCenter_Distribution = IddObjectType(\"ElectricLoadCenter:Distribution\");" << std::endl
  << "const IddObjectType IddObjectType::WaterUse_Equipment = IddObjectType(\"WaterUse:Equipment\");" << std::endl
  << "const IddObjectType IddObjectType::WaterUse_Connections = IddObjectType(\"WaterUse:Connections\");" << std::endl
  << "const IddObjectType IddObjectType::WaterUse_Storage = IddObjectType(\"WaterUse:Storage\");" << std::endl
  << "const IddObjectType IddObjectType::WaterUse_Well = IddObjectType(\"WaterUse:Well\");" << std::endl
  << "const IddObjectType IddObjectType::WaterUse_RainCollector = IddObjectType(\"WaterUse:RainCollector\");" << std::endl
  << "const IddObjectType IddObjectType::Matrix_TwoDimension = IddObjectType(\"Matrix:TwoDimension\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Linear = IddObjectType(\"Curve:Linear\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_QuadLinear = IddObjectType(\"Curve:QuadLinear\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Quadratic = IddObjectType(\"Curve:Quadratic\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Cubic = IddObjectType(\"Curve:Cubic\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Quartic = IddObjectType(\"Curve:Quartic\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Exponent = IddObjectType(\"Curve:Exponent\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Bicubic = IddObjectType(\"Curve:Bicubic\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Biquadratic = IddObjectType(\"Curve:Biquadratic\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_QuadraticLinear = IddObjectType(\"Curve:QuadraticLinear\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Triquadratic = IddObjectType(\"Curve:Triquadratic\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Functional_PressureDrop = IddObjectType(\"Curve:Functional:PressureDrop\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_FanPressureRise = IddObjectType(\"Curve:FanPressureRise\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_ExponentialSkewNormal = IddObjectType(\"Curve:ExponentialSkewNormal\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_Sigmoid = IddObjectType(\"Curve:Sigmoid\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_RectangularHyperbola1 = IddObjectType(\"Curve:RectangularHyperbola1\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_RectangularHyperbola2 = IddObjectType(\"Curve:RectangularHyperbola2\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_ExponentialDecay = IddObjectType(\"Curve:ExponentialDecay\");" << std::endl
  << "const IddObjectType IddObjectType::Curve_DoubleExponentialDecay = IddObjectType(\"Curve:DoubleExponentialDecay\");" << std::endl
  << "const IddObjectType IddObjectType::Table_OneIndependentVariable = IddObjectType(\"Table:OneIndependentVariable\");" << std::endl
  << "const IddObjectType IddObjectType::Table_TwoIndependentVariables = IddObjectType(\"Table:TwoIndependentVariables\");" << std::endl
  << "const IddObjectType IddObjectType::Table_MultiVariableLookup = IddObjectType(\"Table:MultiVariableLookup\");" << std::endl
  << "const IddObjectType IddObjectType::FluidProperties_Name = IddObjectType(\"FluidProperties:Name\");" << std::endl
  << "const IddObjectType IddObjectType::FluidProperties_GlycolConcentration = IddObjectType(\"FluidProperties:GlycolConcentration\");" << std::endl
  << "const IddObjectType IddObjectType::FluidProperties_Temperatures = IddObjectType(\"FluidProperties:Temperatures\");" << std::endl
  << "const IddObjectType IddObjectType::FluidProperties_Saturated = IddObjectType(\"FluidProperties:Saturated\");" << std::endl
  << "const IddObjectType IddObjectType::FluidProperties_Superheated = IddObjectType(\"FluidProperties:Superheated\");" << std::endl
  << "const IddObjectType IddObjectType::FluidProperties_Concentration = IddObjectType(\"FluidProperties:Concentration\");" << std::endl
  << "const IddObjectType IddObjectType::CurrencyType = IddObjectType(\"CurrencyType\");" << std::endl
  << "const IddObjectType IddObjectType::ComponentCost_Adjustments = IddObjectType(\"ComponentCost:Adjustments\");" << std::endl
  << "const IddObjectType IddObjectType::ComponentCost_Reference = IddObjectType(\"ComponentCost:Reference\");" << std::endl
  << "const IddObjectType IddObjectType::ComponentCost_LineItem = IddObjectType(\"ComponentCost:LineItem\");" << std::endl
  << "const IddObjectType IddObjectType::UtilityCost_Tariff = IddObjectType(\"UtilityCost:Tariff\");" << std::endl
  << "const IddObjectType IddObjectType::UtilityCost_Qualify = IddObjectType(\"UtilityCost:Qualify\");" << std::endl
  << "const IddObjectType IddObjectType::UtilityCost_Charge_Simple = IddObjectType(\"UtilityCost:Charge:Simple\");" << std::endl
  << "const IddObjectType IddObjectType::UtilityCost_Charge_Block = IddObjectType(\"UtilityCost:Charge:Block\");" << std::endl
  << "const IddObjectType IddObjectType::UtilityCost_Ratchet = IddObjectType(\"UtilityCost:Ratchet\");" << std::endl
  << "const IddObjectType IddObjectType::UtilityCost_Variable = IddObjectType(\"UtilityCost:Variable\");" << std::endl
  << "const IddObjectType IddObjectType::UtilityCost_Computation = IddObjectType(\"UtilityCost:Computation\");" << std::endl
  << "const IddObjectType IddObjectType::LifeCycleCost_Parameters = IddObjectType(\"LifeCycleCost:Parameters\");" << std::endl
  << "const IddObjectType IddObjectType::LifeCycleCost_RecurringCosts = IddObjectType(\"LifeCycleCost:RecurringCosts\");" << std::endl
  << "const IddObjectType IddObjectType::LifeCycleCost_NonrecurringCost = IddObjectType(\"LifeCycleCost:NonrecurringCost\");" << std::endl
  << "const IddObjectType IddObjectType::LifeCycleCost_UsePriceEscalation = IddObjectType(\"LifeCycleCost:UsePriceEscalation\");" << std::endl
  << "const IddObjectType IddObjectType::LifeCycleCost_UseAdjustment = IddObjectType(\"LifeCycleCost:UseAdjustment\");" << std::endl
  << "const IddObjectType IddObjectType::Parametric_SetValueForRun = IddObjectType(\"Parametric:SetValueForRun\");" << std::endl
  << "const IddObjectType IddObjectType::Parametric_Logic = IddObjectType(\"Parametric:Logic\");" << std::endl
  << "const IddObjectType IddObjectType::Parametric_RunControl = IddObjectType(\"Parametric:RunControl\");" << std::endl
  << "const IddObjectType IddObjectType::Parametric_FileNameSuffix = IddObjectType(\"Parametric:FileNameSuffix\");" << std::endl
  << "const IddObjectType IddObjectType::Output_VariableDictionary = IddObjectType(\"Output:VariableDictionary\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Surfaces_List = IddObjectType(\"Output:Surfaces:List\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Surfaces_Drawing = IddObjectType(\"Output:Surfaces:Drawing\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Schedules = IddObjectType(\"Output:Schedules\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Constructions = IddObjectType(\"Output:Constructions\");" << std::endl
  << "const IddObjectType IddObjectType::Output_EnergyManagementSystem = IddObjectType(\"Output:EnergyManagementSystem\");" << std::endl
  << "const IddObjectType IddObjectType::OutputControl_SurfaceColorScheme = IddObjectType(\"OutputControl:SurfaceColorScheme\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Table_SummaryReports = IddObjectType(\"Output:Table:SummaryReports\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Table_TimeBins = IddObjectType(\"Output:Table:TimeBins\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Table_Monthly = IddObjectType(\"Output:Table:Monthly\");" << std::endl
  << "const IddObjectType IddObjectType::OutputControl_Table_Style = IddObjectType(\"OutputControl:Table:Style\");" << std::endl
  << "const IddObjectType IddObjectType::OutputControl_ReportingTolerances = IddObjectType(\"OutputControl:ReportingTolerances\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Variable = IddObjectType(\"Output:Variable\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Meter = IddObjectType(\"Output:Meter\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Meter_MeterFileOnly = IddObjectType(\"Output:Meter:MeterFileOnly\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Meter_Cumulative = IddObjectType(\"Output:Meter:Cumulative\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Meter_Cumulative_MeterFileOnly = IddObjectType(\"Output:Meter:Cumulative:MeterFileOnly\");" << std::endl
  << "const IddObjectType IddObjectType::Meter_Custom = IddObjectType(\"Meter:Custom\");" << std::endl
  << "const IddObjectType IddObjectType::Meter_CustomDecrement = IddObjectType(\"Meter:CustomDecrement\");" << std::endl
  << "const IddObjectType IddObjectType::Output_SQLite = IddObjectType(\"Output:SQLite\");" << std::endl
  << "const IddObjectType IddObjectType::Output_EnvironmentalImpactFactors = IddObjectType(\"Output:EnvironmentalImpactFactors\");" << std::endl
  << "const IddObjectType IddObjectType::EnvironmentalImpactFactors = IddObjectType(\"EnvironmentalImpactFactors\");" << std::endl
  << "const IddObjectType IddObjectType::FuelFactors = IddObjectType(\"FuelFactors\");" << std::endl
  << "const IddObjectType IddObjectType::Output_Diagnostics = IddObjectType(\"Output:Diagnostics\");" << std::endl
  << "const IddObjectType IddObjectType::Output_DebuggingData = IddObjectType(\"Output:DebuggingData\");" << std::endl
  << "const IddObjectType IddObjectType::Output_PreprocessorMessage = IddObjectType(\"Output:PreprocessorMessage\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Version = IddObjectType(\"OS:Version\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ComponentData = IddObjectType(\"OS:ComponentData\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ConvergenceLimits = IddObjectType(\"OS:ConvergenceLimits\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ProgramControl = IddObjectType(\"OS:ProgramControl\");" << std::endl
  << "const IddObjectType IddObjectType::OS_HeatBalanceAlgorithm = IddObjectType(\"OS:HeatBalanceAlgorithm\");" << std::endl
  << "const IddObjectType IddObjectType::OS_RunPeriod = IddObjectType(\"OS:RunPeriod\");" << std::endl
  << "const IddObjectType IddObjectType::OS_RunPeriodControl_DaylightSavingTime = IddObjectType(\"OS:RunPeriodControl:DaylightSavingTime\");" << std::endl
  << "const IddObjectType IddObjectType::OS_RunPeriodControl_SpecialDays = IddObjectType(\"OS:RunPeriodControl:SpecialDays\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ShadowCalculation = IddObjectType(\"OS:ShadowCalculation\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SimulationControl = IddObjectType(\"OS:SimulationControl\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Sizing_Parameters = IddObjectType(\"OS:Sizing:Parameters\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SurfaceConvectionAlgorithm_Inside = IddObjectType(\"OS:SurfaceConvectionAlgorithm:Inside\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SurfaceConvectionAlgorithm_Outside = IddObjectType(\"OS:SurfaceConvectionAlgorithm:Outside\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Timestep = IddObjectType(\"OS:Timestep\");" << std::endl
  << "const IddObjectType IddObjectType::OS_YearDescription = IddObjectType(\"OS:YearDescription\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneAirContaminantBalance = IddObjectType(\"OS:ZoneAirContaminantBalance\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneAirHeatBalanceAlgorithm = IddObjectType(\"OS:ZoneAirHeatBalanceAlgorithm\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial = IddObjectType(\"OS:ZoneCapacitanceMultiplier:ResearchSpecial\");" << std::endl
  << "const IddObjectType IddObjectType::OS_RadianceParameters = IddObjectType(\"OS:RadianceParameters\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ClimateZones = IddObjectType(\"OS:ClimateZones\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Site = IddObjectType(\"OS:Site\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Site_GroundTemperature_BuildingSurface = IddObjectType(\"OS:Site:GroundTemperature:BuildingSurface\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Site_GroundReflectance = IddObjectType(\"OS:Site:GroundReflectance\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Site_WaterMainsTemperature = IddObjectType(\"OS:Site:WaterMainsTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SizingPeriod_DesignDay = IddObjectType(\"OS:SizingPeriod:DesignDay\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SizingPeriod_WeatherFileConditionType = IddObjectType(\"OS:SizingPeriod:WeatherFileConditionType\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SizingPeriod_WeatherFileDays = IddObjectType(\"OS:SizingPeriod:WeatherFileDays\");" << std::endl
  << "const IddObjectType IddObjectType::OS_TimeDependentValuation = IddObjectType(\"OS:TimeDependentValuation\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityCost_Charge_Block = IddObjectType(\"OS:UtilityCost:Charge:Block\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityCost_Charge_Simple = IddObjectType(\"OS:UtilityCost:Charge:Simple\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityCost_Computation = IddObjectType(\"OS:UtilityCost:Computation\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityCost_Qualify = IddObjectType(\"OS:UtilityCost:Qualify\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityCost_Ratchet = IddObjectType(\"OS:UtilityCost:Ratchet\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityCost_Tariff = IddObjectType(\"OS:UtilityCost:Tariff\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityCost_Variable = IddObjectType(\"OS:UtilityCost:Variable\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WeatherFile = IddObjectType(\"OS:WeatherFile\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WeatherProperty_SkyTemperature = IddObjectType(\"OS:WeatherProperty:SkyTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::OS_BuildingStory = IddObjectType(\"OS:BuildingStory\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DefaultConstructionSet = IddObjectType(\"OS:DefaultConstructionSet\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DefaultScheduleSet = IddObjectType(\"OS:DefaultScheduleSet\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DefaultSubSurfaceConstructions = IddObjectType(\"OS:DefaultSubSurfaceConstructions\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DefaultSurfaceConstructions = IddObjectType(\"OS:DefaultSurfaceConstructions\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Rendering_Color = IddObjectType(\"OS:Rendering:Color\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SpaceType = IddObjectType(\"OS:SpaceType\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Material = IddObjectType(\"OS:Material\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Material_AirGap = IddObjectType(\"OS:Material:AirGap\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Material_AirWall = IddObjectType(\"OS:Material:AirWall\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Material_InfraredTransparent = IddObjectType(\"OS:Material:InfraredTransparent\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Material_NoMass = IddObjectType(\"OS:Material:NoMass\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Material_RoofVegetation = IddObjectType(\"OS:Material:RoofVegetation\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_Blind = IddObjectType(\"OS:WindowMaterial:Blind\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_Gas = IddObjectType(\"OS:WindowMaterial:Gas\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_GasMixture = IddObjectType(\"OS:WindowMaterial:GasMixture\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_Glazing = IddObjectType(\"OS:WindowMaterial:Glazing\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic = IddObjectType(\"OS:WindowMaterial:GlazingGroup:Thermochromic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod = IddObjectType(\"OS:WindowMaterial:Glazing:RefractionExtinctionMethod\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_Screen = IddObjectType(\"OS:WindowMaterial:Screen\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_Shade = IddObjectType(\"OS:WindowMaterial:Shade\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WindowMaterial_SimpleGlazingSystem = IddObjectType(\"OS:WindowMaterial:SimpleGlazingSystem\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Construction = IddObjectType(\"OS:Construction\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Construction_CfactorUndergroundWall = IddObjectType(\"OS:Construction:CfactorUndergroundWall\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Construction_FfactorGroundFloor = IddObjectType(\"OS:Construction:FfactorGroundFloor\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Construction_InternalSource = IddObjectType(\"OS:Construction:InternalSource\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Construction_WindowDataFile = IddObjectType(\"OS:Construction:WindowDataFile\");" << std::endl
  << "const IddObjectType IddObjectType::OS_StandardsInformation_Construction = IddObjectType(\"OS:StandardsInformation:Construction\");" << std::endl
  << "const IddObjectType IddObjectType::OS_InternalMass_Definition = IddObjectType(\"OS:InternalMass:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_People_Definition = IddObjectType(\"OS:People:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Lights_Definition = IddObjectType(\"OS:Lights:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Luminaire_Definition = IddObjectType(\"OS:Luminaire:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ElectricEquipment_Definition = IddObjectType(\"OS:ElectricEquipment:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_GasEquipment_Definition = IddObjectType(\"OS:GasEquipment:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_HotWaterEquipment_Definition = IddObjectType(\"OS:HotWaterEquipment:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SteamEquipment_Definition = IddObjectType(\"OS:SteamEquipment:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_OtherEquipment_Definition = IddObjectType(\"OS:OtherEquipment:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Exterior_Lights_Definition = IddObjectType(\"OS:Exterior:Lights:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_Compact = IddObjectType(\"OS:Schedule:Compact\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_Day = IddObjectType(\"OS:Schedule:Day\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_Week = IddObjectType(\"OS:Schedule:Week\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_Year = IddObjectType(\"OS:Schedule:Year\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_Rule = IddObjectType(\"OS:Schedule:Rule\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_Ruleset = IddObjectType(\"OS:Schedule:Ruleset\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_Constant = IddObjectType(\"OS:Schedule:Constant\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_FixedInterval = IddObjectType(\"OS:Schedule:FixedInterval\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Schedule_VariableInterval = IddObjectType(\"OS:Schedule:VariableInterval\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ScheduleTypeLimits = IddObjectType(\"OS:ScheduleTypeLimits\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Building = IddObjectType(\"OS:Building\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DaylightingDevice_Shelf = IddObjectType(\"OS:DaylightingDevice:Shelf\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Facility = IddObjectType(\"OS:Facility\");" << std::endl
  << "const IddObjectType IddObjectType::OS_InteriorPartitionSurface = IddObjectType(\"OS:InteriorPartitionSurface\");" << std::endl
  << "const IddObjectType IddObjectType::OS_InteriorPartitionSurfaceGroup = IddObjectType(\"OS:InteriorPartitionSurfaceGroup\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ShadingSurface = IddObjectType(\"OS:ShadingSurface\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ShadingSurfaceGroup = IddObjectType(\"OS:ShadingSurfaceGroup\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Space = IddObjectType(\"OS:Space\");" << std::endl
  << "const IddObjectType IddObjectType::OS_StandardsInformation_Building = IddObjectType(\"OS:StandardsInformation:Building\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Surface = IddObjectType(\"OS:Surface\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SubSurface = IddObjectType(\"OS:SubSurface\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ShadingControl = IddObjectType(\"OS:ShadingControl\");" << std::endl
  << "const IddObjectType IddObjectType::OS_InternalMass = IddObjectType(\"OS:InternalMass\");" << std::endl
  << "const IddObjectType IddObjectType::OS_People = IddObjectType(\"OS:People\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Lights = IddObjectType(\"OS:Lights\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Luminaire = IddObjectType(\"OS:Luminaire\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ElectricEquipment = IddObjectType(\"OS:ElectricEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::OS_GasEquipment = IddObjectType(\"OS:GasEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::OS_HotWaterEquipment = IddObjectType(\"OS:HotWaterEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SteamEquipment = IddObjectType(\"OS:SteamEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::OS_OtherEquipment = IddObjectType(\"OS:OtherEquipment\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SpaceInfiltration_DesignFlowRate = IddObjectType(\"OS:SpaceInfiltration:DesignFlowRate\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea = IddObjectType(\"OS:SpaceInfiltration:EffectiveLeakageArea\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Exterior_Lights = IddObjectType(\"OS:Exterior:Lights\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Daylighting_Control = IddObjectType(\"OS:Daylighting:Control\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Glare_Sensor = IddObjectType(\"OS:Glare:Sensor\");" << std::endl
  << "const IddObjectType IddObjectType::OS_IlluminanceMap = IddObjectType(\"OS:IlluminanceMap\");" << std::endl
  << "const IddObjectType IddObjectType::OS_LightingDesignDay = IddObjectType(\"OS:LightingDesignDay\");" << std::endl
  << "const IddObjectType IddObjectType::OS_LightingSimulationControl = IddObjectType(\"OS:LightingSimulationControl\");" << std::endl
  << "const IddObjectType IddObjectType::OS_LightingSimulationZone = IddObjectType(\"OS:LightingSimulationZone\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirLoopHVAC = IddObjectType(\"OS:AirLoopHVAC\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirLoopHVAC_ControllerList = IddObjectType(\"OS:AirLoopHVAC:ControllerList\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem = IddObjectType(\"OS:AirLoopHVAC:OutdoorAirSystem\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirLoopHVAC_UnitaryCoolOnly = IddObjectType(\"OS:AirLoopHVAC:UnitaryCoolOnly\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir = IddObjectType(\"OS:AirLoopHVAC:UnitaryHeatPump:AirToAir\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirLoopHVAC_ZoneMixer = IddObjectType(\"OS:AirLoopHVAC:ZoneMixer\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirLoopHVAC_ZoneSplitter = IddObjectType(\"OS:AirLoopHVAC:ZoneSplitter\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat = IddObjectType(\"OS:AirTerminal:SingleDuct:ConstantVolume:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat = IddObjectType(\"OS:AirTerminal:SingleDuct:ParallelPIU:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam = IddObjectType(\"OS:AirTerminal:SingleDuct:ConstantVolume:CooledBeam\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled = IddObjectType(\"OS:AirTerminal:SingleDuct:Uncontrolled\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirTerminal_SingleDuct_VAV_NoReheat = IddObjectType(\"OS:AirTerminal:SingleDuct:VAV:NoReheat\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat = IddObjectType(\"OS:AirTerminal:SingleDuct:VAV:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AvailabilityManagerAssignmentList = IddObjectType(\"OS:AvailabilityManagerAssignmentList\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AvailabilityManager_Scheduled = IddObjectType(\"OS:AvailabilityManager:Scheduled\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AvailabilityManager_NightCycle = IddObjectType(\"OS:AvailabilityManager:NightCycle\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Boiler_HotWater = IddObjectType(\"OS:Boiler:HotWater\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Boiler_Steam = IddObjectType(\"OS:Boiler:Steam\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Chiller_Electric_EIR = IddObjectType(\"OS:Chiller:Electric:EIR\");" << std::endl
  << "const IddObjectType IddObjectType::OS_CoolingTower_SingleSpeed = IddObjectType(\"OS:CoolingTower:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_CoolingTower_VariableSpeed = IddObjectType(\"OS:CoolingTower:VariableSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DistrictCooling = IddObjectType(\"OS:DistrictCooling\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DistrictHeating = IddObjectType(\"OS:DistrictHeating\");" << std::endl
  << "const IddObjectType IddObjectType::OS_GroundHeatExchanger_Vertical = IddObjectType(\"OS:GroundHeatExchanger:Vertical\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_CooledBeam = IddObjectType(\"OS:Coil:Cooling:CooledBeam\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_DX_SingleSpeed = IddObjectType(\"OS:Coil:Cooling:DX:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_DX_TwoSpeed = IddObjectType(\"OS:Coil:Cooling:DX:TwoSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow = IddObjectType(\"OS:Coil:Cooling:LowTemperatureRadiant:ConstantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow = IddObjectType(\"OS:Coil:Cooling:LowTemperatureRadiant:VariableFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_Water = IddObjectType(\"OS:Coil:Cooling:Water\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit = IddObjectType(\"OS:Coil:Cooling:WaterToAirHeatPump:EquationFit\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_DX_SingleSpeed = IddObjectType(\"OS:Coil:Heating:DX:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_Electric = IddObjectType(\"OS:Coil:Heating:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_Gas = IddObjectType(\"OS:Coil:Heating:Gas\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow = IddObjectType(\"OS:Coil:Heating:LowTemperatureRadiant:ConstantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow = IddObjectType(\"OS:Coil:Heating:LowTemperatureRadiant:VariableFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_Water = IddObjectType(\"OS:Coil:Heating:Water\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_Water_Baseboard = IddObjectType(\"OS:Coil:Heating:Water:Baseboard\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit = IddObjectType(\"OS:Coil:Heating:WaterToAirHeatPump:EquationFit\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Connection = IddObjectType(\"OS:Connection\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Connector_Mixer = IddObjectType(\"OS:Connector:Mixer\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Connector_Splitter = IddObjectType(\"OS:Connector:Splitter\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Controller_MechanicalVentilation = IddObjectType(\"OS:Controller:MechanicalVentilation\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Controller_OutdoorAir = IddObjectType(\"OS:Controller:OutdoorAir\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Controller_WaterCoil = IddObjectType(\"OS:Controller:WaterCoil\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Linear = IddObjectType(\"OS:Curve:Linear\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Quadratic = IddObjectType(\"OS:Curve:Quadratic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Cubic = IddObjectType(\"OS:Curve:Cubic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Quartic = IddObjectType(\"OS:Curve:Quartic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Exponent = IddObjectType(\"OS:Curve:Exponent\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Bicubic = IddObjectType(\"OS:Curve:Bicubic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Biquadratic = IddObjectType(\"OS:Curve:Biquadratic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_QuadraticLinear = IddObjectType(\"OS:Curve:QuadraticLinear\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Triquadratic = IddObjectType(\"OS:Curve:Triquadratic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Functional_PressureDrop = IddObjectType(\"OS:Curve:Functional:PressureDrop\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_FanPressureRise = IddObjectType(\"OS:Curve:FanPressureRise\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_ExponentialSkewNormal = IddObjectType(\"OS:Curve:ExponentialSkewNormal\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_Sigmoid = IddObjectType(\"OS:Curve:Sigmoid\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_RectangularHyperbola1 = IddObjectType(\"OS:Curve:RectangularHyperbola1\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_RectangularHyperbola2 = IddObjectType(\"OS:Curve:RectangularHyperbola2\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_ExponentialDecay = IddObjectType(\"OS:Curve:ExponentialDecay\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Curve_DoubleExponentialDecay = IddObjectType(\"OS:Curve:DoubleExponentialDecay\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DesignSpecification_OutdoorAir = IddObjectType(\"OS:DesignSpecification:OutdoorAir\");" << std::endl
  << "const IddObjectType IddObjectType::OS_DesignSpecification_ZoneAirDistribution = IddObjectType(\"OS:DesignSpecification:ZoneAirDistribution\");" << std::endl
  << "const IddObjectType IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial = IddObjectType(\"OS:EvaporativeCooler:Direct:ResearchSpecial\");" << std::endl
  << "const IddObjectType IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed = IddObjectType(\"OS:EvaporativeFluidCooler:SingleSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Fan_ConstantVolume = IddObjectType(\"OS:Fan:ConstantVolume\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Fan_VariableVolume = IddObjectType(\"OS:Fan:VariableVolume\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Fan_OnOff = IddObjectType(\"OS:Fan:OnOff\");" << std::endl
  << "const IddObjectType IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent = IddObjectType(\"OS:HeatExchanger:AirToAir:SensibleAndLatent\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Node = IddObjectType(\"OS:Node\");" << std::endl
  << "const IddObjectType IddObjectType::OS_PortList = IddObjectType(\"OS:PortList\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Pipe_Adiabatic = IddObjectType(\"OS:Pipe:Adiabatic\");" << std::endl
  << "const IddObjectType IddObjectType::OS_PlantLoop = IddObjectType(\"OS:PlantLoop\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Pump_ConstantSpeed = IddObjectType(\"OS:Pump:ConstantSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Pump_VariableSpeed = IddObjectType(\"OS:Pump:VariableSpeed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature = IddObjectType(\"OS:SetpointManager:FollowOutdoorAirTemperature\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SetpointManager_MixedAir = IddObjectType(\"OS:SetpointManager:MixedAir\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SetpointManager_OutdoorAirReset = IddObjectType(\"OS:SetpointManager:OutdoorAirReset\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SetpointManager_SingleZone_Reheat = IddObjectType(\"OS:SetpointManager:SingleZone:Reheat\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SetpointManager_Scheduled = IddObjectType(\"OS:SetpointManager:Scheduled\");" << std::endl
  << "const IddObjectType IddObjectType::OS_SetpointManager_Warmest = IddObjectType(\"OS:SetpointManager:Warmest\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Sizing_Plant = IddObjectType(\"OS:Sizing:Plant\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Sizing_System = IddObjectType(\"OS:Sizing:System\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Sizing_Zone = IddObjectType(\"OS:Sizing:Zone\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Splitter = IddObjectType(\"OS:Splitter\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ThermalZone = IddObjectType(\"OS:ThermalZone\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ThermostatSetpoint_DualSetpoint = IddObjectType(\"OS:ThermostatSetpoint:DualSetpoint\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_EquipmentList = IddObjectType(\"OS:ZoneHVAC:EquipmentList\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric = IddObjectType(\"OS:ZoneHVAC:Baseboard:Convective:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water = IddObjectType(\"OS:ZoneHVAC:Baseboard:Convective:Water\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric = IddObjectType(\"OS:ZoneHVAC:LowTemperatureRadiant:Electric\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem = IddObjectType(\"OS:ZoneHVAC:IdealLoadsAirSystem\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_FourPipeFanCoil = IddObjectType(\"OS:ZoneHVAC:FourPipeFanCoil\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow = IddObjectType(\"OS:ZoneHVAC:LowTemperatureRadiant:ConstantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow = IddObjectType(\"OS:ZoneHVAC:LowTemperatureRadiant:VariableFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump = IddObjectType(\"OS:ZoneHVAC:PackagedTerminalHeatPump\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner = IddObjectType(\"OS:ZoneHVAC:PackagedTerminalAirConditioner\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump = IddObjectType(\"OS:ZoneHVAC:WaterToAirHeatPump\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_UnitHeater = IddObjectType(\"OS:ZoneHVAC:UnitHeater\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WaterHeater_Mixed = IddObjectType(\"OS:WaterHeater:Mixed\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WaterUse_Equipment = IddObjectType(\"OS:WaterUse:Equipment\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WaterUse_Equipment_Definition = IddObjectType(\"OS:WaterUse:Equipment:Definition\");" << std::endl
  << "const IddObjectType IddObjectType::OS_WaterUse_Connections = IddObjectType(\"OS:WaterUse:Connections\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ComponentCost_Adjustments = IddObjectType(\"OS:ComponentCost:Adjustments\");" << std::endl
  << "const IddObjectType IddObjectType::OS_LifeCycleCost = IddObjectType(\"OS:LifeCycleCost\");" << std::endl
  << "const IddObjectType IddObjectType::OS_CurrencyType = IddObjectType(\"OS:CurrencyType\");" << std::endl
  << "const IddObjectType IddObjectType::OS_LifeCycleCost_Parameters = IddObjectType(\"OS:LifeCycleCost:Parameters\");" << std::endl
  << "const IddObjectType IddObjectType::OS_LifeCycleCost_UsePriceEscalation = IddObjectType(\"OS:LifeCycleCost:UsePriceEscalation\");" << std::endl
  << "const IddObjectType IddObjectType::OS_UtilityBill = IddObjectType(\"OS:UtilityBill\");" << std::endl
  << "const IddObjectType IddObjectType::OS_OutputControl_ReportingTolerances = IddObjectType(\"OS:OutputControl:ReportingTolerances\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Meter = IddObjectType(\"OS:Meter\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Output_Variable = IddObjectType(\"OS:Output:Variable\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ModelObjectList = IddObjectType(\"OS:ModelObjectList\");" << std::endl
  << "const IddObjectType IddObjectType::OS_AirConditioner_VariableRefrigerantFlow = IddObjectType(\"OS:AirConditioner:VariableRefrigerantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Cooling_DX_VariableRefrigerantFlow = IddObjectType(\"OS:Coil:Cooling:DX:VariableRefrigerantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Coil_Heating_DX_VariableRefrigerantFlow = IddObjectType(\"OS:Coil:Cooling:DX:VariableRefrigerantFlow\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Case = IddObjectType(\"OS:Refrigeration:Case\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Condenser_AirCooled = IddObjectType(\"OS:Refrigeration:Condenser:AirCooled\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Condenser_EvaporativeCooled = IddObjectType(\"OS:Refrigeration:Condenser:EvaporativeCooled\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Condenser_WaterCooled = IddObjectType(\"OS:Refrigeration:Condenser:WaterCooled\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Condenser_Cascade = IddObjectType(\"OS:Refrigeration:Condenser:Cascade\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Compressor = IddObjectType(\"OS:Refrigeration:Compressor\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Subcooler_Mechanical = IddObjectType(\"OS:Refrigeration:Subcooler:Mechanical\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction = IddObjectType(\"OS:Refrigeration:Subcooler:LiquidSuction\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_System = IddObjectType(\"OS:Refrigeration:System\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_SecondarySystem = IddObjectType(\"OS:Refrigeration:SecondarySystem\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_WalkIn = IddObjectType(\"OS:Refrigeration:WalkIn\");" << std::endl
  << "const IddObjectType IddObjectType::OS_Refrigeration_WalkIn_ZoneBoundary = IddObjectType(\"OS:Refrigeration:WalkIn:ZoneBoundary\");" << std::endl
  << "const IddObjectType IddObjectType::OS_CoolingTowerPerformance_CoolTools = IddObjectType(\"OS:CoolingTowerPerformance:CoolTools\");" << std::endl
  << "const IddObjectType IddObjectType::OS_CoolingTowerPerformance_YorkCalc = IddObjectType(\"OS:CoolingTowerPerformance:YorkCalc\");" << std::endl
  << "const IddObjectType IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow = IddObjectType(\"OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow\");" << std::endl;

  // create function for CommentOnly IddObject
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "IddObject createCommentOnlyIddObject() {" << std::endl
    << std::endl
    << "  static IddObject object;" << std::endl
    << std::endl
    << "  if (object.type() == iddobjectname::Catchall) {" << std::endl
    << "    std::stringstream ss;" << std::endl
    << "    ss << \"CommentOnly; ! Autogenerated comment only object.\" << std::endl;" << std::endl
    << std::endl
    << "    IddObjectType objType(iddobjectname::CommentOnly);" << std::endl
    << "    OptionalIddObject oObj = IddObject::load(\"CommentOnly\"," << std::endl
    << "                                             \"\"," << std::endl
    << "                                             ss.str()," << std::endl
    << "                                             objType);" << std::endl
    << "    OS_ASSERT(oObj);" << std::endl
    << "    object = *oObj;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return object;" << std::endl
    << "}" << std::endl;

  // constructor
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "IddFactorySingleton::IddFactorySingleton() {" << std::endl
    << std::endl
    << "  // initialize callback map" << std::endl
    << std::endl;
  // register create functions in the callback map
  // Catchall
  outFiles.iddFactoryCxx.tempFile
    << "  m_callbackMap.insert(IddObjectCallbackMap::value_type(iddobjectname::Catchall,"
    << "createCatchallIddObject));" << std::endl;
  // parsed objects
  BOOST_FOREACH(const IddFileFactoryData& idd,iddFiles) {
    outFiles.iddFactoryCxx.tempFile
      << "  register" << idd.fileName() << "ObjectsInCallbackMap();" << std::endl;
  }
  // CommentOnly
  outFiles.iddFactoryCxx.tempFile
    << "  m_callbackMap.insert(IddObjectCallbackMap::value_type(iddobjectname::CommentOnly,"
    << "createCommentOnlyIddObject));" << std::endl
    << std::endl
    << "  // instantiate IddObjectType to IddFileType multimap" << std::endl
    << std::endl;
  // register IddObjectTypes with IddFileTypes
  BOOST_FOREACH(const IddFileFactoryData& idd, iddFiles) {
    std::string fileName = idd.fileName();

    // register local objects
    BOOST_FOREACH(const StringPair& objectName, idd.objectNames()) {
      outFiles.iddFactoryCxx.tempFile
        << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(IddObjectType(\"" 
        << objectName.second << "\"),IddFileType::" << fileName << "));" << std::endl;
    }

    // register imported objects
    for (unsigned i = 0, ni = idd.numIncludedFiles(); i < ni; ++i) {
      /* Included files are identified by name. Optionally, there may be a list of removed objects
         that should not be inherited by the composite file. */
      IddFileFactoryData::FileNameRemovedObjectsPair includedFileData = idd.includedFile(i);
      // Get the included file by name. Will throw if there is no match.
      IddFileFactoryData includedFile = getFile(includedFileData.first,iddFiles);

      std::vector<std::string> excludedObjects;
      BOOST_FOREACH(const StringPair& objectName, includedFile.objectNames()) {

        // If objectName is in list of removed objects, do not add it to m_sourceFileMap,
        if (std::find(includedFileData.second.begin(),includedFileData.second.end(),objectName.first) 
            != includedFileData.second.end()) {
          // and keep its name in case we need to write a warning.
          excludedObjects.push_back(objectName.first);  
          continue;
        }

        // objectName is not to be removed, so add it to the composite file.
        outFiles.iddFactoryCxx.tempFile
          << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(iddobjectname::" 
          << objectName.first << ",IddFileType::" << fileName << "));" << std::endl;

      } // BOOST_FOREACH

      // Write warning if we did not encounter all of the objects that were to be removed.
      if (excludedObjects.size() != includedFileData.second.size()) {
        std::cout << "Warning: Did not encounter all 'objects to remove' while including file " << std::endl
                  << "'" << includedFileData.first << "' in file '" << fileName << "'." << std::endl
                  << "\\remove-object Tags      Objects Actually Removed " << std::endl
                  << "------------------------- -------------------------" << std::endl;
        for (int x = 0, y = 0, nx = includedFileData.second.size(), ny = excludedObjects.size();
             (x < nx) || (y < ny); ++x, ++y) {
          if (x < nx) { std::cout << std::setw(25) << includedFileData.second[x] << " "; }
          else { std::cout << std::setw(25) << " " << " "; }
          if (y < ny) { std::cout << std::setw(25)<< excludedObjects[y] << std::endl; }
          else { std::cout << std::setw(25) << " " << std::endl; }
        } // for
      } // if

    } // for

    // Register CommentOnly object for all files.
    outFiles.iddFactoryCxx.tempFile
        << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(iddobjectname::CommentOnly"
        << ",IddFileType::" << fileName << "));" << std::endl;
  }
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "}" << std::endl;

  // version and header getters
  outFiles.iddFactoryCxx.tempFile
      << std::endl
      << "std::string IddFactorySingleton::getVersion(IddFileType fileType) const {" << std::endl
      << "  std::string result;" << std::endl
      << std::endl
      << "  switch (fileType.value()) {" << std::endl;
  BOOST_FOREACH(const IddFileFactoryData& idd, iddFiles) {
    outFiles.iddFactoryCxx.tempFile
        << "    case IddFileType::" << idd.fileName() << " :" << std::endl
        << "      result = \"" << idd.version() << "\";" << std::endl
        << "      break;" << std::endl;
  }
  outFiles.iddFactoryCxx.tempFile
      << "    default :" << std::endl
      << "      LOG_AND_THROW(\"No version to return for IddFileType \" << fileType.valueDescription() << \".\");" << std::endl
      << "  } // switch" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::string IddFactorySingleton::getHeader(IddFileType fileType) const {" << std::endl
      << "  std::stringstream result;" << std::endl
      << "  switch (fileType.value()) {" << std::endl;
  BOOST_FOREACH(const IddFileFactoryData& idd, iddFiles) {
    outFiles.iddFactoryCxx.tempFile
        << "    case IddFileType::" << idd.fileName() << " :" << std::endl
        << "      result";
    std::stringstream headerStream(idd.header());
    std::string line;
    while (std::getline(headerStream,line)) {
      outFiles.iddFactoryCxx.tempFile
          << std::endl
          << "        << \"" << line << "\" << std::endl";
    }
    // print information about included files
    for (unsigned i = 0, ni = idd.numIncludedFiles(); i < ni; ++i) {
      IddFileFactoryData::FileNameRemovedObjectsPair includedFileData = idd.includedFile(i);
      IddFileFactoryData includedFile = getFile(includedFileData.first,iddFiles);
      outFiles.iddFactoryCxx.tempFile
        << std::endl
        << "        << \"!\\n\"" << std::endl
        << "        << \"! **************************************************************************\\n\"" << std::endl
        << "        << \"! Includes File: '" << includedFile.fileName() << "'\\n\"" << std::endl
        << "        << \"!\\n\"" << std::endl
        << "        << \"! Contains all objects from " << includedFile.fileName() << " IDD_Version " << includedFile.version() << ", except: \\n\"" << std::endl;
      BOOST_FOREACH(const std::string& objectName, includedFileData.second) {
        outFiles.iddFactoryCxx.tempFile
          << "        << \"!   " << objectName << "\\n\"" << std::endl;
      }
      outFiles.iddFactoryCxx.tempFile
        << "        << \"! **************************************************************************\\n\"";
    }
    outFiles.iddFactoryCxx.tempFile
      << ";" << std::endl
      << std::endl
      << "      break;" << std::endl;
  }
 outFiles.iddFactoryCxx.tempFile
     << "    default :" << std::endl
     << "      LOG_AND_THROW(\"No header to return for IddFileType \" << fileType.valueDescription() << \".\");" << std::endl
     << "  } // switch" << std::endl
     << "  return result.str();" << std::endl
     << "}" << std::endl;

  // object getters
  outFiles.iddFactoryCxx.tempFile
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::objects() const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << std::endl
      << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
      << "       itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
      << "    QMutexLocker l(&m_callbackmutex);" << std::endl
      << "    result.push_back(it->second());" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjects(IddFileType fileType) const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << std::endl
      << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
      << "      itend = m_callbackMap.end(); it != itend; ++it) {" << std::endl
      << "    if (isInFile(it->first,fileType)) { " << std::endl
      << "      // This lock is necessary to protect construction of the statics used in the callbacks " << std::endl
      << "      QMutexLocker l(&m_callbackmutex);" << std::endl
      << "      result.push_back(it->second()); " << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<std::string> IddFactorySingleton::groups() const {" << std::endl
      << "  StringSet result;" << std::endl
      << "  BOOST_FOREACH(const IddObject& object,objects()) {" << std::endl
      << "    result.insert(object.group());" << std::endl
      << "  }" << std::endl
      << "  return StringVector(result.begin(),result.end());" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<std::string> IddFactorySingleton::getGroups(IddFileType fileType) const {" << std::endl
      << "  StringSet result;" << std::endl
      << "  BOOST_FOREACH(const IddObject& object,getObjects(fileType)) {" << std::endl
      << "    result.insert(object.group());" << std::endl
      << "  }" << std::endl
      << "  return StringVector(result.begin(),result.end());" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjectsInGroup(const std::string& group) const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << "  BOOST_FOREACH(const IddObject& object,objects()) {" << std::endl
      << "    if (istringEqual(object.group(),group)) {" << std::endl
      << "      result.push_back(object);" << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjectsInGroup(const std::string& group, IddFileType fileType) const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << "  BOOST_FOREACH(const IddObject& object,getObjects(fileType)) {" << std::endl
      << "    if (istringEqual(object.group(),group)) {" << std::endl
      << "      result.push_back(object);" << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      //<< "std::vector<IddObject> IddFactorySingleton::getObjects(const boost::regex& objectRegex) const {" << std::endl
      //<< "  IddObjectVector result;" << std::endl
      //<< std::endl
      //<< "  BOOST_FOREACH(int value,IddObjectType::getValues()) {" << std::endl
      //<< "    IddObjectType candidate(value);" << std::endl
      //<< "    if (boost::regex_match(candidate.valueName(),objectRegex) || " << std::endl
      //<< "        boost::regex_match(candidate.valueDescription(),objectRegex))" << std::endl
      //<< "    {" << std::endl
      //<< "      if (OptionalIddObject object = getObject(candidate)) {" << std::endl
      //<< "        result.push_back(*object);" << std::endl
      //<< "      }" << std::endl
      //<< "    }" << std::endl
      //<< "  }" << std::endl
      //<< std::endl
      //<< "  return result;" << std::endl
      //<< "}" << std::endl
      //<< std::endl
      //<< "std::vector<IddObject> IddFactorySingleton::getObjects(const boost::regex& objectRegex," << std::endl
      //<< "                                                       IddFileType fileType) const " << std::endl
      //<< "{" << std::endl
      //<< "  IddObjectVector result;" << std::endl
      //<< std::endl
      //<< "  BOOST_FOREACH(int value,IddObjectType::getValues()) {" << std::endl
      //<< "    IddObjectType candidate(value);" << std::endl
      //<< "    if (isInFile(candidate,fileType) && " << std::endl
      //<< "        (boost::regex_match(candidate.valueName(),objectRegex) || " << std::endl
      //<< "         boost::regex_match(candidate.valueDescription(),objectRegex)))" << std::endl
      //<< "    {" << std::endl
      //<< "      if (OptionalIddObject object = getObject(candidate)) {" << std::endl
      //<< "        result.push_back(*object);" << std::endl
      //<< "      }" << std::endl
      //<< "    }" << std::endl
      //<< "  }" << std::endl
      //<< std::endl
      //<< "  return result;" << std::endl
      //<< "}" << std::endl
      << std::endl
      << "IddObject IddFactorySingleton::getVersionObject(IddFileType fileType) const {" << std::endl
      << "  if (fileType == IddFileType::EnergyPlus) {" << std::endl
      << "    return getObject(IddObjectType(\"Version\")).get();" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  if (fileType == IddFileType::OpenStudio) {" << std::endl
      << "    return getObject(IddObjectType(\"OS:Version\")).get();" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  LOG_AND_THROW(\"Unable to identify unique version object for IddFileType \" << fileType.valueName() << \".\");" << std::endl
      << "  return IddObject();" << std::endl
      << "}" << std::endl
      << std::endl
      << "boost::optional<IddObject> IddFactorySingleton::getObject(const std::string& objectName) const" << std::endl
      << "{" << std::endl
      << "  OptionalIddObject result;" << std::endl
      << std::endl
      << "  // let IddObjectType OPENSTUDIO_ENUM handle the string processing" << std::endl
      << "  try {" << std::endl
      << "    IddObjectType objectType(objectName);" << std::endl
      << "    result = getObject(objectType);" << std::endl
      << "  }" << std::endl
      << "  catch (...) {}" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "boost::optional<IddObject> IddFactorySingleton::getObject(IddObjectType objectType) const" << std::endl
      << "{" << std::endl
      << "  OptionalIddObject result;" << std::endl
      << std::endl
      << "  IddObjectCallbackMap::const_iterator lookupPair;" << std::endl
      << "  lookupPair = m_callbackMap.find(objectType);" << std::endl
      << "  if (lookupPair != m_callbackMap.end()) { " << std::endl
      << "    QMutexLocker l(&m_callbackmutex);" << std::endl
      << "    result = lookupPair->second(); " << std::endl
      << "  }" << std::endl
      << "  else { " << std::endl
      << "    OS_ASSERT(objectType == iddobjectname::UserCustom); " << std::endl
      << "    LOG(Info,\"UserCustom objects are not available through the IddFactory. Please query your IddFile by IddObject.name().\");" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl;

  // required, unique, and required or unique objects
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::requiredObjects() const {" << std::endl
    << std::endl
    << "  IddObjectVector result;" << std::endl
    << std::endl
    << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "    itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    // This lock is necessary to protect construction of the statics used in the callbacks " << std::endl
    << "    QMutexLocker l(&m_callbackmutex);" << std::endl
    << "    IddObject candidate = it->second();" << std::endl
    << "    l.unlock(); " << std::endl
    << "    if (candidate.properties().required) {" << std::endl
    << "      result.push_back(candidate);" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::getRequiredObjects(IddFileType fileType) const {" << std::endl
    << std::endl
    << "  IddObjectVector result; " << std::endl
    << std::endl
    << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "      itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    if (isInFile(it->first,fileType)) {" << std::endl
    << "      OptionalIddObject candidate = getObject(it->first);" << std::endl
    << "      if (candidate->properties().required) {" << std::endl
    << "        result.push_back(*candidate);" << std::endl
    << "      }" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::uniqueObjects() const {" << std::endl
    << std::endl
    << "  IddObjectVector result;" << std::endl
    << std::endl
    << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "    itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    QMutexLocker l(&m_callbackmutex);" << std::endl
    << "    IddObject candidate = it->second();" << std::endl
    << "    l.unlock(); " << std::endl
    << "    if (candidate.properties().unique) {" << std::endl
    << "      result.push_back(candidate);" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::getUniqueObjects(IddFileType fileType) const {" << std::endl
    << std::endl
    << "  IddObjectVector result; " << std::endl
    << std::endl
    << "   for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "      itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    if (isInFile(it->first,fileType)) {" << std::endl
    << "      OptionalIddObject candidate = getObject(it->first);" << std::endl
    << "      if (candidate->properties().unique) {" << std::endl
    << "        result.push_back(*candidate);" << std::endl
    << "      }" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << std::endl
    << "}" << std::endl;

  // iddFile getters
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "IddFile IddFactorySingleton::getIddFile(IddFileType fileType) const {" << std::endl
    << "  IddFile result;" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::UserCustom) {" << std::endl
    << "    return result; " << std::endl
    << "  }" << std::endl
    << std::endl
    << "  // Add the IddObjects." << std::endl
    << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "      itend = m_callbackMap.end(); it != itend; ++it) {" << std::endl
    << "    if (isInFile(it->first,fileType)) {" << std::endl
    << "      // This lock is necessary to protect construction of the statics used in the callbacks " << std::endl
    << "      QMutexLocker l(&m_callbackmutex);" << std::endl
    << "      result.addObject(it->second());" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  // Set the file version and header." << std::endl
    << "  try {" << std::endl
    << "    result.setVersion(getVersion(fileType));" << std::endl
    << "    result.setHeader(getHeader(fileType));" << std::endl
    << "  }" << std::endl
    << "  catch (...) {}" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "boost::optional<IddFile> IddFactorySingleton::getIddFile(IddFileType fileType, const VersionString& version) const {" << std::endl
    << "  OptionalIddFile result;" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::UserCustom) {" << std::endl
    << "    return result; " << std::endl
    << "  }" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::WholeFactory) {" << std::endl
    << "    LOG(Warn,\"Cannot return the WholeFactory IddFile by version.\");" << std::endl
    << "    return result;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::EnergyPlus) {" << std::endl
    << "    LOG(Warn,\"At this time, OpenStudio cannot return EnergyPlus IDD files by version.\");" << std::endl
    << "    return result;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  VersionString currentVersion(openStudioVersion());" << std::endl
    << "  OS_ASSERT(fileType == IddFileType::OpenStudio);" << std::endl
    << "  if (version == currentVersion) {" << std::endl
    << "    return getIddFile(fileType);" << std::endl
    << "  }" << std::endl
    << "  else {" << std::endl
    << "    std::map<VersionString, IddFile>::const_iterator it = m_osIddFiles.find(version);" << std::endl
    << "    if (it != m_osIddFiles.end()) {" << std::endl
    << "      return it->second;" << std::endl
    << "    }" << std::endl
    << "    openstudio::path iddPath = getSharedResourcesPath() / toPath(\"osversion\");" << std::endl
    << "    std::stringstream folderString;" << std::endl
    << "    folderString << version.major() << \"_\" << version.minor() << \"_\" << version.patch().get();" << std::endl
    << "    iddPath = iddPath / toPath(folderString.str() + \"/OpenStudio.idd\");" << std::endl
    << "    if (boost::filesystem::exists(iddPath) && (version < currentVersion)) {" << std::endl
    << "      result = IddFile::load(iddPath);" << std::endl
    << "    }" << std::endl
    << "    if (result) {" << std::endl
    << "      QMutexLocker l(&m_callbackmutex);" << std::endl
    << "      m_osIddFiles[version] = *result;" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl;

  // query whether object is in file
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "bool IddFactorySingleton::isInFile(IddObjectType objectType, IddFileType fileType) const {" << std::endl
    << "  typedef IddObjectSourceFileMap::const_iterator const_iterator;" << std::endl
    << "  std::pair<const_iterator,const_iterator> range;" << std::endl
    << "  range = m_sourceFileMap.equal_range(objectType);" << std::endl
    << "  for (const_iterator it = range.first; it != range.second; ++it) {" << std::endl
    << "    if ((it->second == fileType) || (fileType == IddFileType::WholeFactory)) {" << std::endl
    << "      return true;" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return false;" << std::endl
    << "}" << std::endl;

  // close out file
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "} // openstudio" << std::endl;

  // close out other IddFactory cxx files
  BOOST_FOREACH(boost::shared_ptr<IddFactoryOutFile>& cxxFile,outFiles.iddFactoryIddFileCxxs) {
    cxxFile->tempFile
      << std::endl
      << "} // openstudio" << std::endl;
  }

  outFiles.finalize();

  std::cout << "IddFactory files generated." << std::endl;
}

IddFileFactoryData getFile(const std::string& fileName,
                           const IddFileFactoryDataVector& iddFiles) {
  BOOST_FOREACH(const IddFileFactoryData& idd, iddFiles) {
    if (idd.fileName() == fileName) { return idd; }
  }

  // unsuccessful search, throw an informative message
  std::stringstream ss;
  ss << "Unable to located included Idd file '" << fileName << "' in list of Idd files. "
     << "The available files are named:" << std::endl;
  BOOST_FOREACH(const IddFileFactoryData& idd, iddFiles) {
    ss << "  " << idd.fileName() << std::endl;
  }
  throw std::runtime_error(ss.str().c_str());
}

} // openstudio
