#ifndef SCENE
#define SCENE

#include <iostream>


#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../../material/ShaderController/ShaderController.h"

#include "../../object/CameraObject/CameraObject.h"
#include "../../object/EmptyObject/EmptyObject.h"
#include "../../object/ModelObject/ModelObject.h"
#include "../../object/CanvasObject/CanvasObject.h"
#include "../../object/Cube/Cube.h"
#include "../../object/LightObject/PointLightObject.h"
#include "../../object/ObjectWithShader/ObjectWithShader.h"

#include "../../component/Transform/Transform.h"
#include "../../component/Camera/Camera.h"
#include "../../component/AutoRotation/AutoRotation.h"
#include "../../component/AutoColor/AutoColor.h"
#include "../../component/AutoConfiguration/AutoConfiguration.h"
#include "../../component/ParticleSystem/ParticleSystem.h"
#include "../../component/WorkPiece/WorkPiece.h"
#include "../../component/CoordinateSystemLines/CoordinateSystemLines.h"
#include "../../component/Manager/WorkManager.h"
#include "../../component/Manager/UIManager.h"
#include "../../component/UI/Label.h"
#include "../../component/UI/Button.h"
#include "../../component/UI/Slider.h"
#include "../../component/UI/BezierPanel.h"
#include "../../component/UI/Text.h"


namespace MAT
{
    extern ShaderController *shader_controller;
}

enum class SceneType
{
    SCREEN_2D,
    WORKPIECE,
    min = SCREEN_2D,
    max = WORKPIECE,
};

OBJ::EmptyObject* makeScene(SceneType scene_type);

OBJ::EmptyObject* switchUpScene(SceneType &scene_type);

OBJ::EmptyObject* switchDownScene(SceneType &scene_type);

#endif // !SCENE
