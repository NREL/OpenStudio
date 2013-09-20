/**********************************************************************
 *  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
 *  Copyright (c) 2013, The Pennsylvania State University.
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

#include <contam/InfiltrationCalculator.hpp>
#include <contam/SimTxt.hpp>

//#include <contam/PrjData.hpp>
//#include <contam/WindPressure.hpp>

//#include <model/Model.hpp>
//#include <model/Building.hpp>
//#include <model/Building_Impl.hpp>
//#include <model/BuildingStory.hpp>
//#include <model/BuildingStory_Impl.hpp>
//#include <model/ThermalZone.hpp>
//#include <model/ThermalZone_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp>
//#include <model/AirLoopHVAC.hpp>
//#include <model/AirLoopHVAC_Impl.hpp>
//#include <model/Node.hpp>
//#include <model/Node_Impl.hpp>
//#include <model/PortList.hpp>

//#include <utilities/sql/SqlFile.hpp>
#include <utilities/core/Logger.hpp>
//#include <utilities/geometry/Geometry.hpp>
#include <utilities/plot/ProgressBar.hpp>

#include <boost/foreach.hpp>
//#include <boost/math/constants/constants.hpp>

//#include <QFile>
//#include <QTextStream>
//#include <QList>
//#include <QMap>
#include <QProcess>
#include <QThread>

namespace openstudio {
namespace contam {

InfiltrationCalculator::InfiltrationCalculator(openstudio::model::Model model,
                                               std::string leakageDescriptor, int ndirs,
                                               ProgressBar *progressBar) 
  : m_model(model), m_leakageDescriptor(leakageDescriptor), m_progressBar(progressBar)
{
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.contam\\.InfiltrationCalculator"));
  m_logSink.setThreadId(QThread::currentThread());

  setDirections(ndirs);
  m_flowSpec=false;

  // Ugly hard code to programs
  contamExe = openstudio::toPath("C:\\Program Files (x86)\\NIST\\CONTAM 3.1\\ContamX3.exe");
  simreadExe = openstudio::toPath("C:\\Users\\jwd131\\Software\\CONTAM\\simread31.exe");
 
}

int InfiltrationCalculator::directions() const
{
  return m_ndirs;
}

void InfiltrationCalculator::setDirections(int ndirs)
{
  if(ndirs <= 0)
  {
    LOG(Error, "Positive number of directions required, defaulting to 4 directions");
    ndirs = 4;
  }
  m_ndirs=ndirs;
}

std::map<Handle,DesignFlowRateCoeffs> InfiltrationCalculator::coeffs() const
{
  return m_coeffMap;
}

bool InfiltrationCalculator::run()
{
  boost::optional<std::map<Handle,DesignFlowRateCoeffs> > coeffs = windSpeed2Pt(m_leakageDescriptor);
  if(!coeffs)
  {
    LOG(Error, "Infiltration calculation failed, check errors and warnings for more information.");
    return false;
  }
  m_coeffMap = coeffs.get();
  return true;
}

void InfiltrationCalculator::apply()
{
  // Remove previous infiltration objects
  std::vector<openstudio::model::SpaceInfiltrationDesignFlowRate> dfrInf = m_model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationDesignFlowRate>();
  BOOST_FOREACH(openstudio::model::SpaceInfiltrationDesignFlowRate inf, dfrInf)
  {
    inf.remove();
  }
  std::vector<openstudio::model::SpaceInfiltrationEffectiveLeakageArea> elaInf = m_model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationEffectiveLeakageArea>();
  BOOST_FOREACH(openstudio::model::SpaceInfiltrationEffectiveLeakageArea inf, elaInf)
  {
    inf.remove();
  }

  // Generate infiltration objects and attach to spaces
  std::pair <openstudio::Handle,DesignFlowRateCoeffs> handleCoeff;
  BOOST_FOREACH(handleCoeff, m_coeffMap)
  {
    boost::optional<openstudio::model::Space> space = m_model.getModelObject<openstudio::model::Space>(handleCoeff.first);
    if(!space)
    {
      LOG(Warn, "Failed to find space " << openstudio::toString(handleCoeff.first));
      continue;
    }
    openstudio::model::SpaceInfiltrationDesignFlowRate infObj(m_model);
    infObj.setDesignFlowRate(handleCoeff.second.Idesign());
    infObj.setConstantTermCoefficient(handleCoeff.second.A());
    infObj.setTemperatureTermCoefficient(handleCoeff.second.B());
    infObj.setVelocityTermCoefficient(handleCoeff.second.C());
    infObj.setVelocitySquaredTermCoefficient(handleCoeff.second.D());
    infObj.setSpace(*space);
  }
}

boost::optional<QVector<double> > InfiltrationCalculator::simulate(std::map <openstudio::Handle,int> spaceMap,
                                                    openstudio::contam::ForwardTranslator &translator,
                                                    std::vector<openstudio::model::Surface> extSurfaces,
                                                    double windSpeed, double windDirection)
{
  QVector<double> results(spaceMap.size(),0.0);
  std::map<openstudio::Handle,int> surfaceMap = translator.surfaceMap();
  translator.setSteadyWeather(windSpeed,windDirection);
  QString fileName = QString("temporary-%1-%2.prj").arg(windSpeed).arg(windDirection);
  openstudio::path inputPath = openstudio::toPath(fileName); // This should do something else - where should these files go?
  
  QFile file(fileName);
  if(!file.open(QFile::WriteOnly))
  {
    LOG(Error,"Failed to open file '"<< fileName.toStdString() << "'.");
    return false;
  }
  QTextStream textStream(&file);
  boost::optional<std::string> output = translator.toString();
  textStream << openstudio::toQString(*output);
  file.close();

  // Run simulation
  LOG(Info,"Running CONTAM simulation (" << windSpeed << "," << windDirection << ")");

  QProcess contamProcess;
  contamProcess.start(openstudio::toQString(contamExe), QStringList() << fileName);
  if(!contamProcess.waitForStarted(-1))
  {
    LOG(Error,"Failed to start CONTAM process.");
    return false;
  }
  if(!contamProcess.waitForFinished(-1))
  {
    LOG(Error,"Failed to complete CONTAM process.");
    return false;
  }

  // Run simread - this needs to get worked out soon: simread3 or simreadx?
  LOG(Info,"Running SimRead on SIM file.");

  QProcess simreadProcess;
#ifndef SIMREADX
  simreadProcess.start(openstudio::toQString(simreadExe), QStringList() << fileName);
  if(!simreadProcess.waitForStarted(-1))
  {
    LOG(Error,"Failed to start SimRead process.");
    return false;
  }
  simreadProcess.write("y\n\ny\n\n"); // This should work for no contaminants
#else
  simreadProcess.start(openstudio::toQString(simreadExePath), QStringList() << "-F" << fileName);
  if(!simreadProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start SimRead process." << std::endl;
    return QVector<double>();
  }
#endif
  if(!simreadProcess.waitForFinished(-1))
  {
    LOG(Error,"Failed to complete SimRead process.");
    return false;
  }

  // Collect results
  openstudio::contam::sim::LinkFlow lfr;
  openstudio::path lfrPath = inputPath.replace_extension(openstudio::toPath("lfr").string());
  if(!lfr.read(lfrPath))
  {
    LOG(Error,"Failed to read link flow results.");
    return false;
  }

  // Process results
  std::vector<std::vector<double> > flow0 = lfr.F0();
  //if(flow0.size() != data->paths.size())  // THIS NEEDS TO COME BACK!
  //{
  //  std::cout << "Mismatch between lfr data and model path count." << std::endl;
  //  return QVector<double>();
  //}
  for(unsigned i=0;i<flow0.size();i++)
  {
    if(flow0[i].size() != 1)
    {
      LOG(Error,"Missing or additional data for path " << i+1 << ".");
      return false;
    }
  }

  // Everything should be good to go if we made it to this point
  // Loop over the exterior surfaces and see what has happened
  BOOST_FOREACH(openstudio::model::Surface surface, extSurfaces)
  {
    std::map<openstudio::Handle,int>::iterator indexIter;
    indexIter = surfaceMap.find(surface.handle());
    if(indexIter == surfaceMap.end())
    {
      LOG(Error,"Missing surface " << openstudio::toString(surface.handle()) << " in PRJ.");
      return false;
    }
    int index = indexIter->second - 1;
    if(flow0[index][0] < 0.0)
    {
      boost::optional<openstudio::model::Space> space = surface.space();
      if(!space)
      {
        LOG(Error,"Surface " << openstudio::toString(surface.handle()) << " is not attached to a space.");
        return false;
      }
      indexIter = spaceMap.find(space->handle());
      if(indexIter == spaceMap.end())
      { 
        LOG(Error,"Missing space " << openstudio::toString(surface.handle()) << " in space map.");
        return false;
      }
      // Only sum up the negative flows (flows in, not flows out)
      results[indexIter->second] += flow0[index][0];
    }
  }
  return results;
}

boost::optional<std::map<Handle,DesignFlowRateCoeffs> > InfiltrationCalculator::windSpeed2Pt(std::string leakageDescriptor)
{
  double density = 1.2041;
  openstudio::contam::ForwardTranslator translator;

  if(!translator.translate(m_model,false,leakageDescriptor))
  {
    LOG(Error, "Translation failed, check errors and warnings for more information.");
    return false;
  }

  // Get all the surfaces
  std::vector<openstudio::model::Surface> surfaces = m_model.getConcreteModelObjects<openstudio::model::Surface>();
  // Cut that down to the external surfaces
  std::vector<openstudio::model::Surface> extSurfaces;
  BOOST_FOREACH(openstudio::model::Surface surface, surfaces)
  {
    std::string bc = surface.outsideBoundaryCondition();
    if(bc == "Outdoors")
    {
      extSurfaces.push_back(surface);
    }
  }

  // Get all the spaces
  std::vector<openstudio::model::Space> spaces = m_model.getConcreteModelObjects<openstudio::model::Space>();
  // Build a map to the index
  std::map<openstudio::Handle,int> spaceMap;
  int index=0;
  BOOST_FOREACH(openstudio::model::Space space, spaces)
  {
    spaceMap[space.handle()] = index++;
  }

  int nspaces = m_model.getConcreteModelObjects<openstudio::model::Space>().size();

  // This is a stand-in for a more complete procedure (such as Ng et al.)

  QVector<double> Q10(nspaces,0.0);
  QVector<double> Q20(nspaces,0.0);

  double delta = 360.0/(double)m_ndirs;

  for(int n=0;n<m_ndirs;n++)
  {
    double angle = n*delta;
    boost::optional<QVector<double> > results = simulate(spaceMap, translator, extSurfaces, 4.4704, angle);
    if(!results)
    {
      LOG(Error, "CONTAM simulation failed, check errors and warnings for more information.");
      return false;
    }
    for(int i=0;i<results->size();i++)
    {
      Q10[i] += results->at(i);
    }
    results = simulate(spaceMap, translator, extSurfaces, 8.9408, angle);
    if(!results)
    {
      LOG(Error, "CONTAM simulation failed, check errors and warnings for more information.");
      return false;
    }
    for(int i=0;i<results->size();i++)
    {
      Q20[i] += results->at(i);
    }
  }

  for(int i=0;i<Q10.size();i++)
  {
    Q10[i] *= -0.25;
    Q20[i] *= -0.25;
  }

  std::map<Handle,DesignFlowRateCoeffs> coeffMap;
  for(int i=0;i<Q10.size();i++)
  {
    double denom = 4.4704*8.9408*8.9408 - 8.9408*4.4704*4.4704;
    double C = (8.9408*8.9408 - 4.4704*4.4704*Q20[i]/Q10[i])/denom;
    double D = (4.4704*Q20[i]/Q10[i] - 8.9408)/denom;
    coeffMap[spaces[i].handle()] = DesignFlowRateCoeffs(density*Q10[i],0.0,0.0,C,D);
  }
  return boost::optional<std::map<Handle,DesignFlowRateCoeffs> >(coeffMap);
}

std::vector<LogMessage> InfiltrationCalculator::warnings() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages())
  {
    if (logMessage.logLevel() == Warn)
    {
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> InfiltrationCalculator::errors() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages())
  {
    if (logMessage.logLevel() > Warn)
    {
      result.push_back(logMessage);
    }
  }

  return result;
}

} // contam
} // openstudio
