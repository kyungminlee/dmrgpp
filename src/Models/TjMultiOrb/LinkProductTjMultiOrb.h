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

/*! \file LinkProductTjMultiOrb.h
 *
 *  FIXME
 *
 */
#ifndef LINK_PROD_TJ_MULTIORB_H
#define LINK_PROD_TJ_MULTIORB_H
#include "ProgramGlobals.h"

namespace Dmrg {

template<typename ModelHelperType>
class LinkProductTjMultiOrb {

	typedef typename ModelHelperType::SparseMatrixType SparseMatrixType;
	typedef std::pair<SizeType,SizeType> PairType;

	enum {TERM_CICJ, TERM_SPSM, TERM_SZSZ, TERM_NINJ};

	static SizeType orbitals_;

public:

	typedef typename ModelHelperType::RealType RealType;
	typedef typename SparseMatrixType::value_type SparseElementType;

	static void setOrbitals(SizeType orbitals)
	{
		orbitals_=orbitals;
	}

	template<typename SomeStructType>
	static void setLinkData(SizeType term,
	                        SizeType dofs,
	                        bool,
	                        ProgramGlobals::FermionOrBosonEnum& fermionOrBoson,
	                        PairType& ops,
	                        std::pair<char,char>& mods,
	                        SizeType& angularMomentum,
	                        RealType& angularFactor,
	                        SizeType& category,
	                        const SomeStructType&)
	{
		char tmp = mods.first;
		if (term==TERM_CICJ) {
			fermionOrBoson = ProgramGlobals::FERMION;
			SizeType spin = getSpin(dofs);
			ops = operatorDofs(dofs,term);
			angularFactor = 1;
			if (spin==1) angularFactor = -1;
			angularMomentum = 1;
			category = spin;
			return;
		}

		if (term==TERM_SPSM) {
			fermionOrBoson = ProgramGlobals::BOSON;
			SizeType spin = getSpin(dofs);
			switch (spin) {
			case 0: // S+ S-
				angularFactor = -1;
				category = 2;
				angularMomentum = 2;
				ops = operatorDofs(dofs,term);
				break;
			case 1: // S- S+
				angularFactor = -1;
				category = 0;
				mods.first = mods.second;
				mods.second = tmp;
				angularMomentum = 2;
				ops = operatorDofs(dofs,term);
				break;
			}

			return;
		}

		if (term==TERM_SZSZ) {
			fermionOrBoson = ProgramGlobals::BOSON;
			angularFactor = 0.5;
			category = 1;
			angularMomentum = 2;
			ops = operatorDofs(dofs,term);
			return;
		}

		if (term==TERM_NINJ) {
			fermionOrBoson = ProgramGlobals::BOSON;
			ops = operatorDofs(dofs,term);
			angularFactor = 1;
			angularMomentum = 0;
			category = 0;
			return;
		}

		assert(false);
	}

	template<typename SomeStructType>
	static void valueModifier(SparseElementType& value,
	                          SizeType term,
	                          SizeType,
	                          bool isSu2,
	                          const SomeStructType&)
	{
		if (term==TERM_CICJ) return;

		if (term==TERM_NINJ) {
			value *= 0.5;
			return;
		}

		assert(term==TERM_SPSM || term == TERM_SZSZ);

		if (isSu2) value = -value;
		if (term == TERM_SPSM) value *= 0.5;
		value *= 0.5;
	}

	// connections are:
	// up up and down down
	// S+ S- and S- S+
	// Sz Sz
	template<typename SomeStructType>
	static SizeType dofs(SizeType term,const SomeStructType&)
	{
		if (term==TERM_CICJ) return 2*orbitals_*orbitals_; // c^\dagger c
		if (term==TERM_SPSM) return 2*orbitals_*orbitals_; // S+ S- and S- S+
		if (term==TERM_SZSZ) return 1*orbitals_*orbitals_; // Sz Sz
		if (term==TERM_NINJ) return 1*orbitals_*orbitals_; // ninj
		assert(false);
		return 0; // bogus
	}

	template<typename SomeStructType>
	static std::pair<SizeType,SizeType> connectorDofs(SizeType term,
	                                                  SizeType dofs,
	                                                  const SomeStructType&)
	{
		SizeType orbitalsSquared = orbitals_*orbitals_;
		SizeType spin = dofs/orbitalsSquared;
		SizeType xtmp = (spin==0) ? 0 : orbitalsSquared;
		xtmp = dofs - xtmp;
		SizeType orb1 = xtmp/orbitals_;
		SizeType orb2 = xtmp % orbitals_;
		return PairType(orb1,orb2); //  orbital dependence, no spin dependence
	}

	static SizeType terms() { return 4; }

private:

	// spin is diagonal
	static std::pair<SizeType,SizeType> operatorDofs(SizeType dofs, SizeType term)
	{
		SizeType orbitalsSquared = orbitals_*orbitals_;
		SizeType spin = dofs/orbitalsSquared;
		SizeType xtmp = (spin==0) ? 0 : orbitalsSquared;
		xtmp = dofs - xtmp;
		SizeType orb1 = xtmp/orbitals_;
		SizeType orb2 = xtmp % orbitals_;
		if (term == TERM_SPSM) spin = 0;
		SizeType op1 = orb1 + spin*orbitals_;
		SizeType op2 = orb2 + spin*orbitals_;

		SizeType offset = 0;
// Need to look at the crationMatrices order to get the offset correct!
		if (term == TERM_SPSM) offset = 2*orbitals_;
		if (term == TERM_SZSZ) offset = 3*orbitals_;
		if (term == TERM_NINJ) offset = 4*orbitals_;

		return std::pair<SizeType,SizeType>(op1+offset,op2+offset);
	}

	static SizeType getSpin(SizeType dofs)
	{
		SizeType orbitalsSquared = orbitals_*orbitals_;
		return dofs/orbitalsSquared;
	}
}; // class LinkProductTjMultiOrb

template<typename ModelHelperType>
SizeType LinkProductTjMultiOrb<ModelHelperType>::orbitals_ = 1;
} // namespace Dmrg
/*@}*/
#endif
