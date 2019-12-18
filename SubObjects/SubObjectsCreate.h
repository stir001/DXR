#pragma once
#include "Comptr.h"
#include <memory>
#include <vector>
#include <string>

struct D3D12_STATE_SUBOBJECT;
struct ID3D12RootSignature;
struct D3D12_ROOT_SIGNATURE_DESC;
struct ID3D12Device5;

namespace sub_objects
{
	class SubObject;

	//AnyHitShaderとClosestHitShaderをまとめたグループを作成する
	std::shared_ptr<SubObject> CreateHitGroup(const std::wstring& ahsName, const std::wstring& chsName, const std::wstring& hitGroupName);
	//ShaderとrootSignatureを関連付けるものを作成　複数のShaderとの関連付けもできる
	//HitGroupとの関連付けも可能？
	//ShaderConfigの設定もできる
	std::shared_ptr<SubObject> CreateExportAssociation(const std::vector<std::wstring>& exportNames, unsigned int subObjectIndex);
	//LocalRootSignatureまたは、GlobalRootSignatureを作成する
	std::shared_ptr<SubObject> CreateSubObjectRootSignature(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal);
	std::shared_ptr<SubObject> CreateSubObjectRootSignature(const MWCptr<ID3D12RootSignature>& rootSignature, bool isLocal);
	//Shade内でのMaxAttributeSize(ClosestHitShaderとAnyHitShaderで使用する引数の最大サイズ)とMaxPayLoadSizeを設定する
	//Attribute:デフォルトの交差判定を使用する場合は必ずBuiltInTriangleIntersectionAttributesを使用しなければならない
	//そのサイズはfloat2相当
	std::shared_ptr<SubObject> CreateShaderConfig(unsigned int maxAttributeSize, unsigned int maxPayloadSize);
	//MaxTraceRecursionDepthを設定する
	std::shared_ptr<SubObject> CreatePipelineConfig(unsigned int maxTraceRecursionDepth);
	std::shared_ptr<SubObject> CreateStateConfig();
}
