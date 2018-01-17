/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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
