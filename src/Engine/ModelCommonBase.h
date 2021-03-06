/*
Copyright (c) 2009-2012, UT-Battelle, LLC
All rights reserved

[DMRG++, Version 2.0.0]
[by G.A., Oak Ridge National Laboratory]

UT Battelle Open Source Software License 11242008

OPEN SOURCE LICENSE

Subject to the conditions of this License, each
contributor to this software hereby grants, free of
charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), a
perpetual, worldwide, non-exclusive, no-charge,
royalty-free, irrevocable copyright license to use, copy,
modify, merge, publish, distribute, and/or sublicense
copies of the Software.

1. Redistributions of Software must retain the above
copyright and license notices, this list of conditions,
and the following disclaimer.  Changes or modifications
to, or derivative works of, the Software should be noted
with comments and the contributor and organization's
name.

2. Neither the names of UT-Battelle, LLC or the
Department of Energy nor the names of the Software
contributors may be used to endorse or promote products
derived from this software without specific prior written
permission of UT-Battelle.

3. The software and the end-user documentation included
with the redistribution, with or without modification,
must include the following acknowledgment:

"This product includes software produced by UT-Battelle,
LLC under Contract No. DE-AC05-00OR22725  with the
Department of Energy."

*********************************************************
DISCLAIMER

THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER, CONTRIBUTORS, UNITED STATES GOVERNMENT,
OR THE UNITED STATES DEPARTMENT OF ENERGY BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

NEITHER THE UNITED STATES GOVERNMENT, NOR THE UNITED
STATES DEPARTMENT OF ENERGY, NOR THE COPYRIGHT OWNER, NOR
ANY OF THEIR EMPLOYEES, REPRESENTS THAT THE USE OF ANY
INFORMATION, DATA, APPARATUS, PRODUCT, OR PROCESS
DISCLOSED WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*********************************************************


*/
/** \ingroup DMRG */
/*@{*/

/*! \file ModelCommonBase.h
 *
 *  An abstract class to represent the strongly-correlated-electron models that
 *  can be used with the DmrgSolver
 *
 */

#ifndef MODEL_COMMONBASE_H
#define MODEL_COMMONBASE_H
#include <iostream>

#include "LinkProductStruct.h"

namespace Dmrg {


template<typename ModelHelperType,typename SolverParamsType,typename GeometryType>
class ModelCommonBase  {


	typedef typename ModelHelperType::SparseMatrixType SparseMatrixType;
	typedef typename SparseMatrixType::value_type SparseElementType;
	typedef typename ModelHelperType::LinkType LinkType;
	typedef typename ModelHelperType::OperatorsType OperatorsType;
	typedef typename ModelHelperType::BlockType BlockType;
	typedef typename ModelHelperType::RealType RealType;
	typedef typename ModelHelperType::BasisType MyBasis;
	typedef typename ModelHelperType::BasisWithOperatorsType BasisWithOperatorsType;
	typedef typename ModelHelperType::LeftRightSuperType LeftRightSuperType;
	typedef typename OperatorsType::OperatorType OperatorType;
	typedef typename MyBasis::SymmetryElectronsSzType SymmetryElectronsSzType;
	typedef typename PsimagLite::Vector<OperatorType>::Type VectorOperatorType;

public:

	typedef LinkProductStruct<SparseElementType> LinkProductStructType;
	typedef typename PsimagLite::Vector<SparseElementType>::Type VectorType;

	ModelCommonBase(const SolverParamsType& params,const GeometryType& geometry)
	    : params_(params),geometry_(geometry)
	{}

	virtual ~ModelCommonBase() {}

	virtual void matrixVectorProduct(VectorType &x,
	                                 const VectorType &y,
	                                 ModelHelperType const &modelHelper) const = 0;

	virtual void addHamiltonianConnection(SparseMatrixType &matrix,
	                                      const LeftRightSuperType& lrs,
	                                      RealType currentTime) const = 0;


	virtual void hamiltonianConnectionProduct(VectorType& x,
	                                  const VectorType& y,
	                                  ModelHelperType const &modelHelper) const = 0;

	virtual void fullHamiltonian(SparseMatrixType& matrix,const ModelHelperType& modelHelper) const = 0;


	virtual void addConnectionsInNaturalBasis(SparseMatrixType& hmatrix,
	                                          const VectorOperatorType& cm,
	                                          const BlockType& block,
	                                          bool sysEnvOnly,
	                                          RealType time) const = 0;

	virtual SizeType getLinkProductStruct(const ModelHelperType& modelHelper) const = 0;

	virtual LinkType getConnection(const SparseMatrixType** A,
	                       const SparseMatrixType** B,
	                       SizeType ix,
	                       const ModelHelperType& modelHelper) const = 0;

	const SolverParamsType& params() const {return params_; }

	const GeometryType& geometry() const {return geometry_; }

private:

	const SolverParamsType& params_;
	const GeometryType& geometry_;

};     //class ModelCommonBase
} // namespace Dmrg
/*@}*/
#endif
