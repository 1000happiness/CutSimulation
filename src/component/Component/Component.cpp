#include "Component.h"
#include "../Transform/Transform.h"
#include "../Camera/Camera.h"
#include "../AutoRotation/AutoRotation.h"
#include "../AutoColor/AutoColor.h"
#include "../AutoConfiguration/AutoConfiguration.h"
#include "../Light/AmbientLight.h"
#include "../Light/DiffuseLight.h"
#include "../Light/SpecularLight.h"
#include "../ParticleSystem/ParticleSystem.h"
#include "../WorkPiece/WorkPiece.h"
#include "../CoordinateSystemLines/CoordinateSystemLines.h"
#include "../UI/Label.h"
#include "../UI/BezierPanel.h"
#include "../UI/Text.h"
#include "../../object/Object/Object.h"

namespace CMP
{
    Component::Component(Kind kind, OBJ::Object *game_object)
    {
        this->kind = kind;
        this->game_object = game_object;
        this->removed = false;
        this->changeFlag = true;
    }

    Component::~Component()
    {
    }

    void Component::apply(OBJ::Prefab *prefab)
    {
        delete (*(prefab->components))[kind];
        (*(prefab->components))[kind] = createComponent(this);
    }

    void Component::render(MAT::ShaderProgram* shader) {

    }

    Component *Component::createComponent(Component *component)
    {
        switch (component->kind)
        {
        case Kind::TRANSFORM:
            return new Transform(*((Transform *)component));//直接使用默认的拷贝构造函数
        case Kind::CAMERA:
            return new Camera(*((Camera *)component));
        case Kind::AUTOROTATION:
            return new AutoRotation(*((AutoRotation*)component));
        case Kind::AUTOCOLOR:
            return new AutoColor(*((AutoColor*)component));
        case Kind::AUTOCOMFIGURATION:
            return new AutoConfiguration(*(AutoConfiguration*)component);
        case Kind::AMBIENTLIGHT:
            return new AmbientLight(*(AmbientLight*)component);
        case Kind::DIFFUSELIGHT:
            return new DiffuseLight(*(DiffuseLight*)component);
        case Kind::SPECULARLIGHT:
            return new SpecularLight(*(SpecularLight*)component);
        case Kind::PARTICLESYSTEM:
            return new ParticleSystem(*(ParticleSystem*)component);
        case Kind::WORKPIECE:
            return new WorkPiece(*(WorkPiece*)component);
        case Kind::COORDINATESYSTEMLINES:
            return new CoordinateSystemLines(*(CoordinateSystemLines*)component);
        case Kind::LABEL:
            return new Label(*(Label*)component);
        case Kind::BEZIERPANEL:
            return new BezierPanel(*(BezierPanel*)component);
        case Kind::TEXT:
            return new Text(*(Text*)component);
        default:
            break;
        }
        return NULL;
    }
} // namespace CMP
