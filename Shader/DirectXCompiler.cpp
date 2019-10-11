#include "DirectXCompiler.h"
#include "CharConvert.h"
#include <fstream>
#include <sstream>
#include <vector>

DirectXCompiler::DirectXCompiler() : mDxc(nullptr), mLib(nullptr), mIncludeHandler(nullptr)
{
	Init();
}

DirectXCompiler::~DirectXCompiler()
{
}

MWCptr<IDxcBlob> DirectXCompiler::Compile(const std::string& shaderPath, const std::string& target)
{
	std::wstring wPath;
	ToWChar(wPath, shaderPath);
	std::wstring wTarget;
	ToWChar(wTarget, target);

	return Compile(wPath, wTarget);
}

MWCptr<IDxcBlob> DirectXCompiler::Compile(const std::wstring& shaderPath, const std::wstring& target)
{
	std::ifstream shaderFile(shaderPath);
	std::stringstream strStream;
	strStream << shaderFile.rdbuf();
	std::string shader = strStream.str();

	MWCptr<IDxcBlobEncoding> textBlob;
	mLib->CreateBlobWithEncodingFromPinned((LPBYTE)shader.c_str(), (uint32_t)shader.size(), 0, textBlob.GetAddressOf());

	MWCptr<IDxcOperationResult> result;
	mDxc->Compile(textBlob.Get(), shaderPath.data(), L"", target.data(), nullptr, 0, nullptr, 0, mIncludeHandler.Get(), result.GetAddressOf());

	HRESULT hr;
	result->GetStatus(&hr);
	if(FAILED(hr)){
		MWCptr<IDxcBlobEncoding> error;
		result->GetErrorBuffer(&error);

		std::vector<char> info(error->GetBufferSize() + 1);
		memcpy(info.data(), error->GetBufferPointer(), error->GetBufferSize());
		info[error->GetBufferSize()] = '\0';
		std::string log = info.data();
		std::wstring logw;
		ToWChar(logw, log);

		MessageBox(nullptr, (L"Compiler Error : " + logw).data(), L"ERROR", MB_OK);
		return 0;
	}

	MWCptr<IDxcBlob> blob;
	result->GetResult(blob.GetAddressOf());

	return blob;
}

void DirectXCompiler::Init()
{
	DxcCreateInstanceProc fn;
	auto dll = LoadLibraryW(L"dxcompiler.dll");
	if (dll != nullptr)
	{
		fn = (DxcCreateInstanceProc)GetProcAddress(dll, "DxcCreateInstance");
	}
	else
	{
		MessageBox(nullptr, L"can not find dxcompiler.dll", L"ERROR", MB_OK);
		fn = nullptr;
		return;
	}

	HRESULT hr;
	if (FAILED(hr = fn(CLSID_DxcCompiler, IID_PPV_ARGS(mDxc.GetAddressOf()))))
	{
		MessageBox(nullptr, L"create faild : DxcCompiler", L"ERROR", MB_OK);
		return;
	}

	if (FAILED(hr = fn(CLSID_DxcLibrary, IID_PPV_ARGS(mLib.GetAddressOf()))))
	{
		MessageBox(nullptr, L"create faild : DxcLibrary", L"ERROR", MB_OK);
		return;
	}

	if (FAILED(hr = mLib->CreateIncludeHandler(mIncludeHandler.GetAddressOf())))
	{
		MessageBox(nullptr, L"create falid : IncludeHandler", L"ERROR", MB_OK);
		return;
	}
}
