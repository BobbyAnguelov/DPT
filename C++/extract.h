#ifndef _PE
#define _PE

void DPT::extractPulse(int n)
{
	vector<pulse> dpulses, upulses;
	vector<int> newActiveSets;
	newActiveSets.reserve(8000);	//guestimate of set size
	
	//for each active set
	for (int i=0; i < activeSets.size(); i++)
	{
		//shortcut to set
		pixelSet* cSet = &csets[activeSets[i]];

		//is set is active
		if ( cSet->isActive() )
		{	
			//if set is within extraction range
			if ( cSet->getSet().size() <= n )
			{
				//get min max values of neighbors
				int min, max;
				getMinMax(min, max, cSet->getAdj());

				//if set is a local minimum
				if ( min > cSet->getSetLum() )
				{
					//extract down pulse
					pulse p = { *cSet, cSet->getSetLum() - min, cSet->getSetLum()};
					dpulses.push_back(p);

					//set set luminosity to min value
					for ( int k=0; k < cSet->getSet().size(); k++ )
					{
						image[cSet->getSet()[k].y][cSet->getSet()[k].x] = min;
					}
					cSet->setLum(min);
					
					//clear adjacents and grow set
					vector<pixel> nbrs = cSet->getAdj();
					cSet->clearAdj();
					grow(nbrs, *cSet);
				
					//add to new active sets					
					newActiveSets.push_back(activeSets[i]);					
				}

				//if set is a local maximum
				else if ( max < cSet->getSetLum() )
				{
						//extract up pulse
						pulse p = { *cSet, cSet->getSetLum() - max, cSet->getSetLum()};
						upulses.push_back(p);

						//set set luminosity to max value
						for ( int k=0; k < cSet->getSet().size(); k++ )
						{
							image[cSet->getSet()[k].y][cSet->getSet()[k].x] = max;
						}
						cSet->setLum(max);
	
						//clear adjacents and grow set
						vector<pixel> nbrs = cSet->getAdj();
						cSet->clearAdj();
						grow(nbrs, *cSet);		
						
						//add to new active sets					
						newActiveSets.push_back(activeSets[i]);					
				}				
			}

			//add to active set for future processing
			else
			{				
				newActiveSets.push_back(activeSets[i]);
			}			
		}		
	}

	//add pulses to class storage
	downPulses.push_back(dpulses);
	upPulses.push_back(upulses);

	//remove old active sets list and set it to the new list
	activeSets.erase(activeSets.begin(), activeSets.end());
	activeSets = newActiveSets;
}

#endif
