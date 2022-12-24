#include <iostream>
#include <Windows.h>
#include <stdio.h>

HINSTANCE mHinst = 0, mHinstDLL = 0, mHinst_dxgi_DLL = 0;
extern "C" UINT_PTR mProcs[18] = {0};

void load_dxgi_dll();

LPCSTR mImportNames[] = {"D3DPERF_BeginEvent", "D3DPERF_EndEvent", "D3DPERF_GetStatus", "D3DPERF_QueryRepeatFrame", "D3DPERF_SetMarker", "D3DPERF_SetOptions", "D3DPERF_SetRegion", "DebugSetLevel", "DebugSetMute", "Direct3D9EnableMaximizedWindowedModeShim", "Direct3DCreate9", "Direct3DCreate9Ex", "Direct3DShaderValidatorCreate9", "PSGPError", "PSGPSampleTexture", (LPCSTR)16, (LPCSTR)17, (LPCSTR)18};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) {
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
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

	
	char current_working_directory[MAX_PATH];
	DWORD result = GetCurrentDirectory(MAX_PATH, current_working_directory);
	if (result == 0) {
		std::cerr << "Failed to get current working directory: " << GetLastError() << std::endl;
		return 1;
	}

	// Concatenate the current working directory with the file name
	std::string file_path = std::string(current_working_directory) + "\\sythe.dll";

	// Load the library
	HMODULE hmod = LoadLibraryA(file_path.c_str());
	if (hmod == NULL) {
		std::cerr << "Failed to load library: " << GetLastError() << std::endl;
		return 1;
	}

	std::cout << "Successfully loaded library: " << file_path << std::endl;
	
	
	// Debug
	if (!mHinst_dxgi_DLL)
	{
		OutputDebugString("PROXYDLL: Original dxgi.dll not loaded ERROR ****\r\n");
		ExitProcess(0); // Exit the hard way
	}
}
