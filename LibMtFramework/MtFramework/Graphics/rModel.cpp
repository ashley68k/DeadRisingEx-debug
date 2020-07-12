/*

*/

#include "rModel.h"
#include "rModelTypeInfo.h"

void rModelImpl::InitializeTypeInfo()
{
	// Register type info:
	RegisterTypeInfo(&JointTypeInfo);
	RegisterTypeInfo(&PrimitiveTypeInfo);
	RegisterTypeInfo(&MaterialTypeInfo);
	RegisterTypeInfo(&rModelTypeInfo);
}