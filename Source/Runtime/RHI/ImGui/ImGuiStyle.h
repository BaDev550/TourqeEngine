#pragma once
#include <imgui.h>

namespace Runtime::RHI {
    enum class Accent { Source, Blue, Violet, Teal, Orange, Rose };

    inline ImVec4 Hex(unsigned int rgb, float a = 1.f) {
        return ImVec4(((rgb >> 16) & 0xFF) / 255.f, ((rgb >> 8) & 0xFF) / 255.f, (rgb & 0xFF) / 255.f, a);
    }

    inline ImVec4 AccentColor(Accent a) {
        switch (a)
        {
        case Accent::Source: return Hex(0xC4B550);
        case Accent::Violet: return Hex(0x8B7CF6);
        case Accent::Teal:   return Hex(0x2DD4BF);
        case Accent::Orange: return Hex(0xFB923C);
        case Accent::Rose:   return Hex(0xFB7185);
        default:             return Hex(0x4C8DFF);

        }
    }

    inline ImVec4 Mul(ImVec4 c, float k, float a = -1.f) {
        return ImVec4(c.x * k, c.y * k, c.z * k, a < 0 ? c.w : a);
    }

    inline void ApplyStyle(ImVec4 accent)
    {
        ImGuiStyle& s = ImGui::GetStyle();

        s.WindowRounding = 0.f;
        s.ChildRounding = 0.f;
        s.FrameRounding = 0.f;
        s.PopupRounding = 0.f;
        s.ScrollbarRounding = 0.f;
        s.GrabRounding = 0.f;
        s.TabRounding = 0.f;

        s.WindowBorderSize = 1.f;
        s.ChildBorderSize = 1.f;
        s.PopupBorderSize = 1.f;
        s.FrameBorderSize = 1.f;
        s.TabBorderSize = 0.f;
        s.TabBarBorderSize = 1.f;

        s.WindowPadding = ImVec2(8, 8);
        s.FramePadding = ImVec2(8, 4);
        s.CellPadding = ImVec2(6, 3);
        s.ItemSpacing = ImVec2(8, 5);
        s.ItemInnerSpacing = ImVec2(5, 4);
        s.IndentSpacing = 16.f;
        s.ScrollbarSize = 10.f;
        s.GrabMinSize = 9.f;
        s.WindowTitleAlign = ImVec2(0.0f, 0.5f);
        s.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        s.SeparatorTextBorderSize = 1.f;
        s.SeparatorTextPadding = ImVec2(10, 4);
        s.WindowMenuButtonPosition = ImGuiDir_None;
        s.AntiAliasedLines = true;
        s.AntiAliasedFill = true;

        const ImVec4 bg0 = Hex(0x050505);
        const ImVec4 bg1 = Hex(0x0F0F0F);
        const ImVec4 bg2 = Hex(0x161616);
        const ImVec4 bg3 = Hex(0x0A0A0A);
        const ImVec4 bg4 = Hex(0x232323);
        const ImVec4 bg5 = Hex(0x303030);
        const ImVec4 line = Hex(0x4F4F4F);
        const ImVec4 text = Hex(0xBFBFBF);
        const ImVec4 muted = Hex(0x6A6A6A);

        const ImVec4 acc = accent;
        const ImVec4 accHover = Mul(accent, 1.12f);
        const ImVec4 accActive = Mul(accent, 0.88f);
        const ImVec4 accSoft = Mul(accent, 1.f, 0.22f); 
        const ImVec4 accSoft2 = Mul(accent, 1.f, 0.38f);

        ImVec4* c = s.Colors;
        c[ImGuiCol_Text] = text;
        c[ImGuiCol_TextDisabled] = muted;
        c[ImGuiCol_WindowBg] = bg1;
        c[ImGuiCol_ChildBg] = bg2;
        c[ImGuiCol_PopupBg] = Hex(0x101010, 0.99f);
        c[ImGuiCol_Border] = line;
        c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

        c[ImGuiCol_FrameBg] = bg3;
        c[ImGuiCol_FrameBgHovered] = bg4;
        c[ImGuiCol_FrameBgActive] = bg5;

        c[ImGuiCol_TitleBg] = bg1;
        c[ImGuiCol_TitleBgActive] = bg1;
        c[ImGuiCol_TitleBgCollapsed] = bg1;
        c[ImGuiCol_MenuBarBg] = bg2;

        c[ImGuiCol_ScrollbarBg] = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_ScrollbarGrab] = bg5;
        c[ImGuiCol_ScrollbarGrabHovered] = Hex(0x4A4A4A);
        c[ImGuiCol_ScrollbarGrabActive] = acc;

        c[ImGuiCol_CheckMark] = acc;
        c[ImGuiCol_SliderGrab] = acc;
        c[ImGuiCol_SliderGrabActive] = accHover;

        c[ImGuiCol_Button] = bg3;
        c[ImGuiCol_ButtonHovered] = bg4;
        c[ImGuiCol_ButtonActive] = bg5;

        c[ImGuiCol_Header] = Hex(0x2E2E2E);
        c[ImGuiCol_HeaderHovered] = Hex(0x242424);
        c[ImGuiCol_HeaderActive] = Hex(0x3A3A3A);

        c[ImGuiCol_Separator] = line;
        c[ImGuiCol_SeparatorHovered] = acc;
        c[ImGuiCol_SeparatorActive] = accHover;

        c[ImGuiCol_ResizeGrip] = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_ResizeGripHovered] = accSoft2;
        c[ImGuiCol_ResizeGripActive] = acc;

        c[ImGuiCol_Tab] = bg1;
        c[ImGuiCol_TabHovered] = bg4;
        c[ImGuiCol_TabSelected] = bg2;
        c[ImGuiCol_TabSelectedOverline] = acc;
        c[ImGuiCol_TabDimmed] = bg1;
        c[ImGuiCol_TabDimmedSelected] = bg2;
        c[ImGuiCol_TabDimmedSelectedOverline] = muted;

        c[ImGuiCol_PlotLines] = acc;
        c[ImGuiCol_PlotLinesHovered] = accHover;
        c[ImGuiCol_PlotHistogram] = acc;
        c[ImGuiCol_PlotHistogramHovered] = accHover;

        c[ImGuiCol_TableHeaderBg] = bg2;
        c[ImGuiCol_TableBorderStrong] = line;
        c[ImGuiCol_TableBorderLight] = Mul(line, 0.75f);
        c[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_TableRowBgAlt] = ImVec4(1, 1, 1, 0.025f);

        c[ImGuiCol_TextSelectedBg] = accSoft2;
        c[ImGuiCol_DragDropTarget] = acc;
        c[ImGuiCol_NavHighlight] = acc;
        c[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 1, 1, 0.6f);
        c[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.5f);
        c[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.6f);
    }

    inline void ApplyStyle(Accent a = Accent::Blue) { ApplyStyle(AccentColor(a)); }
}