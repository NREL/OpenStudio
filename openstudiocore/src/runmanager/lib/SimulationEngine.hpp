#ifndef OPENSTUDIO_RUNMANAGER_SIMULATIONENGINE_HPP
#define OPENSTUDIO_RUNMANAGER_SIMULATIONENGINE_HPP

#include <utilities/core/Path.hpp>
#include <utilities/core/UUID.hpp>
#include <vector>
#include <model/Model.hpp>
#include <utilities/core/Logger.hpp>
#include <runmanager/lib/RunManager.hpp>
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
            const openstudio::path &t_weatherFile = openstudio::path());

        /// \returns the most accurate FuelUses available for the given set of variables. If a model has not 
        ///          yet been queued for these variables, an exception is thrown
        openstudio::runmanager::FuelUses fuelUses(const std::vector<double> &t_variables) const;

        /// \returns the SimulationDetails for the given set of variables if the final simulation has been
        ///          executed. Returns a SimulationDetails with confidence 0 if nothing could be found
        openstudio::runmanager::SimulationDetails details(const std::vector<double> &t_variables) const;

      private slots:
        void jobTreeChanged(const openstudio::UUID &);

      private:
        REGISTER_LOGGER("opendtudio.runmanager.SimulationEngine");

        size_t m_numVariables;
        openstudio::path m_folder;
        ErrorEstimation m_errorEstimations;
        RunManager m_runManager;
        std::set<std::vector<double> > m_simulations; // simulations in progress already
        std::map<std::vector<double>, SimulationDetails> m_details;

        static openstudio::path validateFolder(const openstudio::path &t_folder);
        void connectSignals(const openstudio::runmanager::Job &t_job);
        void loadResults(const openstudio::runmanager::Job &t_job);
        void enqueueSimulations(const openstudio::model::Model &t_model, const std::vector<double> &t_variables, 
            const openstudio::path &t_weatherFile);

        void validateNumVariables(const std::vector<double> &t_variables) const;

        openstudio::runmanager::Job createEstimationJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
            const std::vector<double> &t_variables, const openstudio::path &t_weatherFile);

        openstudio::runmanager::Job createFullJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
            const std::vector<double> &t_variables, const openstudio::path &t_weatherFile);

        openstudio::runmanager::Job createRadianceJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
            const std::vector<double> &t_variables, const openstudio::path &t_weatherFile);

        openstudio::runmanager::Job createJob(const openstudio::path &t_path, openstudio::model::Model t_model,
            const std::vector<double> &t_variables, const std::string &t_runType,
            bool t_estimation, int t_numParallel, bool t_radiance, const openstudio::path &t_weatherFile);

        static std::string estimationJobString();
        static std::string fullJobString();
        static std::string radianceJobString();

        static std::vector<double> getVariables(const openstudio::runmanager::Job &t_job);
        static std::string toString(const std::vector<double> &t_variables);
        static std::vector<double> fromString(const std::string &_string);

        static openstudio::path getSqlFilePath(const openstudio::runmanager::Job &t_job);
    };

  }
}


#endif
