#include <arts_constants.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

namespace Python {
namespace py = pybind11;

struct ConstantDummy {};

#define PythonInterfaceConstant(name) constants.attr(#name) = Constant::name;

void py_constants(py::module_& m) {
  auto constants = m.def_submodule("constants", R"--(Contain copies of constants of Arts internals
)--");

  PythonInterfaceConstant(pi);
  PythonInterfaceConstant(c);
  PythonInterfaceConstant(h);
  PythonInterfaceConstant(h_bar);
  PythonInterfaceConstant(k);
}
}  // namespace Python
