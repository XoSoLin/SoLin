#include<SoLin.h>
#include<SoLin/Core/EntryPoint.h>

#include"EditorLayer.h"

namespace SoLin {

    class SoLinEditor :public SoLin::Application { 
    public:
	    SoLinEditor()
            :Application("SoLin Editor")
        {
		    PushLayer(new EditorLayer());
	    }
	    ~SoLinEditor() {

	    }
    };


    SoLin::Application* SoLin::CreateApplication() {
	    return new SoLinEditor();
    }
}
