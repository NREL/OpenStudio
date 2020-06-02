# imports all of the openstudio libraries into a friendly namespace


def import_module(module_name, namespace=None):
    """ Helper to require a given module, and place it in a namespace or import
    all in global namespace.

    """
    statement = None
    if namespace:
        if __package__ or "." in __name__:
            statement = "from . import {} as {}".format(module_name, namespace)
        else:
            statement = "import {} as {}".format(module_name, namespace)
    else:
        if __package__ or "." in __name__:
            statement = "from .{} import *".format(module_name)
        else:
            statement = "from {} import *".format(module_name)
    try:
        exec(statement)
    except ImportError as e:
        # Output expected (until fixed) ImportErrors.
        print(e.__class__.__name__ + ": " + str(e))
    except Exception as e:
        # Output unexpected Exceptions.
        print(e, False)
        print(e.__class__.__name__ + ": " + str(e))


imports = [
    # (Module, Namespace)
    ('openstudioairflow', 'airflow'),
    ('openstudioenergyplus', 'energyplus'),
    ('openstudiogbxml', 'gbxml'),
    ('openstudioisomodel', 'isomodel'),
    ('openstudiomeasure', 'measure'),
    ('openstudiomodel', 'model'),
    # ('openstudiomodelcore', 'modelcore'),
    # ('openstudiomodelgenerators', 'modelgenerators'),
    # ('openstudiomodelgeometry', 'modelgeometry'),
    # ('openstudiomodelhvac', 'modelhvac'),
    # ('openstudiomodelrefrigeration', 'modelrefrigeration'),
    # ('openstudiomodelresources', 'modelresources'),
    # ('openstudiomodelsimulation', 'modelsimulation'),
    # ('openstudiomodelairflow', 'modelairflow'),
    # ('openstudiomodelavailabilitymanager', 'modelavailabilitymanager'),
    # ('openstudiomodelplantequipmentoperationscheme', 'modelplantequipmentoperationscheme'),
    # ('openstudiomodelstraightcomponent', 'modelstraightcomponent'),
    # ('openstudiomodelzonehvac', 'momodelzonehvac'),
    ('openstudioosversion', 'osversion'),
    ('openstudioradiance', 'radiance'),
    ('openstudiosdd', 'sdd'),
    ('openstudioutilities', None),
    ('openstudioutilitiesbcl', None),
    ('openstudioutilitiescore', None),
    ('openstudioutilitiesdata', None),
    ('openstudioutilitiesgeometry', None),
    ('openstudioutilitiesidd', None),
    ('openstudioutilitiesidf', None),
    # ('openstudioutilitiesfiletypes', None),
    # ('openstudioutilitiesplot', None),
    ('openstudioutilitiessql', None),
    ('openstudioutilitiestime', None),
    ('openstudioutilitiesunits', None),
]

for (module_name, namespace) in imports:
    import_module(module_name, namespace)
