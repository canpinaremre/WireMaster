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

void WireMaster::OnFrame(float deltaTime) 
{
    auto& io = ImGui::GetIO();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
    ImGui::Separator();
    ed::SetCurrentEditor(m_Context);
    // Start interaction with editor.
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));

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