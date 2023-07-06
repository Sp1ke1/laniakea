#pragma once
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"



namespace lk {
    unsigned int Window::s_WindowCount = 0;

    Window::Window(const lk::WindowProperties &Properties) {
        Init ( Properties );
    }

    bool lk::Window::IsVSync() const {
        return m_WindowData.VSync;
    }


    Window::~Window() {
        Shutdown();
    }

    void Window::Update() {
        glfwPollEvents();
    }


    uint32_t Window::GetWidth() const {
        return m_WindowData.Width;
    }

    uint32_t Window::GetHeight() const {
        return m_WindowData.Height;
    }

    void Window::SetEventCallback(const EventCallbackFn &callback) {
        m_WindowData.EventCallback = callback;
    }

    void Window::SetVSync(bool enabled) {
        if ( enabled )
            glfwSwapInterval ( 1 );
        else
            glfwSwapInterval ( 0 );
        m_WindowData.VSync = enabled;
    }

    void Window::Shutdown() {
        glfwDestroyWindow ( m_Window );
        s_WindowCount--;
    }

    void Window::Init(const WindowProperties &Properties) {
        m_WindowData = Properties;

        Logger::Get() ->Log( { "Application", LogMessageType::Log, std::string("Creating window with title: ") + Properties.Title } );

        LK_ASSERT ( (int)m_WindowData.Height > 0 && (int)m_WindowData.Width > 0 );
        if ( Window::s_WindowCount == 0 )
        {
            int success = glfwInit();
            LK_ASSERT ( success );
        }
        m_Window = glfwCreateWindow ( (int)m_WindowData.Width, (int)m_WindowData.Height, m_WindowData.Title.c_str(), nullptr, nullptr );
        if ( !m_Window )
        {
            Logger::Get() -> Log ( { "Application", LogMessageType::Error, "Window::Init(). Can't create window" } );
            return;
        }

        // Binds some user data pointer and allows to get it using glfwGetWindowUserPointer( glfwWindow * )
        glfwSetWindowUserPointer( m_Window, &m_WindowData );

        // If the window being resized => fetch data WindowData pointer via window pointer, update it and call callback fn
        glfwSetWindowSizeCallback ( m_Window, []( GLFWwindow * window, int width, int height )
                                    {
                                        WindowProperties & data = *(WindowProperties*) glfwGetWindowUserPointer( window );
                                        data.Width = width;
                                        data.Height = height;

                                        WindowResizeEvent event (width, height);
                                        data.EventCallback ( event );
                                    }
        );

        glfwSetWindowCloseCallback( m_Window, [](GLFWwindow * window )
        {
            WindowProperties & data = *(WindowProperties*) glfwGetWindowUserPointer( window );
            WindowCloseEvent event;
            data.EventCallback ( event );
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowProperties & data = *(WindowProperties*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, true);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });

        s_WindowCount++;
    }


}

