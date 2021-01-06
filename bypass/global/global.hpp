#pragma
namespace driver
{
	namespace global
	{
		// anti cheat driver name
		char* target_driver;

		// driver altitude; see:
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/ifs/allocated-altitudes
		wchar_t* target_altitude;

		// our target game
		char* target_process;

		// our usermode process that creats the handle
		char* usermode_process;
	}
}