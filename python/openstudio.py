# imports all of the openstudio libraries into a friendly namespace

if __package__ or "." in __name__:

    from . import openstudioairflow as airflow
    from . import openstudioenergyplus as energyplus
    from . import openstudiogbxml as gbxml
    from . import openstudioisomodel as isomodel
    from . import openstudiomeasure as measure

    from . import openstudiomodel as model
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

    from . import openstudioosversion as osversion
    from . import openstudioradiance as radiance
    from . import openstudiosdd as sdd
    from .openstudioutilities import *
    from .openstudioutilitiesbcl import *
    from .openstudioutilitiescore import *
    from .openstudioutilitiesdata import *
    from .openstudioutilitiesgeometry import *
    from .openstudioutilitiesidd import *
    from .openstudioutilitiesidf import *
    # from .openstudioutilitiesfiletypes import *
    # from .openstudioutilitiesplot import *
    from .openstudioutilitiessql import *
    from .openstudioutilitiestime import *
    from .openstudioutilitiesunits import *

else:

    import openstudioairflow as airflow
    import openstudioenergyplus as energyplus
    import openstudiogbxml as gbxml
    import openstudioisomodel as isomodel
    import openstudiomeasure as measure

    import openstudiomodel as model
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
    from openstudioutilitiesgeometry import *
    from openstudioutilitiesidd import *
    from openstudioutilitiesidf import *
    # from openstudioutilitiesfiletypes import *
    # from openstudioutilitiesplot import *
    from openstudioutilitiessql import *
    from openstudioutilitiestime import *
    from openstudioutilitiesunits import *
