#ifndef _DPT
#define _DPT

#include <vector>

using namespace std;

//include pixel and critical classes
#include "pixel.h"
#include "pixelSet.h"
#include "pulse.h"

class DPT
{

private:

	//image and critical set image map
	int width, height;
	int** image;
	int** csmap;

	//critical set storage
	vector<pixelSet> csets;

	//active min max sets (stores set ID)
	vector<int> activeSets;

	//extracted pulse storage
	vector< vector<pulse> > downPulses, upPulses;
	
public:

	//constructor
	DPT(int** img, int w, int h):width(w),height(h)
	{
		//create image copy and blank image map
		image = new int*[height];
		csmap = new int*[height];		
		
		for (int y=0; y < height; y++)
		{			
			image[y] = new int[width];
			csmap[y] = new int[width];

			//copy values
			for (int x=0; x<width; x++)
			{
				image[y][x] = img[y][x];
				csmap[y][x] = -1;				
			}			
		}
		
		//locate the initial critical sets
		locateCriticalSets();		
	}

	//destructor
	~DPT()
	{
		delete[] csmap;
		delete[] image;
	}

	//decompose image
	void decompose()
	{
		cout << endl << endl << "STARTING DECOMPOSITION : " << endl << "================================================" << endl << endl;

		//extract pulses
		int n = 1;

		while ( activeSets.size() > 1) 
		{				
			extractPulse(n);
			n++;
		}	
		
		cout << endl << "END OF DECOMPOSITION : ended at size " << n << endl << "================================================" << endl << endl;	
		
		int f = image[0][0];
		int avg = 0;

		for (int y=0; y<height; y++ )
		{
			for (int x=0; x<width; x++)
			{
				avg = image[y][x] - f;
				if (avg != 0) cout << "ERROR!";
			}
		}

		//debugImage("image");
		//debugCsmap("csind");
	}

	//full reconstructions from pulses
	int** fullReconstruction()
	{
		//create new image storage and set background color
		int** img = new int*[height];
		
		for (int y=0; y < height; y++) 
		{
			img[y] = new int[width];

			for (int x=0; x < width; x++) img[y][x] = image[y][x];			
		}

		//recontruct from pulses
		for ( int i=0; i < downPulses.size(); i++ )
		{
			//for each up pulse add contribution
			for ( int p=0; p < upPulses[i].size(); p++ )
			{
				for (int j=0; j < upPulses[i][p].set.getSet().size(); j++)
				{
					img[upPulses[i][p].set.getSet()[j].y][upPulses[i][p].set.getSet()[j].x] += upPulses[i][p].relativeLum;
				}
			}

			//for each down pulse add contribution
			for ( int p=0; p < downPulses[i].size(); p++ )
			{
				for (int j=0; j < downPulses[i][p].set.getSet().size(); j++)
				{
					img[downPulses[i][p].set.getSet()[j].y][downPulses[i][p].set.getSet()[j].x] += downPulses[i][p].relativeLum;
				}
			}
		}

		//return reconstructed image
		return img;
	}

	//partially reconstruction from pulses 
	int** partialReconstruction(int lowerRange, int upperRange)
	{
		//check ranges
		if ( lowerRange >= 0 && upperRange < downPulses.size() )
		{		
			//create new image storage and set background color
			int** img = new int*[height];
			
			for (int y=0; y < height; y++) 
			{
				img[y] = new int[width];

				for (int x=0; x < width; x++) img[y][x] = image[y][x];			
			}

			//recontruct from pulses
			for ( int i=lowerRange-1; i < upperRange; i++ )
			{
				//for each up pulse add contribution
				for ( int p=0; p < upPulses[i].size(); p++ )
				{
					for (int j=0; j < upPulses[i][p].set.getSet().size(); j++)
					{
						img[upPulses[i][p].set.getSet()[j].y][upPulses[i][p].set.getSet()[j].x] += upPulses[i][p].relativeLum;
					}
				}

				//for each down pulse add contribution
				for ( int p=0; p < downPulses[i].size(); p++ )
				{
					for (int j=0; j < downPulses[i][p].set.getSet().size(); j++)
					{
						img[downPulses[i][p].set.getSet()[j].y][downPulses[i][p].set.getSet()[j].x] += downPulses[i][p].relativeLum;
					}
				}
			}

			//return reconstructed image
			return img;
		}
		else
		{
			cout << "invalid reconstruction ranges!" << endl;
			return 0;
		}
	}

	//accessors
	int getWidth() { return width; }
	int getHeight() { return height; }

private:

	//grow prototype
	void grow(vector<pixel>& nbrs, pixelSet& current);
	void growSet(vector<pixel>& nbrs, pixelSet& current, vector<undoPixel>& undoSet);

