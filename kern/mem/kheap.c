#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"

//==================================================================//
//==================================================================//
//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)//
//==================================================================//
//==================================================================//

void initialize_dyn_block_system()
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] initialize_dyn_block_system
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("initialize_dyn_block_system() is not implemented yet...!!");

	//[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]
	LIST_INIT(&AllocMemBlocksList);
	LIST_INIT(&FreeMemBlocksList);
#if STATIC_MEMBLOCK_ALLOC
	//DO NOTHING
#else
	/*[2] Dynamically allocate the array of MemBlockNodes
	 * 	remember to:
	 * 		1. set MAX_MEM_BLOCK_CNT with the chosen size of the array
	 * 		2. allocation should be aligned on PAGE boundary
	 * 	HINT: can use alloc_chunk(...) function
	 */

MAX_MEM_BLOCK_CNT=NUM_OF_KHEAP_PAGES;
uint32 x =ROUNDDOWN(KERNEL_HEAP_START,PAGE_SIZE) ;
//uint32 y=ROUNDUP(MAX_MEM_BLOCK_CNT*)
//MemBlockNodes=(void *)x;
//struct MemBlock *MemBlockNodes;
MemBlockNodes=(struct MemBlock*)KERNEL_HEAP_START;
int size=sizeof(struct MemBlock);
int size_of=ROUNDUP(MAX_MEM_BLOCK_CNT*size,PAGE_SIZE);
int size3 =ROUNDUP(NUM_OF_KHEAP_PAGES * sizeof(struct MemBlock), PAGE_SIZE);
//int z= pt_get_page_permissions(ptr_page_directory,x);
allocate_chunk(ptr_page_directory,KERNEL_HEAP_START,size3,PERM_WRITEABLE);





#endif
	//[3] Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
//LIST_INIT(&AvailableMemBlocksList);
initialize_MemBlocksList(MAX_MEM_BLOCK_CNT-1);

struct MemBlock* ptr1=LIST_FIRST(&AvailableMemBlocksList);

ptr1->size=(KERNEL_HEAP_MAX-KERNEL_HEAP_START-size3);

ptr1->sva=KERNEL_HEAP_START+size3;

//insert_sorted_with_merge_freeList(ptr1);
	//[4] Insert a new MemBlock with the remaining heap size into the FreeMemBlocksList
LIST_INSERT_HEAD(&FreeMemBlocksList,ptr1);

}

void* kmalloc(unsigned int size)
{
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	// use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy
	   size = ROUNDUP(size,PAGE_SIZE);

		struct MemBlock * FF_block;

		if(isKHeapPlacementStrategyFIRSTFIT())
		{
			FF_block = alloc_block_FF(size);
			if(FF_block != NULL )
			{
			int bac= allocate_chunk(ptr_page_directory,FF_block->sva,size,PERM_WRITEABLE);
			if(bac==0)
			{
				insert_sorted_allocList(FF_block);
	         	return (uint32*)FF_block->sva;

			}
			}
			else
			{
				return NULL;
			}

		}
		 if(isKHeapPlacementStrategyBESTFIT())
		{
			struct MemBlock * BF_block = alloc_block_BF(size);
			if(BF_block != NULL )
		  {
			int bac=allocate_chunk(ptr_page_directory,BF_block->sva,size,PERM_WRITEABLE);
			if (bac==0)
			  {
				insert_sorted_allocList(BF_block);
			    return (uint32*)BF_block->sva;
			  }
		      }
			else
			 {

				return NULL;
			 }

		}

		return 0;
}


void kfree(void* virtual_address)
{

	//TODO: [PROJECT MS2] [KERNEL HEAP] kfree
	// Write your code here, remove the panic and write your code
     //panic("kfree() is not implemented yet...!!");
     uint32 va1 = (uint32)virtual_address;
     struct MemBlock* ptr1=find_block(&AllocMemBlocksList,va1);

      if(ptr1!=NULL)
      {
        LIST_REMOVE(&AllocMemBlocksList,ptr1);
        uint32 end1=ptr1->sva +ptr1->size;

     for(uint32 i=ROUNDDOWN(ptr1->sva,PAGE_SIZE);i<ROUNDUP(end1,PAGE_SIZE);i+=PAGE_SIZE)
     {

	     unmap_frame(ptr_page_directory,i);
     }
     insert_sorted_with_merge_freeList(ptr1);
      }

}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_virtual_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");

    struct FrameInfo * ptr_info ;
    ptr_info = to_frame_info(physical_address);

    uint32 backed_va = ptr_info->va;
    return backed_va;

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_physical_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");

	uint32 backed_pa =virtual_to_physical(ptr_page_directory,virtual_address);
	 return backed_pa;
	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details
}


void kfreeall()
{
	panic("Not implemented!");

}

void kshrink(uint32 newSize)
{
	panic("Not implemented!");
}

void kexpand(uint32 newSize)
{
	panic("Not implemented!");
}




//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT MS2 - BONUS] [KERNEL HEAP] krealloc
	// Write your code here, remove the panic and write your code
	panic("krealloc() is not implemented yet...!!");
}
