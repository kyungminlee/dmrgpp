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

/*! \file LabelWithKnownSize.h
 *
 *  LabelWithKnownSizeing
 */
#ifndef LABEL_WITH_KNOWN_SIZE_H
#define LABEL_WITH_KNOWN_SIZE_H

#include <string>
#include <vector>
#include <utility>

namespace PsimagLite {


	class LabelWithKnownSize {

		typedef std::pair<size_t,size_t> PairType;

	public:

		typedef PairType ValueType;

		LabelWithKnownSize(const std::string& name,std::vector<PairType>& expectedSizes)
			: name_(name),expectedSizes_(expectedSizes)
		{
			assert(expectedSizes_.size()<3);
		}

		std::string name() const { return name_; }

		void check(const std::vector<std::string>& vec,size_t linSize,size_t line) const
		{
			if (expectedSizes_.size()==0) return;

			std::string s(__FILE__);
			if (expectedSizes_.size()==1) {
				size_t n = getSize(linSize,0);
				if (n==vec.size()-1 && size_t(atoi(vec[0].c_str()))==n) return;
				std::cout<<" Number of numbers to follow is wrong, expected\n";
				std::cerr<<"Line="<<line<<"\n";
				throw std::runtime_error(s.c_str());
			}
			if (expectedSizes_.size()==2) {
				size_t row = getSize(linSize,0);
				size_t col = getSize(linSize,1);
				size_t n = row*col;

				if (n==vec.size()-2 && size_t(atoi(vec[0].c_str()))==row && size_t(atoi(vec[1].c_str()))==col) return;
				std::cout<<" Number of numbers to follow is wrong, expected\n";
				std::cerr<<"Line="<<line<<"\n";
				throw std::runtime_error(s.c_str());
			}
		}

	private:

		size_t getSize(size_t linSize,size_t index) const
		{
			return expectedSizes_[index].first + linSize*expectedSizes_[index].second;
		}

		std::string name_;
		std::vector<PairType> expectedSizes_;


	}; // class LabelWithKnownSize
} // namespace Dmrg 

/*@}*/
#endif