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

	//AnyHitShader��ClosestHitShader���܂Ƃ߂��O���[�v���쐬����
	std::shared_ptr<SubObject> CreateHitGroup(const std::wstring& ahsName, const std::wstring& chsName, const std::wstring& hitGroupName);
	//Shader��rootSignature���֘A�t������̂��쐬�@������Shader�Ƃ̊֘A�t�����ł���
	//HitGroup�Ƃ̊֘A�t�����\�H
	//ShaderConfig�̐ݒ���ł���
	std::shared_ptr<SubObject> CreateExportAssociation(const std::vector<std::wstring>& exportNames, unsigned int subObjectIndex);
	//LocalRootSignature�܂��́AGlobalRootSignature���쐬����
	std::shared_ptr<SubObject> CreateSubObjectRootSignature(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal);
	std::shared_ptr<SubObject> CreateSubObjectRootSignature(const MWCptr<ID3D12RootSignature>& rootSignature, bool isLocal);
	//Shade���ł�MaxAttributeSize(ClosestHitShader��AnyHitShader�Ŏg�p��������̍ő�T�C�Y)��MaxPayLoadSize��ݒ肷��
	//Attribute:�f�t�H���g�̌���������g�p����ꍇ�͕K��BuiltInTriangleIntersectionAttributes���g�p���Ȃ���΂Ȃ�Ȃ�
	//���̃T�C�Y��float2����
	std::shared_ptr<SubObject> CreateShaderConfig(unsigned int maxAttributeSize, unsigned int maxPayloadSize);
	//MaxTraceRecursionDepth��ݒ肷��
	std::shared_ptr<SubObject> CreatePipelineConfig(unsigned int maxTraceRecursionDepth);
	std::shared_ptr<SubObject> CreateStateConfig();
}
