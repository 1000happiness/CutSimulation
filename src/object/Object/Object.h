#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <vector>
#include <set>

#include "../../component/Component/Component.h"

namespace OBJ
{
    enum Kind {
        EMPTY,
        CUBE,
        CAMERA,
        MODEL,
        CANVAS,
        POINTLIGHT,
        WITHSHADER,
    };

    const unsigned int POS_SIZE = 3;
    const unsigned int COLOR_RGB_SIZE = 3;
    const unsigned int TEXTURE_SIZE = 2;
    const unsigned int NORMAL_SIZE = 3;

    class Prefab;

    class Object
    {
    protected:
        
        Prefab *prefab;

    public:
        //id
        unsigned int object_id;

        //kind
        Kind kind;

        //object tree
        Object *parent;
        std::vector<Object *> childs;        

        //active
        bool isActive;

        //prefab
        bool isPrefab;

        //lifetime
        Object(unsigned int object_id, Kind kind);
        ~Object();

        //component
        void setComponent(CMP::Component * component);
        CMP::Component *getComponent(CMP::Kind kind);
        void removeComponent(CMP::Kind kind);
        void applyComponent(CMP::Kind kind);

        //prefab
        std::map<CMP::Kind, CMP::Component *> *components;
        Prefab *exportPrefab();

        //object tree
        void setParent(Object *object);
        void addChild(Object *object);
        void removeChild(unsigned int index);
        
        //runtime
        virtual void start();//调用所有组件的start，再调用所有子物体的start
        virtual void update();//调用所有组件的update，再调用所有子物体的update
        virtual void render();
        void renderChilds();
    };

    class Prefab
    {
    private:
        /* data */
    public:
        Object *object;
        std::set<Object *> *used_objects;
        std::map<CMP::Kind, CMP::Component *> *components;        

        Prefab(Object *object);
        ~Prefab();
    };
} // namespace OBJ

#endif


