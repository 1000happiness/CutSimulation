#ifndef EMPTYOBJECT_H
#define EMPTYOBJECT_H

#include "../Object/Object.h"

namespace OBJ
{
    class EmptyObject: public Object
    {
    private:
        
    public:
        EmptyObject(unsigned object_id);
        ~EmptyObject();
    };
} // namespace OBJ

#endif

