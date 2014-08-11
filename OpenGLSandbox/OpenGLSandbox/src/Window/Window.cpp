#include "Window/Window.h"
#include "Window/Event.h"

namespace
{
    unsigned int windowCount = 0;
    const char* windowClassName = "GL_Window";
}

Window::Window(VideoMode videoMode, const std::string& title, Window::Style style)
    :   m_size(videoMode.getModeWidth(), videoMode.getModeHeight())
    ,   m_windowHandle(nullptr)
    ,   m_deviceContextHandle(nullptr)
    ,   m_GLContext(nullptr)
    ,   m_isOpen(false)
    ,   m_containsMouse(false)
{
    // Register the window class when during first window creation
    if (windowCount == 0)
        registerWindowClass();

    // screenDC has access to desktop's size in pixels
    HDC screenDC = GetDC(NULL);
    int left   = (GetDeviceCaps(screenDC, HORZRES) - videoMode.getModeWidth()) / 2;
    int top    = (GetDeviceCaps(screenDC, VERTRES) - videoMode.getModeHeight()) / 2;
    int width  = videoMode.getModeWidth();
    int height = videoMode.getModeHeight();
    ReleaseDC(NULL, screenDC);

    // Select the appropriate windows style
    DWORD win32Style = WS_VISIBLE;
    if (style == Style::None)
    {
        win32Style |= WS_POPUP;
    }
    else
    {
        if (style & Style::TitleBar)      win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        if (style & Style::Resizable)     win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        if (style & Style::CloseControls) win32Style |= WS_SYSMENU;
    }

    // Adjustment for the window client area
    RECT rectangle = {0, 0, width, height};
    AdjustWindowRect(&rectangle, win32Style, false);
    width  = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;

    // Create the window
    m_windowHandle = CreateWindow(windowClassName, title.c_str(), win32Style, 
                                  left, top, width, height, NULL, NULL, GetModuleHandle(NULL), this);

    // Set the instance pointer as user data associated with hWnd
    SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, (LONG_PTR)this);

    // Create the OpenGL Context
    m_deviceContextHandle = GetDC(m_windowHandle);
    m_GLContext = new GLContext(m_deviceContextHandle);

    // Everything went fine, open the window
    m_isOpen = true;

    // Increment window count
    windowCount++;
}

Window::~Window()
{
    delete m_GLContext;

    // Destroy the window
    if (m_windowHandle != nullptr)
        DestroyWindow(m_windowHandle);

    // Decrement the window count
    windowCount--;

    // Unregister window class on last destroyed window
    if (windowCount == 0)
        UnregisterClass(windowClassName, GetModuleHandleW(NULL));
}

bool Window::isOpen() const
{
    return m_isOpen;
}

