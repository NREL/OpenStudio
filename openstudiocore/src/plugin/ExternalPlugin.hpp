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

#ifndef OPENSTUDIO_EXTERNALPLUGIN_HPP
#define OPENSTUDIO_EXTERNALPLUGIN_HPP

#include <plugin/PluginAPI.hpp>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/core/Logger.hpp>

#include <vector>

#include <QObject>
#include <QVariant>

namespace openstudio {
namespace model {
  namespace detail {
    class Model_Impl;
  }
}
namespace plugin {

class ExternalModelInterface;
class OSPlugin;
class OSPluginThread;
class OSDocument;
class ModelHolder;

/** ExternalPlugin is a class which may be used to create an OpenStudio Plugin
*   in an external program such as SketchUp. 
**/
class PLUGIN_API ExternalPlugin : public QObject
{

  Q_OBJECT

  public:

    /// Constructor, there should only be one of these objects
    ExternalPlugin();

    /// Destructor, shuts down OSPlugin
    virtual ~ExternalPlugin();

    /// Processes events on the main thread, use a timer to create an event loop
    void processEvents(int maxTime);

    /// Sends a signal to request the startup prompt.
    void requestStartup(const std::string& key);

    /// Returns true if the matching document is visible.
    bool isVisible(const std::string& key) const;

    /// Shows the matching document
    void show(const std::string& key);

    /// Hides the matching document
    void hide(const std::string& key);

    /// Called when OSPlugin is initialized
    virtual void start();

    /// Called when a new OpenStudio Model is created
    virtual void onNewModel(const std::string& key, const ModelHolder& modelHolder);

    /// Called when an OpenStudio Model is closed
    virtual void onModelClosed(const std::string& key);

    /// Called when an OSDocument is shown
    virtual void onDocumentShown(const std::string& key);

    /// Called when an OSDocument is hidden
    virtual void onDocumentHidden(const std::string& key);

  protected slots:

    /// Called when a new OpenStudio Model is created
    void newModel(const std::string& key, openstudio::model::detail::Model_Impl* model);

    /// Called when an OpenStudio Model is closed
    void modelClosed(const std::string& key);

    /// Called when an OSDocument is shown
    void documentShown(const std::string& key);

    /// Called when an OSDocument is hidden
    void documentHidden(const std::string& key);

  signals:

    /// Signal to request the startup prompt.
    void startupRequested(const std::string& key);

    /// Signal to show the given document
    void showRequested(const std::string& key);

    /// Signal to hide the given document
    void hideRequested(const std::string& key);

    /// Signal that the external plugin is quitting
    void quitting();

  private:

    REGISTER_LOGGER("ExternalPlugin");

    bool m_started;
    OSPluginThread* m_osPluginThread;
    std::map<std::string, bool> m_isVisibleMap;

};

} // plugin
} // openstudio

#endif // OPENSTUDIO_EXTERNALPLUGIN_HPP

