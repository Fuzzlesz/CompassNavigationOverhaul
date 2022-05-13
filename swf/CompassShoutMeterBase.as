﻿class CompassShoutMeterBase extends MovieClip
{
	var Compass:MovieClip;
	var ShoutMeterInstance:MovieClip;
	var ShoutWarningInstance:MovieClip;
	var All:Boolean;
	var Favor:Boolean;
	var DialogueMode:Boolean;
	var StealthMode:Boolean;
	var Swimming:Boolean;
	var HorseMode:Boolean;
	var WarHorseMode:Boolean;

	public function CompassShoutMeterBase()
	{
		var hud = _root.HUDMovieBaseInstance;

		var hadTemperatureMeter:Boolean = hud["TemperatureMeter_mc"] != undefined;

		var hudElementIndex:Number = -1;
		for (var i:Number = 0; i < hud.HudElements.length; i++)
		{
			if (hud.HudElements[i] == hud.CompassShoutMeterHolder)
			{
				hudElementIndex = i;
			}
		}

		hud.CompassShoutMeterHolder.swapDepths(1);
		hud.CompassShoutMeterHolder.removeMovieClip();
		hud.CompassShoutMeterHolder = this;

		delete hud.ShoutMeter_mc;
		hud.ShoutMeter_mc = new ShoutMeter(ShoutMeterInstance, ShoutWarningInstance);

		if (hadTemperatureMeter)
		{
			hud.TemperatureMeter_mc = Compass.CompassTemperatureHolderInstance;
		}
		else
		{
			Compass.CompassTemperatureHolderInstance.gotoAndStop("Empty");
		}

		hud.CompassRect = Compass.DirectionRect;

		hud.HudElements[hudElementIndex] = this;

		All = true;
		Favor = true;
		DialogueMode = true;
		StealthMode = true;
		Swimming = true;
		HorseMode = true;
		WarHorseMode = true;
	}
}
