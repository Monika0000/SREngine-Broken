#include "SRGUI.h"

#include <sciter-x.h>
#include <sciter-x-behavior.h>
#include <sciter-x-dom.hpp>

#include "C:/Users/Nikita/source/repos/SciterTest/SciterTest/resources.cpp"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <Debug.h>

namespace SpaRcle {
    using namespace Helper;

    namespace GUI {
        static MOUSE_BUTTONS mbutton = MOUSE_BUTTONS(0);

        static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers)
        {
            MOUSE_EVENTS  me = action == GLFW_PRESS ? MOUSE_DOWN : MOUSE_UP;
            switch (button) {
            default:
            case GLFW_MOUSE_BUTTON_1: mbutton = MAIN_MOUSE_BUTTON; break;
            case GLFW_MOUSE_BUTTON_2: mbutton = PROP_MOUSE_BUTTON; break;
            case GLFW_MOUSE_BUTTON_3: mbutton = MIDDLE_MOUSE_BUTTON; break;
            }
            //KEYBOARD_STATES 
            UINT ks = 0;
            if (modifiers & GLFW_MOD_SHIFT) ks |= SHIFT_KEY_PRESSED;
            if (modifiers & GLFW_MOD_CONTROL) ks |= CONTROL_KEY_PRESSED;
            if (modifiers & GLFW_MOD_ALT) ks |= ALT_KEY_PRESSED;

            double x, y; glfwGetCursorPos(window, &x, &y);
            POINT pos = { int(x),int(y) };

            SciterProcX(window, SCITER_X_MSG_MOUSE(me, mbutton, KEYBOARD_STATES(ks), pos));
            if (me == MOUSE_UP)
                mbutton = MOUSE_BUTTONS(0);
        }
        static void mouse_move_callback(GLFWwindow* window, double x, double y)
        {
            MOUSE_EVENTS  me = MOUSE_MOVE;
            POINT pos = { int(x),int(y) };
            SciterProcX(window, SCITER_X_MSG_MOUSE(me, mbutton, KEYBOARD_STATES(0), pos));
        }
        static void mouse_enter_leave_callback(GLFWwindow* window, int enter) {
            MOUSE_EVENTS  me = enter ? MOUSE_ENTER : MOUSE_LEAVE;
            UINT ks = 0;
            double x, y; glfwGetCursorPos(window, &x, &y);
            POINT pos = { int(x),int(y) };
            SciterProcX(window, SCITER_X_MSG_MOUSE(me, MOUSE_BUTTONS(0), KEYBOARD_STATES(ks), pos));
        }
        static void mouse_wheel_callback(GLFWwindow* window, double dx, double dy)
        {
            MOUSE_EVENTS  me = MOUSE_WHEEL;
            double x, y; glfwGetCursorPos(window, &x, &y);
            POINT pos = { int(x),int(y) };
            dx *= 16;
            dy *= 16;
            UINT  deltas = ((short)dx) << 16 | (short)dy;
            SciterProcX(window, SCITER_X_MSG_MOUSE(me, MOUSE_BUTTONS(deltas), KEYBOARD_STATES(0), pos));
        }

        bool SRGUI::InitOptions() {
            UINT version = SciterVersion(true);
            Debug::Graph("SRGUI::InitOptions() : initializing gui " + std::to_string(version) + " version...");

            if (!SciterSetOption(NULL, SCITER_SET_UX_THEMING, TRUE)) {
                Debug::Error("SRGUI::InitOptions() : failed SCITER_SET_UX_THEMING!");
                return false;
            }

            // these two calls are optional, used here to enable communication with inspector.exe (CTRL+SHIFT+I with running inspector) 
            if (!SciterSetOption(NULL, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
                ALLOW_FILE_IO |
                ALLOW_SOCKET_IO |
                ALLOW_EVAL |
                ALLOW_SYSINFO)) {
                Debug::Error("SRGUI::InitOptions() : failed SCITER_SET_SCRIPT_RUNTIME_FEATURES!");
                return false;
            }

            if (!SciterSetOption(NULL, SCITER_SET_DEBUG_MODE, TRUE)) {
                Debug::Error("SRGUI::InitOptions() : failed SCITER_SET_DEBUG_MODE!");
                return false;
            }
                
            return true;
        }

        bool SRGUI::AttachGLFW(GLFWwindow* glfw_window) {
            Debug::Graph("SRGUI::AttachGLFW() : attaching glfw window to gui...");
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

            m_glfw_window = glfw_window;

            // setup event callbacks:
            glfwSetMouseButtonCallback(m_glfw_window, mouse_button_callback);
            glfwSetCursorPosCallback(m_glfw_window, mouse_move_callback);
            glfwSetCursorEnterCallback(m_glfw_window, mouse_enter_leave_callback);
            glfwSetScrollCallback(m_glfw_window, mouse_wheel_callback);

            int used_width = 0, used_height = 0;

            if (!SciterProcX(m_glfw_window, SCITER_X_MSG_CREATE(GFX_LAYER_SKIA_OPENGL, FALSE))) {
                Debug::Error("SRGUI::AttachGLFW() : failed SCITER_X_MSG_CREATE!");
                return false;
            }
            // set device resoultion ( pixels per inch )

            float xscale = 1;
            float yscale = 1;
            // glfwGetWindowContentScale(window, &xscale, &yscale);

             //std::cout << "scale : " << xscale << std::endl;

            // !SciterProcX(window, SCITER_X_MSG_RESOLUTION(UINT(96 * xscale)));
             // set surface size

            glfwGetFramebufferSize(m_glfw_window, &used_width, &used_height);

            if(!SciterProcX(m_glfw_window, SCITER_X_MSG_SIZE(used_width, used_height))) {
                Debug::Error("SRGUI::AttachGLFW() : failed SCITER_X_MSG_SIZE!");
                return false;
            }

            sciter::archive::instance().open(aux::elements_of(resources));
            SciterLoadFile(m_glfw_window, WSTR("file://J:\\C++\\SREngine\\Resources\\GUI\\facade\\main.htm"));

            return true;
        }



        bool SRGUI::LoadFile(std::string path) {
            return true;
        }

        bool SRGUI::Draw() {
            UINT ticks = UINT(glfwGetTime() * 1000); // inmilliseconds
// give sciter a chance to process animations, timers and other timed things
            SciterProcX(m_glfw_window, SCITER_X_MSG_HEARTBIT(ticks));
            SciterProcX(m_glfw_window, SCITER_X_MSG_PAINT());
            return true;
        }
        bool SRGUI::Destroy() {
            SciterProcX(m_glfw_window, SCITER_X_MSG_DESTROY());
            return true;
        }
    }
}

//int main() {
//    std::cout << SpaRcle::GUI::SRGUI::InitOptions() << std::endl;
//    Sleep(10000);
//    return 1;
//}