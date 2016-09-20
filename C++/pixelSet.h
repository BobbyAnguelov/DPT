#ifndef _PS
#define _PS

#include <vector>

using namespace std;

class pixelSet
{

private:

	//member sets
	vector<pixel> set;
	vector<pixel> adj;	

	//details
	int lum;
	int num;

	//set status
	bool active;

public:

	//constructor
	pixelSet():active(false) {}
	pixelSet(int n, int l, bool s):num(n), lum(l), active(s) {}
	
	//destructor
	~pixelSet() {}

	//get
	vector<pixel>& getSet() { return set; }
	vector<pixel>& getAdj() { return adj; }
	int& getSetNum() { return num; }
	int& getSetLum() { return lum; }
	bool& isActive() { return active; }

	//set
	void setActive(bool state) { active = state; }	
	void setLum(int l) { lum = l; }	
	void clearAdj() { adj.erase(adj.begin(), adj.end()); }

	//debug
	void debug()
	{	

		cout << "Pixel Set Number: " << num << " active: " << (bool) active << endl << "======================" << endl;
		cout << "Set Luminosity: " << lum << endl;
		cout << "Number of Pixels in Set: " << set.size() << endl;
		cout << "Number of Adjacent Pixels: " <<  adj.size() << endl << endl;

		cout << "Pixels: ";

		for (int i=0; i < set.size(); i++)
		{
			cout << set[i].x << ":" << set[i].y << ", ";
		}

		cout << endl << "Adjacent Pixels: ";

		for (int i=0; i < adj.size(); i++)
		{
			cout << adj[i].x << ":" << adj[i].y << ", ";
		}

		cout << endl << "======================" << endl << endl;
	}

};

#endif