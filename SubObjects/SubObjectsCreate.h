#pragma once
#include "Comptr.h"
#include <memory>

struct D3D12_STATE_SUBOBJECT;
struct ID3D12RootSignature;

namespace sub_objects
{
	class SubObject;

	std::shared_ptr<SubObject> CreateHitProgram();
	std::shared_ptr<SubObject> CreateExportAssociation();
	std::shared_ptr<SubObject> CreateSubObjectRootSignature();
	std::shared_ptr<SubObject> CreateShaderConfig();
	std::shared_ptr<SubObject> CreatePipelineConfig();
}
