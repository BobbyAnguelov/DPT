#ifndef _GROW
#define _GROW

#include "DPT.h"

void DPT::grow( vector<pixel>& nbrs, pixelSet& current )
{
	//create undo set
	vector<undoPixel> undoSet;

	//grow critical set
	growSet( nbrs, current, undoSet );

	//undo operation
	for (int i=0; i < undoSet.size(); i++)
	{
		//set cset indicator back to original value (using lum to store set number)
		csmap[ undoSet[i].y ][ undoSet[i].x ] = undoSet[i].setNum;
	}	
}

void DPT::growSet( vector<pixel>& nbrs, pixelSet& current, vector<undoPixel>& undoSet )
{
	//per pixel
	for (int i=0; i < nbrs.size(); i++)
	{		
		//if pixel doesnt belong to set
		if( csmap[nbrs[i].y][nbrs[i].x] == -1 )
		{
			//if neighbor pixel luminosity is the same as the set luminosity
			if ( image[nbrs[i].y][nbrs[i].x] == current.getSetLum() )
			{
				//add neighbor pixel to set
				current.getSet().push_back(nbrs[i]);				

				//update csmap
				csmap[nbrs[i].y][nbrs[i].x] = current.getSetNum();

				//get neighbours of pixel
				vector<pixel> pnbrs = getNeighbours( nbrs[i].x, nbrs[i].y );
				growSet(pnbrs, current, undoSet);
			}
			
			//different luminosities
			else 
			{
				//add neighbor pixel to adjacent set			
				current.getAdj().push_back(nbrs[i]);

				//add to undo set
				undoPixel p = {nbrs[i].x, nbrs[i].y, csmap[nbrs[i].y][nbrs[i].x]};
				undoSet.push_back(p);

				//update csmap
				csmap[nbrs[i].y][nbrs[i].x] = current.getSetNum();					
			}
		}
		
		else if ( csmap[nbrs[i].y][nbrs[i].x] == current.getSetNum() ) 
		{ 
			/*do nothing*/				
		}		
		
		else
		{			
			//if neighbor pixel luminosity is the same as the set luminosity		
			if ( image[nbrs[i].y][nbrs[i].x] == current.getSetLum() )
			{
				//merge sets
				//--------------------------------------------------
				int setID = csmap[nbrs[i].y][nbrs[i].x];

				//for all elements in new set
				for ( int j=0; j < csets[setID].getSet().size(); j++)
				{
					//copy pixel value from one set into another (dont delete from old set - speed vs memory tradeoff)
					current.getSet().push_back( csets[setID].getSet()[j] );

					//update csmap
					csmap[ csets[setID].getSet()[j].y ][ csets[setID].getSet()[j].x ] = current.getSetNum();			
				}

				//set new set to inactive				
				csets[ setID ].setActive(false);				

				//grow set with new set's adjacent pixels
				growSet( csets[ setID ].getAdj(), current, undoSet);
			}

			//different luminosities
			else 
			{				
				//add neighbor pixel to adjacent set			
				current.getAdj().push_back(nbrs[i]);

				//add to undo set
				undoPixel p = {nbrs[i].x, nbrs[i].y, csmap[nbrs[i].y][nbrs[i].x]};
				undoSet.push_back(p);

				//update csmap
				csmap[nbrs[i].y][nbrs[i].x] = current.getSetNum();				
			}
		}
	}		
}

#endif
