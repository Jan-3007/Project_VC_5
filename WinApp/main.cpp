#include "VC5_global.h"





int
main()
{
	VC5Application app;

	WinError err = app.init_WinUSB();


	std::cout << std::format("Reading events ... press any key to exit.\n");

	while (true)
	{
		err = app.check_event();
		if (err != NO_ERROR)
		{
			return -1;
		}


		if (_kbhit())
		{
			_getch();
			break;
		}

		Sleep(100);
	}


	return 0;
}