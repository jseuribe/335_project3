#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "dsexceptions.h"
#include "soldierPair.h"
#include "QuadraticProbing.h"
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

// BinaryHeap class (Weiss, modified by Jose Uribe
//
// CONSTRUCTION: with an optional capacity (that defaults to 100)
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )	  --> Insert x
// deleteMin( minItem )   --> Remove (and optionally return) smallest i$
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )	  --> Remove all items
//ADDED FUNCTIONS
// bool interact_with_a_Soldier(bool faction, int ID)//The soldier is attack the soldier with the ID number in the second param. 
// void decreaseKey(int p, int delta).
// void printAllSoldiers()
// void hash_Soldier(Comparable& x, int position)
// void remove_Soldier(Comparable& x, int position)
// void update_Position(Comparable& x, int newPosition)
// int return_hash_Position(int n)
// Comparable find_by_ID(int iD)
// void remove(int p, Comparable & minItem)
// void increaseKey(int p, int delta)
// void increase_by_iD(int n, int delta)
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinaryHeap
{
  public:
    explicit BinaryHeap( int capacity = 100 )
      : array( capacity + 1 ), currentSize{ 0 }
    {
    }

    explicit BinaryHeap( const vector<Comparable> & items )
      : array( items.size( ) + 10 ), currentSize{ items.size( ) }
    {
     	for( int i = 0; i < items.size( ); ++i )
            array[ i + 1 ] = items[ i ];
        buildHeap( );
    }

    bool isEmpty( ) const
      { return currentSize == 0; }

    /**
     * Find the smallest item in the priority queue.
     * Return the smallest item, or throw Underflow if empty.
     */
    const Comparable & findMin( ) const
    {
     	if( isEmpty( ) )
            throw UnderflowException{ };
        return array[ 1 ];
    }

    /**
     * Insert item x, allowing duplicates.
     */
    void insert( Comparable & x )
    {
	//cout << "I'm in insert!" << endl;
        if( currentSize == array.size( ) - 1 )
            array.resize( array.size( ) * 2 );

            // Percolate up
        int hole = ++currentSize;
	hash_Soldier( x, hole );//Hash the soldier. Reserves a spot in the table, and the position will be updated later
	//cout << "current size is:  " << currentSize << endl;
        Comparable copy = x;

        Comparable update_original;

	soldierPair currentlyMoved;

        array[ 0 ] = std::move( copy );
        for( ; x < array[ hole / 2 ]; hole /= 2 ){
	    update_original = array[hole / 2];//In order to get the value of the iD, we save our stored item.
	    //cout << update_original.return_iD() << " Is going to be moved to " << hole << endl;
	    currentlyMoved = table[update_original.return_iD()];
	    //cout << "soldier iD " << currentlyMoved.return_iD() << " was at positionc " << currentlyMoved.return_Position() << endl;
            array[ hole ] = std::move( array[ hole / 2 ] );//We move our value above our hole, down

	    update_Position(update_original, hole);//We percolate the item up as needed. This update is reflected in position changing
	    currentlyMoved = table[update_original.return_iD()];
	    //cout << "soldier iD " << currentlyMoved.return_iD() << " is now in position " << currentlyMoved.return_Position() << endl;
	    //update_Position(x, hole/2);
	    
	}
        array[ hole ] = std::move( array[ 0 ] );
	update_Position(x, hole);//update the position of the inserted object at the end.

	soldierPair thisPair = table[x.return_iD()];
	//cout << "the soldier with the iD number: " << thisPair.return_iD() << " is at position: " << thisPair.return_Position() << endl;
    }

    /**
     * Insert item x, allowing duplicates.
     */
    /*
    void insert( Comparable && x )
    {
     	if( currentSize == array.size( ) - 1 )
            array.resize( array.size( ) * 2 );

            // Percolate up
        int hole = ++currentSize;
        for( ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 )
            array[ hole ] = std::move( array[ hole / 2 ] );
        array[ hole ] = std::move( x );
    } */


    /**
     * Remove the minimum item.
     * Throws UnderflowException if empty.
     */
    void deleteMin( Comparable & minItem )
    {
     	if( isEmpty( ) )
            throw UnderflowException{ };

        minItem = std::move( array[ 1 ] );
	//cout << "the ID of our smallest item is: " << minItem.return_iD() << endl;
        array[ 1 ] = std::move( array[ currentSize-- ] );
        percolateDown( 1 );
    }

    void decreaseKey(int p, int delta)
    {

	if( isEmpty( ) )
            throw UnderflowException{ };

        Comparable temp = array[p];
	temp.set_actionTime(temp.return_actionTime() - delta);
	array[p] = temp;

        int hole = p;
        
        Comparable update_original;

        array[ 0 ] = std::move( temp );
        for( ; temp < array[ hole / 2 ]; hole /= 2 ){
	    update_original = array[hole/2];//In order to get the value of the iD, we save our stored item.
            array[ hole ] = std::move( array[ hole / 2 ] );
	    update_Position(update_original, hole);//We percolate the item up as needed. This update is reflected in position changing
	    
	}

        array[ hole ] = std::move( array[ 0 ] );
	update_Position(temp, hole);//update the position of the inserted object at the end.

	//cout << "decreaseKey call will now end..." << endl;
    }

    void increaseKey(int p, int delta)//Increases the soldier 
    {
	Comparable temp = array[p];
	temp.set_actionTime(temp.return_actionTime() + delta);
	array[p] = temp;
	percolateDown(p);
    }

    void remove(int p, Comparable & minItem)
    {
	decreaseKey(p, 100000);//Temporary value; pretty much: reduces the value to the abs smallest
	deleteMin( minItem );
	//cout << "Remove has finished decreaseKey and deleteMin successfully!" << endl;
    }

    void decrease_by_iD(int n, int delta)//finds the iD value in the heap, and modifies the action time.
    {
	soldierPair position = table[n]; //hashes your iD, then returns the position in the heap.

	decreaseKey(position.return_Position(), delta);//Increases by delta.
	
	
    }

    void increase_by_iD(int n, int delta)//Find the soldier by ID. Then increase (push them down the queue) the value.
    {
	soldierPair position = table[n];
	
	increaseKey(position.return_Position(), delta);
    }

    int return_hash_Position(int n)
    {
	soldierPair currentItem = table[n];//Hash the iD, and retrieve the value stored at that position.

	if(currentItem.return_Position() != -1) return currentItem.return_Position();
	else return -1;
    }

    Comparable find_by_ID(int iD)//Look up the soldier in our heap by ID. Hashes the ID.
    {
	soldierPair currentItem = table[iD];

	return array[currentItem.return_Position()];
    }

    bool interact_with_Soldier(bool attacker, int iD)//Attacker is who is attacking, IE; our 1st value. iD is our attackee.
    {
	//cout << "Hello! We're in the heap right now!" << endl;
	int position;
	soldier temp;//In order to store our to be removed soldier.
	//cout << "The current size of our array is: " << currentSize << endl;
	position = return_hash_Position(iD);//Hash the iD. We'll get back our B-Heap position after.
	//cout << "The position of our target: " << iD << " is at position: " << position << endl;

	if( position == -1) return false;//The soldier doesn't exist in the heap.

	if(attacker) //A Spartan. We are therefore attacking a Persian
	{
		//cout << "A persian has been killed!" << endl;
		remove(position, temp);//The persian is going to be killed, since Spartans are very efficient killers
		return true;//A flag sent back. True is returned
	}
	else //A persian. Remember, they attack 5% of the time.
	{
		int x = rand() % 20;
		if( x == 0 ) {
			//cout << "It's a hit!" << endl;
			array[position].takeHit(1);
			if(array[position].return_lifePoints() >= 1) {
				//cout << "The spartan is wounded!" << endl;
				 //Decrement the life by 1.
				increaseKey(position, rand() % 5 + 1 );//Increase the Spartan's reaction time by 1-4
				return false;//Since the spartan has 1 lp left, we will not invigorate
				
			}
			else if(array[position].return_lifePoints() == 0){
				//cout << "A Spartan has just been killed!" << endl;
				remove(position, temp);//Our spartan is hit, his last life: gone. He is removed.
				return true; //Invigorate() will be activated.
			}
		}	
		else{
		 //cout << "Looks like our Persian missed..." << endl;
		 return false;//The Spartan was not killed, or wounded, Invigorate() will not be executed.
		}
	}
    }


    void printAllSoldiers()
    {
	for(int i = 1; i <= currentSize; i++)
	{
		cout << array[i].return_iD() << endl;

	}
    }
    void makeEmpty( )
      { 
	currentSize = 0;
	array.clear();//Destroy all objects in the array. A fresh heap, with no elements is initialized.
	table.makeEmpty();//All objects in the table are also simultaneously destroyed. Table handles this itself. 
      }

  private:
    int                currentSize;  // Number of elements in heap
    vector<Comparable> array;        // The heap array
    HashTable<soldierPair> table;    //Bookkeeping of our soldiers.

    /**
     * Establish heap order property from an arbitrary

     * arrangement of items. Runs in linear time.
     */
    void buildHeap( )
    {
     	for( int i = currentSize / 2; i > 0; --i )
            percolateDown( i );
    }

    /**
     * Internal method to percolate down in the heap.
     * hole is the index at which the percolate begins.
     */
    void percolateDown( int hole )
    {
     	int child;
        Comparable tmp = std::move( array[ hole ] );
	Comparable updateOriginal;
        for( ; hole * 2 <= currentSize; hole = child )
        {
            child = hole * 2;
            if( child != currentSize && array[ child + 1 ] < array[ child ] )
                ++child;
            if( array[ child ] < tmp ){
                array[ hole ] = std::move( array[ child ] );
		updateOriginal = array[hole];
		update_Position(updateOriginal, hole);
	    }
            else
                break;
        }
	array[ hole ] = std::move( tmp );
	update_Position(tmp, hole);
    }

    void hash_Soldier(Comparable& x, int position)//Insert our new soldier into the hash table
    {
	soldierPair newHashedObj;
	newHashedObj.set_iD(x.return_iD());
	newHashedObj.set_Position(position);//Where does position come from?
	table.insert(newHashedObj);
    }

    void remove_Soldier(Comparable& x, int position)//Removes a value from the table
    {
	table.remove( x );
    }

    void update_Position(Comparable& x, int newPosition)//Updates a value's position in the hash table
    {
	table.update_Element(x.return_iD(), newPosition);
    }
};

#endif




