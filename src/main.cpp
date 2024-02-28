# include <imgui.h>
# include <imgui_node_editor.h>
# include <application.h>
# include <WireMaster.h>




int main()
{
    
    WireMaster wm("Wire Master", 0, nullptr);

    if (wm.Create())
        return wm.Run();

    return 0;
}