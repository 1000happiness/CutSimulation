#ifndef CMP_COMPONENT_H
#define CMP_COMPONENT_H

#include "../../material/ShaderProgram/ShaderProgram.h"

namespace OBJ
{
    class Object;
    class Cube;
    class Prefab;
    class EmptyObject;
    class CameraObject;
    class ModelObject;
}

namespace CMP
{
    enum class Kind {
        TRANSFORM,
        CAMERA,
        AUTOROTATION,
        AUTOCOLOR,
        AUTOCOMFIGURATION,
        AMBIENTLIGHT,
        DIFFUSELIGHT,
        SPECULARLIGHT,
        PARTICLESYSTEM,
        WORKPIECE,
        COORDINATESYSTEMLINES,
        WORKMANAGER,
        UIMANAGER,
        LABEL,
        BEZIERPANEL,
        TEXT,
    };
    
    class Transform;
    class AutoColor;
    class Camera;
    class AutoRotation;
    class AutoConfiguration;
    class AmbientLight;
    class DiffuseLight;
    class SpecularLight;
    class ParticleSystem;
    class WorkPiece;
    class CoordinateSystemLines;
    class WorkManager;
    class Label;
    class BezierPanel;
    class UIManager;
    class Text;

    class Component
    {
    private:
        
    public:
        bool changeFlag;
        Kind kind;
        bool removed;
        OBJ::Object *game_object;

        Component(Kind Kind, OBJ::Object *game_object);
        ~Component();

        virtual void start() = 0;
        virtual void update() = 0;
        virtual void render(MAT::ShaderProgram* shader);

        virtual void apply(OBJ::Prefab *prefab);

        static Component *createComponent(Component *component);
    };
} // namespace CMP

#endif