	//lower prototype
	void extractPulse(int n);
	//find initial critical sets
	void locateCriticalSets()
	{	
		int setNum = -1;
		
		//per pixel
		for (int y=0; y < height; y++)
		{
			for (int x=0; x < width; x++)
			{
				//if not part of a set
				if ( csmap[y][x] == -1 )
				{
					//get pixel luminosity
					int pixelLum = image[y][x];

					//get pixel neighbours
					vector<pixel> nbrs = getNeighbours(x,y);	
					
					//get min/max luminosity values of neighbours
					int max, min;
					getMinMax(min, max, nbrs);

					//if pixel luminosity greater or lower than neighbour then critical set
					if ( pixelLum  <= min || pixelLum >= max )
					{
						//increment set number
						setNum++;
					
						//create critical set and add initial pixel to it
						pixelSet cset(setNum, pixelLum, true);
						pixel p = {x,y};
						cset.getSet().push_back(p);
						csmap[y][x] = setNum;
						
						//grow critical set
						grow( nbrs, cset );					

						//add to active sets
						activeSets.push_back(cset.getSetNum());						

						//store cset
						csets.push_back(cset);						
					}								
				}				
			}
		}
		
		cout << endl << "Active Sets: " << activeSets.size() << endl;			
	}	

	//get neighbours helper function
	inline void addToVector(int x, int y, vector<pixel>& nbrs)
	{		
		pixel t = {x,y};		
		nbrs.push_back(t);
	}

	//return xy co-ords of pixel neighbours
	vector<pixel> getNeighboursOld(int x, int y)
	{
		vector<pixel> nbrs;
		nbrs.reserve(8);
		
		//left edge
		if ( x == 0 )
		{
			//add right
			addToVector(x+1,y, nbrs);

			//on upper edge
			if ( y == 0 )
			{
				//only add bottom row
				addToVector(x,y+1, nbrs);
				//addToVector(x+1,y+1, nbrs);
			}
			
			//on lower edge
			else if ( y == height - 1 )
			{
				//only add top row
				addToVector(x,y-1, nbrs);
				//addToVector(x+1,y-1, nbrs);
			}

			else
			{
				//add both top and bottom row
				addToVector(x,y-1, nbrs);
				//addToVector(x+1,y-1, nbrs);			
				addToVector(x,y+1, nbrs);
				//addToVector(x+1,y+1, nbrs);
			}
		}

		//right edge
		else if ( x == width - 1 )
		{
			//add left
			addToVector(x-1,y, nbrs);

			//on upper edge
			if ( y == 0 )
			{
				//only add bottom row
				addToVector(x,y+1, nbrs);
				//addToVector(x-1,y+1, nbrs);
			}
			
			//on lower edge
			else if ( y == height - 1 )
			{
				//only add top row
				addToVector(x,y-1, nbrs);
				//addToVector(x-1,y-1, nbrs);
			}

			else
			{
				//add both top and bottom row
				addToVector(x,y-1, nbrs);
				//addToVector(x-1,y-1, nbrs);			
				addToVector(x,y+1, nbrs);
				//addToVector(x-1,y+1, nbrs);
			}
		}

		//add all elements
		else 
		{			
			//top row
			if ( y != 0 )
			{
				//addToVector(x-1,y-1, nbrs);
				addToVector(x,y-1, nbrs);			
				//addToVector(x+1,y-1, nbrs);
			}

			//middle row
			addToVector(x-1,y, nbrs);		
			addToVector(x+1,y, nbrs);

			//bottom row
			if (y != height - 1 )
			{
				//addToVector(x-1,y+1, nbrs);			
				addToVector(x,y+1, nbrs);
				//addToVector(x+1,y+1, nbrs);	
			}
		}
		
		return nbrs;
	}

	//return xy co-ords of pixel neighbours
	vector<pixel> getNeighbours(int x, int y)
	{
		vector<pixel> nbrs;
		nbrs.reserve(4);

		if ( x > 0 ) addToVector(x-1,y, nbrs);
		if ( y > 0 ) addToVector(x,y-1, nbrs);
		if ( x < width - 1 ) addToVector(x+1,y, nbrs);		
		if ( y < height - 1 ) addToVector(x,y+1, nbrs);		
		
		return nbrs;
	}

	//get minimum and maximum luminosities of a set of pixels
	void getMinMax(int &min, int &max, vector<pixel> &nbrs)
	{
		//set initial values
		min = image[nbrs[0].y][nbrs[0].x];
		max = image[nbrs[0].y][nbrs[0].x];

		//get min max values
		for ( int i=1; i < nbrs.size(); i++ )
		{
			if ( image[nbrs[i].y][nbrs[i].x] < min ) min = image[nbrs[i].y][nbrs[i].x];
			else if ( image[nbrs[i].y][nbrs[i].x] > max ) max = image[nbrs[i].y][nbrs[i].x];
		}
	}	

	//debug functions
	void debugImage(char* msg = "")
	{
		cout << "Image: " << msg << endl;

		for (int y=0; y < height; y++)
		{				
			for (int x=0; x<width; x++)
			{
				cout << image[y][x] << "\t";							
			}	

			cout << endl;
		}

		cout << endl;
	}

	void debugCsmap(char* msg = "")
	{
		cout << "CSmap: " << msg << endl;

		for (int y=0; y < height; y++)
		{				
			for (int x=0; x<width; x++)
			{
				cout << csmap[y][x] << "\t";							
			}	

			cout << endl;
		}

		cout << endl;
	}
};

#endif
