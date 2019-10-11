#pragma once
#include <windows.h>
#include <dxcapi.h>
#include "Comptr.h"
#include <string>

class DirectXCompiler
{
public:
	DirectXCompiler();
	~DirectXCompiler();

	MWCptr<IDxcBlob> Compile(const std::string& shaderPath, const std::string& target);
	MWCptr<IDxcBlob> Compile(const std::wstring& shaderPath, const std::wstring& target);

private:
	MWCptr<IDxcCompiler> mDxc;
	MWCptr<IDxcLibrary> mLib;
	MWCptr<IDxcIncludeHandler> mIncludeHandler;

	void Init();
};

