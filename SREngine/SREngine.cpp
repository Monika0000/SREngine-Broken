#include "pch.h"
#include "SREngine.h"
#include <Input.h>
#include <EventsManager.h>
#include <ResourceManager.h>

#include <Scene.h>

#include <SRFile.h>
#include <imgui.h>

#include <vector>
#include <map>
#include <imgui.cpp>

#include <Camera.h>

#include <addons/imguitoolbar/imguitoolbar.h>

#define SHOW_ENGINE_LOGO 0

using namespace SpaRcle::Helper;
using namespace SpaRcle::Graphics;

static bool shift_pressed = false;

ImGuiTreeNodeFlags node_flags_with_childs = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
ImGuiTreeNodeFlags node_flags_without_childs = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

bool ButtonWithId(const char* _id, const char* label, ImVec2 button_size = ImVec2(0, 0), ImGuiButtonFlags flags = ImGuiButtonFlags_None) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false; 

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(_id);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = ImGui::CalcItemSize(button_size, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    ImGui::RenderNavHighlight(bb, id);
    ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
    ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
    return pressed;
}

void IsSelect(GameObject* gm) {
    if(ImGui::IsItemClicked() && shift_pressed) 
        gm->InvertSelect();
}
void Child(GameObject* parent) {
    for (int p = 0; p < parent->GetChilderns().size(); p++)
    {
        if (!parent->GetChilderns()[p]->HasChildrens()) {
            ImGui::TreeNodeEx((void*)(intptr_t)p,
                node_flags_without_childs
                    | (parent->GetChilderns()[p]->IsSelect() ? ImGuiTreeNodeFlags_Selected : 0),
                parent->GetChilderns()[p]->GetName().c_str()
            );

            IsSelect(parent->GetChilderns()[p]);
        }
        else {
            bool open = ImGui::TreeNodeEx((void*)(intptr_t)p,
                node_flags_with_childs | (parent->GetChilderns()[p]->IsSelect() ? ImGuiTreeNodeFlags_Selected : 0),
                parent->GetChilderns()[p]->GetName().c_str()
            );

            IsSelect(parent->GetChilderns()[p]);

            if (open)
                Child(parent->GetChilderns()[p]);
        }
    }
    ImGui::TreePop();
}

void TextCenter(std::string text) {
    float font_size = ImGui::GetFontSize() * text.size() / 2;
    ImGui::SameLine(
        ImGui::GetWindowSize().x / 2 -
        font_size + (font_size / 2)
    );

    ImGui::Text(text.c_str());
}

char EnterNumber() {
    if (GetKeyDown(KeyCode::_0))
        return '0';
    else if (GetKeyDown(KeyCode::_1))
        return '1';
    else if (GetKeyDown(KeyCode::_2))
        return '2';
    else if (GetKeyDown(KeyCode::_3))
        return '3';
    else if (GetKeyDown(KeyCode::_4))
        return '4';
    else if (GetKeyDown(KeyCode::_5))
        return '5';
    else if (GetKeyDown(KeyCode::_6))
        return '6';
    else if (GetKeyDown(KeyCode::_7))
        return '7';
    else if (GetKeyDown(KeyCode::_8))
        return '8';
    else if (GetKeyDown(KeyCode::_9))
        return '9';
    else if (GetKeyDown(KeyCode::BackSpace))
        return 'B';
    else if (GetKeyDown(KeyCode::Enter))
        return 'E';
    else if (GetKeyDown(KeyCode::Minus))
        return '-';
    else if (GetKeyDown(KeyCode::Dot))
        return '.';
    else
        return 'N';
}

void DrawSelectableFloat(std::string object, float& number) {
    static std::string current_object = "";
    static std::string text = "";
    bool edit = object == current_object;

    if (ButtonWithId(object.c_str(), edit ? text.c_str() : (std::to_string(number)).c_str(), ImVec2(60, 20))) {
        current_object = object;
        text = SRString::FloatToStringWTZ(number);
    }
    if (edit) {
        char c = EnterNumber();
       if (c == 'B') {
            if (text.size() > 0) {
                text.pop_back();
            }
        }
        else if (c == 'E') {
            if (text.size() == 0)
                number = 0;
            else
                number = std::stod(text.c_str());

            current_object.clear();
        }
        else if (c == '-' && text.size() == 0) {
            text += '-';
        }
        else if (c != 'N') {
            text += c;
        } 
    }
}
void DrawFloatInputButton(std::string object, float& number) {
    DrawSelectableFloat(object, number);
    ImGui::SameLine();
    if (ButtonWithId((object + "_less").c_str(), "<")) {
        number -= 0.01;
    }
    ImGui::SameLine();
    if (ButtonWithId((object + "_more").c_str(), ">")) {
        number += 0.01;
    }
}

