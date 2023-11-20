#include "Core.hpp"

namespace vw
{
	std::unordered_map <Sides, glm::vec3> const directionVectors
	{
		{ Sides::up,		{  0,  1,  0 } },
		{ Sides::down,		{  0, -1,  0 } },
		{ Sides::left,		{ -1,  0,  0 } },
		{ Sides::right,		{  1,  0,  0 } },
		{ Sides::forward,	{  0,  0,  1 } },
		{ Sides::back,		{  0,  0, -1 } },
	};

	std::unordered_map <Sides, Sides> const oppositeSides
	{
		{ Sides::up,		Sides::down },
		{ Sides::down,		Sides::up },
		{ Sides::left,		Sides::right },
		{ Sides::right,		Sides::left },
		{ Sides::forward,	Sides::back },
		{ Sides::back,		Sides::forward },
	};

	std::array <Sides, 6> const sides { Sides::up, Sides::down, Sides::left, Sides::right, Sides::forward, Sides::back };

	std::unordered_map <Sides, std::string> const sideNames
	{
		{ Sides::up,		"Up" },
		{ Sides::down,		"Down" },
		{ Sides::left,		"Left" },
		{ Sides::right,		"Right" },
		{ Sides::forward,	"Forward" },
		{ Sides::back,		"Back" },
	};


	Sides GetOppositeSide ( Sides side )
	{
		switch ( side )
		{
		case Sides::up:			return Sides::down;
		case Sides::down:		return Sides::up;
		case Sides::left:		return Sides::right;
		case Sides::right:		return Sides::left;
		case Sides::forward:	return Sides::back;
		case Sides::back:		return Sides::forward;
		}
	}

	glm::vec3 GetDirectionVector ( Sides side )
	{
		switch ( side )
		{
		case Sides::up:			return { 0,  1,  0 };
		case Sides::down:		return { 0, -1,  0 };
		case Sides::left:		return { -1,  0,  0 };
		case Sides::right:		return { 1,  0,  0 };
		case Sides::forward:	return { 0,  0,  1 };
		case Sides::back:		return { 0,  0, -1 };
		}
	}
}