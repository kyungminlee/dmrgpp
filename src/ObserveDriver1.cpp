#include "ObserveDriver1.h"

namespace Dmrg {

typedef VectorWithOffset<RealType> VectorWithOffset1Type;
typedef VectorWithOffset<ComplexType> VectorWithOffset2Type;
typedef VectorWithOffsets<RealType> VectorWithOffset3Type;
typedef VectorWithOffsets<ComplexType> VectorWithOffset4Type;

typedef PsimagLite::Geometry<RealType,
InputNgType::Readable,
ProgramGlobals> Geometry1Type;

typedef PsimagLite::Geometry<ComplexType,
InputNgType::Readable,
ProgramGlobals> Geometry2Type;

typedef Basis<MySparseMatrixReal, CvectorSizeType> Basis1Type;
typedef Operators<Basis1Type> Operators1Type;
typedef BasisWithOperators<Operators1Type> BasisWithOperators1Type;
typedef LeftRightSuper<BasisWithOperators1Type,Basis1Type> LeftRightSuper1Type;

typedef Basis<MySparseMatrixComplex, CvectorSizeType> Basis2Type;
typedef Operators<Basis2Type> Operators2Type;
typedef BasisWithOperators<Operators2Type> BasisWithOperators2Type;
typedef LeftRightSuper<BasisWithOperators2Type,Basis2Type> LeftRightSuper2Type;

typedef ModelHelperSu2<LeftRightSuper1Type> ModelHelper3Type;
typedef ModelHelperSu2<LeftRightSuper2Type> ModelHelper4Type;


typedef ModelBase<ModelHelper3Type,
ParametersDmrgSolverType,
InputNgType::Readable,
Geometry1Type> ModelBase3Type;

typedef ModelBase<ModelHelper4Type,
ParametersDmrgSolverType,
InputNgType::Readable,
Geometry2Type> ModelBase4Type;


template bool observeOneFullSweep<VectorWithOffset1Type,ModelBase3Type>(IoInputType& io,
const ModelBase3Type& model,
const PsimagLite::String& list,
bool hasTimeEvolution,
SizeType orbitals);

template bool observeOneFullSweep<VectorWithOffset2Type,ModelBase4Type>(IoInputType& io,
const ModelBase4Type& model,
const PsimagLite::String& list,
bool hasTimeEvolution,
SizeType orbitals);

template bool observeOneFullSweep<VectorWithOffset3Type,ModelBase3Type>(IoInputType& io,
const ModelBase3Type& model,
const PsimagLite::String& list,
bool hasTimeEvolution,
SizeType orbitals);

template bool observeOneFullSweep<VectorWithOffset4Type,ModelBase4Type>(IoInputType& io,
const ModelBase4Type& model,
const PsimagLite::String& list,
bool hasTimeEvolution,
SizeType orbitals);
}
