#pragma once

struct D3D12_STATE_SUBOBJECT;
class RtPipelineState;

namespace sub_objects
{
	class SubObject
	{
	public:
		virtual const D3D12_STATE_SUBOBJECT& Get() const = 0;
		virtual void Create(RtPipelineState& pipeline) {};
	protected:
		SubObject();
		virtual ~SubObject();

	};
}

