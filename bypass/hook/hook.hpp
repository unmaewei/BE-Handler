#pragma
#include "../defs.hpp"
#include "../io/io.hpp"
#include "../global/global.hpp"
using namespace driver;

// l0l
#define RAPE_BATTLEYE 0x1fffffL
POB_PRE_OPERATION_CALLBACK pre_operation_callback = nullptr;

OB_PREOP_CALLBACK_STATUS pre_operation_callback_hook( PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information )
{
	// if the callback object does not our process type return our callback
	if ( operation_information->ObjectType != *PsProcessType )
		return pre_operation_callback(registration_context, operation_information);

	// get raw image name
	const char* current_process = PsGetProcessImageFileName( IoGetCurrentProcess( ) );
	const char* target_process = PsGetProcessImageFileName( reinterpret_cast< PEPROCESS >(operation_information->Object ) );

	// ensure the target proces and current process are our desired processes
	if ( strstr( target_process, global::target_process ) && strstr( current_process, global::usermode_process ) )
	{
		io::dbgprint( "found, setting info..." );

		// set the access mask; see:
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/ns-wdm-_ob_pre_create_handle_information
		operation_information->Parameters->CreateHandleInformation.DesiredAccess = RAPE_BATTLEYE;
		operation_information->Parameters->CreateHandleInformation.OriginalDesiredAccess = RAPE_BATTLEYE;

		io::dbgprint( "set" );

		// ObjectPreCallback returns an OB_PREOP_CALLBACK_STATUS value. Drivers must return OB_PREOP_SUCCESS; see:
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nc-wdm-pob_pre_operation_callback
		return OB_PREOP_SUCCESS;
	}

	io::dbgprint( "finished callback" );

	// if the callback object does not our process type return our callback
	return pre_operation_callback( registration_context, operation_information );
}

// we need to set post operation, so this is just a null function
void post_operation_callback_hook( PVOID, POB_POST_OPERATION_INFORMATION ) { /* do nothing */ }

void callback( PLIST_ENTRY list_entry )
{
	for ( PLIST_ENTRY current_entry = list_entry->Flink; current_entry != list_entry; current_entry = current_entry->Flink )
	{
		// get the base address of an instance of a structure given the type of the 
		//structure and the address of a field within the containing structure; see:
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/mm-bad-pointer
		PCALLBACK_ENTRY_ITEM callback_item = CONTAINING_RECORD( current_entry, CALLBACK_ENTRY_ITEM, CallbackList );

		// check if the atitude of this callback item is the altidude of our desired driver
		if ( !wcscmp( callback_item->CallbackEntry->Altitude.Buffer, global::target_altitude ) )
		{
			// if pre_operation_callback_hook hasnt been set yet
			if ( pre_operation_callback == nullptr )
				 pre_operation_callback = callback_item->PreOperation;

			// set operations
			callback_item->PreOperation = pre_operation_callback_hook;
			callback_item->PostOperation = post_operation_callback_hook;
		}
	}
}
