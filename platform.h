#ifndef BF_PLATFORM
#define BF_PLATFORM

#ifdef _WIN64
	#define PLATFORM_WINDOWS_64
#elif _WIN32
	#define PLATFORM_WINDOWS_32
#else
	#error Unsupported platform, linux and MacOS coming soon
#endif

#endif