const bool Vec2Equals(const ImVec2& v1, const ImVec2& v2) { return (v1.x == v2.x) && (v1.y == v2.y); }

void DrawImage(const ImVec2 win_size, ImVec2 img_size, const GLuint tex, const bool centralize = true) {
    const float dx = win_size.x / img_size.x;
    const float dy = win_size.y / img_size.y;

    if (dx > dy)
        img_size *= dy;
    else
        if (dy > dx)
            img_size *= dx;
        else
            img_size *= dy;

    // Because I use the texture from OpenGL, I need to invert the V from the UV.

    if (centralize) {
        ImVec2 initialCursorPos = ImGui::GetCursorPos();
        ImVec2 centralizedCursorpos = (win_size - img_size) * 0.5f;
        centralizedCursorpos = ImClamp(centralizedCursorpos, initialCursorPos, centralizedCursorpos);
        ImGui::SetCursorPos(centralizedCursorpos);
    }

    ImGui::Image((ImTextureID)tex, img_size, ImVec2(0, 1), ImVec2(1, 0));
}
void DrawTextureHorizontal(Texture* tex, ImVec2 win_size) {
    if (!tex) { ImGui::Text(""); return; }

    ImVec2 img_size = tex->GetSize();
    img_size *= win_size.x / img_size.x;
    ImGui::Image((ImTextureID)tex->GetID(), img_size, ImVec2(0, 1), ImVec2(1, 0));
}

