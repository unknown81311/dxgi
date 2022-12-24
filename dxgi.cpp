#include <Windows.h>
#include <stdio.h>

HINSTANCE mHinst = 0, mHinstDLL = 0, mHinst_dxgi_DLL = 0;
extern "C" UINT_PTR mProcs[18] = {0};

void load_d3d9_dll();
void load_dxgi_dll();

LPCSTR mImportNames[] = {"D3DPERF_BeginEvent", "D3DPERF_EndEvent", "D3DPERF_GetStatus", "D3DPERF_QueryRepeatFrame", "D3DPERF_SetMarker", "D3DPERF_SetOptions", "D3DPERF_SetRegion", "DebugSetLevel", "DebugSetMute", "Direct3D9EnableMaximizedWindowedModeShim", "Direct3DCreate9", "Direct3DCreate9Ex", "Direct3DShaderValidatorCreate9", "PSGPError", "PSGPSampleTexture", (LPCSTR)16, (LPCSTR)17, (LPCSTR)18};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) {
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		load_d3d9_dll();
		load_dxgi_dll();
		for ( int i = 0; i < 18; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" void D3DPERF_BeginEvent_wrapper();
extern "C" void D3DPERF_EndEvent_wrapper();
extern "C" void D3DPERF_GetStatus_wrapper();
extern "C" void D3DPERF_QueryRepeatFrame_wrapper();
extern "C" void D3DPERF_SetMarker_wrapper();
extern "C" void D3DPERF_SetOptions_wrapper();
extern "C" void D3DPERF_SetRegion_wrapper();
extern "C" void DebugSetLevel_wrapper();
extern "C" void DebugSetMute_wrapper();
extern "C" void Direct3D9EnableMaximizedWindowedModeShim_wrapper();
extern "C" void Direct3DCreate9_wrapper();
extern "C" void Direct3DCreate9Ex_wrapper();
extern "C" void Direct3DShaderValidatorCreate9_wrapper();
extern "C" void PSGPError_wrapper();
extern "C" void PSGPSampleTexture_wrapper();
extern "C" void ExportByOrdinal16();
extern "C" void ExportByOrdinal17();
extern "C" void ExportByOrdinal18();


// Loads the original d3d9.dll from the default system directory
//	Function originally written by Michael Koch
void load_d3d9_dll()
{
	char buffer[MAX_PATH];

	// Get path to system dir and to d3d9.dll
	GetSystemDirectory(buffer, MAX_PATH);

	// Append DLL name
	strcat_s(buffer, "\\d3d9.dll");

	// Try to load the system's d3d9.dll, if pointer empty
	if (!mHinstDLL) mHinstDLL = LoadLibrary(buffer);

	// Debug
	if (!mHinstDLL)
	{
		OutputDebugString("PROXYDLL: Original d3d9.dll not loaded ERROR ****\r\n");
		ExitProcess(0); // Exit the hard way
	}
}

// Loads the original dxgi.dll from the default system directory
void load_dxgi_dll()
{
	char buffer[MAX_PATH];

	// Get path to system directory and to dxgi.dll
	GetSystemDirectory(buffer, MAX_PATH);

	// Append DLL name
	strcat_s(buffer, "\\dxgi.dll");

	// Try to load the system's dxgi.dll, if pointer empty
	if (!mHinst_dxgi_DLL) mHinst_dxgi_DLL = LoadLibrary(buffer);

	// Debug
	if (!mHinst_dxgi_DLL)
	{
		OutputDebugString("PROXYDLL: Original dxgi.dll not loaded ERROR ****\r\n");
		ExitProcess(0); // Exit the hard way
	}
}
