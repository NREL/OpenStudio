/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.
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

#include "LocalLibraryController.hpp"
#include "BaseApp.hpp"

#include "BuildingComponentDialog.hpp"
#include "OSDialog.hpp"

#include <analysisdriver/CurrentAnalysis.hpp>

#include <analysis/Measure.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/MeasureGroup_Impl.hpp>
#include <analysis/InputVariable.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Analysis.hpp>
#include <analysis/AnalysisObject.hpp>
#include <analysis/AnalysisObject_Impl.hpp>

#include <runmanager/lib/RunManager.hpp>

#include <ruleset/OSArgument.hpp>

#include <model/Model.hpp>

#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/RubyException.hpp>
#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/bcl/RemoteBCL.hpp>

#include <QAbstractButton>
#include <QBoxLayout>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QUrl>
#include <QRadioButton>
#include <QProgressDialog>

#include "MeasureManager.hpp"
#include "BCLMeasureDialog.hpp"
#include "ProcessEventsProgressBar.hpp"

namespace openstudio {


MeasureManager::MeasureManager(const QSharedPointer<ruleset::RubyUserScriptArgumentGetter> &t_argumentGetter, BaseApp *t_app)
  : m_app(t_app), m_argumentGetter(t_argumentGetter)
{
}


std::pair<bool,std::string> MeasureManager::updateMeasure(analysisdriver::SimpleProject &t_project, 
                                                          const BCLMeasure &t_measure)
{
  std::pair<bool,std::string> result(true,"");
  try {
    ruleset::OSArgumentVector args = getArguments(t_project, t_measure);
    bool differentVersions = t_project.updateMeasure(t_measure, args);
    
    if (!differentVersions) {
      OptionalBCLMeasure existingMeasure = t_project.getMeasureByUUID(t_measure.uuid());
      if (existingMeasure) {
        LOG(Debug, "Measure hasn't changed, but args were reloaded, forcing argument resetting");
        t_project.registerArguments(*existingMeasure,args);
        t_project.analysis().problem().updateMeasure(*existingMeasure, args, false);
      }
    } 
  } catch ( const RubyException &e ) {
    std::stringstream ss;
    ss << "An error occurred while updating measure '" << t_measure.name() << "':" << std::endl;
    ss << "  " << e.what();
    LOG(Error, ss.str());
    result = std::pair<bool,std::string>(false,ss.str());
  }

  return result;
}

BCLMeasure MeasureManager::insertReplaceMeasure(analysisdriver::SimpleProject &t_project, const UUID &t_id)
{
  boost::optional<BCLMeasure> measure = getMeasure(t_id);
  OS_ASSERT(measure);
  bool isMyMeasure = (m_myMeasures.find(t_id) != m_myMeasures.end());
  if (isMyMeasure) {
    bool updated = measure->checkForUpdates();
    if (updated) {
      measure->save();     
    }
    m_myMeasures.erase(t_id);
    m_myMeasures.insert(std::map<UUID,BCLMeasure>::value_type(t_id,*measure));
  }

  boost::optional<BCLMeasure> existingMeasure = t_project.getMeasureByUUID(t_id);

  if (existingMeasure && (existingMeasure->versionUUID() != measure->versionUUID()))
  {
    QDialog dialog(m_app->mainWidget(), Qt::WindowFlags(Qt::Dialog | Qt::WindowTitleHint));
    QVBoxLayout * mainContentVLayout = new QVBoxLayout();
    dialog.setWindowTitle(QCoreApplication::applicationName());

    dialog.setLayout(mainContentVLayout);
    QLabel *label = new QLabel("A modified copy of this measure is already being used in this project.");
    QRadioButton *replace = new QRadioButton("Replace all instances of this measure in this project with this one.");
    replace->setChecked(true);
    QRadioButton *copy = new QRadioButton("Create a new instance using this project's copy of this measure.");
    mainContentVLayout->addWidget(label);
    mainContentVLayout->addWidget(replace);
    mainContentVLayout->addWidget(copy);

    QHBoxLayout * buttons = new QHBoxLayout();

    QPushButton *cancel = new QPushButton("Cancel");
    QPushButton *apply = new QPushButton("Apply");

    buttons->addStretch();
    buttons->addWidget(cancel);
    buttons->addWidget(apply);

    bool connected = connect(cancel, SIGNAL(pressed()), &dialog, SLOT(reject()));
    OS_ASSERT(connected);

    connected = connect(apply, SIGNAL(pressed()), &dialog, SLOT(accept()));
    OS_ASSERT(connected);

    mainContentVLayout->addLayout(buttons);

    if (dialog.exec() == QDialog::Accepted)
    {
      if (replace->isChecked())
      {
        LOG(Info, "User chose to replace existing instances with new version of measure");
        std::pair<bool,std::string> updateResult = updateMeasure(t_project, *measure);
        if (updateResult.first)
        {
          boost::optional<BCLMeasure> updatedMeasure = getMeasure(t_id);
          OS_ASSERT(updatedMeasure);
          return *updatedMeasure;
        } else {
          QMessageBox::critical(m_app->mainWidget(), QString("Error Updating Measure"), QString::fromStdString(updateResult.second));
          throw std::runtime_error("Uknown error occured when calling project.updateMeasure, false was returned");
        }
      } else {
        LOG(Info, "User chose to use existing copy of measure for new instance");
        return *existingMeasure;
      }
    } else {
      LOG(Info, "User canceled drop event for mismatched measure version");
      throw std::runtime_error("User canceled update / insert of measure");
    }
  } else if (existingMeasure) {
    return *existingMeasure;
  } else {
    BCLMeasure projectmeasure = t_project.insertMeasure(*measure);
    return projectmeasure;
  }
}

std::vector<ruleset::OSArgument> MeasureManager::getArguments(analysisdriver::SimpleProject &t_project, const BCLMeasure &t_measure)
{
  boost::optional<BCLMeasure> projectMeasure = t_project.getMeasureByUUID(t_measure.uuid());
  boost::optional<openstudio::model::Model> appmodel = m_app->currentModel();

  if (projectMeasure
      && projectMeasure->versionUUID() == t_measure.versionUUID()
      && t_project.hasStoredArguments(*projectMeasure)
      && !appmodel)
  {
    LOG(Info, "returning stored arguments for measure " << t_measure.name() << "(" << toString(t_measure.uuid()) << " version: " << toString(t_measure.versionUUID()) << ")");
    return t_project.getStoredArguments(*projectMeasure);
  } else {
    boost::optional<openstudio::model::Model> model = t_project.seedModel();
    boost::optional<openstudio::Workspace> idf = t_project.seedIdf();

    if (appmodel)
    {
      model = appmodel;
      idf = boost::none;
    }

    std::vector<ruleset::OSArgument> args = m_argumentGetter->getArguments(t_measure, model, idf);
    LOG(Info, "Loaded " << args.size() << " arguments for measure " << t_measure.name() << "(" << toString(t_measure.uuid()) << " version: " << toString(t_measure.versionUUID()) << ")");
    t_project.registerArguments(t_measure, args);
    return args;
  }
}

std::string MeasureManager::suggestMeasureGroupName(const BCLMeasure &t_measure)
{
  std::string baseName = t_measure.name();

  baseName.append(" Group");

  std::set<std::string> allNames;
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() ){
    analysis::Analysis analysis = project->analysis();
    analysis::Problem problem = analysis.problem();
    Q_FOREACH(const analysis::InputVariable& variable, problem.variables()){
      allNames.insert(variable.name());
      allNames.insert(variable.displayName());
    }
  }

