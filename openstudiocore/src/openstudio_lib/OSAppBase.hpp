/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef OPENSTUDIO_OSAPPBASE_HPP
#define OPENSTUDIO_OSAPPBASE_HPP

#include "../shared_gui_components/BaseApp.hpp"

#include "../ruleset/RubyUserScriptArgumentGetter.hpp"

#include "OpenStudioAPI.hpp"
#include "../utilities/core/Logger.hpp"

#include <QApplication>

#include <boost/smart_ptr.hpp>

namespace openstudio {

class OSDocument;

class WaitDialog;

class OPENSTUDIO_API OSAppBase : public QApplication, public BaseApp
{

  Q_OBJECT

  public:

  OSAppBase( int & argc, char ** argv, const QSharedPointer<MeasureManager> &t_measureManager);

  virtual ~OSAppBase();

  virtual std::shared_ptr<OSDocument> currentDocument() const = 0;

  static OSAppBase * instance();

  virtual boost::optional<openstudio::analysisdriver::SimpleProject> project();
  virtual QWidget *mainWidget();
  virtual MeasureManager &measureManager();
  virtual boost::optional<openstudio::model::Model> currentModel();
  virtual void updateSelectedMeasureState();
  virtual void addMeasure();
  virtual void duplicateSelectedMeasure();
  virtual void updateMyMeasures();
  virtual void updateBCLMeasures();
  virtual void downloadUpdatedBCLMeasures();
  virtual void openBclDlg();
  virtual void chooseHorizontalEditTab();
  virtual QSharedPointer<openstudio::EditController> editController();
  boost::shared_ptr<WaitDialog> waitDialog() {return m_waitDialog;}

  public slots:

    virtual void reloadFile(const QString& fileToLoad, bool modified, bool saveCurrentTabs) = 0;

  private:

  REGISTER_LOGGER("openstudio.OSAppBase");

  QSharedPointer<openstudio::MeasureManager> m_measureManager;

  boost::shared_ptr<WaitDialog> m_waitDialog;
};

} // openstudio

#endif // OPENSTUDIO_OSAPPBASE_HPP