void Window::processMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // Triggered on cursor entering the window area
    case WM_SETCURSOR:
        {
            // If the mouse cursor has entered the window, show the default one
            if (LOWORD(lParam) == HTCLIENT)
            {
                HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
                SetCursor(cursor);
            }

            break;
        }

    // Triggered once a close request is made ("X" button, Alt-F4, ...)
    case WM_CLOSE:
        {
            Event event;
            event.type = Event::Closed;
            pushEvent(event);
            break;
        }

    // Triggered on resizing
    case WM_SIZE:
    {
        // Consider only events triggered by a maximize or a un-maximize
        if (wParam != SIZE_MINIMIZED && !m_isResizing && m_prevSize != getSize())
        {
            // Update previous housekept size
            m_prevSize = getSize();

            // Push a resize event
            Event event;
            event.type = Event::Resized;
            event.size.width = m_prevSize.getX();
            event.size.height = m_prevSize.getY();
            pushEvent(event);
        }
        break;
    }

    // Start resizing
    case WM_ENTERSIZEMOVE:
    {
        m_isResizing = true;
       
        break;
    }

    // Stop resizing
    case WM_EXITSIZEMOVE:
    {
        m_isResizing = false;

        // Ignore cases where the window has only been moved
        if (m_prevSize != getSize())
        {
            // Update previous housekept size
            m_prevSize = getSize();

            // Push a resize event
            Event event;
            event.type = Event::Resized;
            event.size.width = m_prevSize.getX();
            event.size.height = m_prevSize.getY();
            pushEvent(event);
        }
        break;
    }

    // Triggered on any key press event
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        {
            if ((HIWORD(lParam) & KF_REPEAT) == 0)
            {
                Event event;
                event.type        = Event::KeyPressed;
                event.key.alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
                event.key.control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
                event.key.shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
                event.key.code    = VirtualKeyToKeyEvent(wParam, lParam);
                pushEvent(event);
            }
            break;
        }

    // Triggered on any key release event
    case WM_KEYUP:
    case WM_SYSKEYUP:
        {
            Event event;
            event.type        = Event::KeyReleased;
            event.key.alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
            event.key.control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
            event.key.shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
            event.key.code    = VirtualKeyToKeyEvent(wParam, lParam);
            pushEvent(event);
            break;
        }

    // Mouse left button press event
    case WM_LBUTTONDOWN:
        {
            Event event;
            event.type               = Event::MouseButtonPressed;
            event.mouseButton.button = Mouse::Left;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse left button release event
    case WM_LBUTTONUP:
        {
            Event event;
            event.type               = Event::MouseButtonReleased;
            event.mouseButton.button = Mouse::Left;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse right button press event
    case WM_RBUTTONDOWN:
        {
            Event event;
            event.type               = Event::MouseButtonPressed;
            event.mouseButton.button = Mouse::Right;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse right button release event
    case WM_RBUTTONUP:
        {
            Event event;
            event.type               = Event::MouseButtonReleased;
            event.mouseButton.button = Mouse::Right;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse wheel button press event
    case WM_MBUTTONDOWN:
        {
            Event event;
            event.type               = Event::MouseButtonPressed;
            event.mouseButton.button = Mouse::Middle;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse wheel button release event
    case WM_MBUTTONUP:
        {
            Event event;
            event.type               = Event::MouseButtonReleased;
            event.mouseButton.button = Mouse::Middle;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse extra button press event
    case WM_XBUTTONDOWN:
        {
            Event event;
            event.type               = Event::MouseButtonPressed;
            event.mouseButton.button = HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse extra button release event
    case WM_XBUTTONUP:
        {
            Event event;
            event.type               = Event::MouseButtonReleased;
            event.mouseButton.button = HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            event.mouseButton.x      = static_cast<short int>(LOWORD(lParam));
            event.mouseButton.y      = static_cast<short int>(HIWORD(lParam));
            pushEvent(event);
            break;
        }

    // Mouse leave event
    case WM_MOUSELEAVE:
        {
            // Avoid this firing a second time in case the cursor is dragged outside
            if (m_containsMouse)
            {
                m_containsMouse = false;

                Event event;
                event.type = Event::MouseLeft;
                pushEvent(event);
            }
            break;
        }

    // Mouse move event
    case WM_MOUSEMOVE:
        {
            // Extract the mouse local coordinates
            int x = static_cast<short int>(LOWORD(lParam));
            int y = static_cast<short int>(HIWORD(lParam));

            // Get the client area of the window
            RECT area;
            GetClientRect(m_windowHandle, &area);

            // Capture the mouse in case the user wants to drag it outside
            if ((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0)
            {
                // Only release the capture if it matches the window handle
                if (GetCapture() == m_windowHandle)
                    ReleaseCapture();
            }
            else if (GetCapture() != m_windowHandle)
            {
                // Continue receiving mouse events on the window
                SetCapture(m_windowHandle);
            }

            // If the cursor is outside the client area...
            if ((x < area.left) || (x > area.right) || (y < area.top) || (y > area.bottom))
            {
                // and it used to be inside, the mouse left it
                if (m_containsMouse)
                {
                    m_containsMouse = false;

                    // Unsubscribe from mouse tracking (hover, drag) events
                    TRACKMOUSEEVENT mouseEvent;
                    mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
                    mouseEvent.dwFlags = TME_CANCEL;
                    mouseEvent.hwndTrack = m_windowHandle;
                    mouseEvent.dwHoverTime = HOVER_DEFAULT;
                    TrackMouseEvent(&mouseEvent);

                    // Generate an appropriate event
                    Event event;
                    event.type = Event::MouseLeft;
                    pushEvent(event);
                }
            }
            else
            {
                // In case it is inside, and it was outside, adjust mouse tracking
                if (!m_containsMouse)
                {
                    m_containsMouse = true;

                    // Look for the mouse leaving the window
                    TRACKMOUSEEVENT mouseEvent;
                    mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
                    mouseEvent.dwFlags = TME_LEAVE;
                    mouseEvent.hwndTrack = m_windowHandle;
                    mouseEvent.dwHoverTime = HOVER_DEFAULT;
                    TrackMouseEvent(&mouseEvent);

                    // Generate a MouseEntered event
                    Event event;
                    event.type = Event::MouseEntered;
                    pushEvent(event);
                }
            }

            // Generate a MouseMove event, in any case
            Event event;
            event.type        = Event::MouseMoved;
            event.mouseMove.x = x;
            event.mouseMove.y = y;
            pushEvent(event);
            break;
        }
    }
}

void Window::display()
{
    SwapBuffers(m_deviceContextHandle);
}

void Window::registerWindowClass()
{
    WNDCLASS windowClass;
    windowClass.style         = CS_OWNDC;
    windowClass.lpfnWndProc   = &WndProc;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = GetModuleHandle(nullptr);
    windowClass.hIcon         = nullptr;
    windowClass.hCursor       = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName  = nullptr;
    windowClass.lpszClassName = windowClassName;
    RegisterClass(&windowClass);
}

void Window::processIncomingMessages(bool blocking)
{
    MSG message;
    if (blocking)
    {
        while (GetMessage(&message, NULL, 0, 0))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
    else
    {
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // If extra data has been set correctly, obtain a pointer to concrete Window class
    Window* window;
    if (hWnd != nullptr)
        window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    else
        window = nullptr;

    // If window instance was found, delegate event processing to its non-static handler
    if (window != nullptr)
    {
        window->processMessage(message, wParam, lParam);
    }
    
    // Prevent WM_CLOSE to reach default handling procedure because it will destroy the window
    if (message == WM_CLOSE)
        return 0;

    // Default windows procedure for handling and discarding unprocessed messages
    return DefWindowProc(hWnd, message, wParam, lParam);
}

Keyboard::Key Window::VirtualKeyToKeyEvent(WPARAM wParam, LPARAM lParam)
{
    Keyboard::Key registeredKey = Keyboard::Unknown;
    switch (wParam)
    {
    // Check the scan-code to discern between left and right shift
    case VK_SHIFT:
        {
            static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scanCode = static_cast<UINT>((lParam & (0xFF << 16)) >> 16);
            if (scanCode == lShift)
                registeredKey = Keyboard::LeftShift;
            else
                registeredKey = Keyboard::RightShift;
            break;
        }

    // Check the lParam to discern between left and right control
    case VK_CONTROL:
        {
            if (HIWORD(lParam) & KF_EXTENDED)
                registeredKey = Keyboard::LeftControl;
            else
                registeredKey = Keyboard::RightControl;
            break;
        }

    // Check the lParam flag to discern between left and right alt
    case VK_MENU:
        {
            if (HIWORD(lParam) & KF_EXTENDED)
                registeredKey = Keyboard::LeftAlt;
            else
                registeredKey = Keyboard::RightAlt;
            break;
        }

    // Other keys do not need lParam
    case VK_ESCAPE:     registeredKey = Keyboard::Escape;        break;
    case VK_F1:         registeredKey = Keyboard::F1;            break;
    case VK_F2:         registeredKey = Keyboard::F2;            break;
    case VK_F3:         registeredKey = Keyboard::F3;            break;
    case VK_F4:         registeredKey = Keyboard::F4;            break;
    case VK_F5:         registeredKey = Keyboard::F5;            break;
    case VK_F6:         registeredKey = Keyboard::F6;            break;
    case VK_F7:         registeredKey = Keyboard::F7;            break;
    case VK_F8:         registeredKey = Keyboard::F8;            break;
    case VK_F9:         registeredKey = Keyboard::F9;            break;
    case VK_F10:        registeredKey = Keyboard::F10;           break;
    case VK_F11:        registeredKey = Keyboard::F11;           break;
    case VK_F12:        registeredKey = Keyboard::F12;           break;
    case VK_PRINT:      registeredKey = Keyboard::PrintScreen;   break;
    case VK_SCROLL:     registeredKey = Keyboard::ScrollLock;    break;
    case VK_PAUSE:      registeredKey = Keyboard::Pause;         break;
    case VK_OEM_3:      registeredKey = Keyboard::Tilde;         break;
    case '1':           registeredKey = Keyboard::Num1;          break;
    case '2':           registeredKey = Keyboard::Num2;          break;
    case '3':           registeredKey = Keyboard::Num3;          break;
    case '4':           registeredKey = Keyboard::Num4;          break;
    case '5':           registeredKey = Keyboard::Num5;          break;
    case '6':           registeredKey = Keyboard::Num6;          break;
    case '7':           registeredKey = Keyboard::Num7;          break;
    case '8':           registeredKey = Keyboard::Num8;          break;
    case '9':           registeredKey = Keyboard::Num9;          break;
    case '0':           registeredKey = Keyboard::Num0;          break;
    case VK_OEM_MINUS:  registeredKey = Keyboard::UnderScore;    break;
    case VK_OEM_PLUS:   registeredKey = Keyboard::Equals;        break;
    case VK_BACK:       registeredKey = Keyboard::BackSpace;     break;
    case VK_TAB:        registeredKey = Keyboard::Tab;           break;
    case 'Q':           registeredKey = Keyboard::Q;             break;
    case 'W':           registeredKey = Keyboard::W;             break;
    case 'E':           registeredKey = Keyboard::E;             break;
    case 'R':           registeredKey = Keyboard::R;             break;
    case 'T':           registeredKey = Keyboard::T;             break;
    case 'Y':           registeredKey = Keyboard::Y;             break;
    case 'U':           registeredKey = Keyboard::U;             break;
    case 'I':           registeredKey = Keyboard::I;             break;
    case 'O':           registeredKey = Keyboard::O;             break;
    case 'P':           registeredKey = Keyboard::P;             break;
    case VK_OEM_4:      registeredKey = Keyboard::LeftBracket;   break;
    case VK_OEM_6:      registeredKey = Keyboard::RightBracket;  break;
    case VK_OEM_5:      registeredKey = Keyboard::BackSlash;     break;
    case VK_CAPITAL:    registeredKey = Keyboard::CapsLock;      break;
    case 'A':           registeredKey = Keyboard::A;             break;
    case 'S':           registeredKey = Keyboard::S;             break;
    case 'D':           registeredKey = Keyboard::D;             break;
    case 'F':           registeredKey = Keyboard::F;             break;
    case 'G':           registeredKey = Keyboard::G;             break;
    case 'H':           registeredKey = Keyboard::H;             break;
    case 'J':           registeredKey = Keyboard::J;             break;
    case 'K':           registeredKey = Keyboard::K;             break;
    case 'L':           registeredKey = Keyboard::L;             break;
    case VK_OEM_1:      registeredKey = Keyboard::SemiColon;     break;
    case VK_OEM_7:      registeredKey = Keyboard::Quote;         break;
    case VK_RETURN:     registeredKey = Keyboard::Return;        break;
    case 'Z':           registeredKey = Keyboard::Z;             break;
    case 'X':           registeredKey = Keyboard::X;             break;
    case 'C':           registeredKey = Keyboard::C;             break;
    case 'V':           registeredKey = Keyboard::V;             break;
    case 'B':           registeredKey = Keyboard::B;             break;
    case 'N':           registeredKey = Keyboard::N;             break;
    case 'M':           registeredKey = Keyboard::M;             break;
    case VK_OEM_COMMA:  registeredKey = Keyboard::Comma;         break;
    case VK_OEM_PERIOD: registeredKey = Keyboard::Period;        break;
    case VK_OEM_2:      registeredKey = Keyboard::FrontSlash;    break;
    case VK_LWIN:       registeredKey = Keyboard::LeftSystem;    break;
    case VK_SPACE:      registeredKey = Keyboard::Space;         break;
    case VK_RWIN:       registeredKey = Keyboard::RightSystem;   break;
    case VK_APPS:       registeredKey = Keyboard::Menu;          break;
    case VK_INSERT:     registeredKey = Keyboard::Insert;        break;
    case VK_HOME:       registeredKey = Keyboard::Home;          break;
    case VK_PRIOR:      registeredKey = Keyboard::PageUp;        break;
    case VK_DELETE:     registeredKey = Keyboard::Delete;        break;
    case VK_END:        registeredKey = Keyboard::End;           break;
    case VK_NEXT:       registeredKey = Keyboard::PageDown;      break;
    case VK_UP:         registeredKey = Keyboard::Up;            break;
    case VK_LEFT:       registeredKey = Keyboard::Left;          break;
    case VK_DOWN:       registeredKey = Keyboard::Down;          break;
    case VK_RIGHT:      registeredKey = Keyboard::Right;         break;
    case VK_NUMLOCK:    registeredKey = Keyboard::NumLock;       break;
    case VK_DIVIDE:     registeredKey = Keyboard::Divide;        break;
    case VK_MULTIPLY:   registeredKey = Keyboard::Multiply;      break;
    case VK_SUBTRACT:   registeredKey = Keyboard::Subtract;      break;
    case VK_NUMPAD7:    registeredKey = Keyboard::NumPad7;       break;
    case VK_NUMPAD8:    registeredKey = Keyboard::NumPad8;       break;
    case VK_NUMPAD9:    registeredKey = Keyboard::NumPad9;       break;
    case VK_ADD:        registeredKey = Keyboard::Add;           break;
    case VK_NUMPAD4:    registeredKey = Keyboard::NumPad4;       break;
    case VK_NUMPAD5:    registeredKey = Keyboard::NumPad5;       break;
    case VK_NUMPAD6:    registeredKey = Keyboard::NumPad6;       break;
    case VK_NUMPAD1:    registeredKey = Keyboard::NumPad1;       break;
    case VK_NUMPAD2:    registeredKey = Keyboard::NumPad2;       break;
    case VK_NUMPAD3:    registeredKey = Keyboard::NumPad3;       break;
    case VK_NUMPAD0:    registeredKey = Keyboard::NumPad0;       break;
    case VK_DECIMAL:    registeredKey = Keyboard::NumPadDecimal; break;
    }

    return registeredKey;
}

bool Window::popEvent(Event& event, bool blocking)
{
    // Add any new events in case the current eventQueue is empty
    if (m_eventQueue.empty())
    {
        processIncomingMessages(blocking);
    }

    // If the event queue is not empty, get the first event from the queue
    if (!m_eventQueue.empty())
    {
        event = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }
    
    return false;
}

void Window::pushEvent(const Event& event)
{
    m_eventQueue.push(event);
}

void Window::close()
{
    m_isOpen = false;
}

bool Window::pollEvent(Event& event)
{
    return popEvent(event, false);
}

bool Window::waitEvent(Event& event)
{
    return popEvent(event, true);
}


Vector2Du Window::getSize() const
{
    RECT rect;
    GetClientRect(m_windowHandle, &rect);

    return Vector2Du(rect.right - rect.left, rect.bottom - rect.top);
}