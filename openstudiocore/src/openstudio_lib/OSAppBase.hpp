/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef OPENSTUDIO_OSAPPBASE_HPP
#define OPENSTUDIO_OSAPPBASE_HPP

#include "../shared_gui_components/BaseApp.hpp"

#include "../measure/OSMeasureInfoGetter.hpp"

#include "OpenStudioAPI.hpp"
#include "../utilities/core/Logger.hpp"

#include <QApplication>

#include <boost/smart_ptr.hpp>

class QEvent;

namespace openstudio {

class OSDocument;

class WaitDialog;

class OPENSTUDIO_API OSAppBase : public QApplication, public BaseApp
{

  Q_OBJECT;

  public:

  OSAppBase( int & argc, char ** argv, const QSharedPointer<MeasureManager> &t_measureManager);

  virtual ~OSAppBase();

  virtual std::shared_ptr<OSDocument> currentDocument() const = 0;

  static OSAppBase * instance();

  virtual QWidget *mainWidget() override;
  virtual MeasureManager &measureManager() override;
  virtual boost::optional<openstudio::path> tempDir() override;
  virtual boost::optional<openstudio::model::Model> currentModel() override;
  //virtual boost::optional<openstudio::Workspace> currentWorkspace() override;
  virtual void updateSelectedMeasureState() override;
  virtual void addMeasure() override;
  virtual void duplicateSelectedMeasure() override;
  virtual void updateMyMeasures() override;
  virtual void updateBCLMeasures() override;
  virtual void downloadUpdatedBCLMeasures() override;
  virtual void openBclDlg() override;
  virtual void chooseHorizontalEditTab() override;
  virtual QSharedPointer<openstudio::EditController> editController() override;
  boost::shared_ptr<WaitDialog> waitDialog() {return m_waitDialog;}
  virtual bool notify(QObject * receiver, QEvent * e) override;

  // Slots
  void addWorkspaceObject(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);
  void addWorkspaceObjectPtr(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  void removeWorkspaceObject(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);
  void removeWorkspaceObjectPtr(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid );

  signals:
  void workspaceObjectAdded(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);
  void workspaceObjectAddedPtr(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  void workspaceObjectRemoved(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);
  void workspaceObjectRemovedPtr(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  protected:

  virtual bool event(QEvent * e) override;

  virtual void childEvent(QChildEvent * e) override;

  REGISTER_LOGGER("openstudio.OSAppBase");

  private:

  QSharedPointer<openstudio::MeasureManager> m_measureManager;

  boost::shared_ptr<WaitDialog> m_waitDialog;

  public slots:

  virtual void reloadFile(const QString& osmPath, bool modified, bool saveCurrentTabs) = 0;

  void showMeasureUpdateDlg();
};

} // openstudio

#endif // OPENSTUDIO_OSAPPBASE_HPP

