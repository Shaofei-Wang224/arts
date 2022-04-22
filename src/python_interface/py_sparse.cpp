#include <py_auto_interface.h>

#include <pybind11/eigen.h>

#include "py_macros.h"

namespace Python {
void py_sparse(py::module_& m) {
  py::class_<Sparse>(m, "Sparse")
      .def(py::init([]() { return new Sparse{}; }))
      .def(py::init([](Eigen::SparseMatrix<Numeric, Eigen::RowMajor> es) {
        Sparse s;
        s.matrix.swap(es);
        return s;
      }))
      .PythonInterfaceCopyValue(Sparse)
      .PythonInterfaceWorkspaceVariableConversion(Sparse)
      .PythonInterfaceFileIO(Sparse)
      .PythonInterfaceBasicRepresentation(Sparse)
      .def(
          "__getitem__",
          [](Sparse& x, std::tuple<Index, Index> ind) -> Numeric& {
            const auto [r, c] = ind;
            if (r < 0 or r >= x.nrows())
              throw std::out_of_range(var_string("row ", r));
            if (c < 0 or c >= x.ncols())
              throw std::out_of_range(var_string("col ", c));
            return x.rw(r, c);
          },
          py::return_value_policy::reference_internal)
      .def("__setitem__",
           [](Sparse& x, std::tuple<Index, Index> ind, Numeric y) {
             const auto [r, c] = ind;
             if (r < 0 or r >= x.nrows())
               throw std::out_of_range(var_string("row ", r));
             if (c < 0 or c >= x.ncols())
               throw std::out_of_range(var_string("col ", c));
             x.rw(r, c) = y;
           })
      .def_property(
          "value",
          [](Sparse& s) { return s.matrix; },
          [](Sparse& s, Eigen::SparseMatrix<Numeric, Eigen::RowMajor> ns) {
            s.matrix.swap(ns);
          })
      .def("toarray", [](Sparse& sp) { return Eigen::MatrixXd(sp.matrix); })
      .def(py::pickle(
          [](const Sparse& self) { return py::make_tuple(self.matrix); },
          [](const py::tuple& t) {
            ARTS_USER_ERROR_IF(t.size() != 1, "Invalid state!")

            auto* out = new Sparse{};
            out->matrix = t[0].cast<decltype(out->matrix)>();

            return out;
          }));
  py::implicitly_convertible<Eigen::SparseMatrix<Numeric, Eigen::RowMajor>, Sparse>();

  py::enum_<Block::MatrixType>(m, "BlockMatrixType")
      .value("dense", Block::MatrixType::dense)
      .value("sparse", Block::MatrixType::sparse)
      .PythonInterfaceCopyValue(Block::MatrixType)
      .def(py::pickle(
          [](const Block::MatrixType& self) {
            return py::make_tuple(static_cast<Index>(self));
          },
          [](const py::tuple& t) {
            ARTS_USER_ERROR_IF(t.size() != 1, "Invalid state!")
            
            return static_cast<Block::MatrixType>(t[0].cast<Index>());
          }));

  py::class_<Block>(m, "Block")
      .PythonInterfaceCopyValue(Block)
      .def("get_matrix_type", &Block::get_matrix_type)
      .def_property_readonly("dense",
                             [](Block& x) {
                               ARTS_USER_ERROR_IF(x.get_matrix_type() not_eq
                                                      Block::MatrixType::dense,
                                                  "Block is not dense")
                               return x.get_dense();
                             })
      .def_property_readonly("sparse", [](Block& x) {
        ARTS_USER_ERROR_IF(x.get_matrix_type() not_eq Block::MatrixType::sparse,
                           "Block is not sparse")
        return x.get_sparse();
      });

  py::class_<CovarianceMatrix>(m, "CovarianceMatrix")
      .def(py::init([]() { return new CovarianceMatrix{}; }))
      .PythonInterfaceCopyValue(CovarianceMatrix)
      .PythonInterfaceWorkspaceVariableConversion(CovarianceMatrix)
      .def_property(
          "blocks",
          [](CovarianceMatrix& x) { return x.get_blocks(); },
          [](CovarianceMatrix& x, std::vector<Block> y) {
            x.get_blocks() = std::move(y);
          })
      .PythonInterfaceFileIO(CovarianceMatrix)
      .PythonInterfaceBasicRepresentation(CovarianceMatrix);

  PythonInterfaceWorkspaceArray(Sparse);
}
}  // namespace Python