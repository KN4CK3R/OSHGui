#include <windows.h>
#include <process.h>
#include <d3dx9.h>
#undef MessageBox
#undef DrawText
//---------------------------------------------------------------------------
// the basic includes for OSHGui
#include <OSHGui.hpp>
#include "Input/WindowsMessage.hpp"
#include "Drawing/Direct3D9/Direct3D9Renderer.hpp"
//---------------------------------------------------------------------------
// optional: make the namespace available
using namespace OSHGui;
using namespace OSHGui::Drawing;
//---------------------------------------------------------------------------
Input::WindowsMessage input;
//---------------------------------------------------------------------------
typedef long(__stdcall * oEndScene)(IDirect3DDevice9* device);
oEndScene pEndScene = nullptr;
typedef long(__stdcall * oReset)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parms);
oReset pReset = nullptr;
WNDPROC oldWndProc;
//---------------------------------------------------------------------------
void* HookVirtualMethod(void* instance, const int methodIndex, void* targetFunctionPtr)
{
	const auto address = *reinterpret_cast<uintptr_t*>(instance) + methodIndex * sizeof(void*);

	DWORD protection;
	if (VirtualProtect(reinterpret_cast<void*>(address), sizeof(void*), PAGE_EXECUTE_READWRITE, &protection))
	{
		const auto originalFunctionPtr = *reinterpret_cast<void**>(address);

		*reinterpret_cast<void**>(address) = targetFunctionPtr;

		VirtualProtect(reinterpret_cast<void*>(address), sizeof(void*), protection, &protection);

		return originalFunctionPtr;
	}

	return nullptr;
}
//---------------------------------------------------------------------------
void InitializeOSHGui(LPDIRECT3DDEVICE9 device)
{
	// 1. create our renderer, Direct3D9 in this case
	auto renderer = std::make_unique<Direct3D9Renderer>(device);
	
	// 2. and initialize the OSHGui with it
	Application::Initialize(std::move(renderer));

	// or 1.+2.
	//Application::Initialize(std::make_unique<Direct3D9Renderer>(device));

	// now we have a valid OSHGui instance, so lets grab it
	auto &app = Application::Instance();

	// 3. create a font which will be used by the OSHGui
	const auto font = FontManager::LoadFont("Arial", 8.0f, false); // Arial, 8PT, no anti-aliasing
	app.SetDefaultFont(font);

	// 4. create our form
	auto form = std::make_shared<Form>();
	form->SetText("Test");

	// 5. set this form as our mainform
	app.Run(form);

	// optional: enable the OSHGui drawing
	app.Enable();

	// optional: register a Hotkey with which we can toggle the OSHGui drawing
	app.RegisterHotkey(Hotkey(Key::Insert, []
	{
		Application::Instance().Toggle();
	}));
}
//---------------------------------------------------------------------------
long __stdcall EndScene_Hook(IDirect3DDevice9* device)
{
	// 0. Initialize the gui once
	static bool once = true;
	if (once)
	{
		once = false;

		InitializeOSHGui(device);
	}

	auto &renderer = Application::Instance().GetRenderer();

	// 1. let our renderer begin its work
	renderer.BeginRendering();

	// 2. render the OSHGui
	Application::Instance().Render();

	// 3. end the rendering
	renderer.EndRendering();

	return pEndScene(device);
}
//---------------------------------------------------------------------------
long __stdcall Reset_Hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parms)
{
	auto& renderer = static_cast<Direct3D9Renderer&>(Application::Instance().GetRenderer());

	// 1. Tell the renderer a reset is happening
	renderer.PreD3DReset();

	const auto result = pReset(device, parms);
	if (SUCCEEDED(result))
	{
		// 2. Tell the renderer the reset was successfull
		renderer.PostD3DReset();
	}

	return result;
}
//---------------------------------------------------------------------------
LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	MSG message;
	message.message = msg;
	message.wParam = wparam;
	message.lParam = lparam;
	if (input.ProcessMessage(&message))
	{
		return 0;
	}

	return CallWindowProcA(oldWndProc, hwnd, msg, wparam, lparam);
}
//---------------------------------------------------------------------------
void InitializeThread(void*)
{
	// TODO Get the HWND of the main window
	const auto hwnd = FindWindowA(...);
	// TODO Get the IDirect3DDevice9 the target application uses
	const auto d3d9Device = reinterpret_cast<void*>(0x...);

	pEndScene = reinterpret_cast<oEndScene>(HookVirtualMethod(d3d9Device, 42, EndScene_Hook));
	pReset = reinterpret_cast<oReset>(HookVirtualMethod(d3d9Device, 16, Reset_Hook));

	oldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG>(NewWndProc)));
}
//---------------------------------------------------------------------------
BOOL __stdcall DllMain(const HMODULE self, const uint32_t reason_for_call, void *reserved)
{
	if (reason_for_call == DLL_PROCESS_ATTACH)
	{
		_beginthread(InitializeThread, 0, nullptr);

		return TRUE;
	}

	return FALSE;
}
//---------------------------------------------------------------------------