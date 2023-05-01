/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"


/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
	// Write your code here, remove the panic and write your code
	//panic("cut_paste_pages() is not implemented yet...!!");
	uint32 *ptr1= NULL;
	uint32 *ptr2= NULL;
	uint32 *ptr3= NULL;
    int x=0;
	uint32 *ptr_perm= NULL;
	uint32 *tabegin= NULL;
    uint32 SDA=ROUNDDOWN(dest_va,PAGE_SIZE);
    uint32 EDA=ROUNDUP((dest_va+(num_of_pages*PAGE_SIZE)),PAGE_SIZE);
    uint32 sva=ROUNDDOWN(source_va,PAGE_SIZE);
    uint32 eva=ROUNDUP((source_va+(num_of_pages*PAGE_SIZE)),PAGE_SIZE);
	for(int i=SDA;i<EDA;i+=PAGE_SIZE)
	{
		       uint32 *ptr_page_table = NULL ;
				struct FrameInfo *ptr_Frame_Info ;
				 ptr_Frame_Info=get_frame_info(page_directory,i, &ptr_page_table) ;
			     if(ptr_Frame_Info==NULL)
			     {
			    	 continue ;
			     }
			     else
			     {
			    	 x=1;
			    	 break;
			     }
	}
	if(x==1)
	{
		return -1;
	}
	if(x==0)
	{

		for(int i=sva;i<eva;i+=PAGE_SIZE)
		{
			   dest_va=ROUNDDOWN(dest_va,PAGE_SIZE);
		       uint32 *ptr_page_table = NULL ;
				struct FrameInfo *ptr_Frame_Info=get_frame_info(page_directory,i, &ptr_page_table) ;
		         int perm=pt_get_page_permissions(page_directory,i);
		           	map_frame(page_directory, ptr_Frame_Info,dest_va,perm) ;
		           	unmap_frame(page_directory,i) ;
		          dest_va=dest_va+PAGE_SIZE;
		}
	}

return 0;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the panic and write your code
	    uint32 *ptr1= NULL;
		uint32 *ptr2= NULL;
		uint32 *ptr3= NULL;
	    int x=0;
		uint32 *ptr_perm= NULL;
		uint32 *tabegin= NULL;
	    uint32 SDA=dest_va;
	    uint32 EDA=dest_va+ size;
	    uint32 sva=source_va;
	    uint32 eva=source_va+ size;
		for(int i=SDA;i<EDA;i+=PAGE_SIZE)
		{
		   uint32 *ptr_page_table = NULL ;
			struct FrameInfo *ptr_Frame_Info ;
			 ptr_Frame_Info=get_frame_info(page_directory,i, &ptr_page_table) ;
			  int perm=pt_get_page_permissions(page_directory,i);
			  if(ptr_Frame_Info!=NULL)
			  {
			  if((perm & PERM_WRITEABLE)==0)
				 {
					   return -1;
					// break;

				 }
			  }
			  else
			  {
				 x = 1;
			  }
		}
		if(x==1)
		{

		for(int i=sva;i<eva;i+=PAGE_SIZE)

			 {
				int ret=get_page_table(page_directory,dest_va,&ptr3);
				if(ret==TABLE_NOT_EXIST)
				{
				  create_page_table(page_directory,dest_va);
				}
				else if(ret==TABLE_IN_MEMORY)
				{
				  uint32 *ptr1=NULL;
				  uint32 ptr2;
				  struct FrameInfo *cont2;
				  int perm=pt_get_page_permissions(page_directory,i);
				  allocate_frame(&cont2);
				  map_frame(page_directory,cont2,dest_va,perm | PERM_WRITEABLE);
                  dest_va=dest_va+PAGE_SIZE;
				}
			 }


	}
		char*copysrc;
			char*copydst;
			copysrc=(char*)source_va;
			copydst=(char*)dest_va;
			for(int i=0;i<size;i++)
			{
				copydst[i]=copysrc[i];
			}

	return 0;

}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should share the given size from dest_va with the source_va
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
	// Write your code here, remove the panic and write your code
	//panic("share_chunk() is not implemented yet...!!");
	uint32 *ptr1= NULL;
	uint32 *ptr2= NULL;
	uint32 *ptr3= NULL;
    int x=0;
	uint32 *ptr_perm= NULL;
	uint32 *tabegin= NULL;
    uint32 SDA=ROUNDDOWN(dest_va,PAGE_SIZE);
    uint32 EDA=ROUNDUP((dest_va+(size)),PAGE_SIZE);
    uint32 sva=ROUNDDOWN(source_va,PAGE_SIZE);
    uint32 eva=ROUNDUP((source_va+(size)),PAGE_SIZE);
	for(int i=SDA;i<EDA;i+=PAGE_SIZE)
	{
		       uint32 *ptr_page_table = NULL ;
				struct FrameInfo *ptr_Frame_Info ;
				 ptr_Frame_Info=get_frame_info(page_directory,i, &ptr_page_table) ;
			     if(ptr_Frame_Info==NULL)
			     {
			    	 continue ;
			     }
			     else
			     {
			    	 x=1;
			    	 break;
			     }
	}
	if(x==1)
	{
		return -1;
	}
	if(x==0)
	{

		for(int i=sva;i<eva;i+=PAGE_SIZE)
		{
			   dest_va=ROUNDDOWN(dest_va,PAGE_SIZE);
		       uint32 *ptr_page_table = NULL ;
				struct FrameInfo *ptr_Frame_Info=get_frame_info(page_directory,i, &ptr_page_table) ;
		         //int perm=pt_get_page_permissions(page_directory,i);
		           	map_frame(page_directory, ptr_Frame_Info,dest_va,perms) ;
		           	//unmap_frame(page_directory,i) ;
		           	//ptr_Frame_Info->references++;

		          dest_va=dest_va+PAGE_SIZE;
		}
	}

