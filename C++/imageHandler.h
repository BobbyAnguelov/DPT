#ifndef _IMAGELOADER
#define _IMAGELOADER

#include <fstream>
#include <string>
#include <vector>

using namespace std;

//custom headers
#include "FreeImage.h"

class imageHandler
{

private:

	int width, height;
	int** img;

public:

	//constructor
	imageHandler():height(0),width(0) {};

	//accessors
	int** getImg() { return img; }
	int getWidth() { return width; }
	int getHeight() { return height; }

	//load from JPEG image
	bool loadImageFromJPEG(char* filename)
	{
		//load from file
		FIBITMAP *bitmap = FreeImage_Load(FIF_JPEG, filename, JPEG_ACCURATE);
	
		//check for successful load
		if (bitmap) 
		{
			//convert to greyscale
			FreeImage_ConvertTo8Bits(bitmap);

			//get width and height
			width = FreeImage_GetWidth(bitmap);
			height = FreeImage_GetHeight(bitmap);

			//create new image storage
			img = new int*[height];
			for (int i=0; i < height; i++) img[i] = new int[width];			

			//copy pixel values to img
			RGBQUAD* color = new RGBQUAD();

			for (int y=height-1; y >= 0; y--)
			{
				for (int x=0; x < width; x++)
				{					
					FreeImage_GetPixelColor(bitmap, x, y, color);
					img[height-y-1][x] = color->rgbRed;					
				}
			}		
			
			//operation succeeded
			return true;
		}
		else return false;		
	}

	//load from CSV file
	bool loadImageFromCSV(char* filename)
	{
		//open file
		ifstream df;
		df.open(filename, fstream::in);
	
		//check for successful load
		if ( df.is_open() ) 
		{
			string line = "";
			
			//data storage
			vector<vector<int>> data;	
			height = 0;

			while ( ! df.eof() )
			{
				//read line
				getline(df, line);				
				
				//process line
				//-------------------------------------------------
					
					vector<int> row;
					char* cstr = new char[line.size()+1];
					char* t;
					strcpy(cstr, line.c_str());

					//tokenise
					int i = 0;
					t=strtok (cstr,",");
					
					while ( t!=NULL )
					{	
						row.push_back(atoi(t));
						t = strtok(NULL,",");
					}

					//add row to storage
					data.push_back(row);

					//free allocated memory;
					delete cstr;

					//increment height
					height++;
			}

			//close file
			df.close();

			//set width and height
			height--;
			width = data[0].size();

			//copy data to img array
			img = new int*[height];
			for (int i=0; i < height; i++) img[i] = new int[width];	

			for (int y=0; y < height; y++)
			{
				for (int x=0; x < width; x++)
				{
					img[y][x] = data[y][x];				
				}
			}

			//operation succeeded
			return true;
		}
		else return false;		
	}

	//save loaded image to CSV file
	void saveToCSV(char* filename)
	{
		//create output file
		ofstream of;
		of.open(filename, fstream::out);

		//write to CSV file
		for (int y=0; y < height; y++)
		{
			for (int x=0; x < width; x++)
			{
				if ( x + 1 == width ) of << (int) img[y][x];
				else of << (int) img[y][x] << ",";
			}

			of << endl;
		}

		//close output file
		of.close();	
	}

	//save inputed image to CSV file
	void saveToCSV(char* filename, int** nImg, int w, int h)
	{
		//create output file
		ofstream of;
		of.open(filename, fstream::out);

		//write to CSV file
		for (int y=0; y < h; y++)
		{
			for (int x=0; x < w; x++)
			{
				if ( x + 1 == width ) of << nImg[y][x];
				else of << nImg[y][x] << ",";
			}

			of << endl;
		}

		//close output file
		of.close();	
	}

};

#endif
