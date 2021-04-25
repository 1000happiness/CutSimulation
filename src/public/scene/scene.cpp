#include "scene.h"
#include <GLFW/glfw3.h>

namespace SCREEN
{
    extern int STANDARD_SIZE;
    extern int screen_width;
    extern int screen_height;
    extern GLFWwindow* window;
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
}

namespace INPUT
{
    extern float mouse_x;
    extern float mouse_y;
    extern bool first_mouse;
    extern float xoffset;
    extern float yoffset;
    extern bool left_button_click;
    extern bool right_button_click;

    void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);

    void mouseMoveCallback(GLFWwindow* window, double x, double y);

    void scrollCallback(GLFWwindow* window, double x, double y);
}

unsigned int getNewID()
{
    static unsigned int id = 0;
    return id++;
}

OBJ::EmptyObject* makeScene2DScene()
{
    //gl configuration
    glDisable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(SCREEN::window, SCREEN::framebufferSizeCallback);
    glfwSetInputMode(SCREEN::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(SCREEN::window, INPUT::mouseMoveCallback);
    glfwSetScrollCallback(SCREEN::window, INPUT::scrollCallback);

    //shader
    MAT::shader_controller = new MAT::ShaderController("./shader/controller/2d_scene.json");

    //root object
    unsigned int root_object_id = 0;
    OBJ::EmptyObject* root_object = new OBJ::EmptyObject(0);
    float ratio = 1;
    root_object->setComponent(new CMP::Transform(root_object, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(ratio, ratio, ratio)));
    CMP::AutoConfiguration* auto_configuration = new CMP::AutoConfiguration(root_object);
    auto_configuration->background_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    auto_configuration->clear_flag = GL_COLOR_BUFFER_BIT;
    root_object->setComponent(auto_configuration);
    
    //canvas
    unsigned int canvas_object_id = 1; 
    OBJ::CanvasObject* canvas = new OBJ::CanvasObject(canvas_object_id, MAT::shader_controller->getShader("canvas"), glm::vec2(1000.0f, 800.0f));

    glm::vec3 positions[16][3];
    glm::vec3 colors[2][3];
    for (int i = 0; i < 3; i++)
    {
        colors[0][i] = glm::vec3(112.0f / 255.0f, 48.0f / 255.0f, 160.0f / 255.0f);
        colors[1][i] = glm::vec3(68.0f / 255.0f, 114.0f / 255.0f, 196.0f / 255.0f);
    }
    canvas->special_depth_test_flag = true;
    for (int i = 0; i < 4; i++)
    {
        positions[4 * i + 0][0] = glm::vec3(-280 + 160 * i,  360, 1);
        positions[4 * i + 0][1] = glm::vec3(-200 + 160 * i,  360, 1);
        positions[4 * i + 0][2] = glm::vec3(-280 + 160 * i, -360, 1);

        positions[4 * i + 1][0] = glm::vec3(-200 + 160 * i, -360, 1);
        positions[4 * i + 1][1] = glm::vec3(-200 + 160 * i,  360, 1);
        positions[4 * i + 1][2] = glm::vec3(-280 + 160 * i, -360, 1);

        positions[4 * i + 2][0] = glm::vec3(-360, 280 - 160 * i, 0);
        positions[4 * i + 2][1] = glm::vec3(-360, 200 - 160 * i, 0);
        positions[4 * i + 2][2] = glm::vec3( 360, 280 - 160 * i, 0);

        positions[4 * i + 3][0] = glm::vec3( 360, 200 - 160 * i, 0);
        positions[4 * i + 3][1] = glm::vec3(-360, 200 - 160 * i, 0);
        positions[4 * i + 3][2] = glm::vec3( 360, 280 - 160 * i, 0);
    }
   
    for (int i = 0; i < 4; i++)
    {
        canvas->addTriangle(OBJ::Triangle(positions[4 * i + 0], colors[0]));
        canvas->addTriangle(OBJ::Triangle(positions[4 * i + 1], colors[0]));
        canvas->addTriangle(OBJ::Triangle(positions[4 * i + 2], colors[1]));
        canvas->addTriangle(OBJ::Triangle(positions[4 * i + 3], colors[1]));
    }
    canvas->sendData2GPU();
    root_object->addChild(canvas);  

    return root_object;
}

OBJ::EmptyObject* makeWorkPieceScene()
{
    //gl configuration
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(SCREEN::window, SCREEN::framebufferSizeCallback);
    glfwSetInputMode(SCREEN::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(SCREEN::window, INPUT::mouseClickCallback);
    glfwSetCursorPosCallback(SCREEN::window, INPUT::mouseMoveCallback);
    glfwSetScrollCallback(SCREEN::window, INPUT::scrollCallback);

    //shader
    MAT::shader_controller = new MAT::ShaderController("./shader/controller/work_piece_scene.json");

    //root object
    OBJ::EmptyObject* root_object = new OBJ::EmptyObject(getNewID());
    //单位1修正
    float ratio = (float(SCREEN::STANDARD_SIZE) / float(std::min(SCREEN::screen_width, SCREEN::screen_height)));
    root_object->setComponent(new CMP::Transform(root_object, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(ratio, ratio, ratio)));
    CMP::AutoConfiguration* auto_configuration = new CMP::AutoConfiguration(root_object);
    auto_configuration->background_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    auto_configuration->clear_flag = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    root_object->setComponent(auto_configuration);

    //camera
    unsigned int camera_object_id = getNewID();
    OBJ::CameraObject* camera_object = new OBJ::CameraObject(
        camera_object_id,
        "camera_matrices",
        new CMP::Camera(NULL, glm::vec3(1, 0.5, 3), glm::vec3(0, 0, -1))
    );
    root_object->addChild(camera_object);

    //light
    unsigned int point_light_object_id = getNewID();
    OBJ::PointLightObject* point_light_object = new OBJ::PointLightObject(
        point_light_object_id,
        new CMP::Transform(NULL, glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f)),
        new CMP::AmbientLight(NULL, 0.3f),
        new CMP::DiffuseLight(NULL),
        new CMP::SpecularLight(NULL, 1.0f),
        OBJ::PointLightObject::Attenuation(1.0f, 0.09f, 0.032f),
        "point_light_property", 0,
        true,
        //MAT::shader_controller->getShader("point_light")
        NULL
    );
    root_object->addChild(point_light_object);

    //background
    OBJ::ObjectWithShader* background_object = new OBJ::ObjectWithShader(
        getNewID(),
        "coordinate_system_lines"
    );
    background_object->setComponent(new CMP::Transform(NULL));
    background_object->setComponent(new CMP::CoordinateSystemLines(NULL, glm::vec3(0.0f), glm::vec3(15.0f), glm::vec3(0.1f), glm::vec3(0.5f)));
    root_object->addChild(background_object);

    //cutter
    OBJ::EmptyObject* cutter_object = new OBJ::EmptyObject(getNewID());
    cutter_object->setComponent(new CMP::Transform());
    {
        const char* model_source = "./model/SVHBL163C.STL";
        OBJ::ModelObject* model_object = new OBJ::ModelObject(getNewID(), model_source, MAT::shader_controller->getShader("light_model"));
        CMP::Transform* transform = (CMP::Transform*)(model_object->getComponent(CMP::Kind::TRANSFORM));
        transform->set(CMP::Transform::PropertyType::POSTION, glm::vec3(-0.32, -0.25, 0));
        transform->set(CMP::Transform::PropertyType::ROTATION, glm::vec3(90, 180, 0));
        transform->set(CMP::Transform::PropertyType::SCALE, glm::vec3(0.01, 0.01, 0.01));
        cutter_object->addChild(model_object);
    }
    CMP::Transform* transform = (CMP::Transform*)(cutter_object->getComponent(CMP::Kind::TRANSFORM));
    transform->set(CMP::Transform::PropertyType::POSTION, glm::vec3(3, 1, 0));
    root_object->addChild(cutter_object);

    //particle system
    OBJ::ObjectWithShader* particle_system_object = new OBJ::ObjectWithShader(
        getNewID(),
        "particle_system"
    );
    particle_system_object->setComponent(new CMP::Transform(NULL, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f)));
    particle_system_object->setComponent(new CMP::ParticleSystem(NULL, 2000, 0.1f, 20.0f, 19.0f, glm::vec3(0, 1, 0), 70, glm::vec3(160.0f / 255.0f, 115.0f / 255.0f, 64.0f / 255.0f), glm::vec3(0.0f, -9.8f, 0.0f)));
    cutter_object->addChild(particle_system_object);

    //work_piece
    OBJ::ObjectWithShader* work_piece_object = new OBJ::ObjectWithShader(
        getNewID(),
        "work_piece"
    );
    work_piece_object->setComponent(new CMP::Transform(NULL, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f)));
    work_piece_object->setComponent(new CMP::WorkPiece(NULL, CMP::WorkPiece::PieceKind::WOOD, 3.0f, 0.5f, NULL));
    work_piece_object->setComponent(new CMP::AutoRotation(NULL, -30.0f, glm::vec3(1.0f, 0.0f, 0)));
    root_object->addChild(work_piece_object);

    //manager
    OBJ::EmptyObject* manager_object = new OBJ::EmptyObject(getNewID());
    manager_object->setComponent(new CMP::WorkManager(NULL, work_piece_object, cutter_object, camera_object, particle_system_object));
    manager_object->setComponent(new CMP::UIManager(NULL, NULL, NULL, std::vector<OBJ::Object*>(), NULL, NULL, NULL));
    root_object->addChild(manager_object);

    //UI
    OBJ::EmptyObject *canvas_object = new OBJ::EmptyObject(getNewID());
    {
        CMP::UIManager* UI_manager = (CMP::UIManager*)manager_object->getComponent(CMP::Kind::UIMANAGER);
        CMP::CALLBACK::instance = UI_manager;
        //console background
        OBJ::ObjectWithShader* console_background_object = new OBJ::ObjectWithShader(getNewID(), "label");
        console_background_object->setComponent(new CMP::Label(NULL, "./image/UI/console_background.png", glm::vec2(800.0f, 90.0f), glm::vec2(1600.0f, 180.0f), 0.9f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(console_background_object);

        //work piece configuration
        OBJ::ObjectWithShader* work_piece_background_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_background_object->setComponent(new CMP::Label(NULL, "./image/UI/region_background.png", glm::vec2(200.0f, 90.0f), glm::vec2(380.0f, 160.0f), 0.8f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(work_piece_background_object);

        OBJ::ObjectWithShader* work_piece_configuration_label_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_configuration_label_object->setComponent(new CMP::Label(NULL, "./image/UI/work_piece_configuration_label.png", glm::vec2(200.0f, 30.0f), glm::vec2(110.0f, 35.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(work_piece_configuration_label_object);

        //kind
        OBJ::ObjectWithShader* work_piece_kind_label_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_kind_label_object->setComponent(new CMP::Label(NULL, "./image/UI/work_piece_kind_label.png", glm::vec2(50.0f, 70.0f), glm::vec2(55.0f, 30.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(work_piece_kind_label_object);

        OBJ::ObjectWithShader* work_piece_wood_kind_button_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_wood_kind_button_object->setComponent(new CMP::Button(NULL, "./image/UI/work_piece_wood_kind_button.png", glm::vec2(150.0f, 70.0f), glm::vec2(55.0f, 30.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f), CMP::CALLBACK::setPieceKind, "wood"));
        canvas_object->addChild(work_piece_wood_kind_button_object);

        OBJ::ObjectWithShader* work_piece_iron_kind_button_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_iron_kind_button_object->setComponent(new CMP::Button(NULL, "./image/UI/work_piece_iron_kind_button.png", glm::vec2(250.0f, 70.0f), glm::vec2(55.0f, 30.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f), CMP::CALLBACK::setPieceKind, "iron"));
        canvas_object->addChild(work_piece_iron_kind_button_object);

        //length
        OBJ::ObjectWithShader* work_piece_length_label_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_length_label_object->setComponent(new CMP::Label(NULL, "./image/UI/work_piece_length_label.png", glm::vec2(50.0f, 110.0f), glm::vec2(55.0f, 30.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(work_piece_length_label_object);

        OBJ::ObjectWithShader* length_slider_object = new OBJ::ObjectWithShader(getNewID(), "label");
        length_slider_object->setComponent(new CMP::Slider(NULL, "./image/UI/slider_line.png", glm::vec2(200.0f, 110.0f), glm::vec2(200.0f, 10.0f), "./image/UI/slider_node.png", glm::vec2(100.0f, 110.0f), glm::vec2(300.0f, 110.0f), glm::vec2(10.0f, 10.0f), 0.7f, 0.69f, glm::vec3(1.0f, 1.0f, 1.0f), 3.0f, glm::vec2(3, 6), CMP::CALLBACK::setLength));
        canvas_object->addChild(length_slider_object);

        //radius
        OBJ::ObjectWithShader* work_piece_radius_label_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_radius_label_object->setComponent(new CMP::Label(NULL, "./image/UI/work_piece_radius_label.png", glm::vec2(50.0f, 150.0f), glm::vec2(55.0f, 30.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(work_piece_radius_label_object);

        OBJ::ObjectWithShader* radius_slider_object = new OBJ::ObjectWithShader(getNewID(), "label");
        radius_slider_object->setComponent(new CMP::Slider(NULL, "./image/UI/slider_line.png", glm::vec2(200.0f, 150.0f), glm::vec2(200.0f, 10.0f), "./image/UI/slider_node.png", glm::vec2(100.0f, 150.0f), glm::vec2(300.0f, 150.0f), glm::vec2(10.0f, 10.0f), 0.7f, 0.69f, glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, glm::vec2(0, 0.5), CMP::CALLBACK::setRadius));
        canvas_object->addChild(radius_slider_object);

        //work piece commit button
        OBJ::ObjectWithShader* work_piece_commit_button_object = new OBJ::ObjectWithShader(getNewID(), "label");
        work_piece_commit_button_object->setComponent(new CMP::Button(NULL, "./image/UI/work_piece_commit_button.png", glm::vec2(350.0f, 100.0f), glm::vec2(55.0f, 55.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f), CMP::CALLBACK::reset, ""));
        canvas_object->addChild(work_piece_commit_button_object);

        //bezier configuration
        OBJ::ObjectWithShader* bezier_background_object = new OBJ::ObjectWithShader(getNewID(), "label");
        bezier_background_object->setComponent(new CMP::Label(NULL, "./image/UI/region_background.png", glm::vec2(800.0f, 90.0f), glm::vec2(800.0f, 160.0f), 0.8f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(bezier_background_object);

        OBJ::ObjectWithShader* bezier_configuration_label_object = new OBJ::ObjectWithShader(getNewID(), "label");
        bezier_configuration_label_object->setComponent(new CMP::Label(NULL, "./image/UI/bezier_configuration_label.png", glm::vec2(800.0f, 30.0f), glm::vec2(120.0f, 30.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(bezier_configuration_label_object);

        OBJ::ObjectWithShader* bezier_control_point_label_object = new OBJ::ObjectWithShader(getNewID(), "label");
        bezier_control_point_label_object->setComponent(new CMP::Label(NULL, "./image/UI/bezier_control_point_label.png", glm::vec2(455.0f, 65.0f), glm::vec2(90.0f, 30.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(bezier_control_point_label_object);

        std::vector<OBJ::Object*> bezier_control_points_buttons;
        for (int i = 0; i < 4; i++)
        {
            OBJ::ObjectWithShader* bezier_control_point_button_object = new OBJ::ObjectWithShader(getNewID(), "label");
            bezier_control_point_button_object->setComponent(new CMP::Button(NULL, std::string("./image/UI/bezier_control_point") + std::to_string(i)+ std::string("_button.png"), glm::vec2(430.0f + (i % 2) * 50.0f, 100.0f + i / 2 * 40.0f), glm::vec2(35.0f, 35.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f), CMP::CALLBACK::setFocus, std::to_string(i)));
            canvas_object->addChild(bezier_control_point_button_object);
            bezier_control_points_buttons.push_back(bezier_control_point_button_object);
        }

        //bezier panel
        OBJ::ObjectWithShader* bezier_panel_object = new OBJ::ObjectWithShader(getNewID(), "bezier_panel");
        bezier_panel_object->setComponent(new CMP::BezierPanel(NULL, glm::vec2(800.0f, 110.0f), glm::vec2(550.0f, 100.0f), 0.0f, manager_object, CMP::CALLBACK::setBezierPoint));
        canvas_object->addChild(bezier_panel_object);

        //bezier commit button
        OBJ::ObjectWithShader* bezier_commit_button_object = new OBJ::ObjectWithShader(getNewID(), "label");
        bezier_commit_button_object->setComponent(new CMP::Button(NULL, "./image/UI/bezier_commit_button.png", glm::vec2(1150.0f, 100.0f), glm::vec2(55.0f, 55.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f), CMP::CALLBACK::setBezier, ""));
        canvas_object->addChild(bezier_commit_button_object);

        //log panel
        OBJ::ObjectWithShader* log_background_object = new OBJ::ObjectWithShader(getNewID(), "label");
        log_background_object->setComponent(new CMP::Label(NULL, "./image/UI/region_background.png", glm::vec2(1400.0f, 90.0f), glm::vec2(350.0f, 160.0f), 0.8f, glm::vec3(1.0f, 1.0f, 1.0f)));
        canvas_object->addChild(log_background_object);

        OBJ::ObjectWithShader* log_content_object = new OBJ::ObjectWithShader(getNewID(), "font");
        log_content_object->setComponent(new CMP::Text(NULL, "./fonts/Arial.ttf", "hello word", glm::vec2(1250.0f, 100.0f), 0.5f, glm::vec3(1.0f)));
        canvas_object->addChild(log_content_object);

        UI_manager->setObject(work_piece_iron_kind_button_object, work_piece_wood_kind_button_object, bezier_control_points_buttons, bezier_panel_object, log_content_object, manager_object);
    }
    root_object->addChild(canvas_object);

    return root_object;
}

OBJ::EmptyObject* makeScene(SceneType scene_type)
{
	switch (scene_type)
	{
    case SceneType::SCREEN_2D:
        return makeScene2DScene();
    case SceneType::WORKPIECE:
        return makeWorkPieceScene();
	default:
		std::cout << "scene type error" << std::endl;
		return NULL;
	}
}

OBJ::EmptyObject* switchUpScene(SceneType &scene_type)
{
    int index = int(scene_type) - 1;
    if (index < int(SceneType::min)) {
        scene_type = SceneType::max;
        return makeScene(SceneType::max);
    }
    else {
        scene_type = SceneType(index);
        return makeScene(scene_type);
    }
}

OBJ::EmptyObject* switchDownScene(SceneType &scene_type)
{
    int index = int(scene_type) + 1; 
    if (index > int(SceneType::max)) {
        scene_type = SceneType::min;
        return makeScene(SceneType::min);
    }
    else {
        scene_type = SceneType(index);
        return makeScene(scene_type);
    }
}