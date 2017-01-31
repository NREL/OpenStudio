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

#ifndef SHAREDGUICOMPONENTS_EDITCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_EDITCONTROLLER_HPP

#include "../measure/OSArgument.hpp"
#include "../utilities/core/Logger.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer> 

class QWidget;

namespace openstudio {

class OSViewSwitcher;
  
class EditRubyMeasureView;
class EditNullView;
class InputController;
class InputView;
class BaseApp;

namespace measuretab {

  class MeasureStepItem;

}

class EditController : public QObject
{
  Q_OBJECT

  public:

  QPointer<OSViewSwitcher> editView;

  QPointer<EditRubyMeasureView> editRubyMeasureView;

  EditController(bool applyMeasureNow = false);

  virtual ~EditController();

  void setMeasureStepItem(measuretab::MeasureStepItem * measureItem, BaseApp *t_app);

  measuretab::MeasureStepItem * measureStepItem() const;

  // Show something when no RubyMeasure is selected
  void reset();

  private slots:

  void updateDescription();

  private:

  REGISTER_LOGGER("openstudio.pat.EditController");

  QPointer<EditNullView> m_editNullView;

  std::vector<QSharedPointer<InputController> > m_inputControllers;

  QPointer<measuretab::MeasureStepItem> m_measureStepItem;
};

class InputController : public QObject
{
  Q_OBJECT

  public:

  InputController(EditController * editController,const measure::OSArgument & argument, BaseApp *t_app);

  virtual ~InputController();

  QPointer<InputView> inputView;

  private slots:

  void setValue(const QString & value);

  void setValue(bool value);

  void setValueForIndex(int index);

  private:

  bool isItOKToClearResults();

  bool isArgumentIncomplete() const;

  BaseApp *m_app;

  QPointer<EditController> m_editController;

  measure::OSArgument m_argument;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_EDITCONTROLLER_HPP

