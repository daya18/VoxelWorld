#pragma once

#include "VectorBuffer.hpp"

namespace vw
{
	template < class KeyType, class ValueType >
	class MapBuffer
	{
	public:
		using KeyValuePair = std::pair <KeyType, ValueType>;

		MapBuffer ();

		// returns the value index of the first element to be inserted
		unsigned int Insert ( std::vector < KeyValuePair > const & );
		void Erase ( std::vector <KeyType> const & );

		VectorBuffer <ValueType> const & GetValueBuffer () const;
		int GetValueIndex ( KeyType const & ) const;
		unsigned int GetElementCount () const;

	private:
		VectorBuffer <ValueType> valueBuffer;
		std::vector <KeyType> keys;
		std::unordered_map <KeyType, unsigned int> mutable valueIndices;
	};



	template < class KeyType, class ValueType >
	MapBuffer<KeyType, ValueType>::MapBuffer () {}

	template < class KeyType, class ValueType >
	unsigned int MapBuffer<KeyType, ValueType>::Insert ( std::vector < KeyValuePair > const & keyValuePairs )
	{
		auto firstIndex { keys.size () };

		if ( keyValuePairs.empty () )
			return firstIndex;

		std::vector <KeyType> keys;
		keys.reserve ( keyValuePairs.size () );

		std::vector <ValueType> values;
		values.reserve ( keyValuePairs.size () );

		for ( auto const & [key, value] : keyValuePairs )
		{
			keys.push_back ( key );
			values.push_back ( value );
		}

		valueBuffer.PushBack ( values );
		this->keys.insert ( this->keys.end (), keys.begin (), keys.end () );

		valueIndices.clear ();

		return firstIndex;
	}

	template < class KeyType, class ValueType >
	void MapBuffer<KeyType, ValueType>::Erase ( std::vector <KeyType> const & targetKeys )
	{
		std::vector <unsigned int> indices;
		indices.reserve ( targetKeys.size () );

		for ( auto const & targetKey : targetKeys )
		{
			auto index { GetValueIndex ( targetKey ) };

			if ( index != -1 )
				indices.push_back ( index );
		}

		valueBuffer.Erase ( indices );

		auto eraseBegin { std::remove_if ( this->keys.begin (), this->keys.end (), [&targetKeys] ( KeyType const & key )
			{ return std::find ( targetKeys.begin (), targetKeys.end (), key ) != targetKeys.end (); } ) };

		this->keys.erase ( eraseBegin, this->keys.end () );

		//for ( auto const & index : indices )
		//	this->keys.remove ( this->keys.begin () + index );

		valueIndices.clear ();
	}

	template < class KeyType, class ValueType >
	VectorBuffer <ValueType> const & MapBuffer<KeyType, ValueType>::GetValueBuffer () const
	{
		return valueBuffer;
	}

	template < class KeyType, class ValueType >
	int MapBuffer<KeyType, ValueType>::GetValueIndex ( KeyType const & key ) const
	{
		auto valueIndexIt { valueIndices.find ( key ) };

		if ( valueIndexIt == valueIndices.end () )
		{
			auto keyIt { std::find ( keys.begin (), keys.end (), key ) };
			
			if ( keyIt == keys.end () )
				return -1;
			
			unsigned int valueIndex { static_cast < unsigned int > ( std::distance ( keys.begin (), keyIt ) ) };
			valueIndexIt = valueIndices.insert ( { key, valueIndex } ).first;
		}

		return valueIndexIt->second;
	}

	template < class KeyType, class ValueType >
	unsigned int MapBuffer <KeyType, ValueType>::GetElementCount () const
	{
		return keys.size ();
	}

}