  std::string result = baseName;
  int i = 1;
  while (allNames.find(result) != allNames.end()){
    result = baseName + " " + QString::number(i).toStdString();
    i++;
  }

  return result;
}

std::string MeasureManager::suggestMeasureName(const BCLMeasure &t_measure, bool t_fixed)
{
  std::string baseName = t_measure.name();

  if (!t_fixed)
  {
    baseName.append(" Alternative");
  }

  std::set<std::string> allNames;
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() ){
    analysis::Analysis analysis = project->analysis();
    analysis::Problem problem = analysis.problem();
    Q_FOREACH(const analysis::InputVariable& variable, problem.variables()){
      boost::optional<analysis::MeasureGroup> discreteVariable = variable.optionalCast<analysis::MeasureGroup>();
      if (discreteVariable){
        Q_FOREACH(const analysis::Measure& measure, discreteVariable->measures(false)){
          allNames.insert(measure.name());
          allNames.insert(measure.displayName());
        }
      }
    }
  }

  std::string result = baseName;
  int i = 1;
  while (allNames.find(result) != allNames.end()){
    result = baseName + " " + QString::number(i).toStdString();
    i++;
  }

  return result;
}

void MeasureManager::updateMeasures(analysisdriver::SimpleProject &t_project, 
                                    const std::vector<BCLMeasure> &t_newMeasures, 
                                    bool t_showMessage)
{
  ProcessEventsProgressBar* progress = new ProcessEventsProgressBar();
  progress->setMaximum(std::numeric_limits<double>::max());

  size_t loc = 0;
  std::vector<std::string> failMessages;
  for (std::vector<BCLMeasure>::const_iterator itr = t_newMeasures.begin();
       itr != t_newMeasures.end();
       ++itr)
  {
    progress->setValue(loc);
    std::pair<bool,std::string> updateResult = updateMeasure(t_project, *itr);
    if (!updateResult.first) {
      failMessages.push_back(updateResult.second);
    }
    ++loc;
  }
  progress->setValue(t_newMeasures.size());

  delete progress;

  if (t_showMessage)
  {
    size_t numUpdated = loc - failMessages.size();
    size_t numFailed = failMessages.size();

    std::stringstream ss;

    ss << numUpdated << " measure";
    if (numUpdated == 0 || numUpdated > 1) {
      ss << "s";
    }
    ss << " updated";

    if (numFailed > 0) {
      ss << std::endl << numFailed << " measure update";
      if (numFailed > 1) {
        ss << "s";
      }
      ss << " failed";
      QString errors;
      Q_FOREACH(const std::string& failMessage,failMessages) {
        errors.append(QString::fromStdString(failMessage));
        errors.append("\n\n");
      }

      QMessageBox* messageBox = new QMessageBox(m_app->mainWidget());
      messageBox->setWindowTitle(QString("Measures Updated"));
      messageBox->setText(toQString(ss.str()));
      messageBox->setDetailedText(errors);
      //messageBox->setMinimumWidth(330);
      //messageBox->setSizeGripEnabled(true);

      // DLM: there is a bug in QMessageBox where setMinimumWidth is not used
      // http://www.qtcentre.org/threads/22298-QMessageBox-Controlling-the-width?p=113348#post113348
      QSpacerItem* horizontalSpacer = new QSpacerItem(330, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
      QGridLayout* layout = (QGridLayout*)messageBox->layout();
      layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
      
      messageBox->exec();  
      delete messageBox;
    }
    else {
      QMessageBox::information(m_app->mainWidget(), QString("Measures Updated"), toQString(ss.str()));
    }
  }
}

void MeasureManager::updateBCLMeasures(analysisdriver::SimpleProject &t_project)
{
  updateMeasuresLists();

  std::vector<BCLMeasure> toUpdate;

  std::vector<BCLMeasure> measures = bclMeasures();

  for (std::vector<BCLMeasure>::iterator itr = measures.begin();
      itr != measures.end();
      ++itr)
  {
    bool isNewVersion = itr->checkForUpdates();
    if (isNewVersion) {
      itr->save();
      m_bclMeasures.erase(itr->uuid());
      m_bclMeasures.insert(std::map<UUID,BCLMeasure>::value_type(itr->uuid(),*itr));
    }

    boost::optional<BCLMeasure> projectmeasure = t_project.getMeasureByUUID(itr->uuid());
    if (projectmeasure)
    {
      if (projectmeasure->versionUUID() != itr->versionUUID())
      {
        toUpdate.push_back(*itr);
      }
    }
  }

  updateMeasures(t_project, toUpdate);
}

void MeasureManager::downloadBCLMeasures()
{
  RemoteBCL* remoteBCL = new RemoteBCL();
  int numUpdates = remoteBCL->checkForMeasureUpdates();
  if (numUpdates == 0){
    QMessageBox::information(m_app->mainWidget(), "Measures Updated", "All measures are up-to-date.");
  }else{
    std::vector<BCLSearchResult> updates = remoteBCL->measuresWithUpdates();
    QStringList measureNames;
    Q_FOREACH(BCLSearchResult update, updates){
      measureNames.push_back(QString(0x2022) + " " + toQString(update.name()));
    }
    remoteBCL->updateMeasures();
    QMessageBox msg(m_app->mainWidget());
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle("Measures Updated");
    if (numUpdates == 1){
      msg.setText("1 measure has been updated.                   ");
    }else{
      msg.setText(QString::number(numUpdates) + " measures have been updated.                   ");
    }
    msg.setDetailedText(measureNames.join("\n"));
    msg.exec();
  }
  delete remoteBCL;
}

void MeasureManager::updateMyMeasures(analysisdriver::SimpleProject &t_project)
{
  updateMeasuresLists();

  std::vector<BCLMeasure> toUpdate;

  std::vector<BCLMeasure> measures = myMeasures();

  for (std::vector<BCLMeasure>::iterator itr = measures.begin();
      itr != measures.end();
      ++itr)
  {
    bool isNewVersion = itr->checkForUpdates();
    if (isNewVersion) {
      itr->save();
      m_myMeasures.erase(itr->uuid());
      m_myMeasures.insert(std::map<UUID,BCLMeasure>::value_type(itr->uuid(),*itr));
    }

    boost::optional<BCLMeasure> projectmeasure = t_project.getMeasureByUUID(itr->uuid());
    if (projectmeasure)
    {
      if (projectmeasure->versionUUID() != itr->versionUUID())
      {
        toUpdate.push_back(*itr);
      }
    }
  }

  updateMeasures(t_project, toUpdate);
}



std::vector<BCLMeasure> MeasureManager::bclMeasures()
{
  std::vector<BCLMeasure> result;

  for(std::map<UUID,BCLMeasure>::const_iterator it = m_bclMeasures.begin();
      it != m_bclMeasures.end();
      it++ )
  {
    result.push_back(it->second);
  }

  return result;
}


void MeasureManager::updateMeasuresLists()
{
  m_bclMeasures.clear();
  m_myMeasures.clear();

  std::vector<BCLMeasure> patAppMeasures = BCLMeasure::patApplicationMeasures();
  for( std::vector<BCLMeasure>::const_iterator it = patAppMeasures.begin();
       it != patAppMeasures.end();
       it++ )
  {
    m_bclMeasures.insert(std::make_pair<UUID,BCLMeasure>(it->uuid(),*it));
  }

  std::vector<BCLMeasure> localBCLMeasures = BCLMeasure::localBCLMeasures();
  for( std::vector<BCLMeasure>::const_iterator it = localBCLMeasures.begin();
       it != localBCLMeasures.end();
       it++ )
  {
    m_bclMeasures.insert(std::make_pair<UUID,BCLMeasure>(it->uuid(),*it));
  }

  std::vector<BCLMeasure> userMeasures = BCLMeasure::userMeasures();
  for( std::vector<BCLMeasure>::const_iterator it = userMeasures.begin();
       it != userMeasures.end();
       it++ )
  {
    m_myMeasures.insert(std::make_pair<UUID,BCLMeasure>(it->uuid(),*it));
  }

  if (m_libraryController)
  {
    m_libraryController->reset();
  }
}

std::vector<BCLMeasure> MeasureManager::myMeasures()
{
  std::vector<BCLMeasure> result;

  for(std::map<UUID,BCLMeasure>::const_iterator it = m_myMeasures.begin();
      it != m_myMeasures.end();
      it++ )
  {
    result.push_back(it->second);
  }

  return result;
}

void MeasureManager::addMeasure()
{
  // open modal dialog
  //QSharedPointer<BCLMeasureDialog> dialog(new BCLMeasureDialog(this->mainWindow));
  QSharedPointer<BCLMeasureDialog> dialog(new BCLMeasureDialog());

  if (dialog->exec()){

    // not canceled, create measure
    boost::optional<BCLMeasure> measure = dialog->createMeasure();
    if (measure){

      // open directory for editing
      QString path = QDir::toNativeSeparators(toQString(measure->directory()));
      QDesktopServices::openUrl(QUrl("file:///" + path));

      updateMeasuresLists();

      // emit signal
      emit newMeasure(*measure);
    }else{
      // error, this should not happen
    }

  }else{
    // user canceled, do nothing
  }
}

void MeasureManager::duplicateSelectedMeasure()
{
  OS_ASSERT(m_libraryController);
  QPointer<LibraryItem> item = m_libraryController->selectedItem();

  if( !item.isNull() )
  {
    boost::optional<BCLMeasure> bclMeasure = this->getMeasure(item->uuid());
    if (bclMeasure){
      // open modal dialog
      //QSharedPointer<BCLMeasureDialog> dialog(new BCLMeasureDialog(*bclMeasure, this->mainWindow));
      QSharedPointer<BCLMeasureDialog> dialog(new BCLMeasureDialog(*bclMeasure));

      if (dialog->exec()){

        // not canceled, create measure
        boost::optional<BCLMeasure> measure = dialog->createMeasure();
        if (measure){

          // open directory for editing
          QString path = QDir::toNativeSeparators(toQString(measure->directory()));
          QDesktopServices::openUrl(QUrl("file:///" + path));

          updateMeasuresLists();

          // emit signal
          emit newMeasure(*measure);
        }else{
          // error, this should not happen
        }

      }else{
        // user canceled, do nothing
      }
    }
  }
}

bool MeasureManager::isMeasureSelected()
{
  if (m_libraryController){
    QPointer<LibraryItem> item = m_libraryController->selectedItem();
    return !item.isNull();
  }
  return false;
}


boost::optional<BCLMeasure> MeasureManager::getMeasure(const UUID & id)
{
  std::map<UUID,BCLMeasure>::iterator it = m_bclMeasures.find(id);

  if( it != m_bclMeasures.end() ) { return it->second; }

  it = m_myMeasures.find(id);

  if( it != m_myMeasures.end() ) { return it->second; }

  return boost::optional<BCLMeasure>();
}

void MeasureManager::setLibraryController(const QSharedPointer<LocalLibraryController> &t_controller)
{
  m_libraryController = t_controller;
}

} // openstudio


