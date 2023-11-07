#include "SignalEmitter.hpp"

namespace vw
{
	void SignalEmitter::AddSignalListener ( std::string const & signal, SignalListener listener )
	{
		signalListeners [ signal ].push_back ( listener );
	}

	void SignalEmitter::EmitSignal ( std::string const & signal )
	{
		auto listenersIt { signalListeners.find ( signal ) };
		
		if ( listenersIt != signalListeners.end () )
		{
			for ( auto const & listener : listenersIt->second )
				listener ();
		}
	}
}