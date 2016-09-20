#include <iostream>
#include <vector>

using namespace std;

//custom includes
#include "imageHandler.h"

//include Discrete Pulse Transform Class
#include "DPT.h"
#include "grow.h"
#include "extract.h"

int main()
{
	//image loader
	imageHandler i;
	i.loadImageFromCSV("bob.csv");
	DPT* d = new DPT(i.getImg(), i.getWidth(), i.getHeight());

	d->decompose();

	int** rImg = d->fullReconstruction();
	i.saveToCSV("full.csv", rImg, d->getWidth(), d->getHeight());

	rImg = d->partialReconstruction(200,350);
	i.saveToCSV("partial200to350.csv", rImg, d->getWidth(), d->getHeight());

	rImg = d->partialReconstruction(350,800);
	i.saveToCSV("partial350to800.csv", rImg, d->getWidth(), d->getHeight());

	rImg = d->partialReconstruction(1000,2000);
	i.saveToCSV("partial1000to2000.csv", rImg, d->getWidth(), d->getHeight());

	rImg = d->partialReconstruction(2000,35000);
	i.saveToCSV("partial2000to35000.csv", rImg, d->getWidth(), d->getHeight());

	rImg = d->partialReconstruction(35000,60000);
	i.saveToCSV("partial35000to60000.csv", rImg, d->getWidth(), d->getHeight());

	rImg = d->partialReconstruction(35000,85000);
	i.saveToCSV("partial35000to85000.csv", rImg, d->getWidth(), d->getHeight());

	//release memory
	delete[] rImg;

	//pause and exit
	cout << "Complete!" << endl;
	char ch;
	cin >> ch;
	return 0;
}
