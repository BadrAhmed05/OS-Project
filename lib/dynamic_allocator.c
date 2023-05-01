/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
	cprintf("\n=========================================\n");
	struct MemBlock* blk ;
	struct MemBlock* lastBlk = NULL ;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1 ;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

	lastBlk = NULL ;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1 ;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	struct MemBlock* ptr1;
    LIST_INIT(&AvailableMemBlocksList);
    int i;
    for(i=0;i < numOfBlocks;i++){
    	if(MAX_MEM_BLOCK_CNT >= numOfBlocks){
    		LIST_INSERT_HEAD(&AvailableMemBlocksList, &(MemBlockNodes[i]));
    	}
    }

}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code

	struct MemBlock *ptrr;
	LIST_FOREACH(ptrr,blockList){
	if( va == ptrr->sva)
			{
				return ptrr ;
			}
	}

	return NULL;

	}
//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert)
{


	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
		//struct MemBlock *ptr3=NULL;

			int size = LIST_SIZE(&(AllocMemBlocksList));


			if(size==0 || LIST_EMPTY(&AllocMemBlocksList))
			{

				LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
			//	global_point=blockToInsert;

			}
			else if(blockToInsert->sva < LIST_FIRST(&AllocMemBlocksList)->sva)
			{
				LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
			}
			else if(blockToInsert->sva > LIST_LAST(&AllocMemBlocksList)->sva)
			{
				//global_point=blockToInsert;
				LIST_INSERT_TAIL(&AllocMemBlocksList,blockToInsert);


			}
			else
			{
				struct MemBlock *ptr3=NULL;
				struct MemBlock *ptr_nx=NULL;
				LIST_FOREACH(ptr3,&AllocMemBlocksList)
						{
					ptr_nx=LIST_NEXT(ptr3);
				        	if(ptr3->sva+ptr3->size <= blockToInsert->sva)
					{
				        		if((blockToInsert->sva) < ptr_nx->sva)
				        		{
						 //global_point=blockToInsert;
                         LIST_INSERT_AFTER(&AllocMemBlocksList,ptr3,blockToInsert);
				        		}
			        }
		        }

		}
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================

struct MemBlock *alloc_block_FF(uint32 size)
{
		//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
		//struct MemBlock *NewBlock = AvailableMemBlocksList.lh_first ;
		struct MemBlock *ptr1 ;
		struct MemBlock *ptr2=NULL ;
		struct MemBlock *NewBLockrmv;
		LIST_FOREACH(ptr1,&FreeMemBlocksList)
		{

			 if ( ptr1->size == size )
		    {
				 struct MemBlock *block=ptr1;
			       LIST_REMOVE(&FreeMemBlocksList,ptr1);
			         ptr2= block;
			         break;

		   	}
		else if(ptr1->size > size)
			{
			struct MemBlock* NewBlock = LIST_LAST(&AvailableMemBlocksList);
			NewBlock->size =size;
			NewBlock->sva = ptr1->sva;
			NewBLockrmv = NewBlock;
			ptr1->sva+=NewBlock->size;
			ptr1->size-=NewBlock->size;
			LIST_REMOVE(&AvailableMemBlocksList , NewBLockrmv);
			ptr2= NewBlock;
			break;


			}
          }

		return ptr2;

}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
struct MemBlock *alloc_block_BF(uint32 size)
{


	struct MemBlock *ptr1=NULL ;
	struct MemBlock *ptr2=NULL ;
	struct MemBlock *temper=NULL ;
	struct MemBlock *ptr3=NULL ;
	int y=0;
	int minimum=2147483647;
	int z=2147483647;
	int iterate=0;
	struct MemBlock *NewBLockrmv;
	int ok=0;
	LIST_FOREACH(ptr1,&FreeMemBlocksList)
	{

		 if ( ptr1->size == size )
	    {
			 struct MemBlock *block=ptr1;
		       LIST_REMOVE(&FreeMemBlocksList,ptr1);
		       ptr3=block;
		       iterate=1;
		       break;

	   	}
	}

	if(iterate==0)
		{

		LIST_FOREACH(ptr2,&FreeMemBlocksList)
		{
			if(ptr2->size>size)
			{
			       y=ptr2->size-size;
				if(y<minimum)
				{
					minimum=y;
					temper=ptr2;
				    ok=1;
				   // break;
				}

			}
		}

	  if(ok==1)
			 {
		                struct MemBlock* NewBlock = LIST_LAST(&AvailableMemBlocksList);
			            NewBlock->size =size;
			 			NewBlock->sva = temper->sva;
			 			NewBLockrmv = NewBlock;
			 			temper->sva+=NewBlock->size;
			 			temper->size-=NewBlock->size;
			 			LIST_REMOVE(&AvailableMemBlocksList , NewBLockrmv);
			 			ptr3= NewBlock;


		}
			 }


	return ptr3;
}
//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock *alloc=NULL ;
struct MemBlock *alloc_block_NF(uint32 size)

{
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF

	struct MemBlock *ptr1 ;
	struct MemBlock *NewBlock1 = AvailableMemBlocksList.lh_first ;
	struct MemBlock *NewBlck;
//	struct MemBlock *ptr2=FreeMemBlocksList.lh_first;
	//int list_size1=LIST_SIZE(&AllocMemBlocksList);
		if(NewBlock1==NULL ||NewBlock1==0)
		{
			LIST_FOREACH(ptr1,&FreeMemBlocksList)
		{
		            if ( ptr1->size == size )
	               {
		         	// struct MemBlock *block=ptr1;
		       	     alloc=LIST_NEXT(ptr1);
		              LIST_REMOVE(&FreeMemBlocksList,ptr1);
		               return ptr1;

	             	}
            	        else if(ptr1->size > size)
             	    	{
		NewBlck = LIST_FIRST(&AvailableMemBlocksList);
		NewBlck->size =size;
		NewBlck->sva = ptr1->sva;
		//NewBLockrmv = NewBlock;
		ptr1->sva=ptr1->sva+size;
		ptr1->size=ptr1->size-size;
	//	alloc=LIST_NEXT(NewBLockrmv);
		LIST_REMOVE(&(AvailableMemBlocksList) , NewBlck);
		return NewBlck;
		              }
		}
		}
		ptr1=alloc;
	while(ptr1!=NULL)
	{
		//LIST_FOREACH(ptr1,&FreeMemBlocksList)
			if(ptr1->size==size)
			{
			// struct MemBlock *block=ptr1;
			     alloc=LIST_NEXT(ptr1);
		       LIST_REMOVE(&FreeMemBlocksList,ptr1);
		         return ptr1;
			}
			else if(ptr1->size>size)
			{
				NewBlck = LIST_FIRST(&AvailableMemBlocksList);
				NewBlck->size =size;
				NewBlck->sva = ptr1->sva;
				//NewBLockrmv = NewBlock;
				ptr1->sva=ptr1->sva+size;
				ptr1->size=ptr1->size-size;
				alloc=ptr1;
				LIST_REMOVE(&AvailableMemBlocksList , NewBlck);
				return NewBlck;
			}
			ptr1=LIST_NEXT(ptr1);


	}
	LIST_FOREACH(ptr1,&FreeMemBlocksList)
			{
			 if ( ptr1->size == size )
		    {
				 struct MemBlock *block=ptr1;
				     alloc=LIST_NEXT(ptr1);
			       LIST_REMOVE(&FreeMemBlocksList,ptr1);
			         return ptr1;

		   	}
	        	else if(ptr1->size > size)
			{
			NewBlck = LIST_FIRST(&AvailableMemBlocksList); ;
			NewBlck->size =size;
			NewBlck->sva = ptr1->sva;
			//NewBLockrmv = NewBlock;
			ptr1->sva=ptr1->sva+size;
			ptr1->size=ptr1->size-size;
		//	alloc=LIST_NEXT(NewBLockrmv);
			alloc=ptr1;

			LIST_REMOVE(&AvailableMemBlocksList , NewBlck);
			return NewBlck;
			}
			}




return 0;

}
//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{
		//cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
		//print_mem_block_lists() ;
		//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
		// Write your code here, remove the panic and write your code

	if(LIST_EMPTY(&FreeMemBlocksList))
	{
	    	 LIST_INSERT_HEAD(&FreeMemBlocksList,blockToInsert);
	     }
	else if(blockToInsert->sva <= LIST_FIRST(&FreeMemBlocksList)->sva)
	{
		if(blockToInsert->sva+blockToInsert->size==LIST_FIRST(&FreeMemBlocksList)->sva)
		{
			LIST_FIRST(&FreeMemBlocksList)->sva = blockToInsert->sva;
	    	LIST_FIRST(&FreeMemBlocksList)->size =LIST_FIRST(&FreeMemBlocksList)->size+blockToInsert->size;
			blockToInsert->sva = 0;
			blockToInsert->size = 0;
			LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
		}
		else
		{
		LIST_INSERT_HEAD(&FreeMemBlocksList,blockToInsert);
		}
	}
	else if(blockToInsert->sva >= LIST_LAST(&FreeMemBlocksList)->sva)
	{
		if(blockToInsert->sva==LIST_LAST(&FreeMemBlocksList)->sva+LIST_LAST(&FreeMemBlocksList)->size)
		{
			LIST_LAST(&FreeMemBlocksList)->size=LIST_LAST(&FreeMemBlocksList)->size+blockToInsert->size;
			blockToInsert->sva = 0;
			blockToInsert->size = 0;
			LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);

		}
		else
		{
			LIST_INSERT_TAIL(&FreeMemBlocksList,blockToInsert);
		}
	}
	else
	{
		struct MemBlock *ptr5;
		struct MemBlock *ptr6 ;
		LIST_FOREACH(ptr5,&FreeMemBlocksList)
		{
			ptr6 = LIST_NEXT(ptr5);
			int sum1 = (blockToInsert->sva+blockToInsert->size);
			if(blockToInsert->sva < ptr6->sva && blockToInsert->sva > ptr5->sva)
			{
				if((ptr5->sva + ptr5->size) == blockToInsert->sva && sum1!= ptr6->sva)
				{
					ptr5->size =ptr5->size + blockToInsert->size;
					blockToInsert->sva = 0;
					blockToInsert->size = 0;
					LIST_INSERT_TAIL(&AvailableMemBlocksList,blockToInsert);
				}
				else if((ptr5->size + ptr5->sva) == blockToInsert->sva && (sum1)== ptr6->sva)
				{
					ptr5->size =ptr5->size + (blockToInsert->size +ptr6->size);
					ptr6->size = 0;
					ptr6->sva = 0;
					blockToInsert->size = 0;
					blockToInsert->sva = 0;
                 	LIST_REMOVE(&FreeMemBlocksList, ptr6);
					LIST_INSERT_HEAD(&AvailableMemBlocksList, ptr6);
					LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);

				}
				else if(sum1 == ptr6->sva &&(ptr5->size + ptr5->sva) != blockToInsert->sva){

					ptr6->sva = blockToInsert->sva;
					ptr6->size = ptr6->size+blockToInsert->size;
					blockToInsert->size = 0;
					blockToInsert->sva = 0;
					LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);

				}

				else
			    	{
					LIST_INSERT_AFTER(&FreeMemBlocksList, ptr5, blockToInsert);
		            }
               break;

			}
		}
	}


}
