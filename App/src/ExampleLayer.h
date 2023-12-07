#include <Lucky.h>

class ExampleLayer : public Lucky::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer();

    void OnUpdate() override;
    void OnEvent(Lucky::Event& event) override;
};