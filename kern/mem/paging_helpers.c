/*
 * paging_helpers.c
 *
 *  Created on: Sep 30, 2022
 *      Author: HP
 */
#include "memory_manager.h"

/*[2.1] PAGE TABLE ENTRIES MANIPULATION */
inline void pt_set_page_permissions(uint32* page_directory, uint32 virtual_address, uint32 permissions_to_set, uint32 permissions_to_clear)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_set_page_permissions
	// Write your code here, remove the panic and write your code
	//panic("pt_set_page_permissions() is not implemented yet...!!");
	uint32 *ptr1= NULL;
	int ret=get_page_table(page_directory, virtual_address ,&ptr1);
		if(ret==TABLE_IN_MEMORY)
		    {
            ptr1 [PTX(virtual_address)]=ptr1 [PTX(virtual_address)]|permissions_to_set;
            ptr1 [PTX(virtual_address)]=ptr1 [PTX(virtual_address)]&(~permissions_to_clear);

			}
		else
		{
			panic("Invalid va");
		}
		tlb_invalidate((void *)NULL, (void *)virtual_address);

}

inline int pt_get_page_permissions(uint32* page_directory, uint32 virtual_address )
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_get_page_permissions
	// Write your code here, remove the panic and write your code
	//panic("pt_get_page_permissions() is not implemented yet...!!");
	uint32 *ptr1= NULL;
	int ret=get_page_table(page_directory, virtual_address ,&ptr1);
	if(ret==TABLE_IN_MEMORY)
	    {
            uint32 backed=ptr1 [PTX(virtual_address)];
           // uint32 sum=ptr1-backed3;

            return backed & 0x00000FFF;
		}
	else
	{
		return -1;
	}

}

inline void pt_clear_page_table_entry(uint32* page_directory, uint32 virtual_address)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_clear_page_table_entry
	// Write your code here, remove the panic and write your code

uint32 *ptr1= NULL;
uint32 directory_entry = page_directory[PTX(virtual_address)];
int ret=get_page_table(page_directory, virtual_address ,&ptr1);
if(ret==TABLE_IN_MEMORY)
    {
	    ptr1 [PTX(virtual_address)]=0;



	}
else
{
	panic("Invalid va");
}
tlb_invalidate((void *)NULL, (void *)virtual_address);
}

/***********************************************************************************************/

/*[2.2] ADDRESS CONVERTION*/
inline int virtual_to_physical(uint32* page_directory, uint32 virtual_address)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] virtual_to_physical
	// Write your code here, remove the panic and write your code
	//panic("virtual_to_physical() is not implemented yet...!!");
	uint32 *ptr1= NULL;
		int ret=get_page_table(page_directory, virtual_address ,&ptr1);
		if(ret==TABLE_IN_MEMORY)
		{
	            uint32 backed2=ptr1 [PTX(virtual_address)];
	            //uint32 backed = backed2>>12;
	            //backed2=backed2>>22 &1111111111&(111111111111);
	           return backed2&(0xFFFF000);

			}
		else
		{
			return -1;
		}
}

/***********************************************************************************************/

/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/

///============================================================================================
/// Dealing with page directory entry flags

inline uint32 pd_is_table_used(uint32* page_directory, uint32 virtual_address)
{
	return ( (page_directory[PDX(virtual_address)] & PERM_USED) == PERM_USED ? 1 : 0);
}

inline void pd_set_table_unused(uint32* page_directory, uint32 virtual_address)
{
	page_directory[PDX(virtual_address)] &= (~PERM_USED);
	tlb_invalidate((void *)NULL, (void *)virtual_address);
}

inline void pd_clear_page_dir_entry(uint32* page_directory, uint32 virtual_address)
{
	page_directory[PDX(virtual_address)] = 0 ;
	tlbflush();
}