bool SpaRcle::Engine::SREngine::InitEngineGUI() {
    this->m_window->GetRender()->AddGUI("docking_space", []() {
        const float toolbarSize = 0;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos + ImVec2(0, toolbarSize));
        ImGui::SetNextWindowSize(viewport->Size - ImVec2(0, toolbarSize));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGuiWindowFlags window_flags = 0
            | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Master DockSpace", NULL, window_flags);
        ImGuiID dockMain = ImGui::GetID("MyDockspace");

        // Save off menu bar height for later.
        float menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

        ImGui::DockSpace(dockMain);
        ImGui::End();
        ImGui::PopStyleVar(3);

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New scene")) {
                    //Do something
                }

                if (ImGui::MenuItem("Load scene")) {
                    //Do something
                }

                if (ImGui::MenuItem("Save scene")) {
                    //Do something
                }

                if (ImGui::MenuItem("Close scene")) {
                    //Do something
                    //ResourceManager::de
                }

                if (ImGui::MenuItem("Exit")) {
                    EventsManager::PushEvent(EventsManager::Event::Exit);
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("GameObject")) {
                if (ImGui::BeginMenu("3D Objects")) {
                    if (ImGui::MenuItem("Cube")) {
                        GameObject* cam = SRGraphics::Get()->GetMainWindow()->GetCameraGameObject();
                        ResourceManager::LoadPrefab("engine/cube", "New cube", cam->GetTransform()->GetPosition() + cam->GetTransform()->Forward() * 10.f);
                    }
                    if (ImGui::MenuItem("Sphere")) {
                        ResourceManager::LoadPrefab("engine/sphere", "New sphere");
                    }
                    if (ImGui::MenuItem("Cylinder")) {
                        ResourceManager::LoadPrefab("engine/cylinder", "New cylinder");
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Config")) {
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    });

    this->m_window->GetRender()->AddGUI("engine_scene", []() {
        static Window* win = SRGraphics::Get()->GetMainWindow();
        static GLuint tex = win->GetPostProcessing()->GetScreenTexture();

        if (!win->GetPostProcessing()->IsEnabledRenderIntoWindow()) {
            if (ImGui::Begin("Scene", 0)) {
                // Using a Child allow to fill all the space of the window.
                // It also alows customization
                ImGui::BeginChild("GameRender");
                // Get the size of the child (i.e. the whole draw size of the windows).
               
                DrawImage(ImGui::GetWindowSize(), win->GetDockSpace(), tex);

                ImGui::EndChild();
            }
            ImGui::End();
        }
    });

    this->m_window->GetRender()->AddGUI("engine_hierarchy", []() {
        if (ImGui::Begin("Hierachy", NULL)) { //, ImGuiWindowFlags_NoMove
            std::vector<GameObject*> gms = ResourceManager::GetGameObjects();

            if (ImGui::TreeNode(Scene::GetName().c_str())) {
                ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
                for (int i = 0; i < gms.size(); i++)
                {
                    if (gms[i]->IsChildren()) continue;

                    if (gms[i]->HasChildrens()) {
                        bool open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags_with_childs
                            | (gms[i]->IsSelect() ? ImGuiTreeNodeFlags_Selected : 0), gms[i]->GetName().c_str());

                        IsSelect(gms[i]);

                        if (open)
                            Child(gms[i]);
                    }
                    else {
                        ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags_without_childs
                            | (gms[i]->IsSelect() ? ImGuiTreeNodeFlags_Selected : 0), gms[i]->GetName().c_str());

                        IsSelect(gms[i]);
                    }
                }
                ImGui::TreePop();
                ImGui::PopStyleVar();
            }

            //ImGui::PopStyleColor();
           // ImGui::EndDock();
        }
        ImGui::End();
    });

    this->m_window->GetRender()->AddGUI("engine_inspector", []() {
        //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(250, 250));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.3f, 0.0f, 0.8f));

        ImGui::Begin("Inspector", NULL); //ImGuiWindowFlags_NoCollapse

        glm::vec3 vec_temp = {};
        std::string temp = "";
        Material* mat = nullptr;
        ImVec2 win_size = ImGui::GetWindowSize();

        auto objs = ResourceManager::GetAllSelectedGameObjects();
        if (objs.size() == 1) {
            bool enb = objs[0]->Enabled();
            if (ImGui::Checkbox("Enabled", &enb)) {
                objs[0]->Enable(enb);
            }

            ImGui::Text(("Name: " + objs[0]->GetName()).c_str());
            ImGui::Text(("Tag:  " + objs[0]->GetTag()).c_str());

            {
                ImGui::Separator();
                TextCenter("Transform");

                if (ImGui::TreeNode("Position:")) {
                    vec_temp = objs[0]->GetTransform()->GetPosition();

                    DrawSelectableFloat("transf_pos_x", vec_temp.x);
                    ImGui::SameLine();
                    DrawSelectableFloat("transf_pos_y", vec_temp.y);
                    ImGui::SameLine();
                    DrawSelectableFloat("transf_pos_z", vec_temp.z);

                    objs[0]->GetTransform()->SetPosition(vec_temp);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Rotation:")) {
                    vec_temp = objs[0]->GetTransform()->GetRotation();

                    DrawSelectableFloat("transf_rot_x", vec_temp.x);
                    ImGui::SameLine();
                    DrawSelectableFloat("transf_rot_y", vec_temp.y);
                    ImGui::SameLine();
                    DrawSelectableFloat("transf_rot_z", vec_temp.z);

                    objs[0]->GetTransform()->SetRotation(vec_temp);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Scale:")) {
                    vec_temp = objs[0]->GetTransform()->GetSclae();

                    DrawSelectableFloat("transf_scl_x", vec_temp.x);
                    ImGui::SameLine();
                    DrawSelectableFloat("transf_scl_y", vec_temp.y);
                    ImGui::SameLine();
                    DrawSelectableFloat("transf_scl_z", vec_temp.z);

                    objs[0]->GetTransform()->SetScale(vec_temp);

                    ImGui::TreePop();
                }

                if (false) {
                    ImGui::Text("");

                    ImGui::Text(("Parent pos:  " + objs[0]->GetTransform()->GetStringParentPosition()).c_str());
                    ImGui::Text(("Parent rot:  " + objs[0]->GetTransform()->GetStringParentRotation()).c_str());
                    ImGui::Text(("Parent scl:  " + objs[0]->GetTransform()->GetStringParentScale()).c_str());
                }

                ImGui::Separator();
            }

            for (auto comp : objs[0]->GetComponents()) {
                temp = std::string(comp->TypeName());
                std::string comp_name = "Component: " + temp;

                TextCenter(comp_name);

                if (temp == "Mesh") {
                    Mesh* mesh = static_cast<Mesh*>(comp);
                    mat = mesh->GetMaterial();

                    ImGui::Text(("Mesh name: " + mesh->GetName()).c_str());
                    ImGui::Text(("Verts: " + std::to_string(mesh->GetCountVertices())).c_str());
                    //ImGui::Text(("Indices: " + std::to_string(mesh->GetCountIndices())).c_str());


                    ImGui::Separator();
                    TextCenter("Component: Material");
                    if (mat->IsTransparent())
                        ImGui::Text("Transparent: true");
                    else
                        ImGui::Text("Transparent: false");

                    if (mat->IsDefault()) {
                        ImGui::Text("Default engine material");
                    }
                    else { 
                        ImGui::Text(("Material name: "+ mat->GetName()).c_str());

                        bool dif = false, normal = false;
                        if (ImGui::TreeNode("Diffuse:")) { ImGui::TreePop(); dif = true; }
                        ImGui::SameLine();
                        if (ImGui::TreeNode("Normal:")) { ImGui::TreePop(); normal = true; }

                        {
                            if (dif)
                                DrawTextureHorizontal(mat->GetDiffuse(), win_size / 2.5);
                            if (dif && normal)
                                ImGui::SameLine();
                            if (normal) {
                                if (!dif) {
                                    DrawTextureHorizontal(ResourceManager::GetTransparentMaterial()->GetDiffuse(), win_size / 2.5);
                                    ImGui::SameLine();
                                }
                                DrawTextureHorizontal(mat->GetNormal(), win_size / 2.5);
                            }
                        }

                        ////////////////////////////////////////////////////////////

                        bool spec = false, gloss = false;
                        if (ImGui::TreeNode("Specular:")) { ImGui::TreePop(); spec = true; }
                        ImGui::SameLine();
                        if (ImGui::TreeNode("Glossines:")) { ImGui::TreePop(); gloss = true; }

                        {
                            if (spec)
                                DrawTextureHorizontal(mat->GetSpecular(), win_size / 2.5);
                            if (spec && gloss)
                                ImGui::SameLine();
                            if (gloss) {
                                if (!spec) {
                                    DrawTextureHorizontal(ResourceManager::GetTransparentMaterial()->GetDiffuse(), win_size / 2.5);
                                    ImGui::SameLine();
                                }
                                DrawTextureHorizontal(mat->GetGlossines(), win_size / 2.5);
                            }
                        }
                    }
                }
                else if (temp == "Camera") {
                   
                }
                else if (temp == "PostProcessing") {
                    ImGui::Text("Gamma: ");
                    ImGui::SameLine();
                    float gamma = static_cast<PostProcessing*>(comp)->GetGamma();
                    DrawFloatInputButton("post_proc_gamma", gamma);
                    static_cast<PostProcessing*>(comp)->SetGamma(gamma);

                    if (ImGui::TreeNode("Color correction:")) {
                        glm::vec3 color = static_cast<PostProcessing*>(comp)->GetColorCorrection();

                        DrawFloatInputButton("post_proc_color_correct_r", color.r);
                        DrawFloatInputButton("post_proc_color_correct_g", color.g);
                        DrawFloatInputButton("post_proc_color_correct_b", color.b);

                        static_cast<PostProcessing*>(comp)->SetColorCorrection(color);
        
                        ImGui::TreePop();
                    }
                }

                ImGui::Separator();
            }
        }

        ImGui::PopStyleColor();
        ImGui::End();
    });

    this->m_window->GetRender()->AddGUI("engine_config", []() {
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("Config");

        ImGui::Text(("FPS: " + std::to_string(SRGraphics::Get()->GetMainWindow()->GetFPS())).c_str());

        static bool b = true;
        if (ImGui::Checkbox("Disable render into window", &b)) {
            if (b)
                SRGraphics::Get()->GetMainWindow()->GetPostProcessing()->DisableRenderIntoWindow();
            else
                SRGraphics::Get()->GetMainWindow()->GetPostProcessing()->EnableRenderIntoWindow();
        }

        //ImGui::PopStyleVar(2);
        ImGui::End();
    });

    return true;
}