return 0;

}
//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate in RAM the given range [va, va+size)
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
	// Write your code here, remove the panic and write your code
	//panic("allocate_chunk() is not implemented yet...!!");

    int x=0;
    uint32 eva = ROUNDUP((va+size),PAGE_SIZE);
    uint32 sva= ROUNDDOWN(va,PAGE_SIZE);
    for(int i=sva; i < eva ; i+= PAGE_SIZE)
    {
	       uint32 *ptr_page_table = NULL ;
			struct FrameInfo *ptr_Frame_Info ;
			 ptr_Frame_Info=get_frame_info(page_directory,i, &ptr_page_table) ;
		     if(ptr_Frame_Info!=0)
    	{
    		x=1;
    		break ;
    	}
    	else
    	{
    		continue ;
    	}

    }
     if(x==1)
    {
    	return -1;
    }
    else if(x==0)
	{
    for(int i=sva ; i < eva ; i += PAGE_SIZE)
    {
        		uint32*ptr_frame_info;
        		struct FrameInfo *ptr_info = get_frame_info(page_directory,i, &ptr_frame_info) ;
        		allocate_frame(&ptr_info);
         		map_frame(page_directory, ptr_info,i,perms);
         		ptr_info->va=i;
    }
  }

    return 0;
}

/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
	// Write your code here, remove the panic and write your code
	//panic("calculate_allocated_space() is not implemented yet...!!");
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	//panic("calculate_required_frames() is not implemented yet...!!");

    int num_tables1=0;
	int num_pages2=0;
	for(int i=ROUNDDOWN(sva,PAGE_SIZE*1024);i<eva;i+=PAGE_SIZE*1024)
	{
		uint32 *ptr1=NULL;
		int ret=get_page_table(page_directory,i,&ptr1);
	     if(ret==0)
		{
	    	 num_tables1++;

		}
	}
	for(int i=sva;i<ROUNDUP(eva,PAGE_SIZE);i+=PAGE_SIZE)
	{
		uint32 *ptr1=NULL;
		struct FrameInfo *ptr_Frame_Info=get_frame_info(page_directory, i, &ptr1);
				if (ptr_Frame_Info!=0)
				{

					num_pages2++;
				}
	}
	*num_tables=num_tables1;
	*num_pages=num_pages2;
	//return num_pages+num_tables;

}

/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	//panic("calculate_required_frames() is not implemented yet...!!");

	uint32 num_tables=0;
	uint32 num_pages=0;
	for(uint32 sva_edit=ROUNDDOWN(sva,PAGE_SIZE*1024);sva_edit<(sva+size);sva_edit+=PAGE_SIZE*1024)
	{
		uint32 *ptr1=NULL;
		int ret=get_page_table(page_directory,sva_edit,&ptr1);
	     if(ptr1==0)
		{
	    	 num_tables++;

		}
	}
	for(uint32 sva_edit=ROUNDDOWN(sva,PAGE_SIZE);sva_edit<sva+size;sva_edit+=PAGE_SIZE)
	{
		uint32 *ptr1=NULL;
		struct FrameInfo *ptr_Frame_Info=get_frame_info(page_directory, sva_edit, &ptr1);
				if (ptr_Frame_Info==0)
				{

					num_pages++;
				}
	}
	return num_pages+num_tables;
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	// Write your code here, remove the panic and write your code
	//panic("free_user_mem() is not implemented yet...!!");
	//This function should:
        size = ROUNDUP(size,PAGE_SIZE);
		for(uint32 i=virtual_address ; i<virtual_address + size;i+= PAGE_SIZE)
		{
			pf_remove_env_page(e,i);
		}




        for(uint32 i=virtual_address ; i<virtual_address + size;i+= PAGE_SIZE)
		{

        	env_page_ws_invalidate(e,i);
            unmap_frame(e->env_page_directory,i);
		}

		for(uint32 i = virtual_address; i < virtual_address+size; i+= PAGE_SIZE)
		{
			int z = 1;
			uint32* ptr=NULL;
			int ret=get_page_table(e->env_page_directory,i,&ptr);
			if(ret == TABLE_IN_MEMORY)
			{
				for(uint32 j=0;j<1024;j++)
				{
					if(ptr[j] != 0)
					{
					z = 0;
					break;
					}
				}
				if(z==1)
				{
					pd_clear_page_dir_entry(e->env_page_directory,i);
					kfree((void*)ptr);
				}
			}

		}
		//2. Free ONLY pages that are resident in the working set from the memory
		//3. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}
//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Free any BUFFERED pages in the given range
	//4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

