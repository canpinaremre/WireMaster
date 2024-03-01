#include <Actions.h>


void HandleActions()
{
    // Handle Space Key
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)))
    {
        ed::NavigateToContent();
    }


    

}
