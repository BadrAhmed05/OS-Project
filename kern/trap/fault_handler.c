/*
 * fault_handler.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */
#include <inc/mmu.h>
#include <inc/x86.h>
#include <inc/assert.h>


#include "trap.h"
#include <kern/proc/user_environment.h>
#include "../cpu/sched.h"
#include "../disk/pagefile_manager.h"
#include "../mem/memory_manager.h"

//2014 Test Free(): Set it to bypass the PAGE FAULT on an instruction with this length and continue executing the next one
// 0 means don't bypass the PAGE FAULT
uint8 bypassInstrLength = 0;

//===============================
// REPLACEMENT STRATEGIES
//===============================
//2020
void setPageReplacmentAlgorithmLRU(int LRU_TYPE)
{
	assert(LRU_TYPE == PG_REP_LRU_TIME_APPROX || LRU_TYPE == PG_REP_LRU_LISTS_APPROX);
	_PageRepAlgoType = LRU_TYPE ;
}
void setPageReplacmentAlgorithmCLOCK(){_PageRepAlgoType = PG_REP_CLOCK;}
void setPageReplacmentAlgorithmFIFO(){_PageRepAlgoType = PG_REP_FIFO;}
void setPageReplacmentAlgorithmModifiedCLOCK(){_PageRepAlgoType = PG_REP_MODIFIEDCLOCK;}
/*2018*/ void setPageReplacmentAlgorithmDynamicLocal(){_PageRepAlgoType = PG_REP_DYNAMIC_LOCAL;}
/*2021*/ void setPageReplacmentAlgorithmNchanceCLOCK(int PageWSMaxSweeps){_PageRepAlgoType = PG_REP_NchanceCLOCK;  page_WS_max_sweeps = PageWSMaxSweeps;}

//2020
uint32 isPageReplacmentAlgorithmLRU(int LRU_TYPE){return _PageRepAlgoType == LRU_TYPE ? 1 : 0;}
uint32 isPageReplacmentAlgorithmCLOCK(){if(_PageRepAlgoType == PG_REP_CLOCK) return 1; return 0;}
uint32 isPageReplacmentAlgorithmFIFO(){if(_PageRepAlgoType == PG_REP_FIFO) return 1; return 0;}
uint32 isPageReplacmentAlgorithmModifiedCLOCK(){if(_PageRepAlgoType == PG_REP_MODIFIEDCLOCK) return 1; return 0;}
/*2018*/ uint32 isPageReplacmentAlgorithmDynamicLocal(){if(_PageRepAlgoType == PG_REP_DYNAMIC_LOCAL) return 1; return 0;}
/*2021*/ uint32 isPageReplacmentAlgorithmNchanceCLOCK(){if(_PageRepAlgoType == PG_REP_NchanceCLOCK) return 1; return 0;}

//===============================
// PAGE BUFFERING
//===============================
void enableModifiedBuffer(uint32 enableIt){_EnableModifiedBuffer = enableIt;}
uint8 isModifiedBufferEnabled(){  return _EnableModifiedBuffer ; }

void enableBuffering(uint32 enableIt){_EnableBuffering = enableIt;}
uint8 isBufferingEnabled(){  return _EnableBuffering ; }

void setModifiedBufferLength(uint32 length) { _ModifiedBufferLength = length;}
uint32 getModifiedBufferLength() { return _ModifiedBufferLength;}

//===============================
// FAULT HANDLERS
//===============================

//Handle the table fault
void table_fault_handler(struct Env * curenv, uint32 fault_va)
{
	//panic("table_fault_handler() is not implemented yet...!!");
	//Check if it's a stack page
	uint32* ptr_table;
#if USE_KHEAP
	{
		ptr_table = create_page_table(curenv->env_page_directory, (uint32)fault_va);
	}
#else
	{
		__static_cpt(curenv->env_page_directory, (uint32)fault_va, &ptr_table);
	}
#endif
}

//Handle the page fault





