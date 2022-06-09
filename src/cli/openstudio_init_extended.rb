# This is called from inside of Kernel::require
# and the autoload constants still exist at this point
# so we manually remove them here to avoid conflicts
# with the modules registered in init_rest_of_openstudio

::OpenStudio.module_eval { remove_const(:Airflow) }
::OpenStudio.module_eval { remove_const(:EnergyPlus) }
::OpenStudio.module_eval { remove_const(:EPJSON) }
::OpenStudio.module_eval { remove_const(:GbXML) }
::OpenStudio.module_eval { remove_const(:Gltf) }
::OpenStudio.module_eval { remove_const(:ISOModel) }
::OpenStudio.module_eval { remove_const(:Measure) }
::OpenStudio.module_eval { remove_const(:Ruleset) }
::OpenStudio.module_eval { remove_const(:Model) }
::OpenStudio.module_eval { remove_const(:OSVersion) }
::OpenStudio.module_eval { remove_const(:Radiance) }
::OpenStudio.module_eval { remove_const(:SDD) }

OpenStudio::init_rest_of_openstudio()