bool SpaRcle::Engine::SREngine::ProcessKeyboard() {
    if (GetKey(KeyCode::LShift)) 
        shift_pressed = true;
    else
        shift_pressed = false;
    
    if (GetKeyDown(KeyCode::M)) {
        Debug::Log("SREngine::Run() : set mouse lock is " + std::to_string(!m_window->MouseLock()));
        m_window->MouseLock(!m_window->MouseLock());
    }

    if (!m_window->MouseLock()) return true;

    static float camera_speed = 0.0005;

    if (GetKey(KeyCode::W)) {
        m_camera->GetTransform()->Translate(camera_speed, 0, 0, true);
    }
    else if (GetKey(KeyCode::S)) {
        m_camera->GetTransform()->Translate(-camera_speed, 0, 0, true);
    }

    if (GetKey(KeyCode::A)) {
        m_camera->GetTransform()->Translate(0, 0, camera_speed, true);
    }
    else if (GetKey(KeyCode::D)) {
        m_camera->GetTransform()->Translate(0, 0, -camera_speed, true);
    }

    if (GetKey(KeyCode::Space)) {
        m_camera->GetTransform()->Translate(0, camera_speed, 0, true);
    }
    else if (shift_pressed)
        m_camera->GetTransform()->Translate(0, -camera_speed, 0, true);

    return true;
}
void SpaRcle::Engine::SREngine::ProcessMouse() {
    if (m_window->MouseLock()) {
        static float y = 0;
        static float x = 0;
        static POINT old_p = POINT();
        static bool  mouse = false;


        if (mouse) {
            GetCursorPos(&old_p);

            m_window->CentralizeMouse();

            POINT p;
            if (GetCursorPos(&p)) {
                y -= (old_p.x - p.x) / 25.f;
                x += (old_p.y - p.y) / 25.f;
            }
        }
        else {
            mouse = true;
            m_window->CentralizeMouse();
            GetCursorPos(&old_p);
        }
        //y = y - (int(y) / 360) * 360;

        //std::cout << y / 3.14 * 45.f * 4.f << std::endl;
        //std::cout << y << std::endl;

        if (x > 89) x = 89;
        else if (x < -89) x = -89;


        this->m_camera->GetTransform()->SetRotation(0, y, x);
    }
}

