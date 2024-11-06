########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

if __package__ or "." in __name__:
    # These are already included in the `model` namespace via Model.i
    # from . import openstudiomodelcore as modelcore
    # from . import openstudiomodelgenerators as modelgenerators
    # from . import openstudiomodelgeometry as modelgeometry
    # from . import openstudiomodelhvac as modelhvac
    # from . import openstudiomodelrefrigeration as modelrefrigeration
    # from . import openstudiomodelresources as modelresources
    # from . import openstudiomodelsimulation as modelsimulation
    # from . import openstudiomodelairflow as modelairflow
    # from . import openstudiomodelavailabilitymanager as modelavailabilitymanager
    # from . import openstudiomodelplantequipmentoperationscheme as modelplantequipmentoperationscheme
    # from . import openstudiomodelstraightcomponent as modelstraightcomponent
    # from . import openstudiomodelzonehvac as momodelzonehvac
    from . import openstudioairflow as airflow
    from . import openstudioenergyplus as energyplus
    from . import openstudioepjson as epjson
    from . import openstudiogbxml as gbxml
    from . import openstudiogltf as gltf
    from . import openstudioisomodel as isomodel
    from . import openstudiomeasure as measure
    from . import openstudiomodel as model
    from . import openstudioosversion as osversion
    from . import openstudioradiance as radiance
    from . import openstudiosdd as sdd
    from . import openstudioalfalfa as alfalfa
    from .openstudioutilities import *
    from .openstudioutilitiesbcl import *
    from .openstudioutilitiescore import *
    from .openstudioutilitiesdata import *
    from .openstudioutilitiesfiletypes import *
    from .openstudioutilitiesgeometry import *
    from .openstudioutilitiesidd import *
    from .openstudioutilitiesidf import *

    # from .openstudioutilitiesplot import *
    from .openstudioutilitiessql import *
    from .openstudioutilitiestime import *
    from .openstudioutilitiesunits import *
    from .openstudioutilitiesxml import *

else:
    import os
    if os.name == 'nt':
        # When we're using system python to load the **installed** C:\openstudio-X.Y-Z\Python stuff (not PyPi package)
        # This allows finding openstudiolib.dll and the msvc ones in the bin/ folder while we're in the Python/ folder
        # Otherwise you'd have to manually copy these DLLs from bin/ to Python/
        os.add_dll_directory(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'bin')))
    import openstudioairflow as airflow
    import openstudioenergyplus as energyplus
    import openstudioepjson as epjson
    import openstudiogbxml as gbxml
    import openstudiogltf as gltf
    import openstudioisomodel as isomodel
    import openstudiomeasure as measure
    import openstudiomodel as model
    import openstudioalfalfa as alfalfa

    # These are already included in the `model` namespace via Model.i
    # import openstudiomodelcore as modelcore
    # import openstudiomodelgenerators as modelgenerators
    # import openstudiomodelgeometry as modelgeometry
    # import openstudiomodelhvac as modelhvac
    # import openstudiomodelrefrigeration as modelrefrigeration
    # import openstudiomodelresources as modelresources
    # import openstudiomodelsimulation as modelsimulation
    # import openstudiomodelairflow as modelairflow
    # import openstudiomodelavailabilitymanager as modelavailabilitymanager
    # import openstudiomodelplantequipmentoperationscheme as modelplantequipmentoperationscheme
    # import openstudiomodelstraightcomponent as modelstraightcomponent
    # import openstudiomodelzonehvac as momodelzonehvac
    import openstudioosversion as osversion
    import openstudioradiance as radiance
    import openstudiosdd as sdd
    from openstudioutilities import *
    from openstudioutilitiesbcl import *
    from openstudioutilitiescore import *
    from openstudioutilitiesdata import *
    from openstudioutilitiesfiletypes import *
    from openstudioutilitiesgeometry import *
    from openstudioutilitiesidd import *
    from openstudioutilitiesidf import *

    # from openstudioutilitiesplot import *
    from openstudioutilitiessql import *
    from openstudioutilitiestime import *
    from openstudioutilitiesunits import *
    from openstudioutilitiesxml import *
