// clang-format off
// SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: Apache-2.0
// clang-format on

#include <clang/AST/Attr.h>
#include <clang/AST/DeclCXX.h>

#include "canopy.hpp"

#include <algorithm>

namespace canopy {

execution_space get_execution_space(const clang::FunctionDecl *FD) {
  using namespace clang;
  if (FD->hasAttr<CUDAGlobalAttr>()) {
    return execution_space::global_;
  } else if (FD->hasAttr<CUDAHostAttr>() && FD->hasAttr<CUDADeviceAttr>()) {
    return execution_space::host_device;
  } else if (FD->hasAttr<CUDAHostAttr>()) {
    return execution_space::host;
  } else if (FD->hasAttr<CUDADeviceAttr>()) {
    return execution_space::device;
  } else {
    return execution_space::undefined;
  }
}

Function::Function(const clang::FunctionDecl *FD)
    : name(FD->getNameAsString()),
      return_type(FD->getReturnType(), FD->getASTContext()) {
  params.reserve(FD->getNumParams());
  std::transform(FD->param_begin(), FD->param_end(), std::back_inserter(params),
                 [](const clang::ParmVarDecl *PVD) { return ParamVar(PVD); });
  exec_space = get_execution_space(FD);
}
} // namespace canopy