bool SpaRcle::Engine::SREngine::Create(Window* win) {
    if (m_isCreated) {
        Debug::Error("Game engine already created!");
        return false;
    } else Debug::Info("Creating game engine...");

    this->m_window = win;

    this->m_resource_folder = ResourceManager::GetAbsoluteResourceFolder();

    {
        this->m_compiler    = new Compiler();
        this->m_graph       = SRGraphics::Get();

        this->m_graph->Create(m_window, m_resource_folder);
    }

    this->m_camera = m_graph->GetMainWindow()->GetCameraGameObject();

    this->m_isCreated = true;
    return true;
}
bool SpaRcle::Engine::SREngine::Init() {
    if (!m_isCreated) {
        Debug::Error("Engine must be created before initialization!");
        return false;
    }

    if (m_isInit) {
        Debug::Error("Game engine already initialize!");
        return false;
    } else Debug::Info("Initializing game engine...");

    if (!this->m_graph->Init()) {
        Debug::Error("Failed initializing graphics engine!");
        return false;
    }

    if (!this->InitEngineGUI()) {
        Debug::Error("Failed initializing engine gui interface!");
        return false;
    }


    this->m_isInit = true;
    return true;
}
bool SpaRcle::Engine::SREngine::Run() {
    if (!m_isInit) {
        Debug::Error("Engine must be initialized before running!");
        return false;
    }

    if (m_isRunning) {
        Debug::Error("Game engine already running!");
        return false;
    } else Debug::Info("Running game engine...");

    if (!this->m_graph->Run()) {
        Debug::Error("Failed running graphics engine!");
        return false;
    }

    this->m_graph->GetMainWindow()->WaitWindowRunning();

    this->m_isRunning = true;
    
    Debug::System("All systems ran successfully!");

#ifndef SHOW_ENGINE_LOGO
    Video* logoVideo = ResourceManager::LoadVideo("logo.avi", Video::PlayMode::OnePlayOnUse, Video::RenderMode::CalculateInRealTime);
    GameObject* logoObject = GameObject::Instance("logo");
    std::vector<Mesh*> logoQuad = ResourceManager::LoadObjModel("Plane");
    logoQuad[0]->SetMaterial(logoVideo);
    logoObject->AddComponent(logoQuad[0]);
    logoObject->GetTransform()->Translate(2.2f, 0, 0);
    logoObject->GetTransform()->SetRotation(90, 0, 0);
    logoObject->GetTransform()->SetScale(logoVideo->GetVideoFormat(), 1, 1);
#endif // SHOW_ENGINE_LOGO

    {
        //this->m_arrows_tool = ResourceManager::LoadPrefab("engine/arrows");
    }

    //!=================================================================================
    {
        Script* scene_manager = new Script("scene_manager");
        this->m_compiler->AddScript(scene_manager);

        GameObject* floor = ResourceManager::LoadPrefab("engine/plane");
        floor->GetTransform()->SetScale(100, 1, 100);
        floor->GetTransform()->SetPosition(0, -5, 0);

        //GameObject* prefab = ResourceManager::LoadPrefab("player");
        //prefab->GetTransform()->SetScale(0.1, 0.1, 0.1);
        //prefab->GetTransform()->Translate(20, -5, 5);

        GameObject* obj = ResourceManager::LoadPrefab("player");
        //obj->GetTransform()->Translate(5, 0, 0);
       // cube->GetTransform()->SetScale(1, 1, 1);
        //cube->GetTransform()->Translate(10, 0, 0);

        /*std::vector<Mesh*> camera_mesh = ResourceManager::LoadObjModel("camera");
        GameObject* camera = GameObject::Instance("camera");
        camera->AddComponents(camera_mesh); 
        camera->GetTransform()->SetScale(15, 15, 15);
        camera->GetTransform()->Translate(20, 10, 5);*/

        /*Material* material = ResourceManager::CreateMaterial(
            false,
            {
                ResourceManager::LoadTexture("sina_body_diffuse.png")
            });*/

        /*
        for (int i = 0; i < 1; i++) {
            std::vector<Mesh*> meshes = ResourceManager::LoadObjModel("Sina");
            meshes[0]->SetMaterial(ResourceManager::LoadMaterial("Sina/body"));
            meshes[1]->SetMaterial(ResourceManager::LoadMaterial("Sina/body"));
            meshes[2]->SetMaterial(ResourceManager::LoadMaterial("Sina/body"));
            meshes[3]->SetMaterial(ResourceManager::LoadMaterial("Sina/bottom"));
            meshes[4]->SetMaterial(ResourceManager::LoadMaterial("Sina/hair"));
            meshes[5]->SetMaterial(ResourceManager::LoadMaterial("Sina/top"));
            meshes[6]->SetMaterial(ResourceManager::LoadMaterial("Sina/shoes"));

            GameObject* player = GameObject::Instance("player");
            player->AddComponents<Mesh*>(meshes);
            player->GetTransform()->SetScale(0.1, 0.1, 0.1);
            player->GetTransform()->Translate(20, -5, 5 * i);

            //GameObject::Destroy(player);

            //Sleep(5000);
        }*/

        this->m_window->GetPostProcessing()->DisableRenderIntoWindow();
    }
    //!=================================================================================

    bool break_event = false;
    bool is_awake_scripts = false;
    bool is_focused = false;

    while (this->m_isRunning) {
        switch (EventsManager::PopEvent()) {
        case SpaRcle::Helper::EventsManager::Event::Exit: 
            Debug::Info("SREngine::Run() : exit from game engine...");
            break_event = true;
            break;
        case SpaRcle::Helper::EventsManager::Event::Error: 
            Debug::Info("SREngine::Run() : An exception has been occured!");
            break_event = true;
            break;
        }
        if (break_event) break;

        is_focused = this->m_window->IsFocusedWindow();

        if (is_focused && GetKey(KeyCode::Esc)) {
            Debug::System("SREngine::Run() : ESC key has been pressed.");
            return false;
        }

        //===============================================
        if (m_compiler->HasNewScripts()) {
            m_compiler->CompileNewScripts();
            if (!is_awake_scripts) {
                m_compiler->AwakeNewScripts();
                is_awake_scripts = true;
            }
            m_compiler->StartNewScripts();
            m_compiler->ClearNewScripts();
        }
        //===============================================

#ifndef SHOW_ENGINE_LOGO
        if (logoVideo) {
            if (!logoVideo->IsFinished()) {
                continue;
            }
            else {
                ResourceManager::Destroy(logoVideo);
                GameObject::Destroy(logoObject);
                logoVideo = nullptr;
            }
        }
#endif // SHOW_ENGINE_LOGO

        if (is_focused) {
            this->ProcessMouse();

            if (!this->ProcessKeyboard()) break;
        }
    }

    return true;
}
bool SpaRcle::Engine::SREngine::Close() {
    if (!m_isRunning) {
        Debug::Error("Game engine is not running!");
        return false;
    } else Debug::Info("Close game engine...");

    this->m_isRunning = false;

    this->m_compiler->Destroy();

    this->m_graph->Close();

    Debug::System("All systems closed successfully!");

    return false;
}
