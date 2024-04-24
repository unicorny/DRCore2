/*/*************************************************************************
*                                                                         *
* UniversumLib, collection of classes for generating and go through a     *
* whole universe. It is for my Gameproject Spacecraft					   *
* Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
* Email: ***REMOVED***   Web: ***REMOVED***                *
*                                                                         *
* This program is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* any later version.													   *
*																		   *
* This program is distributed in the hope that it will be useful,	       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
* GNU General Public License for more details.							   *
*																		   *
* You should have received a copy of the GNU General Public License	   *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                         *
***************************************************************************/

/*!
 
 \brief Container class for mutex protected queue

 \author Dario Rekowski

 \date 08.10.2015
*/

#ifndef _DR_UNIVERSUM_LIB_LIB_MULTITHREAD_QUEUE_H__
#define _DR_UNIVERSUM_LIB_LIB_MULTITHREAD_QUEUE_H__

#include "DRMultithreadContainer.h"
#include <queue>

template <class ResourceType>
class DRMultithreadQueue: protected std::queue<ResourceType>, protected DRMultithreadContainer
{
public:
	virtual ~DRMultithreadQueue() {
		clear();
	}
	void clear() {
		auto _lock = getUniqueLock("DRMultithreadQueue::clear");
		while (!std::queue<ResourceType>::empty()) std::queue<ResourceType>::pop();
	}
	void push(ResourceType val) 
	{
		auto _lock = getUniqueLock("DRMultithreadQueue::push");
		std::queue<ResourceType>::push(val);
	}
	bool empty()
	{
		auto _lock = getUniqueLock("DRMultithreadQueue::empty");
		return std::queue<ResourceType>::empty();
	}
	//! \return false if no values are there
	//! \return true if value is there, gave val a copy from the value on top of queue
	bool pop(ResourceType& val) 
	{
		auto _lock = getUniqueLock("DRMultithreadQueue::pop");
		if(!std::queue<ResourceType>::empty()) {
			val = std::queue<ResourceType>::front();
			std::queue<ResourceType>::pop();
			return true;
		}
		return false;
	}

	inline size_t size() { auto _lock = getUniqueLock("DRMultithreadQueue::size"); return std::queue<ResourceType>::size();}

};

#endif //_DR_UNIVERSUM_LIB_LIB_MULTITHREAD_QUEUE_H__