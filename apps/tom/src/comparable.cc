#include "polymake/client.h"
#include "polymake/Set.h"
#include "polymake/Array.h"
#include <iostream>

//using namespace std;
using namespace pm;


Array<Array<Set<int>, Set<int> > > neighbours(Array<Set<int> >, Array<Set<int> >, int);


namespace polymake { namespace tom {



bool comparable(Array<Set<int> > type1, Array<Set<int> > type2, int d) {

	Array<Array<Set<int>, Set<int> > > nb = neighbours(type1,type2,d);

	for (int i=1; i<=d; ++i) {	// check whether there is a cycle starting with i

		Set<int> undir;	// will contain all vertices that can be reached from i via undir edges
		Set<int> dir;	// will contain all vertices that can be reached from i via at least one dir edge

		undir.insert(i);

		int x = 0;
		
		while (x<dir.size()+undir.size()) {

			x = dir.size()+undir.size();
			
			// go thru all vertices in undir
			for (Set<int>::iterator curr=undir.begin(); curr!=undir.end(); ++curr) {
				// go thru all undir-neighbours of the current vertex
				for (Set<int>::iterator nu= nb[*curr-1][1].begin(); nu!=nb[*curr-1][1].end(); ++nu) {
						undir.insert(*nu);	// and add to undir
				}
				
				// go thru all dir-neighbours of the current vertex
				for (Set<int>::iterator nd= nb[*curr-1][0].begin(); nd!=nb[*curr-1][0].end(); ++nd) {
					if (*nd == i) {		// if one neighbour is the start vertex i ...
						return 0;		// ... we found a cycle
					}
					dir.insert(*nd);	// otherwise add to dir
				}
			}
						
			// go thru all vertices in dir
			for (Set<int>::iterator curr=dir.begin(); curr!=dir.end(); ++curr) {
				// go thru all undir-neighbours of the current vertex
				for (Set<int>::iterator nu= nb[*curr-1][1].begin(); nu!=nb[*curr-1][1].end(); ++nu) {
					if (*nu ==i) {return 0;}	// cycle
					dir.insert(*nu);	// otherwise add to dir
				}
				// go thru all dir-neighbours of the current vertex
				for (Set<int>::iterator nd= nb[*curr-1][0].begin(); nd!=nb[*curr-1][0].end(); ++nd) {
					if (*nd == i) {		// if one neighbour is the start vertex i ...
						return 0;		// ... we found a cycle
					}
					dir.insert(*nd);	// otherwise add to dir
				}
				
			}
		}
	}	
	
	return 1;
}


Function4perl(&comparable, "comparable");



} }



Array<Array<Set<int>, Set<int> > > neighbours(Array<Set<int> > type1, Array<Set<int> > type2, int d) {
	Array<Array<Set<int>, Set<int> > > nb;
	nb.resize(d);
	
	for (int j=0; j<d; ++j) {
		nb[j].resize(2);
	}
		
	for (int i=0; i<type1.size(); ++i) {	// go thru all sets in type1 and type2
		
		for (Set<int>::iterator s=type1[i].begin(); s!=type1[i].end(); ++s) {
			for (Set<int>::iterator s2 = type2[i].begin(); s2!=type2[i].end(); ++s2) {
				//cout<<*s<<" "<<*s2<<endl;
				if (*s!=*s2) {
					if (type1[i].contains(*s2) && type2[i].contains(*s)) {
						nb[*s-1][1] += *s2;
						//cout<<"undir"<<endl;
					} else {
						nb[*s-1][0] += *s2;
						//cout<<"dir"<<endl;
					}
				}
			}
		}
	}

//	for (int k=0; k<d; ++k) {
//		cout<<nb[k][0]<<endl;
//		cout<<nb[k][1]<<endl;
//		cout<<endl;
//	}

	return nb;
}

