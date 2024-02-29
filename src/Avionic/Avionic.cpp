#include "Avionic.h"

void Avionic::AvionicBeginColumn()
{
    ImGui::BeginGroup();
}

void Avionic::AvionicNextColumn()
{
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
}

void Avionic::AvionicEndColumn()
{
    ImGui::EndGroup();
}


void Avionic::Build()
{
    if (m_FirstFrame)
    {
        m_FirstFrame = false;
        ed::SetNodePosition(ID, Position);
    }

    ed::BeginNode(ID);

    ImGui::Text(Name.c_str());
        
    AvionicBeginColumn();
    // Input
    for (auto &ins : InputPorts) 
    {
        ins.Build();
    }

    AvionicNextColumn();
    // Body
    ImGui::Text("Deneme");

    AvionicNextColumn();
    // Output
    for (auto &outs : OutputPorts) 
    {
        outs.Build();
    }

    AvionicEndColumn();
    ed::EndNode();
}

void AvionicPort::Build()
{
    ed::BeginPort(ID, Kind);

    for (auto &pin : Pins) 
    {
        pin.Build();
    }
   
    ed::EndPort();
}

void AvionicPin::Build()
{
    ed::BeginPin(ID, Kind);
    ImGui::Text(Name.c_str());
    ed::EndPin();
}

void Avionic::AddPort(AvionicPort port, ed::PortKind kind)
{
    port.setPortKind(kind);
    if(kind == ed::PortKind::Input)
    {
        InputPorts.push_back(port);
    }
    else if(kind == ed::PortKind::Output)
    {
        OutputPorts.push_back(port);
    }
}

void Avionic::AddPort(AvionicPort port)
{
    if(port.getPortKind() == ed::PortKind::Input)
    {
        InputPorts.push_back(port);
    }
    else if(port.getPortKind() == ed::PortKind::Output)
    {
        OutputPorts.push_back(port);
    }
}

void AvionicPort::AddPin(AvionicPin pin)
{
    pin.setPort(this);

    Pins.push_back(pin);
}

void AvionicPin::setPort(AvionicPort *port)
{
    Port = port->getID();
    Node = port->getNodeID();
}

void AvionicPort::setNode(Avionic *avi)
{
    Node = avi->getID();
}