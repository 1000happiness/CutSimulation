#include "Object.h"
#include <iostream>

namespace OBJ
{
    Object::Object(unsigned int object_id, Kind kind)
    {
        this->object_id = object_id;
        this->kind = kind;
        isActive = true;
        isPrefab = false;        
        parent = NULL;
        childs = std::vector<Object *>();
        components = new std::map<CMP::Kind, CMP::Component *>();
    }

    Object::~Object()
    {
        if(isPrefab) 
        {
            prefab->used_objects->erase(this);
        }

        for (auto i = this->components->begin(); i != this->components->end(); i++)
        {
            delete i->second;
        }
    }

    void Object::setComponent(CMP::Component *component)
    {
        (*(this->components))[component->kind] = component;
        component->game_object = this;
    }

    CMP::Component *Object::getComponent(CMP::Kind kind)
    {
        if (this->components->find(kind) != this->components->end())
        {
            return this->components->at(kind);
        }
        else
        {
            return NULL;
        }
    }

    void Object::removeComponent(CMP::Kind kind)
    {
        if(this->components->find(kind) != this->components->end())
        {
            if(isPrefab)
            {
                (*(this->components))[kind]->removed = true;
            }
            else
            {
                this->components->erase(kind);
            }
        }
    }

    void Object::applyComponent(CMP::Kind kind)
    {
        if(!isPrefab)
        {
            return;
        }

        if(this->components->find(kind) != this->components->end())
        {
            if((*(this->components))[kind]->removed)
            {
                if(this->prefab->components->find(kind) != this->prefab->components->end())
                {
                    this->components->erase(kind);
                    this->prefab->components->erase(kind);
                }
                else
                {
                    this->components->erase(kind);
                }
            }
            else
            {
                (*(this->components))[kind]->apply(prefab);
            }
        }
    }                   

    void Object::start()
    {
        if (!this->isActive)
        {
            return;
        }

        for (auto i = this->components->begin(); i != this->components->end(); i++)
        {
            i->second->start();
        }

        for (auto i = this->childs.begin(); i != this->childs.end(); i++)
        {
            (*i)->start();
        }
    }

    void Object::update()
    {
        if (!this->isActive)
        {
            return;
        }

        for (auto i = this->components->begin(); i != this->components->end(); i++)
        {
            i->second->update();
        }

        for (auto i = this->childs.begin(); i != this->childs.end(); i++)
        {
            (*i)->update();
        }
    }

    void Object::render()
    {
        if (!this->isActive)
        {
            return;
        }

        renderChilds();
    }

    void Object::renderChilds()
    {
        for (auto i = this->childs.begin(); i != this->childs.end(); i++)
        {
            (*i)->render();
        }
    }

    void Object::setParent(Object *object)
    {
        parent = object;
    }

    void Object::addChild(Object *object)
    {
        object->parent = this;
        childs.push_back(object);
    }

    void Object::removeChild(unsigned int index)
    {
        if(index > childs.size())
        {
            return;
        }
        childs[index]->parent = NULL;
        childs.erase(childs.begin() + index);
    }

    Prefab *Object::exportPrefab()
    {
        Prefab *prefab = new Prefab(this);
        this->prefab = prefab;
        this->isPrefab = true;

        return prefab;
    }

    Prefab::Prefab(Object *object)
    {
        this->object = object;
        this->components = new std::map<CMP::Kind, CMP::Component *>();
        this->used_objects = new std::set<OBJ::Object *>();
        for (auto i = object->components->begin(); i != object->components->end(); i++)
        {
            this->components->insert(std::make_pair(i->first, CMP::Component::createComponent(i->second)));
            this->components->at(i->first)->game_object = NULL;
        }
    }
    
    Prefab::~Prefab()
    {
    }
} // namespace OBJ
