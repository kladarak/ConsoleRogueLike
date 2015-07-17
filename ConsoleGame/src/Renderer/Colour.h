#pragma once

enum EColour
{
	EColour_Foreground_Blue			= 1 << 0,
	EColour_Foreground_Green		= 1 << 1,
	EColour_Foreground_Red			= 1 << 2,
	EColour_Foreground_Intensity	= 1 << 3,
	
	EColour_Background_Blue			= 1 << 4,
	EColour_Background_Green		= 1 << 5,
	EColour_Background_Red			= 1 << 6,
	EColour_Background_Intensity	= 1 << 7,

	ETextDarkBlue		= EColour_Foreground_Blue,
	ETextDarkGreen		= EColour_Foreground_Green,
	ETextDarkRed		= EColour_Foreground_Red,
	ETextDarkYellow		= EColour_Foreground_Red | EColour_Foreground_Green,
	ETextDarkMagenta	= EColour_Foreground_Red | EColour_Foreground_Blue,
	ETextDarkCyan		= EColour_Foreground_Green | EColour_Foreground_Blue,
	ETextGrey			= EColour_Foreground_Red | EColour_Foreground_Green | EColour_Foreground_Blue,

	ETextBlue		= ETextDarkBlue		| EColour_Foreground_Intensity,
	ETextGreen		= ETextDarkGreen	| EColour_Foreground_Intensity,
	ETextRed		= ETextDarkRed		| EColour_Foreground_Intensity,
	ETextYellow		= ETextDarkYellow	| EColour_Foreground_Intensity,
	ETextMagenta	= ETextDarkMagenta	| EColour_Foreground_Intensity,
	ETextCyan		= ETextDarkCyan		| EColour_Foreground_Intensity,
	ETextWhite		= ETextGrey			| EColour_Foreground_Intensity,
};
