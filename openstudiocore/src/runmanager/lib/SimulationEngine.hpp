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

#ifndef RUNMANAGER_LIB_SIMULATIONENGINE_HPP
#define RUNMANAGER_LIB_SIMULATIONENGINE_HPP

#include "../../utilities/core/Path.hpp"
#include "../../utilities/core/UUID.hpp"
#include <vector>
#include "../../model/Model.hpp"
#include "../../utilities/core/Logger.hpp"
#include "RunManager.hpp"
#include "RunManagerAPI.hpp"
#include "ErrorEstimation.hpp"

namespace openstudio {
  namespace runmanager {
    struct RUNMANAGER_API SimulationDetails
    {
      SimulationDetails(double t_confidence=0, const openstudio::path &t_sqlFilePath=openstudio::path())
        : confidence(t_confidence), sqlFilePath(t_sqlFilePath)
      {
      }

      double confidence;
      openstudio::path sqlFilePath;
        
    };



    class RUNMANAGER_API SimulationEngine : public QObject
    {
      Q_OBJECT;

      public:
        /// Create a simulation engine which makes available varying levels of accuracy for a simulation
        SimulationEngine(const openstudio::path &t_cacheFolder, size_t t_numVariables);

        /// \returns an estimated FuelUses for the given model / variables. If it's not possible to 
        ///          generate an estimate, an FuelUses with 0 confidence is returned
        openstudio::runmanager::FuelUses fuelUses(const openstudio::model::Model &t_model, const std::vector<double> &t_variables, 
            const std::vector<int> &t_discreteVariables,
            const std::string &t_simulationId = std::string(),
            const openstudio::path &t_weatherFile = openstudio::path());

        /// \returns the most accurate FuelUses available for the given set of variables. If a model has not 
        ///          yet been queued for these variables, an exception is thrown
        openstudio::runmanager::FuelUses fuelUses(const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables) const;

        openstudio::runmanager::FuelUses fuelUses(const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables);

        openstudio::runmanager::FuelUses fuelUses(const std::string &t_simulationId) const;

        /// \returns the SimulationDetails for the given set of variables if the final simulation has been
        ///          executed. Returns a SimulationDetails with confidence 0 if nothing could be found
        openstudio::runmanager::SimulationDetails details(const std::vector<double> &t_variables,
            const std::vector<int> &t_discreteVariables) const;

        openstudio::runmanager::SimulationDetails details(const std::string &t_stimulationId) const;

      private slots:
        void jobTreeChanged(const openstudio::UUID &);

      private:
        REGISTER_LOGGER("opendtudio.runmanager.SimulationEngine");

        size_t m_numVariables;
        openstudio::path m_folder;
        std::map<std::vector<int>, ErrorEstimation> m_errorEstimations;
        RunManager m_runManager;
        std::set<std::pair<std::vector<double>, std::vector<int> > > m_simulations; // simulations in progress already
        std::map<std::pair<std::vector<double>, std::vector<int> >, SimulationDetails> m_details;

        std::map<std::string, std::pair<std::vector<double>, std::vector<int> > > m_simulationIds;

        static openstudio::path validateFolder(const openstudio::path &t_folder);
        void connectSignals(const openstudio::runmanager::Job &t_job);
        void loadResults(const openstudio::runmanager::Job &t_job);
        void enqueueSimulations(const openstudio::model::Model &t_model, const std::vector<double> &t_variables, 
            const std::vector<int> &t_discreteVariables,
            const std::string &t_simulationId,
            const openstudio::path &t_weatherFile);

        void validateNumVariables(const std::vector<double> &t_variables) const;

        openstudio::runmanager::Job createEstimationJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
            const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables,
            const std::string &t_simulationId,
            const openstudio::path &t_weatherFile);

        openstudio::runmanager::Job createFullJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
            const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables,
            const std::string &t_simulationId,
            const openstudio::path &t_weatherFile);

        openstudio::runmanager::Job createRadianceJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
            const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables,
            const std::string &t_simulationId,
            const openstudio::path &t_weatherFile);

        openstudio::runmanager::Job createJob(const openstudio::path &t_path, openstudio::model::Model t_model,
            const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables,
            const std::string &t_simulationId,
            const std::string &t_runType,
            bool t_estimation, int t_numParallel, bool t_radiance, const openstudio::path &t_weatherFile);

        static std::string estimationJobString();
        static std::string fullJobString();
        static std::string radianceJobString();

        static std::vector<double> getVariables(const openstudio::runmanager::Job &t_job);
        static std::vector<int> getDiscreteVariables(const openstudio::runmanager::Job &t_job);

        template<typename T>
        static std::string toString(const std::vector<T> &t_variables);

        template<typename T>
        static std::vector<T> fromString(const std::string &_string);

        static openstudio::path getSqlFilePath(const openstudio::runmanager::Job &t_job);

        ErrorEstimation &getErrorEstimation(const std::vector<int> &t_discreteVariables);
        const ErrorEstimation &getErrorEstimation(const std::vector<int> &t_discreteVariables) const;

        std::pair<std::vector<double>, std::vector<int> > lookupSimulationId(const std::string &t_simulationId) const;
    };

  }
}


#endif // RUNMANAGER_LIB_SIMULATIONENGINE_HPP
