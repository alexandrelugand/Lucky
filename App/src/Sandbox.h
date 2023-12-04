#include <Lucky/Application/EntryPoint.h>
#include <Lucky/Core/Log.h>

class SandBox : public Lucky::Application
{
public:
    SandBox(const std::string& title);
    virtual ~SandBox();
};