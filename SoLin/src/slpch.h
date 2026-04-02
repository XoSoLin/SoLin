#pragma once

#include<iostream>
#include<memory>
#include<utility>
#include<algorithm>
#include<functional>

#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<unordered_set>

#include "SoLin/Core/Log.h"

#ifdef SL_DEBUG
#include"SoLin/Debug/Instrumentor.h"
#endif

#ifdef SL_PLATFORM_WINDOWS
	#include<Windows.h>
#endif
