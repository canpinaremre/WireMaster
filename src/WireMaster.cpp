#include "WireMaster.h"


void WireMaster::draw()
{

    
}


void WireMaster::OnStart()
{
    ed::Config config;
    config.SettingsFile = "BasicInteraction.json";
    m_Context = ed::CreateEditor(&config);
}

void WireMaster::OnStop() 
{
    ed::DestroyEditor(m_Context);
}

void WireMaster::ImGuiEx_BeginColumn()
{
    ImGui::BeginGroup();
}

void WireMaster::ImGuiEx_NextColumn()
{
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
}

void WireMaster::ImGuiEx_EndColumn()
{
    ImGui::EndGroup();
}

void WireMaster::OnFrame(float deltaTime) 
{
    auto& io = ImGui::GetIO();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
    ImGui::Separator();
    ed::SetCurrentEditor(m_Context);
    // Start interaction with editor.
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));
    int uniqueId = 1;
    //
    // 1) Commit known data to editor
    //
    // Submit Node A
    ed::NodeId nodeA_Id = uniqueId++;
    ed::PinId  nodeA_InputPinId = uniqueId++;
    ed::PinId  nodeA_OutputPinId = uniqueId++;
    ed::PinId  nodeA_InputPinId2 = uniqueId++;
    if (m_FirstFrame)
        ed::SetNodePosition(nodeA_Id, ImVec2(0, 0));
    ed::BeginNode(nodeA_Id);
        ImGui::Text("Node A");
        ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input);
            ImGui::Text("-> In");
        ed::EndPin();
        ImGui::SameLine();
        ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output);
            ImGui::Text("Out ->");
        ed::EndPin();
        ed::BeginPin(nodeA_InputPinId2, ed::PinKind::Input);
            ImGui::Text("-> In2");
        ed::EndPin();
    ed::EndNode();
    // Submit Node B
    ed::NodeId nodeB_Id = uniqueId++;
    ed::PortId portId = uniqueId++;
    ed::PortId portId2 = uniqueId++;
    ed::PinId  nodeB_InputPinId1 = uniqueId++;
    ed::PinId  nodeB_InputPinId2 = uniqueId++;
    ed::PinId  nodeB_InputPinId3 = uniqueId++;
    ed::PinId  nodeB_InputPinId4 = uniqueId++;
    ed::PinId  nodeB_InputPinId5 = uniqueId++;
    ed::PinId  nodeB_InputPinId6 = uniqueId++;
    ed::PinId  nodeB_InputPinId7 = uniqueId++;
    ed::PinId  nodeB_OutputPinId = uniqueId++;
    if (m_FirstFrame)
        ed::SetNodePosition(nodeB_Id, ImVec2(100, 100));
    ed::BeginNode(nodeB_Id);
        ImGui::Text("Node B");
        ImGuiEx_BeginColumn();
            
            ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
            ImGui::Text("-> In1");
            ed::EndPin();
            ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
            ImGui::Text("-> In2");
            ed::EndPin();
            ed::BeginPort(portId2, ed::PortKind::Input);
            ed::BeginPin(nodeB_InputPinId3, ed::PinKind::Input);
            ImGui::Text("-> In3");
            ed::EndPin();
            ed::BeginPin(nodeB_InputPinId4, ed::PinKind::Input);
            ImGui::Text("-> In4");
            ed::EndPin();
            ed::EndPort();
            ed::BeginPort(portId, ed::PortKind::Input);
            ed::BeginPin(nodeB_InputPinId5, ed::PinKind::Input);
            ImGui::Text("-> In5");
            ed::EndPin();
            ed::BeginPin(nodeB_InputPinId6, ed::PinKind::Input);
            ImGui::Text("-> In6");
            ed::EndPin();
            ed::BeginPin(nodeB_InputPinId7, ed::PinKind::Input);
            ImGui::Text("-> In7");
            ed::EndPin();
            ed::EndPort();
            ImGuiEx_NextColumn();
            ImGui::Text("Deneme");

            ImGuiEx_NextColumn();
            ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();
            
        ImGuiEx_EndColumn();
    ed::EndNode();
    // Submit Links
    for (auto& linkInfo : m_Links)
        ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
    //
    // 2) Handle interactions
    //
    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ed::BeginCreate())
    {
        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId))
        {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated
            if (inputPinId && outputPinId) // both are valid, let's accept link
            {
                // ed::AcceptNewItem() return true when user release mouse button.
                if (ed::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });
                    // Draw new link.
                    ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
                }
                // You may choose to reject connection between these nodes
                // by calling ed::RejectNewItem(). This will allow editor to give
                // visual feedback by changing link thickness and color.
            }
        }
    }
    ed::EndCreate(); // Wraps up object creation action handling.
    // Handle deletion action
    if (ed::BeginDelete())
    {
        // There may be many links marked for deletion, let's loop over them.
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ed::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& link : m_Links)
                {
                    if (link.Id == deletedLinkId)
                    {
                        m_Links.erase(&link);
                        break;
                    }
                }
            }
            // You may reject link deletion by calling:
            // ed::RejectDeletedItem();
        }
    }
    ed::EndDelete(); // Wrap up deletion action
    // End of interaction with editor.
    ed::End();
    if (m_FirstFrame)
        ed::NavigateToContent(0.0f);
    ed::SetCurrentEditor(nullptr);
    m_FirstFrame = false;
    // ImGui::ShowMetricsWindow();
}
