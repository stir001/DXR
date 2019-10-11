#pragma once
struct D3D12_STATE_SUBOBJECT;

namespace sub_objects
{
	class SubObject
	{
	protected:
		SubObject();
		virtual ~SubObject();

		virtual D3D12_STATE_SUBOBJECT Get() const = 0 ;
	};
}

