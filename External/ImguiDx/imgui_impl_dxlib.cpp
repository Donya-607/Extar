// ImGui Win32 + dxlib binding
// https://github.com/ocornut/imgui


#include "imgui.h"
#define DX_USE_NAMESPACE
#include "imgui_impl_dxlib.h"

// Data
static LONGLONG    g_Time = 0;
static VERTEX2D*   g_pVB = NULL;
static ImDrawIdx*  g_pIB = NULL;
static int         g_VertexBufferSize =  5000;
static int         g_IndexBufferSize  = 10000;
static int         g_FontTexture = -1;


void ImGui_ImplDxLib_RenderDrawLists(ImDrawData* draw_data)
{
    // Avoid rendering when minimized
    ImGuiIO& io = ImGui::GetIO();
    if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f)
    {
        return;
    }

    // Create and grow buffers if needed
    if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount)
    {
        if (g_pVB) { DxLib::DxFree(g_pVB); g_pVB = NULL; }
        g_VertexBufferSize = draw_data->TotalVtxCount + 5000;
        g_pVB = (VERTEX2D*)DxLib::DxAlloc(g_VertexBufferSize * sizeof(VERTEX2D));
        if (!g_pVB) return;
    }
    if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount)
    {
        if (g_pIB) { DxLib::DxFree(g_pIB); g_pIB = NULL; }
        g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
        g_pIB = (ImDrawIdx*)DxLib::DxAlloc(g_IndexBufferSize * sizeof(ImDrawIdx));
        if (!g_pIB) return;
    }

    DxLib::VERTEX2D* vtx_dst = g_pVB;
    ImDrawIdx* idx_dst = g_pIB;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_src = &cmd_list->VtxBuffer[0];
        for (int i = 0; i < cmd_list->VtxBuffer.size(); i++)
        {
            vtx_dst->pos.x = vtx_src->pos.x;
            vtx_dst->pos.y = vtx_src->pos.y;
            vtx_dst->pos.z = 0.0f;
            vtx_dst->rhw = 1.0f;

            vtx_dst->dif.a = (BYTE)((vtx_src->col & 0xFF000000) >> 24);
            vtx_dst->dif.b = (BYTE)((vtx_src->col & 0x00FF0000) >> 16);
            vtx_dst->dif.g = (BYTE)((vtx_src->col & 0x0000FF00) >>  8);
            vtx_dst->dif.r = (BYTE)( vtx_src->col & 0x000000FF);

            vtx_dst->u = vtx_src->uv.x;
            vtx_dst->v = vtx_src->uv.y;

            vtx_dst++;
            vtx_src++;
        }
        memcpy(idx_dst, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx));
        idx_dst += cmd_list->IdxBuffer.size();
    }


    RECT rect;
    //保存
    DxLib::GetDrawArea(&rect) ;

    // Render command lists
    int vtx_offset = 0;
    int idx_offset = 0;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                DxLib::SetDrawArea((int)pcmd->ClipRect.x, (int)pcmd->ClipRect.y,
                                   (int)pcmd->ClipRect.z, (int)pcmd->ClipRect.w);
                int tex = (int)pcmd->TextureId - 1; //使うときは-１する
                DxLib::DrawPrimitiveIndexed2D(
                            g_pVB + vtx_offset, cmd_list->VtxBuffer.size(),
                            g_pIB + idx_offset, pcmd->ElemCount,
                            DX_PRIMTYPE_TRIANGLELIST, tex, TRUE) ;
            }
            idx_offset += pcmd->ElemCount;
        }
        vtx_offset += cmd_list->VtxBuffer.size();
    }

    //復元
    DxLib::SetDrawArea(rect.left, rect.top, rect.right, rect.bottom);
}


bool ImGui_ImplDxLib_CreateFontsTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height, bytes_per_pixel;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height, &bytes_per_pixel);

    g_FontTexture = -1;

    // ソフトウエアで扱うイメージを作成
    int simg = DxLib::MakeARGB8ColorSoftImage(width, height);
    if (simg == -1) {
        return false;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            DxLib::DrawPixelSoftImage(simg, x, y, 255, 255, 255, pixels[width * y + x]);
        }
    }

    // グラフィックハンドルを作成
    int tex = DxLib::CreateGraphFromSoftImage(simg) ;
    if (tex == -1) {
        return false;
    }

    // 使い終わったら解放
    DxLib::DeleteSoftImage(simg) ;

    // Store our identifier
    io.Fonts->TexID = (void *)(tex + 1);//注意 0をnullにするため+1する
    g_FontTexture = tex;

    return true;
}



