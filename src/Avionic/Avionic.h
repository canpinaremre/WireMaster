#ifndef __AVIONIC_H__
#define __AVIONIC_H__

# include <imgui.h>
# include <imgui_node_editor.h>
# include <application.h>
#include <vector>

namespace ed = ax::NodeEditor;

class Avionic;
class AvionicPin;
class AvionicPort;
class AvionicLink;


enum class PinType
{
    Digital,
    PowerPos,
    PowerNeg,
    RF,
    GND,
    RS485A,
    RS485B,
    RS422RXPos,
    RS422TXPos,
    RS422RXNeg,
    RS422TXNeg,
    Undefined,

    PinTypeCount
};

class AvionicPin
{
public:
    void Build();

    AvionicPin(int id, const char* name, int port, int node, PinType type):
        ID(id), Node(node), Port(port), Name(name), Type(type), Kind(ed::PinKind::Output)
    {
    }
    AvionicPin(int id, const char* name, PinType type):
        ID(id), Name(name), Type(type), Kind(ed::PinKind::Output)
    {
    }

    ed::PinId getID() { return ID; }
    ed::PortId getPortID() { return Port; }
    ed::NodeId getNodeID() { return Node; }
    ed::PinKind getPinKind() { return Kind; }
    
    void setPort(ed::PortId portID)
    {
        Port = portID;
    }

    void setNode(ed::NodeId nodeID)
    {
        Node = nodeID;
    }

    void setPort(AvionicPort *port);

    void setPinKind(ed::PinKind kind)
    {
        Kind = kind;
    }

private:
    PinType Type;
    std::string Name;
    ed::PinKind Kind;
    ed::PinId   ID;
    ed::PortId  Port;
    ed::NodeId  Node;
};

class AvionicPort
{

public:
    AvionicPort(int id, const char* name, ed::PortKind kind = ed::PortKind::Output, ImColor color = ImColor(255, 255, 255)):
        ID(id), Name(name), Color(color), Kind(kind)
    {
    }
    void Build();
    void AddPin(AvionicPin pin);

    void setNode(ed::NodeId nodeID)
    {
        Node = nodeID;
    }

    void setNode(Avionic *avi);

    void setPortKind(ed::PortKind kind)
    {
        Kind = kind;
    }

    ed::PortKind getPortKind()
    {
        return Kind;
    }

    ed::PortId getID() { return ID; }
    ed::NodeId getNodeID() { return Node; }

private:
    std::string Name;
    ed::PortId ID;
    ed::NodeId Node;
    // TODO type?
    ed::PortKind Kind;
    std::vector<AvionicPin> Pins;
    ImColor Color;
};

class Avionic
{
public:

    Avionic(int id, const char* name, ImVec2 position = ImVec2(0.0f, 0.0f), ImColor color = ImColor(255, 255, 255)):
        ID(id), Name(name), Position(position), Color(color)
    {
    }

    void Build();
    void AddPort(AvionicPort port, ed::PortKind kind);
    void AddPort(AvionicPort port);
    ed::NodeId getID() { return ID; }
    std::string getName() { return Name; }

    void setBodyText(std::string text) { bodyText = text; }
    std::string getBodyText() { return bodyText; }

private:
    std::string Name;
    ed::NodeId ID;
    // TODO type?
    std::vector<AvionicPort> InputPorts;
    std::vector<AvionicPort> OutputPorts;
    std::string bodyText;
    ImColor Color;
    ImVec2 Position;
    bool m_FirstFrame = true;

    void AvionicBeginColumn();
    void AvionicNextColumn();
    void AvionicEndColumn();



    /*
    ImTextureID HeaderTextureId;
    int         HeaderTextureWidth;
    int         HeaderTextureHeight;
    ImU32       HeaderColor;
    ImVec2      NodeMin;
    ImVec2      NodeMax;
    ImVec2      HeaderMin;
    ImVec2      HeaderMax;
    ImVec2      ContentMin;
    ImVec2      ContentMax;
    bool        HasHeader;
    */


};

class Wire
{

private:
    ed::LinkId ID;
    ed::PinId StartPinID;
    ed::PinId EndPinID;

    ImColor Color;
};

class TwistPairs
{
private:
    std::vector<ed::LinkId> TwistedWires;
};

class ShieldPairs
{
private:
    std::vector<ed::LinkId> ShieldedWires;
};

class AvionicBus
{

private:
    std::string Name;
    ed::LinkId ID;

    std::vector<Wire> Wires;
    std::vector<ed::PortId> ConnectedPorts;

    std::vector<ShieldPairs> BusShield;
    std::vector<TwistPairs> BusTwist;


public:
    ed::LinkId getID() { return ID; }
    std::string getName() { return Name; }

};




#endif // __AVIONIC_H__