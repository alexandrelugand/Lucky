#pragma once

#include <Lucky.h>
#include <Lucky/EntryPoint.h>

namespace Lucky
{
	class LuckyEditor : public Application
	{
	public:
	    LuckyEditor(const WindowProps& props, const ApplicationCommandLineArgs& args);
	    virtual ~LuckyEditor();
	};
}
