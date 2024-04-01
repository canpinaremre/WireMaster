#include <Actions.h>

extern std::vector<Avionic> AvionicList;
extern int uniqId;

void HandleKeyActions()
{
    // Handle Space Key
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)))
    {
        ed::NavigateToContent();
    }

}

void HandleNodeActions()
{

    static ed::NodeId contextNodeId      = 0;
    static ed::LinkId contextLinkId      = 0;
    static ed::PinId  contextPinId       = 0;
    static ed::PortId contextPortId     = 0;


    auto openPopupPosition = ImGui::GetMousePos();
    static auto newNodePostion = openPopupPosition;

    ed::Suspend();
    if (ed::ShowNodeContextMenu(&contextNodeId))
        ImGui::OpenPopup("POP_NodeContextMenu");
    else if (ed::ShowPinContextMenu(&contextPinId))
        ImGui::OpenPopup("POP_PinContextMenu");
    else if (ed::ShowLinkContextMenu(&contextLinkId))
        ImGui::OpenPopup("POP_LinkContextMenu");
    else if (ed::ShowPortContextMenu(&contextPortId))
        ImGui::OpenPopup("POP_PortContextMenu");
    else if (ed::ShowBackgroundContextMenu())
    {
        ImGui::OpenPopup("POP_BackgroundContextMenu");
        newNodePostion = openPopupPosition;
    }
        

    ed::Resume();


    ed::Suspend();
    if (ImGui::BeginPopup("POP_BackgroundContextMenu"))
    {
        if (ImGui::MenuItem("BackgroundContextMenu"))
        {
            
        }
        if (ImGui::MenuItem("Add New Empty Avionic"))
        {
            Avionic av(uniqId++, "Empty Avionic", newNodePostion);
            AvionicList.push_back(av);
        }
        if (ImGui::MenuItem("Add Avionic From Library"))
        {
            // TODO add from library
        }
        ImGui::EndPopup();
    }
    ed::Resume();

    if (ImGui::BeginPopup("POP_PortContextMenu"))
    {
        if (ImGui::MenuItem("PortContextMenu"))
        {

        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("POP_LinkContextMenu"))
    {
        if (ImGui::MenuItem("LinkContextMenu"))
        {

        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("POP_NodeContextMenu"))
    {

        if (ImGui::MenuItem("NodeContextMenu"))
        {

        }
        if (ImGui::MenuItem("Add New Empty Port"))
        {
            
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("POP_PinContextMenu"))
    {
        if (ImGui::MenuItem("PinContextMenu"))
        {

        }
        ImGui::EndPopup();
    }

}