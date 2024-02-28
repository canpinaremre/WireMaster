# include <imgui.h>
# include <imgui_node_editor.h>
# include <application.h>


namespace ed = ax::NodeEditor;

struct WireMaster : public Application
{
    void draw();


    // Struct to hold basic information about connection between
    // pins. Note that connection (aka. link) has its own ID.
    // This is useful later with dealing with selections, deletion
    // or other operations.
    struct LinkInfo
    {
        ed::LinkId Id;
        ed::PinId  InputId;
        ed::PinId  OutputId;
    };

    using Application::Application;

    void OnStart() override;
    void OnStop() override;

    void ImGuiEx_BeginColumn();

    void ImGuiEx_NextColumn();

    void ImGuiEx_EndColumn();

    void OnFrame(float deltaTime) override;

    ed::EditorContext*   m_Context = nullptr;    // Editor context, required to trace a editor state.
    bool                 m_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
    ImVector<LinkInfo>   m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
    int                  m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
};


