#pragma once

namespace vw
{
	class SignalEmitter
	{
	public:
		using SignalListener = std::function < void () >;

		void AddSignalListener ( std::string const &, SignalListener );

	protected:
		void EmitSignal ( std::string const & );

	private:
		std::unordered_map <std::string, std::vector <SignalListener>> signalListeners {};
	};
}