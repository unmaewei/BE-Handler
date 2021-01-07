#include <ntifs.h>
#include <ntimage.h>
#include <ntddk.h>
#include <memory>
#include "hook/hook.hpp"

using namespace driver;

void driver_thread( void* context )
{

	// wait for battleye (ooga booga)
	//https://www.youtube.com/watch?v=qdy0cZf5loU
	while ( !/*get_base*/( "BEDaisy" ) )
	{
		io::dbgprint( "waiting for BEDaisy..." );

		// got this from stack overflow dunno the links soz
		LARGE_INTEGER time; 
		time.QuadPart = -(250) * 10 * 1000; 
		KeDelayExecutionThread(KernelMode, TRUE, &time);
	}

	// credits to rakes thread this this and vergillius project, 0xC8 is list entrys
	uint64_t target_process_callbacks = reinterpret_cast< uint64_t >( *PsProcessType ) + 0xC8;
	uint64_t target_thread_callbacks = reinterpret_cast< uint64_t >( *PsThreadType ) + 0xC8;

	// magic time...
	callback( reinterpret_cast< PLIST_ENTRY >( target_process_callbacks ) );
	callback( reinterpret_cast< PLIST_ENTRY >( target_thread_callbacks ) );

	io::dbgprint( "hooked, terminating thread..." );

	// were done, the callback will do the rest for us. exit.
	PsTerminateSystemThread( STATUS_SUCCESS );
}

// <params> only used for cleaning, u can change params as u need
NTSTATUS DriverEntry( int64 address, int32 size ) 
{
	io::dbgprint( "driver entry called." );

	// change this per your desire; see:
	// global.hpp
	global::target_altitude = L"363220";
	global::target_driver = "BEDaisy";

	// here weo do RainbowSix. so we can desipher
	// between the actual game and RainbowSix_BE.exe
	// (thank god to my friend for telling me this,
	// i was stuck here for over a hour)
	global::target_process = "RainbowSix.";
	global::usermode_process = "Meme";

	// thread defines
	HANDLE thread_handle = nullptr;
	OBJECT_ATTRIBUTES object_attribues{ };
	InitializeObjectAttributes( &object_attribues, nullptr, OBJ_KERNEL_HANDLE, nullptr, nullptr );

	// create thread
	NTSTATUS status = PsCreateSystemThread( &thread_handle, NULL, &object_attribues, nullptr, nullptr, reinterpret_cast< PKSTART_ROUTINE >( &driver_thread ), nullptr );

	// print thread status and exit
	io::dbgprint( "thread status -> 0x%llx", status );
	io::dbgprint( "fininshed driver entry... closing..." );


	return STATUS_SUCCESS;
}
