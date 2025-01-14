
#include <py_auto_interface.h>

#include "py_macros.h"

namespace Python {
void py_nlte(py::module_& m) {
py::class_<EnergyLevelMapType>(m, "EnergyLevelMapType")
      .def(py::init([]() { return EnergyLevelMapType{}; }), "Default type")
      .def(py::init([](const std::string& c) {
        return toEnergyLevelMapTypeOrThrow(c);
      }), "From :class:`str`")
      .PythonInterfaceCopyValue(EnergyLevelMapType)
      .PythonInterfaceBasicRepresentation(EnergyLevelMapType)
      .def(py::pickle(
          [](const EnergyLevelMapType& t) {
            return py::make_tuple(std::string(toString(t)));
          },
          [](const py::tuple& t) {
            ARTS_USER_ERROR_IF(t.size() != 1, "Invalid state!")
            return EnergyLevelMapType{
                toEnergyLevelMapType(t[0].cast<std::string>())};
          })).doc() = "Type of energy level";
  py::implicitly_convertible<std::string, EnergyLevelMapType>();

  py::class_<EnergyLevelMap>(m, "EnergyLevelMap")
      .def(py::init([]() { return std::make_unique<EnergyLevelMap>(); }), "Default map")
      .PythonInterfaceCopyValue(EnergyLevelMap)
      .PythonInterfaceWorkspaceVariableConversion(EnergyLevelMap)
      .PythonInterfaceBasicRepresentation(EnergyLevelMap)
      .PythonInterfaceFileIO(EnergyLevelMap)
      .PythonInterfaceReadWriteData(EnergyLevelMap, type, ":class:`~pyarts.arts.EnergyLevelMapType` Type")
      .PythonInterfaceReadWriteData(EnergyLevelMap, levels, ":class:`~pyarts.arts.ArrayOfQuantumIdentifier` List of levels")
      .PythonInterfaceReadWriteData(EnergyLevelMap, vib_energy, ":class:`~pyarts.arts.Vector` Vibrational energies")
      .PythonInterfaceReadWriteData(EnergyLevelMap, value, ":class:`~pyarts.arts.Tensor4` Data")
      .def(py::pickle(
          [](const EnergyLevelMap& t) {
            return py::make_tuple(t.type, t.levels, t.vib_energy, t.value);
          },
          [](const py::tuple& t) {
            ARTS_USER_ERROR_IF(t.size() != 4, "Invalid state!")
            auto  out = std::make_unique<EnergyLevelMap>();
            out->type = t[0].cast<EnergyLevelMapType>();
            out->levels = t[1].cast<ArrayOfQuantumIdentifier>();
            out->vib_energy = t[2].cast<Vector>();
            out->value = t[3].cast<Tensor4>();
            return out;
          }))
      .PythonInterfaceWorkspaceDocumentation(EnergyLevelMap);
}
}  // namespace Python