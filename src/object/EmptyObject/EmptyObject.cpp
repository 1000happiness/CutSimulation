#include "EmptyObject.h"
#include "../../component/Transform/Transform.h"

namespace OBJ
{
    EmptyObject::EmptyObject(unsigned object_id): Object(object_id, EMPTY)
    {
    }

    EmptyObject::~EmptyObject()
    {
    }
} // namespace OBJ


