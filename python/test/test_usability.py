from pathlib import Path

import openstudio


def test_optional_not_initialized():
    d = openstudio.OptionalDouble()
    assert not d.is_initialized()
    assert d.empty()
    assert not bool(d)


def test_optionalinitialized():
    d = openstudio.OptionalDouble(10.0)
    assert d.is_initialized()
    assert not d.empty()
    assert bool(d)


def test_path():
    """Assert you can load a model with a str or a pathlib.Path without throwing."""
    assert openstudio.model.Model.load("wrong.osm").empty()
    assert openstudio.model.Model.load(Path("wrong.osm")).empty()


def test_json():
    """We can return jsoncpp objects to a native python dict."""
    idfFile = openstudio.IdfFile(openstudio.IddFileType("EnergyPlus"))
    building = openstudio.IdfObject(openstudio.IddObjectType("Building"))
    building.setName("Building 1")
    building.setDouble(1, 0.0)  # North Axis
    idfFile.addObject(building)
    d = openstudio.epjson.toJSON(idfFile)
    assert isinstance(d, dict)
    assert "Version" in d
    assert "Building" in d
    assert d["Building"]["Building 1"]["north_axis"] == 0