void page_fault_handler(struct Env * curenv, uint32 fault_va)
{
	//TODO: [PROJECT MS3] [FAULT HANDLER] page_fault_handler
	// Write your code here, remove the panic and write your code
	//panic("page_fault_handler() is not implemented yet...!!");
	int max= curenv->page_WS_max_size ;
	int ws_size=env_page_ws_get_size(curenv);
	//PLACMENT
	if(ws_size<max)
	{
		{
				struct FrameInfo *alloc_ptr =NULL ;
				int back=allocate_frame(&alloc_ptr);
				if(back != E_NO_MEM)
				{
					map_frame(curenv->env_page_directory, alloc_ptr,fault_va,PERM_PRESENT | PERM_USER | PERM_WRITEABLE);
					int read1=pf_read_env_page(curenv,(void*)fault_va);
					if(read1==E_PAGE_NOT_EXIST_IN_PF)
					{
						if((fault_va>=USER_HEAP_START && fault_va<USER_HEAP_MAX)||(fault_va >= USER_HEAP_MAX && fault_va < USTACKTOP ) )
						{

						}
						else
						{
							unmap_frame(curenv->env_page_directory,fault_va);
							panic("ILLEGAL MEMORY ACCESS %d\n",fault_va);
						}
					}

				}
	}
		if(curenv->page_last_WS_index ==  curenv->page_WS_max_size)
		 {
			curenv->page_last_WS_index=0;
		 }
		 int size = curenv->page_WS_max_size ;
										for( int i =0 ; i<size;i++)
										{

											 if(env_page_ws_is_entry_empty(curenv,i))
											{
												curenv->page_last_WS_index = i ;
												break ;
											}
										else if(env_page_ws_is_entry_empty(curenv,curenv->page_last_WS_index))
									        	{
											    	break;
												}
										}
										env_page_ws_set_entry(curenv,curenv->page_last_WS_index ,fault_va);
										curenv->page_last_WS_index ++ ;
	}

	//replacment
	else
	{
		if(isPageReplacmentAlgorithmCLOCK())
		{
			{

				uint32 *ptr;
				struct FrameInfo *ptr_frame;
			    uint32 permiss;
			    int page_out = 0;
			    int counting =curenv->page_last_WS_index;
			    int z=0;
			    int page_ind = counting;
			    for(int i=0;i<curenv->page_WS_max_size;i++)
			    {
			    	page_out = env_page_ws_get_virtual_address(curenv,counting);
			    	permiss = pt_get_page_permissions(curenv->env_page_directory,page_out);
			    	if ( ((PERM_USED) & permiss ) != 0 )
			    	{
			    		 pt_set_page_permissions( curenv->env_page_directory,page_out, 0 , PERM_USED );

			    	}
			    	else
			    	{
			    	  page_ind = counting;
			      	  break;
			    	}

			    	counting ++;

			    	if ( curenv->page_WS_max_size== counting)
			    	{
			    		counting = 0 ;
			    	}
			    }


			    page_out = env_page_ws_get_virtual_address(curenv,page_ind);
			    permiss=pt_get_page_permissions(curenv->env_page_directory,page_out);
			    if ((permiss &PERM_MODIFIED)!=0)
			    {

			    	      int ret=get_page_table(curenv->env_page_directory,page_out,&ptr);
			    	        ptr_frame= get_frame_info(curenv->env_page_directory,page_out,&ptr);
			    	        pf_update_env_page(curenv,page_out,ptr_frame);
			    	        unmap_frame(curenv->env_page_directory,page_out);

			    }
			    else
			    {
			    	unmap_frame(curenv->env_page_directory,page_out);
			    }
			    env_page_ws_set_entry(curenv,page_ind,fault_va );
			    curenv->page_last_WS_index=page_ind+1;
			    if ( curenv->page_WS_max_size == curenv->page_last_WS_index  )
			    {
			    	curenv->page_last_WS_index = 0;
			    }

			}

			{
					struct FrameInfo *alloc_ptr =NULL ;
					int back=allocate_frame(&alloc_ptr);
					if(back != E_NO_MEM)
					{
						map_frame(curenv->env_page_directory, alloc_ptr,fault_va,PERM_PRESENT | PERM_USER | PERM_WRITEABLE);
						int read1=pf_read_env_page(curenv,(void*)fault_va);
						if(read1==E_PAGE_NOT_EXIST_IN_PF)
						{
							if((fault_va>=USER_HEAP_START && fault_va<USER_HEAP_MAX)||(fault_va >= USER_HEAP_MAX && fault_va < USTACKTOP ) )
							{

							}

						}

					}
		}
			if(curenv->page_last_WS_index ==  curenv->page_WS_max_size)
			 {
				curenv->page_last_WS_index=0;
			 }
		}

	}

	//refer to the project presentation and documentation for details
}
void __page_fault_handler_with_buffering(struct Env * curenv, uint32 fault_va)
{
	// Write your code here, remove the panic and write your code
	panic("__page_fault_handler_with_buffering() is not implemented yet...!!");


}
