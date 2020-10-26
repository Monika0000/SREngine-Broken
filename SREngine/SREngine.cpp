#include "pch.h"
#include "SREngine.h"
#include <Input.h>
#include <EventsManager.h>
#include <ResourceManager.h>

#include <SRFile.h>
#include <imgui.h>

#include <vector>
#include <map>
#include <imgui.cpp>

using namespace SpaRcle::Helper;
using namespace SpaRcle::Graphics;

ImGuiTreeNodeFlags node_flags_with_childs = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
ImGuiTreeNodeFlags node_flags_without_childs = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

static bool shift_pressed = false;

bool ButtonWithId(const char* _id, const char* label, ImGuiButtonFlags flags = ImGuiButtonFlags_None) {
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
    ImVec2 size = ImGui::CalcItemSize(ImVec2(0,0), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

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

int EnterNumber() {
    if (GetKeyDown(KeyCode::_0))
        return 0;
    else if (GetKeyDown(KeyCode::_1))
        return 1;
    else if (GetKeyDown(KeyCode::_2))
        return 2;
    else if (GetKeyDown(KeyCode::_3))
        return 3;
    else if (GetKeyDown(KeyCode::_4))
        return 4;
    else if (GetKeyDown(KeyCode::_5))
        return 5;
    else if (GetKeyDown(KeyCode::_6))
        return 6;
    else if (GetKeyDown(KeyCode::_7))
        return 7;
    else if (GetKeyDown(KeyCode::_8))
        return 8;
    else if (GetKeyDown(KeyCode::_9))
        return 9;
    else if (GetKeyDown(KeyCode::BackSpace))
        return -1;
    else if (GetKeyDown(KeyCode::Enter))
        return -2;
    else if (GetKeyDown(KeyCode::Minus))
        return -3;
    else
        return -4;
}

void DrawSelectableFloat(std::string object, float& number) {
    static std::string current_object = "";
    static std::string text = "";
    bool edit = object == current_object;

    if (ButtonWithId(object.c_str(), edit ? text.c_str() : (std::to_string(number)).c_str())) {
        current_object = object;
        text = ""; //std::to_string(number);
    }

    if (edit) {
        int i = EnterNumber();
        if (i >= 0) {
            text += std::to_string(i);
        } 
        else if (i == -1) {
            if (text.size() > 0) {
                text.pop_back();
            }
        }
        else if (i == -2) {
            if (text.size() == 0)
                number = 0;
            else
                number = std::stod(text.c_str());

            current_object.clear();
        }
        else if (i == -3 && text.size() == 0) {
            text += "-";
        }
    }
}

bool SpaRcle::Engine::SREngine::InitEngineGUI() {
    this->m_window->GetRender()->AddGUI("engine_hierarchy", []() {
        ImGui::Begin("Hierachy", NULL, ImGuiWindowFlags_NoMove);

        std::vector<GameObject*> gms = ResourceManager::GetGameObjects();

        if (ImGui::TreeNode("Game objects")) {
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

        ImGui::End();
    });

    this->m_window->GetRender()->AddGUI("engine_inspector", []() {
        ImGui::Begin("Inspector", NULL);
        
        glm::vec3 vec_temp = {};
        std::string temp = "";

        auto objs = ResourceManager::GetAllSelectedGameObjects();
        if (objs.size() == 1) {
            ImGui::Separator();
            TextCenter("Transform");

            ImGui::Text(("Name: " + objs[0]->GetName()).c_str()); 
            ImGui::Text(("Tag:  " + objs[0]->GetTag()).c_str()); 

            {
                vec_temp = objs[0]->GetTransform()->GetPosition();

                ImGui::Text("Pos: ");
                ImGui::SameLine();
                DrawSelectableFloat("transf_pos_x", vec_temp.x);
                ImGui::SameLine();
                DrawSelectableFloat("transf_pos_y", vec_temp.y);
                ImGui::SameLine();
                DrawSelectableFloat("transf_pos_z", vec_temp.z);

                objs[0]->GetTransform()->SetPosition(vec_temp);
            }

            {
                vec_temp = objs[0]->GetTransform()->GetRotation();

                ImGui::Text("Rot: ");
                ImGui::SameLine();
                DrawSelectableFloat("transf_rot_x", vec_temp.x);
                ImGui::SameLine();
                DrawSelectableFloat("transf_rot_y", vec_temp.y);
                ImGui::SameLine();
                DrawSelectableFloat("transf_rot_z", vec_temp.z);

                objs[0]->GetTransform()->SetRotation(vec_temp);
            }

            {
                vec_temp = objs[0]->GetTransform()->GetSclae();

                ImGui::Text("Scl:  ");
                ImGui::SameLine();
                DrawSelectableFloat("transf_scl_x", vec_temp.x);
                ImGui::SameLine();
                DrawSelectableFloat("transf_scl_y", vec_temp.y);
                ImGui::SameLine();
                DrawSelectableFloat("transf_scl_z", vec_temp.z);

                objs[0]->GetTransform()->SetScale(vec_temp);
            }

            ImGui::Text("");

            ImGui::Text(("Parent pos:  " + objs[0]->GetTransform()->GetStringParentPosition()).c_str());
            ImGui::Text(("Parent rot:  " + objs[0]->GetTransform()->GetStringParentRotation()).c_str());
            ImGui::Text(("Parent scl:  " + objs[0]->GetTransform()->GetStringParentScale()).c_str());

            ImGui::Separator();

            for (auto comp : objs[0]->GetComponents()) {
                temp = std::string(comp->TypeName());
                std::string comp_name = "Component: " + temp;

                TextCenter(comp_name);

                if (temp == "Mesh") {
                    Mesh* mesh = static_cast<Mesh*>(comp);
                    
                    ImGui::Text(("Mesh name: " + mesh->GetName()).c_str());
                    ImGui::Separator();
                    TextCenter("Component: Material");
                    if (mesh->GetMaterial()->IsDefault()) {
                        ImGui::Text("Default engine material");
                    }
                    else {
                        ImGui::Text(("Material name: "+ mesh->GetMaterial()->GetName()).c_str());
                    }
                }
                else if (temp == "Camera") {
                   
                }

                ImGui::Separator();
            }
        }

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
        m_camera->GetTransform()->Translate(0, 0, -camera_speed, true);
    }
    else if (GetKey(KeyCode::D)) {
        m_camera->GetTransform()->Translate(0, 0, camera_speed, true);
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

        this->m_camera->GetTransform()->SetRotation(x, y, 0);
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

    //!===========================================[LOGO]===========================================
    Video* logoVideo = ResourceManager::LoadVideo("logo.avi", Video::PlayMode::OnePlayOnUse, Video::RenderMode::CalculateInRealTime);
    GameObject* logoObject = GameObject::Instance("logo");
    std::vector<Mesh*> logoQuad = ResourceManager::LoadObjModel("Plane");
    logoQuad[0]->SetMaterial(logoVideo);
    logoObject->AddComponent(logoQuad[0]);
    logoObject->GetTransform()->Translate(2.2f, 0, 0);
    logoObject->GetTransform()->SetRotation(90, 0, 0);
    logoObject->GetTransform()->SetScale(logoVideo->GetVideoFormat(), 1, 1);
    //!===========================================[LOGO]===========================================

    //!=================================================================================
    {
        Script* scene_manager = new Script("scene_manager");
        this->m_compiler->AddScript(scene_manager);

        GameObject* prefab = ResourceManager::LoadPrefab("player");
        prefab->GetTransform()->SetScale(0.1, 0.1, 0.1);
        prefab->GetTransform()->Translate(20, -5, 5);

        GameObject* cube = ResourceManager::LoadPrefab("parent_test");
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

        if (logoVideo) {
            if (!logoVideo->IsFinished()) {
             //   continue;
            }
            else {
                ResourceManager::Destroy(logoVideo);
                GameObject::Destroy(logoObject);
                logoVideo = nullptr;
            }
        }

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