bool ImGui_ImplDxLib_Init()
{

    ImGuiIO& io = ImGui::GetIO();

    io.KeyMap[ImGuiKey_Tab] = VK_TAB;                              // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
    io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
    io.KeyMap[ImGuiKey_Home] = VK_HOME;
    io.KeyMap[ImGuiKey_End] = VK_END;
    io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    io.KeyMap[ImGuiKey_A] = 'A';
    io.KeyMap[ImGuiKey_C] = 'C';
    io.KeyMap[ImGuiKey_V] = 'V';
    io.KeyMap[ImGuiKey_X] = 'X';
    io.KeyMap[ImGuiKey_Y] = 'Y';
    io.KeyMap[ImGuiKey_Z] = 'Z';

    io.RenderDrawListsFn = ImGui_ImplDxLib_RenderDrawLists;
    io.ImeWindowHandle = DxLib::GetMainWindowHandle();

    return true;
}

void ImGui_ImplDxLib_Shutdown()
{
    ImGui_ImplDxLib_InvalidateDeviceObjects();

    ImGui::Shutdown();
}

bool ImGui_ImplDxLib_CreateDeviceObjects()
{
    if (!ImGui_ImplDxLib_CreateFontsTexture())
    {
        return false;
    }
    return true;
}

void ImGui_ImplDxLib_InvalidateDeviceObjects()
{
    if (g_pVB) { DxLib::DxFree(g_pVB); g_pVB = NULL; }
    if (g_pIB) { DxLib::DxFree(g_pIB); g_pIB = NULL; }

    if (int tex = (int)ImGui::GetIO().Fonts->TexID)
    {
        DxLib::DeleteGraph(tex - 1) ;
        ImGui::GetIO().Fonts->TexID = 0;
    }
    g_FontTexture = -1;
}



void ImGui_ImplDxLib_NewFrame()
{
    if (g_FontTexture == -1) {
        ImGui_ImplDxLib_CreateDeviceObjects();
    }

    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    RECT rect;
    DxLib::GetWindowCRect(&rect);
    io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

    static LONGLONG Time = 0;
    if (Time == 0) {
        Time = DxLib::GetNowHiPerformanceCount();
        io.DeltaTime = 1.0f / 60.0f;//in seconds
    }
    else {
        // Setup time step
        LONGLONG  current_time = DxLib::GetNowHiPerformanceCount();//マイクロ秒
        io.DeltaTime = (float)(current_time - Time) / 1000000.0f;//in seconds
        Time = current_time;
    }

    // Read keyboard modifiers inputs
    io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    // io.KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
    // io.MousePos : filled by WM_MOUSEMOVE events
    // io.MouseDown : filled by WM_*BUTTON* events
    // io.MouseWheel : filled by WM_MOUSEWHEEL events

    // Hide OS mouse cursor if ImGui is drawing it
    SetCursor(io.MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));

    // Start the frame
    ImGui::NewFrame();
}

LRESULT ImGui_ImplDxLib_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (msg)
    {
    case WM_LBUTTONDOWN:
        io.MouseDown[0] = true;
        return true;
    case WM_LBUTTONUP:
        io.MouseDown[0] = false;
        return true;
    case WM_RBUTTONDOWN:
        io.MouseDown[1] = true;
        return true;
    case WM_RBUTTONUP:
        io.MouseDown[1] = false;
        return true;
    case WM_MBUTTONDOWN:
        io.MouseDown[2] = true;
        return true;
    case WM_MBUTTONUP:
        io.MouseDown[2] = false;
        return true;
    case WM_MOUSEWHEEL:
        io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
        return true;
    case WM_MOUSEMOVE:
        io.MousePos.x = (signed short)(lParam);
        io.MousePos.y = (signed short)(lParam >> 16);
        return true;
    case WM_KEYDOWN:
        if (wParam < 256)
            io.KeysDown[wParam] = 1;
        return true;
    case WM_KEYUP:
        if (wParam < 256)
            io.KeysDown[wParam] = 0;
        return true;
    case WM_CHAR:
        // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
        if (wParam > 0 && wParam < 0x10000)
            io.AddInputCharacter((unsigned short)wParam);
        return true;
    }
    return 0;
}





