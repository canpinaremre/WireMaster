#define IMGUI_DEFINE_MATH_OPERATORS
#include "WireMaster.h"



void WireMaster::OnStart()
{
    ed::Config config;
    config.SettingsFile = "BasicInteraction.json";
    m_Context = ed::CreateEditor(&config);
    initAvionics();
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
bool WireMaster::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size )
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

void WireMaster::OnFrame(float deltaTime) 
{
    


    auto& io = ImGui::GetIO();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
    ImGui::Separator();
    ed::SetCurrentEditor(m_Context);
    
    static float leftPaneWidth  = 400.0f;
    static float rightPaneWidth = 800.0f;
    Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
    ShowLeftPane(leftPaneWidth - 4.0f);
    ImGui::SameLine(0.0f, 12.0f);

    // Handle key and short cut actions
    HandleKeyActions();


    // Start interaction with editor.
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));

    // Draw avionics
    for (auto &avs : AvionicList) 
    {
        avs.Build();
    }

    // Submit Links
    for (auto& linkInfo : m_Links)
        ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
    //
    // 2) Handle interactions
    //
    // Handle creation action, returns true if editor want to create new object (node or link)
    HandleNodeActions();

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

void WireMaster::initAvionics()
{
    // DEBUG TEST SETUP
    // TODO init from file

    //------------------------------------------------
    Avionic Avionic1(uniqId++, "Avionic1");
        AvionicPort port1(uniqId++,"port1");
            AvionicPin pin1(uniqId++,"pin1",PinType::Digital);
            AvionicPin pin2(uniqId++,"pin2",PinType::PowerPos);
                port1.AddPin(pin1);
                port1.AddPin(pin2);
        AvionicPort port2(uniqId++,"port2",ed::PortKind::Input);
            AvionicPin pin3(uniqId++,"pin3",PinType::PowerNeg);
                port2.AddPin(pin3);
        Avionic1.AddPort(port1);
        Avionic1.AddPort(port2);
    //------------------------------------------------
    Avionic Avionic2(uniqId++, "Avionic2", ImVec2(100, 100));
        AvionicPort port3(uniqId++,"port3",ed::PortKind::Input);
            AvionicPin pin4(uniqId++,"pin4",PinType::Digital);
            AvionicPin pin5(uniqId++,"pin5",PinType::GND);
                port3.AddPin(pin4);
                port3.AddPin(pin5);
        AvionicPort port4(uniqId++,"port4");
            AvionicPin pin6(uniqId++,"pin6",PinType::Digital);
            AvionicPin pin7(uniqId++,"pin7",PinType::Digital);
            AvionicPin pin8(uniqId++,"pin8",PinType::Digital);
                port4.AddPin(pin6);
                port4.AddPin(pin7);
                port4.AddPin(pin8);
        Avionic2.AddPort(port3);
        Avionic2.AddPort(port4);
    //------------------------------------------------
    Avionic Avionic3(uniqId++, "Avionic3", ImVec2(200, 500));
        AvionicPort port5(uniqId++,"port5");
            AvionicPin pin9(uniqId++,"pin9",PinType::Digital);
            AvionicPin pin10(uniqId++,"pin10",PinType::Digital);
            AvionicPin pin11(uniqId++,"pin11",PinType::Digital);
                port5.AddPin(pin9);
                port5.AddPin(pin10);
                port5.AddPin(pin11);
        Avionic3.AddPort(port5);
    //------------------------------------------------











    AvionicList.push_back(Avionic1);
    AvionicList.push_back(Avionic2);
    AvionicList.push_back(Avionic3);
}


void WireMaster::ShowLeftPane(float paneWidth)
{
    auto& io = ImGui::GetIO();
    ImGui::BeginChild("Control Pannel", ImVec2(paneWidth, 0));
    paneWidth = ImGui::GetContentRegionAvail().x;
    static bool showStyleEditor = false;

    //------------------------------------------------------------
    ImGui::BeginHorizontal("Style Editor", ImVec2(paneWidth, 0));

    ImGui::Spring(0.0f, 0.0f);
    if (ImGui::Button("Show Diagram"))
        ed::NavigateToContent();
    ImGui::Spring(0.0f);
    if (ImGui::Button("Highlight Flow"))
    {
        // // TODO highlight only selected link
        // for (auto& link : m_Links)
        //     ed::Flow(link.ID);
    }

    ImGui::Spring();

    // TODO excel/XML export
    if (ImGui::Button("Configuration"))
        showStyleEditor = true;

    ImGui::EndHorizontal();
    //------------------------------------------------------------
    // TODO add to menubar
    // if (showStyleEditor)
    //     ShowStyleEditor(&showStyleEditor);


    std::vector<ed::NodeId> selectedNodes;
    std::vector<ed::LinkId> selectedLinks;
    selectedNodes.resize(ed::GetSelectedObjectCount());
    selectedLinks.resize(ed::GetSelectedObjectCount());
    int nodeCount = ed::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
    int linkCount = ed::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));
    selectedNodes.resize(nodeCount);
    selectedLinks.resize(linkCount);
    // int saveIconWidth     = GetTextureWidth(m_SaveIcon);
    // int saveIconHeight    = GetTextureWidth(m_SaveIcon);
    // int restoreIconWidth  = GetTextureWidth(m_RestoreIcon);
    // int restoreIconHeight = GetTextureWidth(m_RestoreIcon);

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetCursorScreenPos(),
        ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
        ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
    ImGui::Spacing(); ImGui::SameLine();
    ImGui::TextUnformatted("Avionics");
    ImGui::Indent();
    for (auto& avionic : AvionicList)
    {
        ImGui::PushID(avionic.getID().AsPointer());

        bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), avionic.getID()) != selectedNodes.end();
        ImGui::SetNextItemAllowOverlap();
        if (ImGui::Selectable((avionic.getName() + "##" + std::to_string(reinterpret_cast<uintptr_t>(avionic.getID().AsPointer()))).c_str(), &isSelected))
        {
            if (io.KeyCtrl)
            {
                if (isSelected)
                    ed::SelectNode(avionic.getID(), true);
                else
                    ed::DeselectNode(avionic.getID());
            }
            else
                ed::SelectNode(avionic.getID(), false);
            ed::NavigateToSelection();
        }

        ImGui::PopID();
    }
    
    ImGui::Unindent();

    ImGui::GetWindowDrawList()->AddRectFilled(
    ImGui::GetCursorScreenPos(),
    ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
    ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
    ImGui::Spacing(); ImGui::SameLine();

    ImGui::TextUnformatted("Selected");
    ImGui::Indent();
    // TODO isim yaz
    for (int i = 0; i < nodeCount; ++i) ImGui::Text("Birim (%p)", selectedNodes[i].AsPointer());
    for (int i = 0; i < linkCount; ++i) ImGui::Text("Bağlantı (%p)", selectedLinks[i].AsPointer());
    ImGui::Unindent();
    // if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
    //     for (auto& link : m_Links)
    //         ed::Flow(link.ID);
    

    ImGui::EndChild();
}