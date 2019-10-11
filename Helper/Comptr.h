#pragma once
#ifndef __MY_COMPTR__
#define __MY_COMPTR__

#include <wrl.h>

template<class T>
using MWCptr = Microsoft::WRL::ComPtr<T>;

#endif