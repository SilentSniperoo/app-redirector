// This is where the magic of the shortcut program happens.
// To allow non-portable programs to be portable, we need to compile in:
// - Any relative paths to the programs we want to make portable.
// - Any necessary command line arguments needed to make the programs portable.

#include "stdafx.h"
#include "Resource.h"

std::wstring quote(std::wstring wide_string)
{
	return L"\"" + wide_string + L"\"";
}

std::wstring concat(
	unsigned count,
	std::wstring * wide_string_array,
	std::wstring between = std::wstring(),
	std::wstring before = std::wstring(),
	std::wstring after = std::wstring())
{
	std::wstring result;
	for (unsigned i = 0; i < count; ++i)
	{
		if (i > 0) result += between;
		result += wide_string_array[i];
	}
	return before + result + after;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// Create a buffer and fill it with the path to the start directory
	WCHAR buffer[1000];
	GetCurrentDirectory(1000, buffer);
	std::wstring start_dir(buffer);

	/* Find the relative path from the start dir to the executable for: */
#if IS_MINECRAFT
	// Minecraft
	std::wstring exe_relative(L"\\bin\\Minecraft.exe");
#elif IS_CHROME
	// Chrome
	std::wstring exe_relative(L"\\chrome.exe");
#endif

	// Construct the absolute path to the executable
	std::wstring exe_absolute(start_dir + exe_relative);

#if IS_MINECRAFT
	// For Minecraft, find the extra directories for the data and temporary files
	std::wstring work_dir(quote(start_dir + L"\\data"));
	std::wstring temp_dir(quote(start_dir + L"\\tmp"));
#endif

	/* For Chrome, set the profile and the app id */
	// Google Keep, Chrome Remote Desktop, and Google Hangouts
	std::wstring profile(L"--profile-directory=Default");
#if IS_GOOGLE_KEEP
	std::wstring app_id(L"--app-id=hmjkmjkepdijhoojdojkdfohbdgmmhki");
#elif IS_CHROME_REMOTE_DESKTOP
	std::wstring app_id(L"--app-id=gbchcmhmhahfdphkhkmpfmihenigjmpp");
#elif IS_GOOGLE_HANGOUTS
	std::wstring app_id(L"--app-id=nckgahadagoaajjgafhacjanaoiihapd");
#endif

	/* Construct the final command */
#if IS_MINECRAFT
	// Minecraft
	std::wstring cmd(exe_absolute + L" --workDir " + work_dir + L" --tmpDir " + temp_dir);
#elif IS_CHROME
	// Chrome
	std::wstring combine[] = { exe_absolute, profile, app_id };
	std::wstring cmd(concat(sizeof(combine) / sizeof(*combine), combine, L"\" \"", L"\"", L"\""));
#endif

	// Print out the final command to the console if run from a console
	std::wcout << cmd.c_str() << std::endl;

	// Run the command
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	return CreateProcess(_wcsdup(exe_absolute.c_str()), // No module name (use command line)
		_wcsdup(cmd.c_str()), // Command line the param is sleeptime_str
		NULL, // Process handle not inheritable
		NULL, // Thread handle not inheritable
		FALSE, // Set handle inheritance to FALSE
		0, // No creation flags
		NULL, // Use parent's environment block
		NULL, // Use parent's starting directory 
		&si, // Pointer to STARTUPINFO structure
		&pi); // Pointer to PROCESS_INFORMATION structure
}
