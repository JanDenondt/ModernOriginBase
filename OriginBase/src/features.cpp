#include "common.hpp"
#include "features.hpp"
#include "logger.hpp"
#include "natives.hpp"
#include "script.hpp"

namespace Origin
{
	void features::run_tick()
	{
		TestBool(testbool);
	}

	void features::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				run_tick();
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}

	bool features::testbool = false;
	void features::TestBool(bool enable)
	{
		// Code to be executed
	}
}
