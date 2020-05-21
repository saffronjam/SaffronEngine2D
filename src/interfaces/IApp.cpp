
#include "IApp.h"

IApp::IApp()
    : m_screenList(std::make_unique<ScreenList>(this)),
      m_currentScreen(nullptr),
      m_isRunning(true)
{
    EventMgr::AddOnEventFunction(this);

    SDL_DisplayMode target,
        closest;

    target.w = 1024;
    target.h = 768;
    target.format = 0;
    target.refresh_rate = 0;
    target.driverdata = 0;

    if (!SDL_GetClosestDisplayMode(0, &target, &closest))
    {
        VEAPP_LAST_EXCEPT();
    }
    else
    {
        m_window.Create("V-2DEngine", closest.w, closest.h);
    }
}

IApp::~IApp()
{
}

void IApp::Run()
{
    Init();
    while (m_isRunning)
    {
        Keyboard::UpdateKeyMaps();
        Mouse::UpdateButtonMaps();
        EventMgr::PollAll();
        Window::Clear();
        Update();
        Draw();
        Window::Present();
    }
}
void IApp::Exit()
{
    if (m_currentScreen)
        m_currentScreen->OnExit();
    if (m_screenList)
        m_screenList.reset();

    m_isRunning = false;
}

void IApp::Update()
{
    if (m_currentScreen)
    {
        switch (m_currentScreen->GetState())
        {
        case ScreenState::Running:
            m_currentScreen->Update();
            break;
        case ScreenState::ChangeNext:
            m_currentScreen->OnExit();
            m_currentScreen = m_screenList->MoveNext();
            if (m_currentScreen)
            {
                m_currentScreen->SetRunning();
                m_currentScreen->OnEntry();
            }
            break;
        case ScreenState::ChangePrevious:
            m_currentScreen->OnExit();
            m_currentScreen = m_screenList->MovePrevious();
            if (m_currentScreen)
            {
                m_currentScreen->SetRunning();
                m_currentScreen->OnEntry();
            }
            break;
        case ScreenState::ExitApp:
            Exit();
            break;
        default:
            break;
        }
    }
}
void IApp::Draw()
{
    if (m_currentScreen && m_currentScreen->GetState() == ScreenState::Running)
    {
        m_currentScreen->Draw();
    }
}

bool IApp::Init()
{
    OnInit();
    AddScreens();

    m_currentScreen = m_screenList->GetCurrent();
    m_currentScreen->OnEntry();
    m_currentScreen->SetRunning();

    return true;
}

void IApp::OnEvent(const SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        Exit();
        break;
    }
}

IApp::Exception::Exception(int line, const char *file, const char *errorString) noexcept
    : VeException(line, file),
      errorString(errorString)
{
}

const char *IApp::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << "[Type] " << GetType() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *IApp::Exception::GetType() const noexcept
{
    return "V-2DEngine App Exception";
}

const char *IApp::Exception::GetErrorString() const noexcept
{
    return errorString.c_str();
}